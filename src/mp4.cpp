#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include <unistd.h>
#include <sys/time.h>

#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#endif
#include <mp4v2/mp4v2.h>
#if defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#include "vlog.h"
#include "mp4.h"
#include "rtp.h"

void getMp4MediaInfo(std::string videoFile, std::string& mediaInfo)
{
    char* info = MP4FileInfo(videoFile.c_str());
    mediaInfo = info;
    free(info);
    return ;
}

CMp4Encoder::CMp4Encoder(const std::string& videoPath,
            int width,
            int height,
            uint32_t timescale = 90000,
            uint32_t frameRate = 25)
          : mVideoPath(videoPath),
            mWidth(width),
            mHeight(height),
            mVideoTimescale(timescale),
            mVideoFrameRate(frameRate),
            mVideoTrackId(0),
            mMp4File(NULL),
            mSpsSet(0),
            mBpsSet(0)
{
    vlogI("CMp4Encoder constructor called");
}

CMp4Encoder::~CMp4Encoder()
{
    vlogI("CMp4Encoder destructor called");
}

int CMp4Encoder::open(void)
{
    if (mVideoPath.empty())
        return -1;

    mMp4File = MP4Create(mVideoPath.c_str());
    if (!mMp4File) {
        vlogE("Mp4Create (%s) error", mVideoPath.c_str());
        return -1;
    }

    bool rc = MP4SetTimeScale(mMp4File, mVideoTimescale);
    if (!rc) {
        vlogE("MP4SetTimeScale error");
        MP4Close(mMp4File);
        return -1;
    }
    return 0;
}

int CMp4Encoder::streamIn(const uint8_t* data, int length)
{
    if (!mMp4File || length <= 0)
        return -1;

    nalu::NaluUnit nalu;
    int off = 0;
    int len = 0;

    len = nalu::readNalu(data, length, off, nalu);
    while(len > 0) {
        if (nalu.nal_unit_type == 0x07 && mSpsSet) {
            mVideoTrackId = MP4AddH264VideoTrack(mMp4File,
                                        mVideoTimescale,
                                        mVideoTimescale/mVideoFrameRate,
                                        mWidth,
                                        mHeight,
                                        nalu.data[1],
                                        nalu.data[2],
                                        nalu.data[3],
                                        3);
            if (!mVideoTrackId) {
               vlogE("MP4AddH264VideoTrack error");
               return -1;
            }
            MP4SetVideoProfileLevel(mMp4File, 1);
            MP4AddH264SequenceParameterSet(mMp4File, mVideoTrackId, nalu.data, nalu.length);
            mSpsSet = 1;

        } else if (nalu.nal_unit_type == 0x08 && mBpsSet) {
            MP4AddH264PictureParameterSet(mMp4File, mVideoTrackId, nalu.data, nalu.length);
            mBpsSet = 1;
        } else {
            if (!mSpsSet)
                continue;

            uint8_t* tmp = (uint8_t*)calloc(1, nalu.length + 4);
            if (!tmp) {
                vlogE("CMp4Encoder::streamIn::calloc error");
                return -1;
            }
            tmp[0] = nalu.length >> 24;
            tmp[1] = nalu.length >> 16;
            tmp[2] = nalu.length >> 8;
            tmp[3] = nalu.length & 0xff;

            memcpy(tmp + 4, nalu.data, nalu.length);
            int rc = MP4WriteSample(mMp4File, mVideoTrackId, tmp, nalu.length + 4, -1, 0, 1);
            free(tmp);
            if (!rc) {
                vlogE("Mp4WriteSample error");
                return -1;
            }
        }
        off += len;
    }
    return off;
}

CMp4Decoder::CMp4Decoder(const std::shared_ptr<CRtpStream>& rtp, const std::string& videoPath)
    : mRtp(rtp),
      mVideoPath(videoPath)
{
    reset();
    vlogI("CMp4Decoder constructor called");
}

CMp4Decoder::~CMp4Decoder()
{
    close();
    vlogI("CMp4Decoder destructor called");
}

void CMp4Decoder::reset()
{
    mMp4File = NULL;

    mVideoTrackId  = 0;
    mAudioTrackId  = 0;
    mnVideoSamples = 0;
    mCurVideoSampleId = 0;

    mVideoDuration = -1;
    mVideoTimeScale = 0;
    mVideoLength = 0;
    mVideoFps = 0;
    mInterval = 0;

    mPause = 0;
    mStop  = 0;
}

void CMp4Decoder::dump(void) const
{
    vlogI(" MP4 file infos:\n");
    vlogI(" Num of video samples(%d)\n", mnVideoSamples);
    vlogI(" video Duration(%llu)\n", mVideoDuration);
    vlogI(" video Time Scale(%d)\n", mVideoTimeScale);
    vlogI(" video Length(%ds)\n", mVideoLength);
    vlogI(" FPS(%d)\n", mVideoFps);
}

int CMp4Decoder::open(void)
{
    if (mVideoPath.empty())
        return -1;

    mMp4File = MP4Read(mVideoPath.c_str());
    if (!mMp4File)
        vlogE("Open mp4 file (%s) error", mVideoPath.c_str());

    const char* type = NULL;
    uint32_t num = 0;

    num = MP4GetNumberOfTracks(mMp4File);
    for (uint32_t i = 0; i < num; ++i) {
        type = MP4GetTrackType(mMp4File, i);
        if (MP4_IS_VIDEO_TRACK_TYPE(type)) {
            mVideoTrackId = i;
            break;
        }

    }
    if (!mVideoTrackId) {
        vlogE("Video track not found in Mp4 file, error");
        MP4Close(mMp4File);
        return -1;
    }

    mVideoDuration  = MP4GetTrackDuration(mMp4File, mVideoTrackId);
    mVideoTimeScale = MP4GetTrackTimeScale(mMp4File, mVideoTrackId);
    mnVideoSamples  = MP4GetTrackNumberOfSamples(mMp4File, mVideoTrackId);
    mVideoFps       = MP4GetTrackVideoFrameRate(mMp4File, mVideoTrackId);

    if (mVideoTimeScale > 0) {
        mVideoLength = (uint32_t)(mVideoDuration / mVideoTimeScale);
    } else {
        vlogE("The time scale of track is 0, error");
        MP4Close(mMp4File);
        return -1;
    }

    if (mVideoFps > 0) {
        mInterval = 1000000 / mVideoFps;
    } else {
        vlogE("Video fps is 0, error");
        MP4Close(mMp4File);
        return -1;
    }
    dump();
    return 0;
}

void CMp4Decoder::close(void)
{
    if (!mMp4File)
        MP4Close(mMp4File);
}

int CMp4Decoder::seek(uint64_t seekPos)
{
    if (seekPos > mVideoLength)
        return -1;

    mPause = 1;
    mCurVideoSampleId = (uint32_t)(mnVideoSamples * (seekPos / mVideoLength));
    if (mCurVideoSampleId == 0)
        mCurVideoSampleId = 1;

    mPause = 0;
    return 0;
}

int CMp4Decoder::streamOut(void)
{
    uint8_t** seqHeader   = NULL;
    uint8_t** pictHeader  = NULL;
    uint32_t* seqHeaderSz = NULL;
    uint32_t* pictHeaderSz= NULL;

    bool rc = MP4GetTrackH264SeqPictHeaders(mMp4File, mVideoTrackId,
                                          &seqHeader,
                                          &seqHeaderSz,
                                          &pictHeader,
                                          &pictHeaderSz);
    if (!rc) {
        vlogE("MP4GetTrackH264SeqPictHeaders error");
        return -1;
    }

    uint8_t naluStartCode[] ={0x0, 0x0, 0x0, 0x1};
    std::vector<uint8_t> sps(naluStartCode, naluStartCode + 4);

    for (int i = 0; seqHeaderSz[i] != 0; ++i) {
        sps.insert(sps.end() + 1, seqHeader[i], seqHeader[i] + seqHeaderSz[i]);
        free(seqHeader[i]);
    }
    free(seqHeader);
    free(seqHeaderSz);

    struct timeval now;
    gettimeofday(&now, NULL);

//    mRtp->streamOut(sps.data, sps.size(), now.tv_sec*1000 + now.tv_usec/1000);
    usleep(mInterval);
    sps.clear();

    std::vector<uint8_t> pps(naluStartCode, naluStartCode + 4);
    for (int i = 0; pictHeaderSz[i] != 0; ++i) {
        pps.insert(pps.end() + 1, pictHeader[i], pictHeader[i] + pictHeaderSz[i]);
        free(pictHeader[i]);
    }
    free(pictHeader);
    free(pictHeaderSz);

    gettimeofday(&now, NULL);
    //mRtp->streamOut(pps.data, sps.size(), now.tv_sec*1000 + now.tv_usec/1000);
    usleep(mInterval);
    pps.clear();

    MP4Timestamp ts = 0;
    MP4Duration dur = 0;
    uint8_t* sample = NULL;
    uint32_t sampleSz = 0;

    for (; mCurVideoSampleId <= mnVideoSamples; ++mCurVideoSampleId) {
        rc = MP4ReadSample(mMp4File, mVideoTrackId, mCurVideoSampleId, &sample, &sampleSz, &ts, &dur);
        if (!rc) {
            vlogE("MP4ReadSample to %u sample error", mCurVideoSampleId);
            break;
        }

        memcpy(sample, naluStartCode, sizeof(naluStartCode));
        gettimeofday(&now, NULL);
        //mRtp->streamOut(sample, sampleSz, now.tv_sec*1000 + now.tv_usec/1000);
        usleep(mInterval);

        if (sample) {
            free(sample);
            sample = NULL;
        }

        while(mPause)
            usleep(500);

        if (mStop)
            break;
    }

    mStop = 1;
    return 0;
}

void CMp4Decoder::pause(int pauseFlag)
{
    mPause = (!!pauseFlag);
}

void CMp4Decoder::stop(void)
{
    mStop = 1;
}

int CMp4Decoder::isStreaming(void) const
{
    return (mStop == 1);
}

