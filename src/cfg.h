#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <memory>
#include <string>

#include "confuse.h"

class CConfig {
public:
    ~CConfig();

public:
    bool load(const char *cfgFile);
    void dump(void) const;

public:
    const char *appId(void) const {
        return mAppId->c_str();
    }

    const char *appKey(void) const {
        return mAppKey->c_str();
    }

    const char *apiServer(void) const {
        return mApiServer->c_str();
    }

    const char *mqttServer(void) const {
        return mMqttServer->c_str();
    }

    const char *trustStore(void) const {
        return mTrustStore ? mTrustStore->c_str() : NULL;
    }

    const char *dataDir(void) const {
        return mDataDir->c_str();
    }

    int getLogLevel(void) const {
        return mLogLevel;
    }

    const char *logPath(void) const {
        return mLogFile->c_str();
    }

    const char *turnHost(void) const {
        return mTurnServer->c_str();
    }

    const char *username(void) const {
        return mUsername->c_str();
    }

    const char *password(void) const {
        return mPassword->c_str();
    }

    int cameraPort(void) const {
        return mCameraPort;
    }

    int widthRes(void) const {
        return mWidthRes;
    }

    int heightRes(void) const {
        return mHeightRes;
    }

    int bitRate(void) const {
        return mBitRate;
    }

    int frameRate(void) const {
        return mFrameRate;
    }

    int profile(void) const {
        if (mProfile->compare("high") == 0)
            return 2;
        else if (mProfile->compare("main") == 0)
            return 1;
        else if (mProfile->compare("baseline") == 0)
            return 0;
        else
            return 0;
    }

    bool isDummy(void) const {
        return mDummy;
    }

    void *dylib() const {
        return mDylib;
    }

private:
    std::shared_ptr<std::string> mAppId;
    std::shared_ptr<std::string> mAppKey;
    std::shared_ptr<std::string> mApiServer;
    std::shared_ptr<std::string> mMqttServer;
    std::shared_ptr<std::string> mTrustStore;
    std::shared_ptr<std::string> mDataDir;

    int mLogLevel;
    std::shared_ptr<std::string> mLogFile;

    // turn server related parameters.
    std::shared_ptr<std::string> mTurnServer;
    std::shared_ptr<std::string> mUsername;
    std::shared_ptr<std::string> mPassword;

    // camera related parameters.
    int mCameraPort;
    int mWidthRes;
    int mHeightRes;
    int mBitRate;
    int mFrameRate;
    std::shared_ptr<std::string> mProfile;

    // run host related parameters.
    bool mDummy;
    void *mDylib;

    cfg_t *mCfg;
};

#endif /* __CONFIG_H__ */
