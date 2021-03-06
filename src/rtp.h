#ifndef __RTPSTREAM_H__
#define __RTPSTREAM_H__

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <array>

#include "agent.h"

namespace nalu {

struct NaluUnit {
    int length;
    int forbidden_bit;
    int nal_rfc_idsc;
    int nal_unit_type;
    uint8_t* data;
};

int readNalu(const uint8_t* data, int length, int offset, nalu::NaluUnit& nalu);

}

const int maxRtpMtu = 1500;
const int maxPktMtu = 1400;

class CRtp{
public:
    CRtp(CAgent *agent): mAgent(agent) {}
    ~CRtp() {}

    int streamFwd(const uint8_t*, int, uint32_t);

private:
    uint8_t mOutbuf[::maxRtpMtu];
    CAgent *mAgent;
};

#endif

