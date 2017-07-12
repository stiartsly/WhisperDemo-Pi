#include <memory>
#include <string>
#include <dlfcn.h>

#include "vlog.h"
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
    if (mAgent->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mAgent->dylib(), "matrix_open");
        return (func() == 0);
    } else {
        return true;
    }
}

void CTorch::flip(bool on)
{
    if (mAgent->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mAgent->dylib(), "matrix_flip");
        (void)func();
    } else {
        vlogI("Torch turned %s", on ? "on": "off");
    }
}

void CTorch::close(void)
{
    if (mAgent->dylib()) {
        void (*func)() = NULL;

        func = (void (*)())dlsym(mAgent->dylib(), "matrix_close");
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

bool CCamera::open(void)
{
    if (mAgent->dylib() != NULL) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mAgent->dylib(), "camera_open");
        return (func() == 0);
    } else {
        return true;
    }
}

void CCamera::flip(bool on)
{
    if (mAgent->dylib()) {
        int (*func)() = NULL;

        func = (int (*)())dlsym(mAgent->dylib(), "camera_flip");
        (void)func();
    } else {
        vlogI("camera turned %s", on ? "on": "off");
    }
}

void CCamera::close(void)
{
    if (mAgent->dylib()) {
        void (*func)() = NULL;

        func = (void (*)())dlsym(mAgent->dylib(), "camera_close");
        func();
    }
}
