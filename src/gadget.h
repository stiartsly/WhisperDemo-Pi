#ifndef __GADGET_H__
#define __GADGET_H__

#include <cstdbool>
#include <memory>
#include <string>
#include <map>

#include "cfg.h"

class CRtp;
class CAgent;

enum GadgetValueTypes {
    Int,
    Bool,
    Float
};

struct GadgetValue {
public:
    GadgetValue(int val): mType(Int) { mValue.i = val; }
    GadgetValue(bool val): mType(Bool) { mValue.b = val; }
    GadgetValue(float val): mType(Float) { mValue.f = val; }

    GadgetValue()  = default;
    ~GadgetValue() = default;

public:
    bool operator ==(const GadgetValue&) const;
    const char *c_str(void) const;

    void value(const GadgetValue &newValue);

    int iValue(void) const { return mValue.i; }
    bool bValue(void) const { return mValue.b; }

    float fValue(void) const { return mValue.f; }
    GadgetValueTypes type(void) const { return mType; }

private:
    union {
        int i;
        bool b;
        float f;
    } mValue;
    GadgetValueTypes mType;
};

class CGadget {
protected:
    CGadget(const char *name, CAgent *agent, int val):
        mName(name), mAgent(agent), mValue(val) {}

    CGadget(const char *name, CAgent *agent, bool val):
        mName(name), mAgent(agent), mValue(val) {}

    CGadget(const char *name, CAgent *agent, float &val):
        mName(name), mAgent(agent), mValue(val) {}

public:
    const std::string &name(void) const { return mName; }
    const GadgetValue &value(void) const { return mValue; }

    virtual bool open(void) { return true; }
    virtual void close(void) { }

    void query(std::map<std::string, GadgetValue> &result) const {
        result[mName] = mValue;
    }

    void query(GadgetValue &value) const {
        value = mValue;
    }

    void flip(const GadgetValue&);
    void flip(const GadgetValue&, const std::string&);

    void sync(const GadgetValue&);

    void status(void) const;
    void status(const std::string&) const;

protected:
    virtual void flip(int val) {}
    virtual void flip(bool val) {}
    virtual void flip(const float &val) {}

protected:
    CAgent *mAgent;

private:
    std::string mName;
    GadgetValue mValue;
};

class CBulb: public CGadget {
public:
    CBulb(CAgent *agent, bool val): CGadget("bulb", agent, val) {}

protected:
    bool open(void) override;
    void flip(bool on) override;
    void close(void) override;
};

class CTorch: public CGadget {
public:
    CTorch(std::shared_ptr<CConfig> cfg, CAgent *agent, bool val):
        mCfg(cfg), CGadget("torch", agent, val) {}

protected:
    bool open(void) override;
    void flip(bool on) override;
    void close(void) override;

private:
    std::shared_ptr<CConfig> mCfg;
};

class CBrightness: public CGadget {
public:
    CBrightness(CAgent *agent, float val): CGadget("brightness", agent, val) {}

protected:
    bool open(void) override;
    void flip(const float& value) override;
    void close(void) override;
};

class CRing: public CGadget {
public:
    CRing(CAgent *agent, bool val): CGadget("ring", agent, val) {}

protected:
    bool open(void) override;
    void flip(bool on) override;
    void close(void) override;
};

class CVolume: public CGadget {
public:
    CVolume(CAgent *agent, float val): CGadget("volume", agent, val) {}

protected:
    bool open(void) override;
    void flip(const float& value) override;
    void close(void) override;
};

class CCamera: public CGadget {
public:
    CCamera(std::shared_ptr<CConfig> cfg, CAgent *agent, bool val):
        mCfg(cfg), CGadget("camera", agent, val) {}

protected:
    bool open(void) override;
    void flip(bool on) override;
    void close(void) override;

private:
    std::shared_ptr<CConfig> mCfg;
    std::shared_ptr<CRtp> mRtp;
};

#endif /* __GADGET_H__*/
