#include <memory>
#include <string>
#include <dlfcn.h>

#include <sys/time.h>

#include "vlog.h"
#include "rtp.h"
#include "agent.h"
#include "gadget.h"

bool GadgetValue::operator ==(const GadgetValue &val) const
{
    if (mType != val.mType)
        return false;

    bool isEqual;

    switch(mType) {
    case Int:
        isEqual = (mValue.i == val.iValue());
        break;

    case Bool:
        isEqual = (mValue.b == val.bValue());
        break;

    case Float:
    default:
        isEqual = false;
        break;
    }

    return isEqual;
}

const char *GadgetValue::c_str(void) const
{
    static char value[32];

    switch(mType) {
    case Int:
        sprintf(value, "%d", mValue.i);
        break;

    case Bool:
        sprintf(value, "%s", mValue.b ? "on": "off");
        break;

    case Float:
        sprintf(value, "%f", mValue.f);
        break;

    default:
        sprintf(value, "unknown");
        break;
    }
    return value;
}

void GadgetValue::value(const GadgetValue &newVal)
{
    mType = newVal.type();

    switch(mType) {
    case Int:
        mValue.i = newVal.iValue();
        break;

    case Bool:
        mValue.b = newVal.bValue();
        break;

    case Float:
        mValue.f = newVal.fValue();
        break;

    default:
        break;
    }
}

void CGadget::flip(const GadgetValue &value)
{
    if (mValue == value)
        return;

    mValue = value;
    switch(mValue.type()) {
    case Int:
        flip(mValue.iValue());
        break;
    case Bool:
        flip(mValue.bValue());
        break;
    case Float:
        flip(mValue.fValue());
        break;
    default:
        return;
    }

    mAgent->didGadgetValueChange(*this);
}

void CGadget::flip(const GadgetValue &value, const std::string &peerName)
{
    if (mValue == value)
        return;

    mValue = value;
    switch(mValue.type()) {
    case Int:
        flip(mValue.iValue());
        break;
    case Bool:
        flip(mValue.bValue());
        break;
    case Float:
        flip(mValue.fValue());
        break;
    default:
        return;
    }

    mAgent->didGadgetValueChange(*this, peerName);
}

void CGadget::sync(const GadgetValue &value)
{
    mValue = value;
}

void CGadget::status(void) const
{
    vlogI("%s %s", mName.c_str(), mValue.c_str());
}

void CGadget::status(const std::string &peerName) const
{
    vlogI("%s %s %s", mName.c_str(), peerName.c_str(), mValue.c_str());
}

bool CBulb::open(void)
{
    //TODO;
    return true;
}

void CBulb::flip(bool on)
{
    vlogI("Blub turned %s", on ? "on" : "off");
}

void CBulb::close(void)
{
    //TODO;
}

bool CTorch::open(void)
{
    if (mCfg && mCfg->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mCfg->dylib(), "matrix_open");
        if (func)
            return (func() == 0);
        else {
            vlogE("Can not find func symbol matrix_open");
            return false;
        }
    } else {
        return true;
    }
}

void CTorch::flip(bool on)
{
    if (mCfg && mCfg->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mCfg->dylib(), "matrix_flip");
        if (func)
           func();
        else {
            vlogE("Can not find func symbol matrix_flip");
            exit(-1);
        }
    } else {
        vlogI("Torch turned %s", on ? "on": "off");
    }
}

void CTorch::close(void)
{
    if (mCfg && mCfg->dylib()) {
        void (*func)() = NULL;

        func = (void (*)())dlsym(mCfg->dylib(), "matrix_close");
        if (func)
            func();
    }
}

bool CBrightness::open(void)
{
    //TODO;
    return true;
}

void CBrightness::flip(const float& value)
{
    vlogI("Brightness changed to %f", value);
}

void CBrightness::close(void)
{
    //TODO;
}

bool CRing::open(void)
{
    //TODO;
    return true;
}

void CRing::flip(bool on)
{
    vlogI("Ring turned %s", on ? "on": "off");
}

void CRing::close(void)
{
    //TODO;
}

bool CVolume::open(void)
{
    //TODO;
    return true;
}

void CVolume::flip(const float& value)
{
    vlogI("Volume changed to be: %f", value);
}

void CVolume::close(void)
{
    //TODO;
}

static
void streamFwd(void *data, int len, void *argv)
{
    CRtp *rtp = static_cast<CRtp*>(argv);

    timeval now;
    gettimeofday(&now, NULL);

    uint32_t ts = (uint32_t)(now.tv_sec * 1000 + now.tv_usec/1000);

    rtp->streamFwd((const uint8_t*)data, len, ts);
}

bool CCamera::open(void)
{
    mRtp = std::shared_ptr<CRtp>(new CRtp(mAgent));
    if (!mRtp) {
        vlogE("Out of memory!!!");
        return false;
    }

    if (mCfg && mCfg->dylib()) {
        typedef void (*CameraCbsPtr)(void *, void *);
        CameraCbsPtr f1;

        f1 = (CameraCbsPtr)dlsym(mCfg->dylib(), "camera_set_callbacks");
        if (f1)
            f1((void *)streamFwd, mRtp.get());
        else {
            vlogE("Can not find symbol camera_set_callbacks");
            return false;
        }

        typedef void (*CameraPortPtr)(int);
        CameraPortPtr f2;

        f2 = (CameraPortPtr)dlsym(mCfg->dylib(), "camera_set_port");
        if (f2)
            f2(mCfg->cameraPort());
        else {
            vlogE("Can not find symbol camera_set_port");
            return false;
        }

        typedef void (*CameraParamsPtr)(int, int, int, int, int);
        CameraParamsPtr f3;

        f3 = (CameraParamsPtr)dlsym(mCfg->dylib(), "camera_set_parameters");
        if (f3) {
            f3(mCfg->widthRes(), mCfg->heightRes(), mCfg->bitRate(),
                 mCfg->frameRate(), mCfg->profile());
        } else {
            vlogE("Can not find symbol camera_set_paramters");
            return false;
        }

        typedef int (*CameraOpenPtr)(void);
        CameraOpenPtr f4 = NULL;
        f4 = (CameraOpenPtr)dlsym(mCfg->dylib(), "camera_open");
        if (f4)
            return (f4() == 0);
        else {
            vlogE("Can not find func symbol camera_open");
            return false;
        }
    } else {
        return true;
    }
}

void CCamera::flip(bool on)
{
    if (mCfg && mCfg->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mCfg->dylib(), "camera_flip");
        if (func)
            func();
        else {
            vlogE("Can not find func symbol camera_flip");
            exit(-1);
        }
    } else {
        vlogI("camera turned %s", on ? "on": "off");
    }
}

void CCamera::close(void)
{
    if (mCfg && mCfg->dylib()) {
        void (*func)() = NULL;

        func = (void (*)())dlsym(mCfg->dylib(), "camera_close");
        if (func)
            func();
    }
}
