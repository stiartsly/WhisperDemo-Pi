#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>
#include <memory>
#include <string>
#include "whisper.h"
#include "vlog.h"
#include "gadget.h"
#include "agent.h"
#include "cmd.h"

class ArgvParser {
public:
    ArgvParser(const std::vector<std::string>& argv): mArgv(argv), mValue() {}

    bool parse(void);

    enum CmdType {
        GetLocal,
        GetPeer,
        SetLocal,
        SetPeer,
    };
    CmdType cmdType(void) const { return mType; }

    const GadgetValue& value(void) const { return mValue; }
    const char *peerName(void) const { return mPeerId; }
    void dump(void) const;

private:
    CmdType mType;
    GadgetValue mValue;
    const char *mPeerId;
    const std::vector<std::string>& mArgv;
};

bool ArgvParser::parse(void)
{
    bool valid = true;
    int i = 0;

    while (i < mArgv.size()) {
        switch(i) {
        case 0:
            mType = GetLocal;
            break;

        case 1:
            if (mArgv[i].compare("me") == 0) {
                mType = GetLocal;
            } else if (whisper_id_is_valid(mArgv[i].c_str())) {
                mType = GetPeer;
                mPeerId = mArgv[1].c_str();
            } else {
                valid = false;
            }
            break;

        case 2:
            mType = (mType == GetLocal) ? SetLocal : SetPeer;

            if (mArgv[i].compare("on") == 0) {
                mValue = GadgetValue(true);
            } else if (mArgv[i].compare("off") == 0) {
                mValue = GadgetValue(false);
            } else {
                const char *str = mArgv[i].c_str();
                char* endPtr = NULL;
                float fv = 0;

                fv = std::strtof(str, &endPtr);
                if (endPtr != str + std::strlen(str))
                    valid = false;

                mValue = GadgetValue(fv);
            }
            break;

        default:
            valid = false;
        }

        if (!valid)
            break;
        else
            ++i;
    }

    return valid;
}

void ArgvParser::dump(void) const
{
    switch(mType) {
    case GetLocal:
        vlogI(">> bulb me");
        break;
    case GetPeer:
        vlogI(">> bulb %s", mPeerId);
        break;
    case SetLocal:
        vlogI(">> bulb me %s", mValue.c_str());
        break;
    case SetPeer:
        vlogI(">> bulb %s %s", mPeerId, mValue.c_str());
        break;
    default:
        break;
    }
}

static
void execCmd(const char *name, ArgvParser &parser, CAgent &agent)
{
    std::string gadget(name);

    switch(parser.cmdType()) {
    case ArgvParser::GetLocal:
        agent.getGadget(gadget)->status();
        break;

    case ArgvParser::GetPeer:
        agent.getGadget(parser.peerName(), gadget)->status(parser.peerName());
        break;

    case ArgvParser::SetLocal:
        agent.getGadget(gadget)->flip(parser.value());
        break;

    case ArgvParser::SetPeer:
        agent.getGadget(parser.peerName(), gadget)->flip(parser.value(), parser.peerName());
        break;

    default:
        break;
    }
}

void CBulbCmd::execute(CAgent &agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("bulb", ap, agent);
}

void CBulbCmd::help(void) const
{
    vlogI("bulb [ me | userid ] [ on | off ]");
}

void CTorchCmd::execute(CAgent &agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("torch", ap, agent);
}

void CTorchCmd::help(void) const
{
    vlogI("torch [ me | userid ] [ on | off ]");
}

void CBrightnessCmd::execute(CAgent& agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("brightness", ap, agent);
}

void CBrightnessCmd::help(void) const
{
    vlogI("brightness [ me | userid ] [ value ]");
}

void CRingCmd::execute(CAgent &agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("ring", ap, agent);
}

void CRingCmd::help(void) const
{
    vlogI("ring [ me | userid ] [ on | off ]");
}

void CVolumeCmd::execute(CAgent &agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("volume", ap, agent);
}

void CVolumeCmd::help(void) const
{
    vlogI("volume [ me | userid ] [value]");
}

void CCameraCmd::execute(CAgent &agent) const
{
    ArgvParser ap(mArgv);
    if (!ap.parse()) {
        vlogE("Invalid command syntax");
        return;
    }

    execCmd("camera", ap, agent);
}

void CCameraCmd::help(void) const
{
    vlogI("camera [ me | userid ] [ on | off ]");
}

void CFrequestCmd::execute(CAgent &agent) const
{
    if (mArgv.size() != 2) {
        vlogI("Invalid command syntax");
        return;
    }

    if (!whisper_id_is_valid(mArgv[1].c_str())) {
        vlogI("Invalid userid");
        return;
    }

    agent.reqAddPeer(mArgv[1]);
}

void CFrequestCmd::help(void) const
{
    vlogI("frequest userid");
}

void CFriendsCmd::execute(CAgent &agent) const
{
    if (mArgv.size() != 1) {
        vlogI("Invalid command syntax");
        return;
    }

    agent.listPeers();
}

void CFriendsCmd::help(void) const
{
    vlogI("friends");
}

void CMeCmd::execute(CAgent &agent) const
{
    if (mArgv.size() != 1) {
        vlogI("Invalid command syntax");
        return;
    }

    agent.showMe();
}

void CMeCmd::help(void) const
{
    vlogI("me");
}

