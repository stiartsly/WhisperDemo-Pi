#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <memory>
#include <vector>
#include <string>

class CAgent;

class CCommand {
protected:
    CCommand(const char *name): mName(name) {}

public:
    virtual void execute(CAgent &agent) const = 0;
    virtual void help(void) const = 0;

    const std::string &name(void) const { return mName; }

private:
    const std::string mName;
};

class CBulbCmd: public CCommand {
public:
    CBulbCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("bulb") {}

public:
    virtual void execute(CAgent &agent) const override;
    virtual void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CTorchCmd: public CCommand {
public:
    CTorchCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("torch") {}

public:
    void execute(CAgent& agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CBrightnessCmd: public CCommand {
public:
    CBrightnessCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("brightness") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CRingCmd: public CCommand {
public:
    CRingCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("ring") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CVolumeCmd: public CCommand {
public:
    CVolumeCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("volume") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string>& mArgv;
};

class CCameraCmd: public CCommand {
public:
    CCameraCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("camera") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CFrequestCmd: public CCommand {
public:
    CFrequestCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("frequest") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CFriendsCmd: public CCommand {
public:
    CFriendsCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("friends") {}

public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

class CMeCmd: public CCommand {
public:
    CMeCmd(const std::vector<std::string> &argv):
        mArgv(argv), CCommand("me") {}
public:
    void execute(CAgent &agent) const override;
    void help(void) const override;

private:
    const std::vector<std::string> &mArgv;
};

#endif /* __COMMAND_H__ */
