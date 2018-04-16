#ifndef __FRIEND_H__
#define __FRIEND_H__

#include <memory>
#include <string>
#include "user.h"
#include <whisper.h>

class CFriend: public CUser {
public:
    CFriend(const WhisperFriendInfo *info): CUser(&info->user_info),
        mEntrusted(info->entrusted),
        mLabel(new std::string(info->label)),
        mPresence(info->presence),
        mStatus(info->status) {}

    CFriend(const WhisperUserInfo *info): CUser(info),
        mEntrusted(0),
        mLabel(NULL),
        mPresence(WhisperPresenceStatus_None),
        mStatus(WhisperConnectionStatus_Disconnected) {}

    ~CFriend() {}
    
public:
    int entrustment(void) const {
        return mEntrusted;
    }

    const std::shared_ptr<std::string> label(void) const {
        return mLabel;
    }

    WhisperPresenceStatus presence(void) const {
        return mPresence;
    }

    WhisperConnectionStatus status(void) const {
        return mStatus;
    }

    void label(const std::shared_ptr<std::string> newValue) {
        if (newValue.get())
            mLabel = newValue;
    }

    void presence(WhisperPresenceStatus presence) {
        mPresence = presence;
    }

    void status(WhisperConnectionStatus status) {
        mStatus = status;
    }

private:
    int mEntrusted;
    WhisperPresenceStatus mPresence;
    WhisperConnectionStatus mStatus;
    std::shared_ptr<std::string> mLabel;
};

#endif /* __FRIEND_H__ */
