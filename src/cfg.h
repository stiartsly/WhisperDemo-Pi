#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <memory>
#include <string>

class CConfig {
public:
    bool load(const std::string &cfgFile);

public:
    int logLevel(void) const {
        return mLogLevel;
    }

    const char *logPath(void) const {
        return mLogFile->c_str();
    }

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
        return mTrustStore->c_str();
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

    const char *persistentLocation(void) const {
        return mPersistentLocation->c_str();
    }

    const char *deviceId(void) const {
        return mDeviceId->c_str();
    }

    bool isDummy(void) const {
        return mDummy;
    }

    const char *dylibName(void) const {
        return mDylibName ? mDylibName->c_str() : nullptr;
    }

private:
    std::shared_ptr<std::string> getString(void*, const char* name);
    int getInt(void*, const char* name);
    
private:
    std::shared_ptr<std::string> mAppId;
    std::shared_ptr<std::string> mAppKey;
    std::shared_ptr<std::string> mApiServer;
    std::shared_ptr<std::string> mMqttServer;
    std::shared_ptr<std::string> mTrustStore;
    std::shared_ptr<std::string> mTurnServer;
    std::shared_ptr<std::string> mUsername;
    std::shared_ptr<std::string> mPassword;

    std::shared_ptr<std::string> mPersistentLocation;
    std::shared_ptr<std::string> mDeviceId;

    int mLogLevel;

    std::shared_ptr<std::string> mLogFile;
    std::shared_ptr<std::string> mCfgFile;

    bool mDummy;
    std::shared_ptr<std::string> mDylibName;
};

#endif /* __CONFIG_H__ */
