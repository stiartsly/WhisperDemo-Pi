#include <cstring>
#include <cstdint>

#include "vlog.h"
#include "session.h"

class state2str {
public:
    state2str(WhisperStreamState state) {
        mState = state;
    }

    operator const char*() const {
        const char *_str;

        switch(mState) {
        case WhisperStreamState_initialized:
            _str = "Initialized";
            break;
        case WhisperStreamState_transport_ready:
            _str = "Transport ready";
            break;
        case WhisperStreamState_connecting:
            _str = "Connecting...";
            break;
        case WhisperStreamState_connected:
            _str = "Connected";
            break;
        case WhisperStreamState_deactivated:
            _str = "Deactivated";
            break;
        case WhisperStreamState_closed:
            _str = "Closed";
            break;
        case WhisperStreamState_failed:
        default:
            _str = "Failed";
            break;
        }
        return _str;
    }

private:
    WhisperStreamState mState;
};

CSession::~CSession()
{
    close();
}

static
void onStreamStateChanged(WhisperSession *session, int stream,
                          WhisperStreamState state, void *context)
{
    CSession *sess = static_cast<CSession*>(context);
    int rc;

    vlogD("Stream %s state changed to %s", state2str(state));

    if (state == WhisperStreamState_initialized) {
        rc = whisper_session_reply_request(session, 0, NULL);
        if (rc < 0) {
            vlogE("Reply session request from %s error: 0x%x", sess->getTo(),
                whisper_get_error());

            sess->close();
            return;
        }
    } else if (state == WhisperStreamState_transport_ready) {
        const char *sdp = sess->getSdp();
        whisper_session_start(session, sdp, strlen(sdp));
        vlogI("Session with %s started", sess->getTo());
    } else if (state == WhisperStreamState_connected) {
        vlogI("Stream with %s connected", sess->getTo());
        sess->stream(stream);
    } else if (state == WhisperStreamState_deactivated) {
        vlogE("Stream with %s not matched, error");
        sess->close();
    } else if (state == WhisperStreamState_closed ||
        state == WhisperStreamState_failed) {
        vlogI("Session with %s closed by remote", sess->getTo());
        sess->close();
    }
}

bool CSession::start(Whisper *whisper)
{
    mSession = whisper_session_new(whisper, mTo->c_str(), WhisperTransportType_ICE, NULL);
    if (!mSession) {
        vlogE("New whisper session to %s error: 0x%x", mTo->c_str(),
            whisper_get_error());
        return false;
    }

    WhisperStreamCallbacks callbacks = {
        .state_changed   = onStreamStateChanged,
        .stream_data     = NULL,
        .channel_open    = NULL,
        .channel_opened  = NULL,
        .channel_close   = NULL,
        .channel_data    = NULL,
        .channel_pending = NULL,
        .channel_resume  = NULL
    };
    int rc;

    rc = whisper_session_add_stream(mSession, WhisperStreamType_application, 0,
            &callbacks, this);
    if (rc < 0) {
        vlogE("Add whisper stream error: 0x%x", whisper_get_error());
        whisper_session_close(mSession);
        mSession = NULL;
        return false;
    }

    vlogI("Add whisper stream %d successs", rc);

    return true;
}

void CSession::close() {
    if (mSession) {
        whisper_session_close(mSession);
        mSession = NULL;
        mStream = -1;
    }
}

void CSession::write(const uint8_t *data, size_t len)
{
    if (mStream < 0) {
        //vlogE("Session not prepared for writing data");
        return;
    }

    ssize_t rc;

    rc = whisper_stream_write(mSession, mStream, data, len);
    if (rc < 0)
        vlogE("Write data to stream %d error: 0x%x", mStream,
            whisper_get_error());
}
