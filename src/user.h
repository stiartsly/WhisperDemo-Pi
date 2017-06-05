#ifndef __USER_H__
#define __USER_H__

#include <memory>
#include <string>
#include "whisper.h"

class CUser {
public:
    CUser(const WhisperUserInfo *info): mHasAvatar(false),
        mUserId(info->userid),
        mName(info->name),
        mDescr(info->description),
        mGender(info->gender),
        mPhone(info->phone),
        mEmail(info->email),
        mRegion(info->region) {}

    ~CUser() {}

public:
    bool hasAvatar(void) const {
        return mHasAvatar;
    }

    const std::string& userid(void) const {
        return mUserId;
    }

    const std::string& name(void) const {
        return mName;
    }

    const std::string& descr(void) const {
        return mDescr;
    }

    const std::string& gender(void) const {
        return mGender;
    }

    const std::string& phone(void) const {
        return mPhone;
    }

    const std::string& email(void) const {
        return mEmail;
    }

    const std::string& region(void) const {
        return mRegion;
    }

private:
    bool mHasAvatar;

    std::string mUserId;
    std::string mName;
    std::string mDescr;
    std::string mGender;
    std::string mPhone;
    std::string mEmail;
    std::string mRegion;
};

#endif /* __USER_H__ */
