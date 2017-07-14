#include <cstdio>
#include <cstring>
#include <memory>
#include <string>
#include <cassert>
#include <dlfcn.h>

#include "vlog.h"
#include "confuse.h"
#include "cfg.h"

static
void configError(cfg_t *cfg, const char *fmt, va_list ap)
{
    fprintf(stderr, "Config file error, line %d: ", cfg->line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
}

static
int notNullValidator(cfg_t *cfg, cfg_opt_t *opt)
{
    if (cfg_getstr(cfg, opt->name) == NULL) {
        cfg_error(cfg, "option '%s' missing.", opt->name);
        return -1;
    }

    return 0;
}

static
int transportValidator(cfg_t *cfg, cfg_opt_t *opt)
{
    //TODO;
    return 0;
}

static
int hostValidator(cfg_t *cfg, cfg_opt_t *opt)
{
    //TODO:
    return 0;
}

static
std::shared_ptr<std::string> getString(cfg_t *cfg, const char *name)
{
    assert(cfg);

    char *val;

    val = cfg_getstr(cfg, name);
    if (!val)
        return nullptr;

    std::string *sval;
    sval = new std::string(val);
    if (!sval) {
        vlogE("Out of memory!!!");
        return nullptr;
    }

    return std::shared_ptr<std::string>(sval);
}

static
int getInt(cfg_t *cfg, const char *name)
{
    return (int)cfg_getint(cfg, name);
}

CConfig::~CConfig()
{
    if (mCfg)
        cfg_free(mCfg);

    if (mDylib)
        dlclose(mDylib);
}

bool CConfig::load(const char *cfgFile)
{
    cfg_opt_t transportOpts[] = {
        CFG_STR("server", NULL, CFGF_NONE),
        CFG_STR("username", NULL, CFGF_NONE),
        CFG_STR("password", NULL, CFGF_NONE),
        CFG_END()
    };

    cfg_opt_t cameraOpts[] = {
        CFG_INT("port", 12300, CFGF_NONE),
        CFG_INT("width", 320, CFGF_NONE),
        CFG_INT("height", 480, CFGF_NONE),
        CFG_STR("profile", "baseline", CFGF_NONE),
        CFG_INT("bitrate", 150000, CFGF_NONE),
        CFG_INT("framerate", 30, CFGF_NONE),
        CFG_INT("intra", 10, CFGF_NONE),
        CFG_END()
    };

    cfg_opt_t hostOpts[] = {
        CFG_SEC("camera", cameraOpts, CFGF_NONE),
        CFG_END()
    };

    cfg_opt_t cfgOpts[] = {
        CFG_STR("apiserver", NULL, CFGF_NONE),
        CFG_STR("mqttserver", NULL, CFGF_NONE),
        CFG_STR("truststore", "whisper.pem", CFGF_NONE),
        CFG_STR("appid", NULL, CFGF_NONE),
        CFG_STR("appkey", NULL, CFGF_NONE),
        CFG_INT("loglevel", 3, CFGF_NONE),
        CFG_STR("logpath", NULL, CFGF_NONE),
        CFG_STR("datadir", NULL, CFGF_NONE),
        CFG_SEC("transport", transportOpts, CFGF_TITLE | CFGF_MULTI | CFGF_NO_TITLE_DUPES),
        CFG_SEC("runhost", hostOpts, CFGF_TITLE | CFGF_MULTI | CFGF_NO_TITLE_DUPES),
        CFG_END()
    };
    int rc;

    mCfg = cfg_init(cfgOpts, CFGF_NONE);
    cfg_set_error_function(mCfg, configError);
    cfg_set_validate_func(mCfg, NULL, notNullValidator);
    cfg_set_validate_func(mCfg, "transport", transportValidator);
    cfg_set_validate_func(mCfg, "runhost", hostValidator);

    rc = cfg_parse(mCfg, cfgFile);
    if (rc != CFG_SUCCESS) {
        vlogE("Parse conf %s error, please check it", cfgFile);
        return false;
    }

    mAppId = getString(mCfg, "appid");
    if (!mAppId) {
        vlogE("Missing appid");
        return false;
    }

    mAppKey = getString(mCfg, "appkey");
    if (!mAppKey) {
        vlogE("Missing appkey");
        return false;
    }

    mApiServer = getString(mCfg, "apiserver");
    if (!mApiServer) {
        vlogE("Missing apiserver");
        return false;
    }

    mMqttServer = getString(mCfg, "mqttserver");
    if (!mMqttServer) {
        vlogE("Missing mqttserver");
        return false;
    }

    if (mMqttServer->compare(0, 6, "ssl://") == 0) {
        mTrustStore = getString(mCfg, "truststore");
        if (!mTrustStore) {
            vlogE("Missing truststore");
            return false;
        }
    } else {
        mTrustStore = nullptr;
    }

    mLogLevel = getInt(mCfg, "loglevel");

    mLogFile = getString(mCfg, "logpath");
    if (!mLogFile) {
        vlogE("Missing logpath");
        return false;
    }

    mDataDir = getString(mCfg, "datadir");
    if (!mDataDir) {
        vlogE("Missing datadir");
        return false;
    }

    cfg_t *sec;
    sec = cfg_getsec(mCfg, "transport");
    if (!sec) {
        vlogE("Missing transport");
        return false;
    }

    const char* title;
    title = cfg_title(sec);
    if (!title || strcmp(title, "ice") != 0) {
        vlogE("Invalid transport type %s", title);
        return false;
    }

    mTurnServer = getString(sec, "server");
    if (!mTurnServer) {
        vlogE("Missing turn server");
        return false;
    }

    mUsername = getString(sec, "username");
    if (!mUsername) {
        vlogE("Missing turn username");
        return false;
    }

    mPassword = getString(sec, "password");
    if (!mPassword) {
        vlogE("Missing turn password");
        return false;
    }

    const char *dylibName;

    sec = cfg_getsec(mCfg, "runhost");
    if (!sec) {
        mDummy = false;
        dylibName = NULL;
    } else {
        title = cfg_title(sec);

        if (!title || strcmp(title, "raspberry") != 0) {
            vlogE("Invalid run host %s", title);
            return false;
        }

        sec = cfg_getsec(sec, "camera");
        if (!sec) {
            vlogE("Missing runhost.camera");
            return false;
        }

        mCameraPort = getInt(sec, "port");
        mWidthRes = getInt(sec, "width");
        mHeightRes = getInt(sec, "height");
        mBitRate = getInt(sec, "bitrate");
        mFrameRate = getInt(sec, "framerate");

        mProfile = getString(sec, "profile");
        if (!mProfile) {
            vlogE("Missing profile");
            return false;
        }

        mDummy = true;
        dylibName = "libraspi.so";
    }

    if (dylibName) {
        mDylib = dlopen(dylibName, RTLD_NOW);
        if (!mDylib) {
            vlogE("Loading dynamic library %s error: %s", dylibName, dlerror());
            return false;
        }
    }

    cfg_free(mCfg);
    mCfg = NULL;

    return true;
}

void CConfig::dump(void) const
{
    vlogI(" dump config content:\n"
          "  +++++++++++++++++++++++\n"
          "       appId: %s\n"
          "      appKey: %s\n"
          "   apiServer: %s\n"
          "  mqttServer: %s\n"
          "  trustStore: %s\n"
          "     dataDir: %s\n"
          "    logLevel: %d\n"
          "     logFile: %s\n"
          " turn server: %s\n"
          "    usernmae: %s\n"
          "    password: %s\n"
          " cmaera port: %d\n"
          "   widht res: %d\n"
          "  height res: %d\n"
          "     bitrate: %d\n"
          "  frame rate: %d\n"
          "     profile: %s\n"
          "       dummy: %s\n"
          " +++++++++++++++++++++++\n",
          mAppId ? mAppId->c_str(): "none",
          mAppKey ? mAppKey->c_str(): "none",
          mApiServer ? mApiServer->c_str(): "none",
          mMqttServer ? mMqttServer->c_str(): "none",
          mTrustStore ? mTrustStore->c_str() : "none",
          mDataDir ? mDataDir->c_str(): "none",
          mLogLevel,
          mLogFile ? mLogFile->c_str() : "none",
          mTurnServer ? mTurnServer->c_str(): "none",
          mUsername ? mUsername->c_str(): "none",
          mPassword ? mPassword->c_str(): "none",
          mCameraPort,
          mWidthRes,
          mHeightRes,
          mBitRate,
          mFrameRate,
          mProfile ? mProfile->c_str(): "none",
          mDummy ? "yes" : "no");
}
