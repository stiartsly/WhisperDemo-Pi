#ifndef __WHISPER_SESSION_H__
#define __WHISPER_SESSION_H__

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>

#include <whisper.h>

#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_IP_STRING_LEN   45

typedef struct WhisperSession WhisperSession;

/**
 * \~English
 * Whisper underlying transport types definitions.
 *
 * \~Chinese
 * Whisper传输层协议类型定义
 */
typedef enum WhisperTransportType {
    /**
     * \~English
     * ICE protocol.
     *
     * \~Chinese
     * ICE 协议。
     */
    WhisperTransportType_ICE = 0x01,
    /**
     * \~English
     * UDP transport protocol.
     *
     * \~Chinese
     * UDP 传输协议。
     */
    WhisperTransportType_UDP = 0x02,
    /**
     * \~English
     * TCP transport protocol.
     *
     * \~Chinese
     * TCP 传输协议。
     */
    WhisperTransportType_TCP = 0x04
} WhisperTransportType;

/**
 * \~English
 * Whisper stream types definition.
 * Reference:
 *      https://tools.ietf.org/html/rfc4566#section-5.14
 *      https://tools.ietf.org/html/rfc4566#section-8
 *
 * \~Chinese
 * Whisper stream 类型定义。
 * 参考如下：
 *      https://tools.ietf.org/html/rfc4566#section-5.14
 *      https://tools.ietf.org/html/rfc4566#section-8
 */
typedef enum WhisperStreamType {
    /**
     * \~English
     *  Audio stream.
     *
     *  \~Chinese
     *  音频流。
     */
    WhisperStreamType_audio = 0,
    /**
     * \~English
     *  Video stream.
     *
     *  \~Chinese
     *  视频流。
     */
    WhisperStreamType_video,
    /**
     * \~English
     *  Text stream.
     *
     *  \~Chinese
     *  文本流。
     */
    WhisperStreamType_text,
    /**
     * \~English
     *  Application stream.
     *
     *  \~Chinese
     *  应用流。
     */
    WhisperStreamType_application,
    /**
     * \~English
     *  Message stream.
     *
     *  \~Chinese
     *  消息流。
     */
    WhisperStreamType_message
} WhisperStreamType;

/**
 * \~English
 * Whisper session initialize options.
 *
 * \~Chinese
 * Whisper session 初始化选项。
 */
typedef struct WhisperSessionOptions {
    /**
     * \~English
     * Underlying transport options.
     *
     * Options can be using ICE, UDP or TCP transport protocols, or
     * combination of the two or all.
     *
     * \~Chinese
     * 传输协议选项
     *
     * 选项可以是使用ICE，UDP 或者TCP等传输协议，或者同时使用二个，甚至全部。
     */
    int transports;
    /**
     * \~English
     * STUN server host name or IP.
     *
     * \~Chinese
     * STUN服务器的主机或者IP。
     */
    const char *stun_host;
    /**
     * \~English
     * STUN server port.
     *
     * The default port is 3478.
     *
     * \~Chinese
     * STUN服务端口。
     *
     * 默认端口port是3478。
     */
    const char *stun_port;
    /**
     * \~English
     * TURN server host name or IP.
     *
     * \~Chinese
     * TURN服务器的主机或者IP。
     */
    const char *turn_host;
    /**
     * \~English
     * TURN server port.
     *
     * The default port is 3478.
     *
     * \~Chinese
     * TURN服务端口。
     *
     * 默认端口port是3478。
     */
    const char *turn_port;
    /**
     * \~English
     * TURN server user name.
     *
     * \~Chinese
     * TURN服务器用户名。
     */
    const char *turn_username;
    /**
     * \~English
     * TURN server password.
     *
     * \~Chinese
     * TURN服务器密码。
     */
    const char *turn_password;
    /**
     * \~English
     * UDP server hostname or ip.
     *
     * Only valid when using UDP protocol.
     *
     * \~Chinese
     * UDP服务器的主机或者IP。
     *
     * 只在使用UDP协议情况有效
     */
    const char *udp_host;
    /**
     * \~English
     * UDP server port.
     *
     * Only valid when using UDP protocol.
     * If set to NULL, the server will running on an random available
     * port.
     *
     * \~Chinese
     * UDP服务的端口。
     *
     * 只在使用UDP协议情况有效。
     * 如果设定为NULL，那么UDP server将运行在一个随机的可用端口。
     */
    const char *udp_port;
    /**
     * \~English
     * UDP server external public address when running behind a NAT, and mapped
     * to public address.
     *
     * Only valid when using UDP protocol behind NAT
     * and mapped to public address.
     *
     * \~Chinese
     * UDP服务的公网地址，适用于运行在NAT之后并且提供和到公网地址映射的情况。
     *
     * 只在使用UDP协议并且提供了NAT公网地址映射的情况有效
     */
    const char *udp_external_host;
    /**
     * \~English
     * UDP server external public port when running behind a NAT, and mapped
     * to public address.
     *
     * Only valid when using UDP protocol behind NAT
     * and mapped to public address
     *
     * \~Chinese
     * UDP服务的公网端口，适用于运行在NAT之后并且提供和到公网地址映射的情况。
     *
     * 只在使用UDP协议并且提供了NAT公网地址映射的情况有效
     */
    const char *udp_external_port;
    /**
     * \~English
     * TCP server hostname or IP.
     *
     * Only valid when using TCP protocol.
     *
     * \~Chinese
     * TCP服务器的主机或者IP。
     *
     * 只在使用TCP协议情况有效
     */
    const char *tcp_host;
    /**
     * \~English
     * TCP server port.
     *
     * Only valid when using TCP protocol.
     * If set to NULL, the server will running on an random available
     * port.
     *
     * \~Chinese
     * TCP服务器的端口
     *
     * 只在使用TCP协议情况有效
     * 如果设定为NULL，那么TCP server将运行在一个随机的可用端口。
     */
    const char *tcp_port;
    /**
     * \~English
     * TCP server external public address when running behind a NAT, and mapped
     * to public address.
     *
     * Only valid when using TCP protocol behind NAT and mapped to 
     * public address.
     *
     * \~Chinese
     * TCP服务器的公网地址，适用于运行在NAT之后并且提供和到公网地址映射的情况。
     *
     * 格式如下：server_name_or_ip[:port]
     * 只在使用TCP协议并且提供了NAT公网地址映射的情况有效
     */
    const char *tcp_external_host;
    /**
     * \~English
     * TCP server external public port when running behind a NAT, and mapped
     * to public address.
     *
     * Only valid when using TCP protocol behind NAT and mapped to
     * public address.
     *
     * \~Chinese
     * TCP服务器的公网端口，适用于运行在NAT之后并且提供和到公网地址映射的情况。
     *
     * 只在使用TCP协议并且提供了NAT公网地址映射的情况有效
     */
    const char *tcp_external_port;
} WhisperSessionOptions;

typedef enum WhisperCandidateType {
    WhisperCandidateType_Host,
    WhisperCandidateType_ServerReflexive,
    WhisperCandidateType_PeerReflexive,
    WhisperCandidateType_Relayed,
} WhisperCandidateType;

typedef enum WhisperNetworkTopology {
    WhisperNetworkTopology_LAN,
    WhisperNetworkTopology_P2P,
    WhisperNetworkTopology_RELAYED,
    WhisperNetWorkTopology_CS
} WhisperNetworkTopology;

typedef struct WhisperAddressInfo {
    WhisperCandidateType type;
    char addr[MAX_IP_STRING_LEN + 1];
    int port;
    char related_addr[MAX_IP_STRING_LEN + 1];
    int related_port;
} WhisperAddressInfo;

typedef struct WhisperTransportInfo {
    WhisperTransportType transport;
    WhisperNetworkTopology topology;
    WhisperAddressInfo local;
    WhisperAddressInfo remote;
} WhisperTransportInfo;

/* Global session APIs */

/**
 * \~English
 * An application-defined function that handle session requests.
 *
 * WhisperSessionRequestCallback is the callback function type.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      from        [in] The id(userid@nodeid) from who send the message.
 * @param
 *      sdp         [in] The remote users SDP. End the null terminal.
 *                       Reference: https://tools.ietf.org/html/rfc4566
 * @param
 *      len         [in] The length of the SDP.
 * @param
 *      context     [in] The application defined context data.
 *
 * \~Chinese
 * 应用定义用于处理session请求的函数。
 *
 * WhisperSessionRequestCallback是回调函数类型。
 *
 * @param
 *      whisper      [输入] 指向Whisper客户实例的句柄。
 * @param
 *      from         [输入] 消息发送者的id（userid@nodeid）
 * @param
 *      sdp          [输入] 对端用户的SDP，以null结尾。
 *                         具体参考：https://tools.ietf.org/html/rfc4566
 * @param
 *      len          [输入] SDP长度。
 * @param
 *      context      [输入] 应用定义的上下文数据。
 */
typedef void WhisperSessionRequestCallback(Whisper *whisper, const char *from,
        const char *sdp, size_t len, void *context);

/**
 * \~English
 * Initialize Whisper session extension.
 *
 * The application must initialize the session extension before calling
 * any session API.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      opts        [in] A pointer to a valid WhisperSessionOptions structure.
 * @param
 *      callback    [in] A pointer to the application-defined function of type
 *                       WhisperSessionRequestCallback.
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 初始化Whisper session扩展。
 *
 * 应用必须在调用session API前初始化session扩展。
 *
 * @param
 *     whisper      [输入] 指向Whisper客户实例的句柄。
 * @param
 *     ops          [输入] 指向一个有效WhisperSessionOptions结构的指针。
 * @param
 *      callback     [输入] 指向应用定义的WhisperSessionRequestCallback类型函数。
 * @param
 *      context      [输入] 应用定义的上下文数据。
 *
 * @return
 *     0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_init(Whisper *whisper, WhisperSessionOptions *opts,
                WhisperSessionRequestCallback *callback, void *context);

/**
 * \~English
 * Clean up Whisper session extension.
 *
 * The application should call whisper_session_cleanup before quit,
 * to clean up the resources associated with the extension.
 *
 * If the extension is not initialized, this function has no effect.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 *
 * \~Chinese
 * 清除Whisper session扩展。
 *
 * 应用必须在退出前调用whisper_session_cleanup，用于清除和扩展相关的资源。
 *
 * @param
 *      whisper     [输入] 指向Whisper客户实例的句柄。
 */
WHISPER_API
void whisper_session_cleanup(Whisper *whisper);

/* Session instance APIs */

/**
 * \~English
 * Create a new session to a friend.
 *
 * The session object represent a conversation handle to a friend.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      to          [in] The target id(userid or userid@nodeid).
 * @param
 *      transport   [in] The transport protocol to use, only can be TCP, UDP
 *                       or ICE exclusively.
 *
 * @return
 *      If no error occurs, return the pointer of WhisperSession object.
 *      Otherwise, return NULL, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 创建一个新session
 *
 * 一个session实例代表一次和friend的会话句柄。
 *
 * @param
 *     whisper      [输入] 指向Whisper客户实例的句柄。
 * @param
 *     to           [输入] 目标id（userid 或者 userid@nodeid）。
 * @param
 *     transport    [输入] 传输协议，目前仅支持ICE，UDP或者TCP。
 *
 * @return
 *     如果成功，返回WishierSession对象的指针，否则返回NULL。
 *     具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
WhisperSession *whisper_session_new(Whisper *whisper, const char *to,
                                    WhisperTransportType transport);

/**
 * \~English
 * Close a session to friend. All resources include streams, multiplexers
 * associated with current session will be destroyed.
 *
 * @param
 *      session     [in] A handle to the Whisper session.
 *
 * \~Chinese
 * 关闭指定session。销毁所有和session有关资源，包括streams和multiplexers。
 *
 * @param
 *      session     [输入] 指向Whisper客户实例的句柄。
 */
WHISPER_API
void whisper_session_close(WhisperSession *session);

WHISPER_API
char *whisper_session_get_peer(WhisperSession *ws, char *peer, size_t size);

WHISPER_API
void whisper_session_set_userdata(WhisperSession *ws, void *userdata);

WHISPER_API
void *whisper_session_get_userdata(WhisperSession *ws);

/**
 * \~English
 * An application-defined function that receive session request complete
 * event.
 *
 * WhisperSessionRequestCompleteCallback is the callback function type.
 *
 * @param
 *      session     [in] A handle to the WhisperSession.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 * @param
 *      sdp         [in] The remote users SDP. End the null terminal.
 *                       Reference: https://tools.ietf.org/html/rfc4566
 * @param
 *      len         [in] The length of the SDP.
 * @param
 *      context     [in] The application defined context data.
 *
 * \~Chinese
 * 应用定义用于接收session请求完成的函数。
 *
 * WhisperSessionRequestCompleteCallback是回调函数类型。
 *
 * @param
 *      session      [输入] 指向Whisper客户实例的句柄。
 * @param
 *      status       [输入] 响应的状态码。
 *                         0表示成功，否则表示失败。
 * @param
 *      reason       [输入] 若状态码表示失败，reason中存放失败的信息；否则为空。
 * @param
 *      sdp          [输入] 对端用户的SDP，以null结尾。
 *                         具体参考：https://tools.ietf.org/html/rfc4566
 * @param
 *      len          [输入] SDP长度。
 * @param
 *      context      [输入] 应用定义的上下文数据。
 */
typedef void WhisperSessionRequestCompleteCallback(WhisperSession *session, int status,
        const char *reason, const char *sdp, size_t len, void *context);

/**
 * \~English
 * Send session request to the friend.
 *
 * @param
 *      session     [in] A handle to the WhisperSession.
 * @param
 *      callback    [in] A pointer to WhisperSessionRequestCompleteCallback
 *                       function to receive the session response.
 * @param
 *      context      [in] The application defined context data.
 *
 * @return
 *      0 if the session request successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 发送session请求。
 *
 * @param
 *     session       [输入] 指向Whisper客户实例的句柄。
 * @param
 *     callback      [输入] 指向接收session响应的WhisperSessionRequestCompleteCallback函数。
 * @param
 *     context       [输入] 应用定义的上下文数据。
 *
 * @return
 *     若session请求发送成功，则返回0。否则，返回-1。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_request(WhisperSession *session,
        WhisperSessionRequestCompleteCallback *callback, void *context);

/**
 * \~English
 * Reply the session request from friend.
 *
 * This function will send a session response to friend.
 *
 * @param
 *      session     [in] A handle to the WhisperSession.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 *                       if success.
 *
 * @return
 *      0 if the session response successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 回复session请求。
 *
 * 函数发送session响应。
 *
 * @param
 *      session      [输入] 指向Whisper客户实例的句柄。
 * @param
 *      status       [输入] 响应的状态码。
 *                         0表示成功，否则表示失败。
 * @param
 *      reason       [输入] 若状态码表示失败，reason中存放失败的信息；否则为空。
 *
 * @return
 *      若session响应发送成功，则返回0。否则，返回-1。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_reply_request(WhisperSession *session, int status,
        const char* reason);

/**
 * \~English
 * Begin to start a session.
 *
 * All streams in current session will try to connect with remote friend,
 * the stream status will update to application by stream's callbacks.
 *
 * @param
 *      session     [in] A handle to the WhisperSession.
 * @param
 *      sdp         [in] The remote users SDP. End the null terminal.
 *                       Reference: https://tools.ietf.org/html/rfc4566
 * @param
 *      len         [in] The length of the SDP.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 开始启动session。
 *
 * 当前session的所有stream都尝试连接对端，相应引起的stream状态更新通过回调传递给应用。
 *
 * @param
 *      session      [输入] 指向WhisperSession的句柄。
 * @param
 *      sdp          [输入] 对端用户的SDP，以null结尾。
 *                         具体参考：https://tools.ietf.org/html/rfc4566
 * @param
 *      len          [输入] SDP长度。
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_start(WhisperSession *session, const char *sdp, size_t len);

/* Session stream APIs */

/**
 * \~English
 * Whisper stream state.
 * The stream status will be changed according to the phase of the stream.
 *
 * \~Chinese
 * Whisper stream状态。
 * stream状态根据stream所处阶段不同而不同。
 */
typedef enum WhisperStreamState {
    /** Initialized stream */
    WhisperStreamState_initialized = 1,
    /** The underlying transport is ready for the stream. */
    WhisperStreamState_transport_ready,
    /** The stream is trying to connecting the remote. */
    WhisperStreamState_connecting,
    /** The stream connected with remote */
    WhisperStreamState_connected,
    /** The stream is deactivated */
    WhisperStreamState_deactivated,
    /** The stream closed normally */
    WhisperStreamState_closed,
    /** The stream is failed, cannot to continue. */
    WhisperStreamState_failed
} WhisperStreamState;

/**
 * \~English
 * Portforwarding supported protocols.
 *
 * \~Chinese
 * Portforwarding支持的协议。
 */
typedef enum PortForwardingProtocol {
    /** UDP protocol. */
    PortForwardingProtocol_UDP = 0,
    /** TCP protocol. */
    PortForwardingProtocol_TCP = 1
} PortForwardingProtocol;

/**
 * \~English
 * Multiplexing channel close reason code.
 *
 * \Chinese
 * 多路复用通道关闭原因。
 */
typedef enum CloseReason {
    /* Channel closed normally. */
    CloseReason_Normal = 0,
    /* Channel closed because timeout. */
    CloseReason_Timeout = 1,
    /* Channel closed because error ocurred. */
    CloseReason_Error = 2
} CloseReason;

/**
 * \~English
 * Whisper stream callbacks.
 *
 * Include stream status callback, stream data callback, and multiplexing
 * callbacks.
 *
 * \~Chinese
 * Whisper stream回调函数组合。
 *
 * 该结构包含回调函数可以更新stream状态，接收stream数据和实现多路复用。
 */
typedef struct WhisperStreamCallbacks {
    /* Common callbacks */
    /**
     * \~English
     * Callback to report status of various stream operations.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      state       [in] Stream state defined in WhisperStreamState.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 该回调提示因不同stream操作引起的状态变化。
     *
     * @param
     *      session     [输入] 指向WhisperSession的句柄。
     * @param
     *      stream      [输入] stream id。
     * @param
     *      state       [输入] Whisper Stream状态。
     * @param
     *      context     [输入] 应用定义的上下文数据。
     */
    void (*state_changed)(WhisperSession *session, int stream,
                          WhisperStreamState state, void *context);

    /* Stream callbacks */
    /**
     * \~English
     * Callback will be called when the stream receives
     * incoming packet.
     *
     * If the stream enabled multiplexing mode, application will not
     * receive stream_data callback any more. All data will reported
     * as multiplexing channel data.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      data        [in] The received packet data.
     * @param
     *      len         [in] The received data length.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 该回调接收对端传输到应用的数据包。
     *
     * 如果stream处于多路复用工作模式，应用将不会通过该回调来接收stream数据。所有数据都将通过
     * 多路复用通道传递给应用。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      data         [输入] 接收到的数据包。
     * @param
     *      len          [输入] 接收到的数据长度。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     */
    void (*stream_data)(WhisperSession *session, int stream,
                        const void *data, size_t len, void *context);

    /* Multiplexer callbacks */
    /**
     * \~English
     * Callback will be called when new multiplexing channel request to open.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      cookie      [in] Application defined string data receive from peer.
     * @param
     *      context     [in] The application defined context data.
     *
     * @return
     *      True on success, or false if an error occurred.
     *      The channel will continue to open only this callback return true,
     *      otherwise the channel will be closed.
     *
     * \~Chinese
     * 应用收到创建一条多路复用请求，调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      cookie       [输入] 对端应用的自定义数据。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     * @return
     *      true表示成功，false表示失败。若返回true， 应用继续创建通道，否则通道关闭。
     */
    bool (*channel_open)(WhisperSession *session, int stream, int channel,
                         const char *cookie, void *context);

    /**
     * \~English
     * Callback will be called when new multiplexing channel opened.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用成功创建一条多路复用通道，调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     */
    void (*channel_opened)(WhisperSession *session, int stream, int channel,
                           void *context);

    /**
     * \~English
     * Callback will be called when channel close.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      reason      [in] Channel close reason code, defined in CloseReason.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用关闭通道，调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      reason       [输入] 关闭通道的原因，原因类型定义在CloseReason。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     */
    void (*channel_close)(WhisperSession *session, int stream, int channel,
                          CloseReason reason, void *context);

    /**
     * \~English
     * Callback will be called when channel received incoming data.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      data        [in] The received data.
     * @param
     *      len         [in] The received data length.
     * @param
     *      context     [in] The application defined context data.
     *
     * @return
     *      True on success, or false if an error occurred.
     *      If this callback return false, the channel will be closed
     *      with CloseReason_Error.
     *
     * \~Chinese
     * 应用接收到对端数据，调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      data         [输入] 接收到的数据包。
     * @param
     *      len          [输入] 接收到的数据长度。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     *
     * @return
     *      true表示成功，false表示失败。若返回false， 通道关闭失败原因保存在CloseReason_Error。
     */
    bool (*channel_data)(WhisperSession *session, int stream, int channel,
                         const void *data, size_t len, void *context);

    /**
     * \~English
     * Callback will be called when remote peer ask to pend data sending.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 接收到对端要求暂停数据发送，应用调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     */
    void (*channel_pending)(WhisperSession *session, int stream, int channel,
                            void *context);

    /**
     * \~English
     * Callback will be called when remote peer ask to resume data sending.
     *
     * @param
     *      session     [in] The handle to the WhisperSession.
     * @param
     *      stream      [in] The stream ID.
     * @param
     *      channel     [in] The current channel ID.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 接收到对端要求回复数据发送，应用调用该函数。
     *
     * @param
     *      session      [输入] 指向WhisperSession的句柄。
     * @param
     *      stream       [输入] stream id。
     * @param
     *      channel      [输入] 当前channel id。
     * @param
     *      context      [输入] 应用定义的上下文数据。
     */
    void (*channel_resume)(WhisperSession *session, int stream, int channel,
                           void *context);
} WhisperStreamCallbacks;

#define WHISPER_STREAM_COMPRESS             0x01
#define WHISPER_STREAM_ENCRYPT              0x02
#define WHISPER_STREAM_RELIABLE             0x04
#define WHISPER_STREAM_MULTIPLEXING         0x08
#define WHISPER_STREAM_PORT_FORWARDING      0x10

/**
 * \~English
 * Add a new stream to session.
 *
 * Whisper stream supports several underlying transport mechanisms:
 *
 *   - Plain/encrypted UDP data gram protocol
 *   - Plain/encrypted TCP like reliable stream protocol
 *   - Multiplexing over UDP
 *   - Multiplexing over TCP like reliable protocol
 *
 *  Application can use options to specify the new stream mode.
 *  Multiplexing over UDP can not provide reliable transport.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      type        [in] The stream type defined in WhisperStreamType.
 * @param
 *      options     [in] The stream mode options. options are constructed
 *                       by a bitwise-inclusive OR of flags from the
 *                       following list:
 *
 *                       - WHISPER_STREAM_ENCRYPT
 *                         Enctypted mode.
 *                       - WHISPER_STREAM_RELIABLE
 *                         Reliable mode.
 *                       - WHISPER_STREAM_MULTIPLEXING
 *                         Multiplexing mode.
 *                       - WHISPER_STREAM_PORT_FORWARDING
 *                         Support portforwarding over multiplexing.
 *
 * @param
 *      callbacks   [in] The Application defined callback functions in
 *                       WhisperStreamCallbacks.
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      Return stream id on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 指定session中添加stream。
 *
 * Whisper stream支持以下几种传输机制：
 *
 *   - 普通/加密UDP数据报传输
 *   - 普通/加密TCP可靠数据流传输
 *   - 基于UDP的多路复用
 *   - 基于TCP可靠传输的多路复用
 *
 *  应用通过参数options来表明添加stream的传输机制。基于UDP的多路复用不能提供可靠传输。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      type        [输入] stream类型，定义在WhisperStreamType。
 * @param
 *      options     [输入] stream工作模式。该参数通过位操作或者以下的flag来设置：
 *
 *                       - WHISPER_STREAM_ENCRYPT
 *                         加密模式。
 *                       - WHISPER_STREAM_RELIABLE
 *                         可靠传输模式。
 *                       - WHISPER_STREAM_MULTIPLEXING
 *                         多路复用模式。
 *                       - WHISPER_STREAM_PORT_FORWARDING
 *                         基于多路复用的端口转发模式。
 *
 * @param
 *      callbacks   [输入] 应用定义的WhisperStreamCallbacks类型回调函数。
 * @param
 *      context     [输入] 应用定义的上下文数据。
 *
 * @return
 *      若添加stream成功，返回stream id，反之，返回-1。
 *      具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_add_stream(WhisperSession *session, WhisperStreamType type,
        int options, WhisperStreamCallbacks *callbacks, void *context);

/**
 * \~English
 * Remove a stream from session.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream id to be removed.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 删除session中的指定stream。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] 需删除的stream id。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_remove_stream(WhisperSession *session, int stream);

/**
 * \~English
 * Add a new portforwarding service to session.
 *
 * The registered services can be used by remote peer in portforwarding
 * request.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      service     [in] The new service name, should be unique
 *                       in session scope.
 * @param
 *      protocol    [in] The protocol of the service.
 * @param
 *      host        [in] The host name or ip of the service.
 * @param
 *      port        [in] The port of the service.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 添加一个新端口转发服务。
 *
 * 新注册的服务可以被对端用于端口转发请求。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      service     [输入] 新服务名，在session中具有唯一性。
 * @param
 *      protocol    [输入] 服务支持的协议。
 * @param
 *      host        [输入] 服务的主机名或者ip地址。
 * @param
 *      port        [输入] 服务的端口。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_session_add_service(WhisperSession *session, const char *service,
        PortForwardingProtocol protocol, const char *host, const char *port);

/**
 * \~English
 * Remove a portforwarding service to session.
 *
 * This function has not effect on existing portforwarings.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      service     [in] The service name.
 *
 * \~Chinese
 * 删除session中的指定的端口转发服务。
 *
 * 该函数不能用于已存在的portforwaring中。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      service     [输入] 服务名。
 */
WHISPER_API
void whisper_session_remove_service(WhisperSession *session, const char *service);

/**
 * \~English
 * Set the whisper stream type.
 *
 * This function has not effect on connected streams.
 * Application can change the stream type before start session.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      type        [in] New stream type defined in WhisperStreamType.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 设置whisper stream类型
 *
 * 该函数不能用于已连接stream。
 * 应用可以在开启session之前改变stream类型。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      type        [输入] 新stream类型，具体定义WhisperStreamType。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_set_type(WhisperSession *session, int stream,
        WhisperStreamType type);

/**
 * \~English
 * Get the whisper stream type.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      type        [out] The stream type defined in WhisperStreamType.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinses
 * 获取whisper stream类型。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      type        [输入] 新stream类型，具体定义WhisperStreamType。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_get_type(WhisperSession *session, int stream,
        WhisperStreamType *type);

WHISPER_API
int whisper_stream_get_transport_info(WhisperSession *session, int stream,
                                      WhisperTransportInfo *info);

/**
 * \~English
 * Send outgoing data to remote peer.
 *
 * If the stream is in multiplexing mode, application can not
 * call this function to send data. If this function is called
 * on multiplexing mode stream, it will return error.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      data        [in] The outgoing data.
 * @param
 *      len         [in] The outgoing data length.
 *
 * @return
 *      Sent bytes on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 发送数据到对端。
 *
 * 若stream处于多路复用模式，应用不能调用该函数去发送数据。如果多路复用stream调用该函数发送数据，
 * 函数返回错误。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      data        [输入] 发送的数据。
 * @param
 *      len         [输入] 发送数据的长度。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
ssize_t whisper_stream_write(WhisperSession *session, int stream,
                             const void *data, size_t len);

/**
 * \~English
 * Open a new channel on multiplexing stream.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      cookie      [in] Application defined data pass to remote peer.
 *
 * @return
 *      New channel ID on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 创建一个新多路复用通道。
 *
 * 若stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      cookie      [输入] 传递给对端的自定义数据。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_open_channel(WhisperSession *session, int stream,
                                const char *cookie);

/**
 * \~English
 * Close a new channel on multiplexing stream.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      channel     [in] The channel ID.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 关闭指定多路复用通道。
 *
 * 若stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      channel     [输入] channel id。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_close_channel(WhisperSession *session, int stream, int channel);

/**
 * \~English
 * Send outgoing data to remote peer.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      channel     [in] The channel ID.
 * @param
 *      data        [in] The outgoing data.
 * @param
 *      len         [in] The outgoing data length.
 *
 * @return
 *      Sent bytes on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 发送数据到对端。
 *
 * 如果stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      channel     [输入] channel id。
 * @param
 *      data        [输入] 发送的数据。
 * @param
 *      len         [输入] 发送数据的长度。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
ssize_t whisper_stream_write_channel(WhisperSession *session, int stream,
                    int channel, const void *data, size_t len);

/**
 * \~English
 * Request remote peer to pend channel data sending.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      channel     [in] The channel ID.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 请求对端暂停数据发送。
 *
 * 如果stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      channel     [输入] channel id。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_pend_channel(WhisperSession *session, int stream, int channel);

/**
 * \~English
 * Request remote peer to resume channel data sending.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      channel     [in] The channel ID.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 请求对端恢复数据传输
 *
 * 如果stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      channel     [输入] channel id。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_resume_channel(WhisperSession *session, int stream, int channel);

/**
 * \~English
 * Open a portforwarding to remote service over multiplexing.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      service     [in] The remote service name.
 * @param
 *      protocol    [in] Portforwarding protocol.
 * @param
 *      host        [in] Local host or ip to binding.
 *                       If host is NULL, portforwarding will bind to 127.0.0.1.
 * @param
 *      port        [in] Local port to binding, can not be NULL.
 *
 * @return
 *      Portforwarding ID on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 创建一个基于多路复用的端口转发。
 *
 * 如果stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      service     [输入] 对端服务名。
 * @param
 *      protocol    [输入] 端口转发协议。
 * @param
 *      host        [输入] 用于绑定的本地主机或者ip地址。
 *                        如果本地主机为空，则绑定127.0.0.1。
 * @param
 *      port        [输入] 用于绑定的本地端口，不可为空。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_open_port_forwarding(WhisperSession *session, int stream,
        const char *service, PortForwardingProtocol protocol,
        const char *host, const char *port);

/**
 * \~English
 * Close a portforwarding to remote service over multiplexing.
 *
 * If the stream is not multiplexing this function will fail.
 *
 * @param
 *      session     [in] The handle to the WhisperSession.
 * @param
 *      stream      [in] The stream ID.
 * @param
 *      portforwarding  [in] The portforwarding ID.
 *
 * @return
 *      0 on success, or -1 if an error occurred.
 *      The specific error code can be retrieved by calling
 *      whisper_get_error().
 *
 * \~Chinese
 * 关闭一个基于多路复用的端口转发。
 *
 * 如果stream不支持多路复用，调用该函数失败。
 *
 * @param
 *      session     [输入] 指向WhisperSession的句柄。
 * @param
 *      stream      [输入] stream id。
 * @param
 *      portforwarding   [输入] portforwarding id。
 *
 * @return
 *      0表示成功，-1表示失败。具体错误可以调用whisper_get_error()来获得。
 */
WHISPER_API
int whisper_stream_close_port_forwarding(WhisperSession *session, int stream,
                                         int portforwarding);

#ifdef __cplusplus
}
#endif

#if defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#endif /* __WHISPER_SESSION_H__ */
