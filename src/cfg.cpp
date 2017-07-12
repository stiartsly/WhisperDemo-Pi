#include <memory>
#include <string>
#include "confuse.h"
#include "vlog.h"
#include "cfg.h"

static
void print_cfg_error(cfg_t *cfg, const char *fmt, va_list ap)
{
    //TODO;
}

std::shared_ptr<std::string> CConfig::getString(void* cfg, const char* name)
{
    char *cVal = cfg_getstr((cfg_t *)cfg, name);
    if (!cVal)
        return nullptr;

    std::string* sVal = new std::string(cVal);
    if(!sVal)
        return nullptr;

    return std::shared_ptr<std::string>(sVal);
}

int CConfig::getInt(void *cfg, const char* name)
{
    return (int)cfg_getint((cfg_t *)cfg, name);
}

bool CConfig::load(const std::string &cfgFile)
{
    cfg_opt_t cfg_opts[] = {
        CFG_STR("apiserver", NULL, CFGF_NONE),
        CFG_STR("mqttserver", NULL, CFGF_NONE),                                                      
        CFG_STR("truststore", "whisper.pem", CFGF_NONE),                                             
        CFG_STR("appid", NULL, CFGF_NONE),
        CFG_STR("appkey", NULL, CFGF_NONE),
        CFG_STR("turn.server", NULL, CFGF_NONE),                                                     
        CFG_STR("turn.username", NULL, CFGF_NONE),                                                   
        CFG_STR("turn.password", NULL, CFGF_NONE),
        CFG_INT("log_level", 4, CFGF_NONE),
        CFG_STR("log_file", NULL, CFGF_NONE),
        CFG_STR("data_location", NULL, CFGF_NONE),
        CFG_STR("deviceid", NULL, CFGF_NONE),
        CFG_STR("run_host", NULL, CFGF_NONE),
        CFG_END()                                                                                    
    };
    cfg_t* cfg;
    int rc;

    cfg = cfg_init(cfg_opts, CFGF_NONE);
    cfg_set_error_function(cfg, print_cfg_error);

    rc = cfg_parse(cfg, cfgFile.c_str());
    if (rc != CFG_SUCCESS) {
        cfg_free(cfg);
        return false;
    }

    mAppId = getString(cfg, "appid");
    mAppKey = getString(cfg, "appkey");
    mApiServer = getString(cfg, "apiserver");
    mMqttServer = getString(cfg, "mqttserver");
    mTrustStore = getString(cfg, "truststore");
    mTurnServer = getString(cfg, "turn.server");
    mUsername = getString(cfg, "turn.username");
    mPassword = getString(cfg, "turn.password");
    mPersistentLocation = getString(cfg, "data_location");
    mDeviceId = getString(cfg, "deviceid");
    mLogLevel = getInt(cfg, "log_level");
    mLogFile = getString(cfg, "log_file");

    std::shared_ptr<std::string> runHost;
    runHost = getString(cfg, "run_host");

    cfg_free(cfg);

    if (runHost) {
        if (runHost->compare("raspberrypi") == 0) {
            mDummy = true;
            mDylibName = std::shared_ptr<std::string>(new std::string("wmdemo_raspi.so"));
            if (!mDylibName)
                return false;
        }
        else if (runHost->compare("macos") == 0) {
            mDummy = false;
            mDylibName = std::shared_ptr<std::string>(new std::string("wmdemo_macos.so"));
            if (!mDylibName)
                return false;
        }
        else {
            return false;
        }
    }
    else {
        mDummy = false;
        mDylibName = nullptr;
    }

    printf("<%s> --->", __FUNCTION__);

    return (mAppId && mAppKey && mApiServer
        && mMqttServer && mTrustStore
        && mTurnServer && mUsername && mPassword
        && mPersistentLocation);
}
