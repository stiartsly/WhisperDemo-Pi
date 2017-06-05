#ifndef __INPUT_H__
#define __INPUT_H__

#include <memory>
#include <vector>
#include <array>
#include <string>

const int LINEBUF_SZ = 128;

class CCommand;

class CInput {
public:
    CInput() { reset(); }
    ~CInput() = default;

public:
    bool setup(void);

    bool readCmd(void);
    std::shared_ptr<CCommand> getCmd(void) const;
    void reset(void);

private:
    int mPos;
    std::array<char, LINEBUF_SZ> mTmpBuf;
    std::vector<std::string> mCmdArgv;
};

#endif /* __INPUT_H__ */
