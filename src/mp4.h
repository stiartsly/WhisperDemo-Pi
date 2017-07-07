#ifndef __MP4_CODEC_H__
#define __MP4_CODEC_H__

#include <cstdint>
#include <cstdlib>
#include <string>
#include <memory>

#include <mp4v2/mp4v2.h>
#include "rtp.h"
#include "mp4.h"

void getMp4MediaInfo(std::string videoFile, std::string& mediaInfo);

class CMp4Encoder
{
public:
    CMp4Encoder(const std::string& videoPath, int, int, uint32_t, uint32_t);
    ~CMp4Encoder();

    int open(void);
    int streamIn(const uint8_t* data, int length);

private:
    std::string mVideoPath;
    MP4FileHandle mMp4File;

    int mWidth;
    int mHeight;
    uint32_t mVideoTrackId;
    uint32_t mVideoTimescale;
    uint32_t mVideoFrameRate;

    int mSpsSet;
    int mBpsSet;
};

class CMp4Decoder
{
public:
    CMp4Decoder(const std::shared_ptr<CRtpStream>& rtp, const std::string& videoPath);
    ~CMp4Decoder();

    int  open(void);
    int  streamOut(void);

    void stop (void);
    void pause(int);
    int  seek (uint64_t seekPos);
    int  isStreaming(void) const;

private:
    void reset(void);
    void close(void);
    void dump (void) const;

private:
    MP4FileHandle mMp4File;

    uint32_t mVideoTrackId;
    uint32_t mAudioTrackId;
    uint32_t mnVideoSamples;
    uint32_t mCurVideoSampleId;

    MP4Duration mVideoDuration;
    uint32_t mVideoTimeScale;
    uint32_t mVideoLength;
    uint32_t mVideoFps;
    uint32_t mInterval;

    std::string mVideoPath;
    std::shared_ptr<CRtpStream>  mRtp;

    int mStop;
    int mPause;
};

#endif

