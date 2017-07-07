#ifndef __SESSION_H__
#define __SESSION_H__

#include <memory>
#include <string>

#include "whisper.h"
#include "whisper_session.h"

class CSession {
public:
    CSession(std::shared_ptr<std::string> to, std::shared_ptr<std::string> sdp)
        :mSession(NULL), mStream(-1),
         mTo(to), mSdp(sdp) {}

    ~CSession();

public:
    const char *getTo (void) const { return mTo->c_str(); }
    const char *getSdp(void) const { return mSdp->c_str(); }
    void stream(int stream) { mStream = stream; }

    bool start(Whisper *whisper);
    void close(void);

    void write(const void *buf, size_t len);

private:
    WhisperSession *mSession;
    int mStream;

    std::shared_ptr<std::string> mTo;
    std::shared_ptr<std::string> mSdp;
};

#endif
