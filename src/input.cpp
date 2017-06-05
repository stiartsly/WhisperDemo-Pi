#include <cstdlib>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include "vlog.h"
#include "cmd.h"
#include "gadget.h"
#include "input.h"

inline bool isLF(int ch)
{
    return (ch == 0x0A);
}

inline bool isCR(int ch)
{
    return (ch == 0x0D);
}

bool CInput::setup(void)
{
    int fd;
    int rc;

    fd = fileno(stdin);
    rc = fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
    if (rc < 0)
        vlogE("Setup input error");

    return (rc != -1);
}

bool CInput::readCmd(void)
{
    int ch;

    do {
        ch = std::getchar();

        if (isLF(ch) || isCR(ch)) {
            if (mPos > 0) {
                mCmdArgv.push_back(std::string(mTmpBuf.data()));
                mTmpBuf.fill('\0');
                mPos = 0;
            }
            return true;
        }

        if (isspace(ch)) {
            if (mPos > 0) {
                mCmdArgv.push_back(std::string(mTmpBuf.data()));
                mTmpBuf.fill('\0');
                mPos = 0;
            }
            continue;
        }

        if (isprint(ch)) {
            if (mPos + 1 == LINEBUF_SZ)
                vlogW("Temprary input buffer full, skipped input");
            else
                mTmpBuf[mPos++] = (char)ch;

            continue;
        }

    } while (ch != EOF);

    return false;
}

std::shared_ptr<CCommand> CInput::getCmd(void) const
{
    if (mCmdArgv.empty())
        return nullptr;

    CCommand *cmd = NULL;

    if (mCmdArgv[0].compare("bulb") == 0)
        cmd = new CBulbCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("torch") == 0)
        cmd = new CTorchCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("brightness") == 0)
        cmd = new CBrightnessCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("ring") == 0)
        cmd = new CRingCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("volume") == 0)
        cmd = new CVolumeCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("camera") == 0)
        cmd = new CCameraCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("frequest") == 0)
        cmd = new CFrequestCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("friends") == 0)
        cmd = new CFriendsCmd(mCmdArgv);
    else if (mCmdArgv[0].compare("me") == 0)
        cmd = new CMeCmd(mCmdArgv);
    else {
        vlogE("Invalid command, skipped");
        return nullptr;
    }

    return std::shared_ptr<CCommand>(cmd);
}

void CInput::reset(void)
{
    mPos = 0;
    mTmpBuf.fill('\0');
    mCmdArgv.clear();
}
