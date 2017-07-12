#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <cstdint>
#include <cstddef>
#include <cassert>
#include <memory>
#include <string>
#include <map>

#include "whisper.h"
#include "gadget.h"
#include "session.h"

class CConfig;
class CUser;
class CFriend;
class CInput;

class CPeer {
public:
    CPeer(std::shared_ptr<CFriend> friendz): mFriend(friendz), mGadgets() {}

public:
    std::shared_ptr<CFriend> getFriend(void) const { return mFriend; }
    void updateFriend(std::shared_ptr<CFriend> f) { mFriend = f; }

    std::shared_ptr<CGadget> getGadget(const std::string &name) const;
    void addGadget(CAgent* agent, const std::string &name, const GadgetValue &value);

    void addSession(std::shared_ptr<CSession> session) { mSession = session; }

    std::shared_ptr<CSession> getSession(void) const { return mSession; }

private:
    std::shared_ptr<CFriend> mFriend;
    std::map<std::string, std::shared_ptr<CGadget>> mGadgets;
    std::shared_ptr<CSession> mSession;
};

class CAgent {
public:
    explicit CAgent(std::shared_ptr<CInput> input):
        mDylib(NULL),
        mWhisper(NULL),
        mIsConnected(false),
        mUser(NULL),
        mInput(input),
        mPeers(),
        mGadgets() {}

    ~CAgent();

public:
    bool setup(std::shared_ptr<CConfig> cfg);
    void run(void);
    void stop(void);

    void handleInput(void);

    void *dylib(void) { return mDylib; }

    void didConnectionStatusChange(WhisperConnectionStatus status);
    void didReady(void);

    // update itself.
    void updateSelf(std::shared_ptr<CUser> me) { mUser = me; }
    void showMe(void) const;

    // about Peers.
    void addPeer(const std::string &name, std::shared_ptr<CFriend>, bool sync = true);
    void delPeer(const std::string &name);
    void updatePeer(const std::string &name, std::shared_ptr<CFriend>);
    void updatePeer(const std::string &name, std::shared_ptr<std::string>);

    // about session.
    void addSession(const std::string &name, std::shared_ptr<CSession>);

    void reqAddPeer(const std::string &name) const;
    void listPeers(bool withGadget = false) const;

    // about gadgets.
    std::shared_ptr<CGadget> getGadget(const std::string &) const;
    std::shared_ptr<CGadget> getGadget(const std::string &, const std::string&) const;
    void addGadget(std::shared_ptr<CGadget> gadget);

    void didGadgetValueChange(const CGadget &gadget) const;
    void didGadgetValueChange(const CGadget &gadget, const std::string&) const;

    void handleQuery(const std::string &peerName) const;
    void handleStatus(const std::string &peerName, const std::map<std::string, GadgetValue>&);
    void handleSync(const std::string &peerName, const std::map<std::string, GadgetValue>&);
    void handleModify(const std::string &peerName, const std::map<std::string, GadgetValue>&);

    void sendVideoFrame(const uint8_t*, int);

private:
    void refreshPeerGadgets(const std::string &peerName) const;
    void refreshPeerGadgets(void) const;
private:
    void *mDylib;

    Whisper *mWhisper;

    bool mIsConnected;
    bool mIsDummy;

    std::shared_ptr<CUser> mUser;

    std::shared_ptr<CInput> mInput;
    std::map<std::string, CPeer> mPeers;
    std::map<std::string, std::shared_ptr<CGadget>> mGadgets;
};

#endif /* __AGENT_H__ */
