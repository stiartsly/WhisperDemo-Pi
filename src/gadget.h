#ifndef __GADGET_H__
#define __GADGET_H__

#include <cstdbool>
#include <memory>
#include <string>
#include <map>

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

private:
    std::string mName;
    GadgetValue mValue;
    CAgent *mAgent;
};

class CBulb: public CGadget {
public:
    CBulb(CAgent *agent, bool val): CGadget("bulb", agent, val) {}

protected:
    void flip(bool on) override;
};

class CTorch: public CGadget {
public:
    CTorch(CAgent *agent, bool val): CGadget("torch", agent, val) {}

protected:
    void flip(bool on) override;
};

class CBrightness: public CGadget {
public:
    CBrightness(CAgent *agent, float val): CGadget("brightness", agent, val) {}

protected:
    void flip(const float& value) override;
};

class CRing: public CGadget {
public:
    CRing(CAgent *agent, bool val): CGadget("ring", agent, val) {}

protected:
    void flip(bool on) override;
};

class CVolume: public CGadget {
public:
    CVolume(CAgent *agent, float val): CGadget("volume", agent, val) {}

protected:
    void flip(const float& value) override;
};

class CCamera: public CGadget {
public:
    CCamera(CAgent *agent, bool val): CGadget("camera", agent, val) {}

protected:
    void flip(bool on) override;
};

#endif /* __GADGET_H__*/
