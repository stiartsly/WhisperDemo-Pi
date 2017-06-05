#ifndef __FRIEND_H__
#define __FRIEND_H__

#include <memory>
#include <string>
#include "user.h"

class CFriend: public CUser {
public:
    CFriend(const WhisperFriendInfo *info): CUser(&info->user_info),
        mEntrusted(info->entrusted),
        mLabel(new std::string(info->label)),
        mPresence(new std::string(info->presence)) {}

    CFriend(const WhisperUserInfo *info): CUser(info),
        mEntrusted(0),
        mLabel(NULL),
        mPresence(NULL) {}

    ~CFriend() {}
    
public:
    int entrustment(void) const {
        return mEntrusted;
    }

    const std::shared_ptr<std::string> label(void) const {
        return mLabel;
    }

    const std::shared_ptr<std::string> presence(void) const {
        return mPresence;
    }

    void label(const std::shared_ptr<std::string> newValue) {
        if (newValue.get())
            mLabel = newValue;
    }

    void presence(const std::shared_ptr<std::string> newValue) {
        if (newValue.get())
            mPresence = newValue;
    }

private:
    int mEntrusted;

    std::shared_ptr<std::string> mLabel;
    std::shared_ptr<std::string> mPresence;
};

#endif /* __FRIEND_H__ */
