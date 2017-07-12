#include <cstring>
#include <cassert>
#include <memory>

#include <dlfcn.h>

#include "whisper.h"
#include "whisper_session.h"
#include "vlog.h"
#include "cfg.h"
#include "agent.h"
#include "user.h"
#include "friend.h"
#include "gadget.h"
#include "json.h"
#include "cmd.h"
#include "input.h"

class status2str {
public:
    status2str(WhisperConnectionStatus status) {
        mStatus = status;
    }

    operator const char*() const {
        const char *_str;

        switch(mStatus) {
        case WhisperConnectionStatus_Connecting:
            _str = "is connecting to";
            break;
        case WhisperConnectionStatus_Connected:
            _str = "connected with";
            break;
        case WhisperConnectionStatus_Disconnected:
        default:
            _str = "disconnected from";
            break;
        }
        return _str;
    }

private:
    WhisperConnectionStatus mStatus;
};

class separator {
public:
    separator(const char *from): mFrom(from) {}

    std::string userid(void) {
        const char *pos;

        pos = strchr(mFrom, '@');
        if (!pos)
            return std::string(mFrom);
        else
            return std::string(mFrom, pos-mFrom);
    }

private:
    const char* mFrom;
};

std::shared_ptr<CGadget> CPeer::getGadget(const std::string &name) const
{
    std::map<std::string, std::shared_ptr<CGadget>>::const_iterator it;
    it = mGadgets.find(name);
    return (it != mGadgets.end() ? it->second : nullptr);
}

void CPeer::addGadget(CAgent* agent, const std::string &name, const GadgetValue &value)
{
    CGadget *gadget;

    if (name.compare("bulb") == 0)
        gadget = new CBulb(agent, value.bValue());
    else if (name.compare("torch") == 0)
        gadget = new CTorch(agent, value.bValue());
    else if (name.compare("brightness") == 0)
        gadget = new CBrightness(agent, value.fValue());
    else if (name.compare("ring") == 0)
        gadget = new CRing(agent, value.bValue());
    else if (name.compare("volume") == 0)
        gadget = new CVolume(agent, value.fValue());
    else if (name.compare("camera") == 0)
        gadget = new CCamera(agent, value.bValue());
    else {
        vlogE("Unknown gadget name, skipped");
        return;
    }

    if (gadget)
        mGadgets[name] = std::shared_ptr<CGadget>(gadget);
}

static
void onIdle(Whisper *whisper, void *context)
{
    CAgent *agent = static_cast<CAgent*>(context);
    assert(agent);

    agent->handleInput();
}

static
void onConnectionStatus(Whisper *whisper, WhisperConnectionStatus status,
                        void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);

    vlogI("Device %s server", (const char*)status2str(status));

    agent->didConnectionStatusChange(status);
}

static
void onReady(Whisper *whisper, void *context)
{
    CAgent *agent = static_cast<CAgent*>(context);

    char login[WHISPER_MAX_ID_LEN + 1];
    vlogI("Device %s is ready", whisper_get_login(whisper, login, sizeof(login)));

    agent->didReady();
}

static
void onSelfInfo(Whisper *whisper, const WhisperUserInfo *info, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    agent->updateSelf(std::shared_ptr<CUser>(new CUser(info)));
}

static
bool onFriendList(Whisper *whisper, const WhisperFriendInfo *info, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    if (!info) return true;

    std::string peerName(info->user_info.userid);
    std::shared_ptr<CFriend> sp(new CFriend(info));

    agent->addPeer(peerName, sp, false);
    return true;
}

static
void onFriendInfo(Whisper *whisper, const char *friendid,
                  const WhisperFriendInfo *info, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    std::string peerName(friendid);
    std::shared_ptr<CFriend> sp(new CFriend(info));

    agent->updatePeer(peerName, sp);
}

static
void onFriendPresence(Whisper *whisper, const char *friendid,
                      const char *presence, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    std::string peerName(friendid);
    std::shared_ptr<std::string> sp(new std::string(presence));

    agent->updatePeer(peerName, sp);
}

static
void onFriendRequest(Whisper *whisper,const char *userid, const WhisperUserInfo *info,
                     const char *hello, void *context)
{
    vlogI("Device received friend request from %s with hello:%s", userid, hello);
    vlogI("where is:");
    vlogI("     UserId: %s", info->userid);
    vlogI("       Name: %s", info->name);
    vlogI("Description: %s", info->description);
    vlogI("     Gender: %s", info->gender);
    vlogI("      Phone: %s", info->phone);
    vlogI("      Email: %s", info->email);
    vlogI("     Region: %s", info->region);
    vlogI("");

    int rc;

    rc = whisper_reply_friend_request(whisper, userid, 0, NULL, true, NULL);
    if (rc < 0) {
        vlogE("Device replied friend request error: 0x%x", whisper_get_error());
        return;
    }

    vlogI("Device confirmed friend request from %s", userid);
}

static
void onFriendResponse(Whisper *whisper, const char *userid, int status,
                      const char *reason, bool entrusted, const char *expire,
                      void *context)
{
    vlogI("Device received friend request reply from %", userid);
    vlogI("which is:");
    vlogI("     status: %d", status);
    vlogI("     reason: %s", (status != 0) ? reason: "[null]");
    vlogI("  entrusted: %s", entrusted ? "true": "false");
    vlogI("     expire: %s", (expire ? expire : "[null]"));
    vlogI("");
}

static
void onFriendAdded(Whisper *whisper, const WhisperFriendInfo *info, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    vlogI("Friend %s added", info->user_info.userid);

    std::string name(info->user_info.userid);
    std::shared_ptr<CFriend> sp(new CFriend(info));
    agent->addPeer(name, sp);
}

static
void onFriendRemoved(Whisper *whisper, const char *friendid, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    vlogI("Friend %s removed", friendid);

    const std::string name(friendid);
    agent->delPeer(name);
}

static
void onFriendMessage(Whisper *whisper, const char *from, const char *msg,
                     size_t len, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    vlogI("Device received message from %s", from);
    vlogI("where message is: %s", msg);

    Json::Reader reader;
    Json::Value root;
    std::string type;

    if (!reader.parse(std::string(msg), root) || root["type"].empty()) {
        vlogI("Parse friend message error, check it");
        return;
    }

    type = root["type"].asString();
    if (type.compare("query") == 0) {
        agent->handleQuery(separator(from).userid());
        return;
    }

    std::map<std::string, GadgetValue> values;
    if (!root["bulb"].empty())
        values[std::string("bulb")] = GadgetValue(root["bulb"].asBool());
    if (!root["torch"].empty())
        values[std::string("torch")] = GadgetValue(root["torch"].asBool());
    if (!root["brightness"].empty())
        values[std::string("brightness")] = GadgetValue(root["brightness"].asFloat());
    if (!root["ring"].empty())
        values[std::string("ring")] = GadgetValue(root["ring"].asBool());
    if (!root["volume"].empty())
        values[std::string("volume")] = GadgetValue(root["volume"].asFloat());
    if (!root["camera"].empty())
        values[std::string("camera")] = GadgetValue(root["camera"].asBool());

    if (type.compare("status") == 0) {
        agent->handleStatus(separator(from).userid(), values);
        return;
    }

    if (type.compare("sync") == 0) {
        agent->handleSync(separator(from).userid(), values);
        return;
    }

    if (type.compare("modify") == 0) {
        agent->handleModify(separator(from).userid(), values);
        return;
    }
}

static
void onSessionRequestCallback(Whisper *whisper, const char *from,
                              const char *sdp, size_t len, void *context)
{
    CAgent* agent = static_cast<CAgent*>(context);
    assert(agent);

    vlogI("Agent received session request from %s", from);
    vlogI("where sdp is:%s\n", sdp);

    std::size_t found = std::string(sdp).find("whisper-ice-session");
    if (found == std::string::npos) {
        vlogE("Agent not supported session except for ice");
        return;
    }

    std::shared_ptr<std::string> spFrom(new std::string(from));
    std::shared_ptr<std::string> spSdp(new std::string(sdp));
    std::shared_ptr<CSession> sess(new CSession(spFrom, spSdp));

    if (sess && sess->start(whisper))
        agent->addSession(separator(from).userid(), sess);
}

static
void logPrint(const char *format, va_list args)
{
    logMsg(VLOG_INFO, format, args);
}

CAgent::~CAgent()
{
    if (mDylib)
        dlclose(mDylib);
}

bool CAgent::setup(const std::shared_ptr<CConfig> cfg)
{
    if (cfg->dylibName()) {
        mDylib = dlopen(cfg->dylibName(), RTLD_NOW);
        if (!mDylib) {
            vlogE("Loading dynamic library %s error: %s", cfg->dylibName(),
                dlerror());
            return false;
        }
    }

    std::map<std::string, std::shared_ptr<CGadget>>::const_iterator it;
    for (it = mGadgets.begin(); it != mGadgets.end(); ++it) {
        bool succ;

        succ = it->second->open();
        if (!succ) {
            vlogE("Open gadget %s error.", it->first.c_str());
            return false;
        }
    }

    WhisperOptions options = {
        .login = NULL,
        .password = NULL,
        .appid = cfg->appId(),
        .appkey = cfg->appKey(),
        .api_server_url = cfg->apiServer(),
        .mqtt_server_uri = cfg->mqttServer(),
        .trust_store = cfg->trustStore(),
        .keepalive_interval = 0,
        .connect_timeout = 0,
        .retry_interval = 0,
        .retry_times = 0,
        .persistent_location = cfg->persistentLocation(),
        .deviceid = cfg->deviceId(),
    };

    WhisperCallbacks callbacks = {
        .idle = onIdle,
        .connection_status = onConnectionStatus,
        .ready = onReady,
        .self_info = onSelfInfo,
        .friend_list = onFriendList,
        .friend_info = onFriendInfo,
        .friend_presence = onFriendPresence,
        .friend_request = onFriendRequest,
        .friend_response = onFriendResponse,
        .friend_added = onFriendAdded,
        .friend_removed = onFriendRemoved,
        .friend_message = onFriendMessage,
        .friend_invite = NULL,
    };

    mIsDummy = cfg->isDummy();

    whisper_log_init((WhisperLogLevel)cfg->logLevel(), cfg->logPath(), logPrint);

    mWhisper = whisper_new(&options, &callbacks, this);
    if (!mWhisper) {
        vlogE("Create whisper instance failed: 0x%x", whisper_get_error());
        return false;
    }

    WhisperSessionOptions wsopts = {
        .transports = WhisperTransportType_ICE,
        .stun_host = cfg->turnHost(),
        .stun_port = NULL,
        .turn_host = cfg->turnHost(),
        .turn_port = NULL,
        .turn_username = cfg->username(),
        .turn_password = cfg->password(),
        .udp_host = NULL,
        .udp_port = NULL,
        .udp_external_host = NULL,
        .udp_external_port = NULL,
        .tcp_host = NULL,
        .tcp_port = NULL,
        .tcp_external_host = NULL,
        .tcp_external_port = NULL
    };

    int rc;

    rc = whisper_session_init(mWhisper, &wsopts, onSessionRequestCallback, this);
    if (rc < 0) {
        vlogE("Initialize session extension failed: 0x%x", whisper_get_error());
        return false;
    }

    return true;
}

void CAgent::run(void)
{
    int rc;

    rc = whisper_run(mWhisper, 50);
    if (rc < 0) {
        vlogE("Start whisper instance failed: 0x%x", whisper_get_error());
        whisper_kill(mWhisper);

    }
    
    vlogI("Whisper instance has been killed");
}

void CAgent::stop(void)
{
    if (mWhisper)
        whisper_kill(mWhisper);
}

void CAgent::handleInput(void)
{
    if (mIsDummy) return;

    if (mInput->readCmd()) {
        std::shared_ptr<CCommand> cmd(mInput->getCmd());
        if (cmd)
            cmd->execute(*this);

        mInput->reset();
    }
}

void CAgent::didConnectionStatusChange(WhisperConnectionStatus status)
{
    bool toRefresh = false;

    switch(status) {
    case WhisperConnectionStatus_Disconnected:
    case WhisperConnectionStatus_Connecting:
    default:
        mIsConnected = false;
        break;

    case WhisperConnectionStatus_Connected:
        if (!mIsConnected) {
            mIsConnected = true;
            toRefresh = true;
        }
        break;
    }

    if (toRefresh)
        refreshPeerGadgets();
}

void CAgent::didReady(void)
{
    refreshPeerGadgets();
}

void CAgent::showMe() const
{
    char nodeid[WHISPER_MAX_ID_LEN + 1];

    vlogI("myid: %s", whisper_get_nodeid(mWhisper, nodeid, sizeof(nodeid)));

    std::map<std::string, std::shared_ptr<CGadget>>::const_iterator it;
    vlogI("gadgets listed:");
    for (it = mGadgets.begin(); it != mGadgets.end(); ++it) {
        vlogI("  %s\t%s", it->first.c_str(), it->second->value().c_str());
    }
}

void CAgent::addPeer(const std::string &name, std::shared_ptr<CFriend> friendz,
                     bool sync)
{
    if (!friendz) return;

    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(name);
    if (it != mPeers.end())
        it->second.updateFriend(friendz);
    else
        mPeers.insert(std::pair<std::string, CPeer>(name, CPeer(friendz)));

    if (sync)
        refreshPeerGadgets(name);
}

void CAgent::delPeer(const std::string &name)
{
    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(name);
    if (it != mPeers.end())
        mPeers.erase(it);
}

void CAgent::updatePeer(const std::string &name, std::shared_ptr<CFriend> friendz)
{
    if (!friendz) return;

    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(name);
    if (it != mPeers.end()) {
        it->second.updateFriend(friendz);
        refreshPeerGadgets(name);
    }
}

void CAgent::updatePeer(const std::string &name, std::shared_ptr<std::string> presence)
{
    if (!presence) return;

    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(name);
    if (it != mPeers.end()) {
        it->second.getFriend()->presence(presence);

        if (presence->compare("online") == 0)
            refreshPeerGadgets(name);
    }
}

void CAgent::addSession(const std::string &name, std::shared_ptr<CSession> sess)
{
    if (!sess) return;

    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(name);
    if (it != mPeers.end())
        it->second.addSession(sess);
}

void CAgent::reqAddPeer(const std::string &name) const
{
    int rc;

    rc = whisper_friend_request(mWhisper, name.c_str(), "hello");
    if (rc < 0) {
        vlogE("Friend request to %s error (%x)", name.c_str(),
              whisper_get_error());
    }
}

void CAgent::listPeers(bool withGadgets) const
{
    std::map<std::string, CPeer>::const_iterator it;
    for (it = mPeers.begin(); it != mPeers.end(); ++it) {
        vlogI("Device %s", it->first.c_str());
    }
}

std::shared_ptr<CGadget> CAgent::getGadget(const std::string &name) const
{
    std::map<std::string, std::shared_ptr<CGadget>>::const_iterator it;
    it = mGadgets.find(name);
    if (it == mGadgets.end())
        return nullptr;
    else
        return it->second;
}

std::shared_ptr<CGadget> CAgent::getGadget(const std::string &peerName,
                                           const std::string &gadgetName) const
{
    std::map<std::string, CPeer>::const_iterator it;
    it = mPeers.find(peerName);
    if (it == mPeers.end())
        return nullptr;
    else
        return it->second.getGadget(gadgetName);
}

void CAgent::addGadget(std::shared_ptr<CGadget> gadget)
{
    if (!gadget) return;

    std::map<std::string, std::shared_ptr<CGadget>>::iterator it;
    it = mGadgets.find(gadget->name());
    if (it != mGadgets.end())
        it->second = gadget;
    else
        mGadgets[gadget->name()] = gadget;
}

void CAgent::didGadgetValueChange(const CGadget &gadget) const
{
    if (!mIsConnected) return;

    Json::StyledWriter writer;
    Json::Value node;
    Json::Value root;

    node = Json::Value("sync");
    root["type"] = node;

    switch(gadget.value().type()) {
    case Int:
        node = Json::Value(gadget.value().iValue());
        break;
    case Bool:
        node = Json::Value(gadget.value().bValue());
        break;
    case Float:
        node = Json::Value(gadget.value().fValue());
        break;
    default:
        return;
    }
    root[gadget.name()] = node;

    std::string msg;
    int rc;

    msg = writer.write(root);
    vlogI("message: %s", msg.c_str());

    std::map<std::string, CPeer>::const_iterator it;
    for (it = mPeers.begin(); it != mPeers.end(); ++it) {
        rc = whisper_send_friend_message(mWhisper, it->first.c_str(),
                msg.c_str(), msg.length() + 1);
        if (rc < 0) {
            vlogE("Broadcast gadget (%s) update value to peer (%s) error (0x%x)",
                  gadget.name().c_str(), it->first.c_str(),
                  whisper_get_error());
        }
    }
}

void CAgent::didGadgetValueChange(const CGadget &gadget, const std::string &peerName) const
{
    if (!mIsConnected) return;

    Json::StyledWriter writer;
    Json::Value node;
    Json::Value root;

    node = Json::Value("modify");
    root["type"] = node;

    switch(gadget.value().type()) {
    case Int:
        node = Json::Value(gadget.value().iValue());
        break;
    case Bool:
        node = Json::Value(gadget.value().bValue());
        break;
    case Float:
        node = Json::Value(gadget.value().fValue());
        break;
    default:
        return;
    }
    root[gadget.name()] = node;

    std::string msg;
    int rc;

    msg = writer.write(root);
    vlogI("message: %s", msg.c_str());

    rc = whisper_send_friend_message(mWhisper, peerName.c_str(),
                                     msg.c_str(), msg.length() + 1);
    if (rc < 0) {
        vlogE("Update peer (%s) gadget (%s) value to be %s error (0x%x)",
              peerName.c_str(), gadget.name().c_str(), gadget.value().c_str(),
              whisper_get_error());
    }
}

void CAgent::handleQuery(const std::string &peerName) const
{
    std::map<std::string, GadgetValue> values;
    std::map<std::string, std::shared_ptr<CGadget>>::const_iterator it;

    for (it = mGadgets.begin(); it != mGadgets.end(); ++it)
        it->second->query(values);

    Json::StyledWriter writer;
    Json::Value node;
    Json::Value root;

    root["type"] = Json::Value("status");

    std::map<std::string, GadgetValue>::const_iterator it1;
    for (it1 = values.begin(); it1 != values.end(); ++it1) {
        switch(it1->second.type()) {
        case Int:
            node = Json::Value(it1->second.iValue());
            break;
        case Bool:
            node = Json::Value(it1->second.bValue());
            break;
        case Float:
            node = Json::Value(it1->second.fValue());
            break;
        default:
            break;
        }

        if (!node.empty())
            root[it1->first] = node;
    }

    std::string msg;
    int rc;

    msg = writer.write(root);
    vlogI("msg: %s", msg.c_str());

    rc = whisper_send_friend_message(mWhisper, peerName.c_str(),
                msg.c_str(), msg.length() + 1);
    if (rc < 0) {
        vlogE("Send local gadgets values to peer (%s) error (0x%x)",
              peerName.c_str(), whisper_get_error());
    }
}

void CAgent::handleStatus(const std::string &peerName,
                          const std::map<std::string, GadgetValue> &values)
{
    if (mIsDummy) return;

    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(peerName);
    if (it == mPeers.end())
        return;

    std::map<std::string, GadgetValue>::const_iterator it1;
    for (it1 = values.begin(); it1 != values.end(); ++it1) {
        std::shared_ptr<CGadget> sp(it->second.getGadget(it1->first));
        if (sp)
            sp->sync(it1->second);
        else
            it->second.addGadget(this, it1->first, it1->second);
    }
}

void CAgent::handleSync(const std::string &peerName,
                          const std::map<std::string, GadgetValue> &values)
{
    if (mIsDummy) return;
    
    std::map<std::string, CPeer>::iterator it;
    it = mPeers.find(peerName);
    if (it == mPeers.end())
        return;

    std::map<std::string, GadgetValue>::const_iterator it1;
    for (it1 = values.begin(); it1 != values.end(); ++it1) {
        std::shared_ptr<CGadget> sp(it->second.getGadget(it1->first));
        if (sp)
            sp->sync(it1->second);
        else
            it->second.addGadget(this, it1->first, it1->second);
    }
}

void CAgent::handleModify(const std::string &peerName,
                          const std::map<std::string, GadgetValue> &values)
{
    std::map<std::string, GadgetValue>::const_iterator it;
    std::map<std::string, std::shared_ptr<CGadget>>::iterator it1;

    //TODO:

    for (it = values.begin(); it != values.end(); ++it) {
        it1 = mGadgets.find(it->first);
        if (it1 != mGadgets.end())
            it1->second->flip(it->second);
    }
}

void CAgent::refreshPeerGadgets(const std::string &peerName) const
{
    Json::StyledWriter writer;
    Json::Value root;

    root["type"] = Json::Value("query");;

    std::string msg;
    int rc;

    msg = writer.write(root);
    vlogI("Send message: %s", msg.c_str());

    rc = whisper_send_friend_message(mWhisper, peerName.c_str(),
                msg.c_str(), msg.length() + 1);
    if (rc < 0) {
        vlogE("Request to get peer (%s) gadgets value error (0x%x)",
              peerName.c_str(), whisper_get_error());
    }
}

void CAgent::refreshPeerGadgets(void) const
{
    if (mIsDummy) return;

    std::map<std::string, CPeer>::const_iterator it;
    for (it = mPeers.begin(); it != mPeers.end(); ++it)
        refreshPeerGadgets(it->first);
}

void CAgent::sendVideoFrame(const uint8_t *frame, int len)
{
    //vlogI("CAgent:%s (len:%d)", __FUNCTION__, len);

    std::map<std::string, CPeer>::const_iterator it;

    for (it = mPeers.begin(); it != mPeers.end(); ++it) {
        std::shared_ptr<CSession> sess = it->second.getSession();
        if (sess)
            sess->write(frame, len);
    }
}
