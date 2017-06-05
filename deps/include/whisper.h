#ifndef __WHISPER_H__
#define __WHISPER_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#if defined(__APPLE__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdocumentation"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WHISPER_STATIC)
  #define WHISPER_API
#elif defined(WHISPER_DYNAMIC)
  #ifdef WHISPER_BUILD
    #if defined(_WIN32) || defined(_WIN64)
      #define WHISPER_API        __declspec(dllexport)
    #else
      #define WHISPER_API        __attribute__((visibility("default")))
    #endif
  #else
    #if defined(_WIN32) || defined(_WIN64)
      #define WHISPER_API        __declspec(dllimport)
    #else
      #define WHISPER_API
    #endif
  #endif
#else
  #define WHISPER_API
#endif

/**
 * \~English
 * Whisper managed App/Node/User ID max length.
 *
 * \~Chinese
 * 应用/节点/用户ID的最大长度。
 */
#define WHISPER_MAX_ID_LEN                  45

/**
 * \~English
 * Whisper managed App/Node/User key max length.
 *
 * \~Chinese
 * 应用/节点/用户密钥的最大长度。
 */
#define WHISPER_MAX_KEY_LEN                 45

/**
 * \~English
 * Whisper managed user login name max length.
 *
 * \~Chinese
 * 用户登录名的最大长度。
 */
#define WHISPER_MAX_LOGIN_LEN               127

/**
 * \~English
 * Whisper managed user password max length.
 *
 * \~Chinese
 * 用户密码的最大长度。
 */
#define WHISPER_MAX_PASSWORD_LEN            63

/**
 * \~English
 * Whisper managed server URI max length.
 *
 * \~Chinese
 * 服务器地址的最大长度。
 */
#define WHISPER_MAX_SERVER_URI_LEN          127

/**
 * \~English
 * Whisper managed user name max length.
 *
 * \~Chinese
 * 用户名的最大长度。
 */
#define WHISPER_MAX_USER_NAME_LEN           63

/**
 * \~English
 * Whisper managed user description max length.
 *
 * \~Chinese
 * 用户描述的最大长度。
 */
#define WHISPER_MAX_USER_DESCRIPTION_LEN    127

/**
 * \~English
 * Whisper managed user phone number max length.
 *
 * \~Chinese
 * 用户电话的最大长度。
 */
#define WHISPER_MAX_PHONE_LEN               31

/**
 * \~English
 * Whisper managed user email address max length.
 *
 * \~Chinese
 * 用户电子邮箱的最大长度。
 */
#define WHISPER_MAX_EMAIL_LEN               127

/**
 * \~English
 * Whisper managed user region max length.
 *
 * \~Chinese
 * 用户地区的最大长度。
 */
#define WHISPER_MAX_REGION_LEN              127

/**
 * \~English
 * Whisper managed user gender max length.
 *
 * \~Chinese
 * 用户性别的最大长度。
 */
#define WHISPER_MAX_GENDER_LEN              31

/**
 * \~English
 * Whisper managed node name max length.
 *
 * \~Chinese
 * 节点名的最大长度。
 */
#define WHISPER_MAX_NODE_NAME_LEN           63

/**
 * \~English
 * Whisper managed node description max length.
 *
 * \~Chinese
 * 节点描述的最大长度。
 */
#define WHISPER_MAX_NODE_DESCRIPTION_LEN    127

/**
 * \~English
 * Whisper managed user presence max length.
 *
 * \~Chinese
 * 用户上下线情况的最大长度。
 */
#define WHISPER_MAX_USER_PRESENCE_LEN       31

/**
 * \~English
 * Whisper managed App message max length.
 *
 * \~Chinese
 * 应用消息的最大长度。
 */
#define WHISPER_MAX_APP_MESSAGE_LEN         2048

typedef struct Whisper Whisper;

/******************************************************************************
 * Creation & destruction
 *****************************************************************************/

/**
 * \~English
 * WhisperOptions defines several settings that control the way the
 * client connects to an Whisper managed server.
 *
 * @remark
 *      Default values are not defined for server_uri of WhisperOptions,
 *      so application should be set server_url clearly.
 *      If the WhisperOptions structure is defined as an automatic
 *      variable, all members are set to random values and thus must
 *      be set by the client application. If the WhisperOptions structure
 *      is defined as a static variable, initialization (in compliant
 *      compilers) sets all values to 0 (NULL for pointers).
 *
 * \~Chinese
 * WhisperOptions定义了一些控制客户端连接到服务器的设置。
 *
 * @remark
 *      WhisperOptions中的server_uri没有默认值，因此应用程序应当显示设置。
 *      如果WhisperOptions类型的变量被定义成自动变量，所有成员都是使用
 *      随机值，因此必须由客户端应用程序设置。如果WhisperOptions类型的变量
 *      被定义成静态变量，初始（在兼容编译器中）设置所有成员为0（指针类型
 *      的成员为空）。
 */
typedef struct WhisperOptions {
    /**
     * \~English
     * The user login identifier passed to the server when the client connects
     * to it.
     *
     * \~Chinese
     * 用户登录ID。
     */
    const char *login;
    /**
     * \~English
     * The user password to the server when the client connects to it.
     *
     * IMPORTANT:
     *     The application should clear the password in memory ASAP for
     *     security issues.
     *
     * \~Chinese
     * 用户登录密码。
     */
    const char *password;
    /**
     * \~English
     * The app identifier passed to the server when the client connects
     * to it.
     *
     * \~Chinese
     * 应用程序ID。
     */
    const char *appid;
    /**
     * \~English
     * The app key passed to the server when the client connects to it.
     *
     * \~Chinese
     * 应用程序密钥。
     */
    const char *appkey;
    /**
     * \~English
     * The Web API server URL;
     *
     * \~Chinese
     * 应用程序接口服务器的地址。
     */
    const char *api_server_url;
    /**
     * \~English
     * A null-terminated string specifying the server to which the client
     * will connect.It takes the form <i>protocol://host:port</i>.
     * Currently, <i>protocol</i> must be <i>tcp</i> or <i>ssl</i>.
     * For <i>host</i>, you can specify either an IP address or a host name.
     * For instance, to connect to a server running on the local machine,
     * specify <i>tcp://localhost:1883</i>.
     *
     * \~Chinese
     * 以'\0'结尾的字符串，用于指定客户端要连接的服务器地址。
     * 采用这种格式：<i>协议：//主机:端口</i>。
     * 目前，<i>协议</i>必须是<i>tcp</i>或者<i>ssl</i>。
     * 至于<i>主机</i>，你可以使用IP地址或者主机名。
     * 例如：为了连接到运行在本机的服务器，
     * 使用<i>tcp://localhost:1883</i>地址。
     */
    const char *mqtt_server_uri;
    /**
     * \~English
     * The file in PEM format containing the server public digital
     * certificates trusted by the client.
     *
     * \~Chinese
     * 包含客户端信任的服务器公开数字证书的PEM格式的文件。
     */
    const char* trust_store;
    /**
     * \~English
     * The "keep alive" interval, measured in seconds, defines the maximum
     * time that should pass without communication between the client and
     * the server. The client will ensure that at least one message travels
     * across the network within each keep alive period. In the absence of
     * a data-related message during the time period, the client sends
     * a very small "ping" message, which the server will acknowledge.
     * The keep alive interval enables the client to detect when the server
     * is no longer available without having to wait for the long TCP/IP
     * timeout. The default value is zero, will use system default interval.
     *
     * \~Chinese
     * “Keep alive”时间间隔，以秒为单位，定义了应当超过的客户端和服务器
     * 没有通信的时长的最大值。客户端应当保证在每个“Keep alive”时间间隔
     * 内，网络中至少有一个消息。在该时间间隔内，如果网络中没有与数据相关
     * 的消息，客户端就会发一个非常小的“ping”消息，服务器随后会向客户端
     * 确认该消息。“Keep alive”这种机制使客户端不用等到TCP/IP连接超时
     * 就能检测到服务器不可用。默认值为0，表示将会使用系统默认的时间间隔。
     */
    int keepalive_interval;
    /**
     * \~English
     * The time interval in seconds to allow a connect to complete.
     * The default value is zero, will use system default timeout.
     *
     * \~Chinese
     * 以秒为单位、允许连接动作持续的时间间隔。
     * 默认值为0，表示将会使用系统默认的时间间隔。
     */
    int connect_timeout;
    /**
     * \~English
     * The time interval in seconds for retry connect.
     * The default value is zero, means retry immediately.
     *
     * \~Chinese
     * 以秒为单位、重新连接的时间间隔。
     * 默认值为0，表示立即重新连接。
     */
    int retry_interval;
    /**
     * \~English
     * The max number of times to retry connect.
     * The default value is zero, means no retry.
     *
     * \~Chinese
     * 重新连接的最大次数。
     * 默认值为0，表示不重新连接。
     */
    int retry_times;
    /**
     * \~English
     * The application defined persistent data location.
     * The location must be set.
     *
     * \~Chinese
     * 应用程序定义的永久数据保存的位置。
     * 必填。
     */
    const char *persistent_location;
    /**
     * \~English
     * The virtual device ID specified by application.
     * This attribute only for advanced user & test purpose
     *
     * \~Chinese
     * 设置ID。
     */
     const char *deviceid;
} WhisperOptions;

/**
 * \~English
 * Get the current version of Whisper client.
 *
 * \~Chinese
 * 获取当前Whisper客户端的版本。
 */
WHISPER_API
const char *whisper_get_version(void);

/**
 * \~English
 * Whisper log level to control or filter log output.
 *
 * \~Chinese
 * 控制或者过滤日志输出的Whisper日志级别。
 */
typedef enum WhisperLogLevel {
    /**
     * \~English
     * Log level None
     * Indicate disable log output.
     *
     * \~Chinese
     * 没有日志级别。
     * 表示禁用日志输出。
     */
    WhisperLogLevel_None = 0,
    /**
     * \~English
     * Log level error.
     * Indicate output log with level 'Error' only.
     *
     * \~Chinese
     * 错误日志级别。
     * 表示只输出错误级别的日志。
     */
    WhisperLogLevel_Error = 1,
    /**
     * \~English
     * Log level warning.
     * Indicate output log with level 'Error' and 'Warning'.
     *
     * \~Chinese
     * 警告日志级别
     * 表示输出错误和警告级别的日志。
     */
    WhisperLogLevel_Warning = 2,
    /**
     * \~English
     * Log level info.
     * Indicate output log with level 'Error', 'Warning' and 'Info'.
     *
     * \~Chinese
     * 信息日志级别
     * 表示输出错误、警告和信息级别的日志。
     */
    WhisperLogLevel_Info = 3,
    /*
     * \~English
     * Log level debug.
     * Indicate output log with no filters.
     *
     * \~Chinese
     * 调试日志级别
     * 表示输出所有级别的日志。
     */
    WhisperLogLevel_Debug = 4

} WhisperLogLevel;

/**
 * \~English
 * Whisper client connection status to the server.
 *
 * \~Chinese
 * Whisper客户端到服务器的连接状态。
 */
typedef enum WhisperConnectionStatus {
    /**
     * \~English
     * Attempting to connect to server.
     * Not connected yet, so it's offline.
     *
     * \~Chinese
     * 尝试连接到服务器。
     * 还没连接上，所以是离线的。
     */
    WhisperConnectionStatus_Connecting,
    /**
     * \~English
     * Whisper client connected to the server.
     * Indicate the client is online.
     *
     * \~Chinese
     * 已连接上服务器。
     * 表明客户端是上线状态。
     */
    WhisperConnectionStatus_Connected,
    /**
     * \~English
     * There is no connection to the server.
     * Indicate the client is offline.
     *
     * \~Chinese
     * 没有连接上服务器。
     * 表明客户端是离线状态。
     */
    WhisperConnectionStatus_Disconnected,
} WhisperConnectionStatus;

/**
 * \~English
 * A structure representing the Whisper node information.
 *
 * \~Chinese
 * 描述Whisper节点信息的结构体。
 */
typedef struct WhisperNodeInfo {
    /**
     * \~English
     * The node id. Read only to the client application.
     *
     * \~Chinese
     * 节点ID。对于客户端应用程序来说是只读的。
     */
    char nodeid[WHISPER_MAX_ID_LEN+1];
    /**
     * \~English
     * The node name.
     *
     * \~Chinese
     * 节点名称。
     */
    char name[WHISPER_MAX_NODE_NAME_LEN+1];
    /**
     * \~English
     * The node description.
     *
     * \~Chinese
     * 节点描述。
     */
    char description[WHISPER_MAX_NODE_DESCRIPTION_LEN+1];
} WhisperNodeInfo;

/**
 * \~English
 * A structure representing the Whisper user information.
 *
 * \~Chinese
 * 描述Whisper用户信息的结构体。
 *
 * \~English
 * In whisper managed SDK, self and all friends are Whisper user, and have
 * same user attributes.
 *
 * \~Chinese
 * 在Whisper SDK中，自己和所有的朋友都是Whisper用户，都有相同的属性。
 */
typedef struct WhisperUserInfo {
    /**
     * \~English
     * User ID. Read only to the client application.
     *
     * \~Chinese
     * 用户ID。对于客户端应用程序来说是只读的。
     */
    char userid[WHISPER_MAX_ID_LEN+1];
    /**
     * \~English
     * Nickname, also known as display name.
     *
     * \~Chinese
     * 昵称，又称为显示名称。
     */
    char name[WHISPER_MAX_USER_NAME_LEN+1];
    /**
     * \~English
     * User's description, also known as what's up.
     *
     * \~Chinese
     * 用户的描述。
     */
    char description[WHISPER_MAX_USER_DESCRIPTION_LEN+1];
    /**
     * \~English
     * If user has an avatar.
     *
     * \~Chinese
     * 用户是否有头像。
     */
    int has_avatar;
    /**
     * \~English
     * User's gender.
     *
     * \~Chinese
     * 用户的性别。
     */
    char gender[WHISPER_MAX_GENDER_LEN+1];
    /**
     * \~English
     * User's phone number.
     *
     * \~Chinese
     * 电话号码。
     */
    char phone[WHISPER_MAX_PHONE_LEN+1];
    /**
     * \~English
     * User's email address.
     *
     * \~Chinese
     * 电子邮箱。
     */
    char email[WHISPER_MAX_EMAIL_LEN+1];
    /**
     * \~English
     * User's region information.
     *
     * \~Chinese
     * 地区。
     */
    char region[WHISPER_MAX_REGION_LEN+1];
} WhisperUserInfo;

/**
 * \~English
 * A structure representing the Whisper friend information.
 *
 * Include the basic user information and the extra friend information.
 *
 * \~Chinese
 * 描述Whisper朋友信息的结构体。
 *
 * 包含基本用户信息和额外朋友信息。
 */
typedef struct WhisperFriendInfo {
    /**
     * \~English
     * Friend's user information.
     *
     * \~Chinese
     * 朋友的信息。
     */
    WhisperUserInfo user_info;
    /**
     * \~English
     * Entrustment relationship with this friend, @see WhisperEntrustmentType
     *
     * \~Chinese
     * 和朋友的委托关系，参见WhisperEntrustmentType。
     */
    int entrusted;
    /**
     * \~English
     * Your label for the friend.
     *
     * \~Chinese
     * 朋友的标签。
     */
    char label[WHISPER_MAX_USER_NAME_LEN+1];
    /**
     * \~English
     * Friend's presence status.
     *
     * \~Chinese
     * 朋友的在线和离线状态。
     */
    char presence[WHISPER_MAX_USER_PRESENCE_LEN+1];
} WhisperFriendInfo;

/**
 * \~English
 * Whisper callbacks, include all global callbacks for whisper.
 *
 * \~Chinese
 * Whisper相关的回调函数，包括Whisper的所有的全局函数。
 */
typedef struct WhisperCallbacks {
    /**
     * \~English
     * An application-defined function that perform idle work.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的执行空闲任务的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*idle)(Whisper *whisper, void *context);

    /**
     * \~English
     * An application-defined function that process the self connection status.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      status      [in] Current connection status. @see WhisperConnection.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理自身连接状态的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      status      [输入] 当前连接状态。参见WhisperConnection。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*connection_status)(Whisper *whisper,
                              WhisperConnectionStatus status, void *context);

    /**
     * \~English
     * An application-defined function that process the ready notification.
     * Notice: application should wait this callback invoked before calling any
     * whisper function to interact with friends.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理就绪通知的函数。
     * 注意：应用程序应当在收到就绪通知后才能调用其他的Whisper函数来与朋友交互。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*ready)(Whisper *whisper, void *context);

    /**
     * \~English
     * An application-defined function that process the self info change event.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      info        [in] The WhisperUserInfo pointer to the new user info.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理自身信息改变事件的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      info        [输入] 保存新用户信息的WhisperUserInfo类型变量的地址。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*self_info)(Whisper *whisper, const WhisperUserInfo *info, void *context);

    /**
     * \~English
     * An application-defined function that iterate the each friends list item.
     *
     * WhisperFriendsIterateCallback is the callback function type.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      info        [in] A pointer to WhisperFriendInfo structure that
     *                       representing a friend
     * @param
     *      context     [in] The application defined context data.
     *
     * @return
     *      Return true to continue iterate next friend user info,
     *      false to stop iterate.
     *
     * \~Chinese
     * 应用程序定义的迭代每个朋友列表项的函数。
     *
     * WhisperFriendInviteResponseCallback为回调函数的类型。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      info        [输入] 描述朋友信息的WhisperFriendInfo类型的变量的地址。
     * @param
     *      context     [输入] 应用程序定义的环境数据。
     *
     * @return
     *      如果继续迭代下一个朋友信息，返回true；如果停止迭代，返回false。
     */
    bool (*friend_list)(Whisper *whisper, const WhisperFriendInfo* info, void* context);

    /**
     * \~English
     * An application-defined function that process the friend information
     * change event.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      info        [in] The WhisperFriendInfo pointer to the new friend info.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友信息改变的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      friendid    [输入] 朋友ID。
     * @param
     *      info        [输入] 保存新朋友信息的WhisperFriendInfo类型变量的地址。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_info)(Whisper *whisper, const char *friendid,
                        const WhisperFriendInfo *info, void *context);

    /**
     * \~English
     * An application-defined function that process the friend presence
     * change event.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      presence    [in] The presence status of the friend.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友在线或者离线状态改变的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      friendid    [输入] 朋友ID。
     * @param
     *      presence    [输入] 朋友的在线或者离线状态。
     * @param
     *      context     [输入] 应用程序定义环境信息。
     */
    void (*friend_presence)(Whisper *whisper, const char *friendid,
                            const char *presence, void *context);

    /**
     * \~English
     * An application-defined function that process the friend request.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      userid      [in] The user id who wants friend with self.
     * @param
     *      info        [in] The user info who wants to friend with self.
     * @param
     *      hello       [in] PIN for target user, or any application defined
     *                       content.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友请求的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      userid      [输入] 想成为自己朋友的用户ID。
     * @param
     *      info        [输入] 想成为自己朋友的用户的信息。
     * @param
     *      hello       [输入] 机密信息，或者应用程序定义的内容。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_request)(Whisper *whisper, const char *userid,
                           const WhisperUserInfo *info,
                           const char *hello, void *context);

    /**
     * \~English
     * An application-defined function that process the friend response.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      userid      [in] The target user id.
     * @param
     *      status      [in] The status code of the response.
     *                       0 is success, otherwise is error.
     * @param
     *      reason      [in] The error message if status is error, or NULL
     *                       if success.
     * @param
     *      entrusted   [in] Entrusted by this friend or not.
     * @param
     *      expire      [in] The expire time, or NULL if never expire.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友响应的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      userid      [输入] 响应的用户ID。
     * @param
     *      status      [输入] 响应的状态码。0表示成功，否则表示出错。
     * @param
     *      reason      [输入] 出错时的原因，成功时为空。
     * @param
     *      entrusted   [输入] 是否委托给当前客户端。
     * @param
     *      expire      [输入] 超时时间，为空表示永不超时。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_response)(Whisper *whisper, const char *userid,
                            int status, const char *reason,
                            bool entrusted, const char *expire,
                            void *context);

    /**
     * \~English
     * An application-defined function that process the new friend added
     * event.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      info        [in] The firend's information.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理添加朋友事件的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      info        [输入] 朋友信息。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_added)(Whisper *whisper, const WhisperFriendInfo *info,
                         void *context);

    /**
     * \~English
     * An application-defined function that process the friend removed
     * event.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      friendid    [in] The friend's user id.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理删除朋友事件的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      friendid    [输入] 朋友ID。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_removed)(Whisper *whisper, const char *friendid,
                           void *context);

    /**
     * \~English
     * An application-defined function that process the friend messages.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      from        [in] The id(userid@nodeid) from who send the message.
     * @param
     *      msg         [in] The message content.
     * @param
     *      len         [in] The message length in bytes.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友请求的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      from        [输入] 发送消息的用户ID(userid@nodeid)。
     * @param
     *      msg         [输入] 消息内容。
     * @param
     *      len         [输入] 以字节为单位的消息的长度。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_message)(Whisper *whisper, const char *from,
                           const char *msg, size_t len, void *context);

    /**
     * \~English
     * An application-defined function that process the friend invite request.
     *
     * @param
     *      whisper     [in] A handle to the Whisper client instance.
     * @param
     *      from        [in] The user id from who send the invite request.
     * @param
     *      data        [in] The application defined data send from friend.
     * @param
     *      len         [in] The data length in bytes.
     * @param
     *      context     [in] The application defined context data.
     *
     * \~Chinese
     * 应用程序定义的处理朋友邀请的函数。
     *
     * @param
     *      whisper     [输入] Whisper客户端对象的句柄。
     * @param
     *      from        [输入] 发送邀请的用户ID。
     * @param
     *      data        [输入] 应用程序定义的由发送邀请用户发出的数据。
     * @param
     *      len         [输入] 以字节为单位的数据长度。
     * @param
     *      context     [输入] 应用程序定义的环境信息。
     */
    void (*friend_invite)(Whisper *whisper, const char *from,
                          const char *data, size_t len,
                          void *context);

} WhisperCallbacks;

/**
 * \~English
 * initialize log options for Whisper. The default level to control log output
 * is 'Info'.
 *
 * @param
 *      level       [in] The log level to control internal log output.
 * @param
 *      log_file    [in] the log file name.
 *                       If the log_file is NULL, Whisper will not write
 *                       log to file.
 * @param
 *      log_printer [in] the user defined log printer. can be NULL.
 *
 * \~Chinese
 * 初始化Whisper的日志选项。默认设置“信息”级别作为日志级别。
 *
 * @param
 *      level       [输入] 要使用输出日志的级别。
 * @param
 *      log_file    [输入] 日志文件名。如果为空，日志信息就不会保存到文件中。
 * @param
 *      log_printer [输入] 用户定义的日志输出函数，可以为空。
 */
WHISPER_API
void whisper_log_init(WhisperLogLevel level, const char *log_file,
        void (*log_printer)(const char *format, va_list args));

/**
 * \~English
 * Check if the whisper ID is valid.
 *
 * @param
 *      id          [in] the whisper id to be check.
 *
 * @return
 *      true if id is valid, or false if id is not valid.
 *
 * \~Chinese
 * 检查Whisper ID是否有效。
 *
 * @param
 *      id          [输入] 要检查的Whisper ID。
 *
 * @return
        如果有效，返回true;如果无效，返回false。
 */
WHISPER_API
bool whisper_id_is_valid(const char *id);

/**
 * \~English
 * Create a new Whisper client instance. after create the instance, it's
 * ready for connection to the server
 *
 * @param
 *      options     [in] A pointer to a valid WhisperOptions structure.
 * @param
 *      callbacks   [in] The Application defined callback functions.
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      If no error occurs, return the pointer of Whisper client instance.
 *      Otherwise, return NULL, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 创建一个Whisper对象。创建之后，准备连接到服务器。
 *
 * @param
 *      options     [输入] 指向一个有效的WhisperOptions结构体的指针。
 * @param
 *      callbacks   [输入] 应用程序定义的回调函数。
 * @param
 *      context     [输入] 应用程序定义的环境信息。
 *
 * @return
 *      如果不出错，返回一个指向Whisper客户端对象的指针。
 *      否则，返回空指针，并可以通过调用whisper_get_error()函数获取错误码。
 */
WHISPER_API
Whisper *whisper_new(const WhisperOptions *options,
                     WhisperCallbacks *callbacks, void *context);

/**
 * \~English
 * Disconnect from the server, and destroy all associated resources with the
 * Whisper client instance.
 *
 * After calling the function, the Whisper pointer becomes invalid.
 * No other function can be called.
 *
 * @param
 *      whisper     [in] A handle identifying the Whisper client instance
 *                       to kill.
 *
 * \~Chinese
 * 断开与服务器的连接，并清理所有与Whisper客户端对象有关的资源。
 *
 * 调用之后，Whisper指针失效。不能再调用其他函数。
 *
 * @param
 *      whisper     [输入] 要终止的客户端对象的句柄。
 */
WHISPER_API
void whisper_kill(Whisper *whisper);

/******************************************************************************
 * \~English
 * Connection & event loop
 *
 * \~Chinese
 * 连接和事件循环
 *****************************************************************************/
/**
 * \~English
 * Attempts to connect the client to server, if the client successfully
 * connects to the server, then start the client's main event loop.
 * The connect options was specified by previously create options.
 * @see whisper_create().
 *
 * @param
 *      whisper     [in] A handle identifying the Whisper client instance.
 * @param
 *      interval    [in] Internal loop interval, in milliseconds.
 *
 * @return
 *      0 if the client successfully connected to the server and start the
 *      event loop. Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 客户端尝试连接到服务器，如果连接成功，开启客户端的事件循环。
 * 连接选项通过之前创建时提供的选项指定。
 * @参照 whisper_create()。
 *
 * @param
 *      whisper     [输入] 标识Whisper客户端对象的句柄。
 *
 * @param
 *      interval    [输入] 内部循环间隔周期，以毫秒为单位。
 *
 * @return
 *      如果客户端成功地连接到服务器并开启了事件循环，返回0。否则，返回-1,
 *      并可以通过调用whisper_get_error()获取错误码。
 */
WHISPER_API
int whisper_run(Whisper *whisper, int interval);

/******************************************************************************
 * Internal client information
 *****************************************************************************/

/**
 * \~English
 * Get app identifier associated with the Whisper client.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      appid       [out] The buffer that will receive the identifier.
 *                        The buffer size should at least
 *                        (WHISPER_MAX_ID_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of appid.
 *
 * @return
 *      The appId string pointer, or NULL if buffer is too small.
 *
 * \~Chinese
 * 获取与客户端关联的应用程序标识。
 *
 * @param
 *      whisper     [输入] Whispser客户端对象的句柄。
 * @param
 *      appid       [输出] 用来存放应用程序标识的存储空间。
 *                         存储空间至少要有WHISPER_MAX_ID_LEN+1个字节。
 * @param
 *      len         [输入] appid存储空间的长度。
 *
 * @return
 *      指向应用程序标识字符串的指针，如果提供的存储空间太小则返回空指针。
 */
WHISPER_API
char *whisper_get_appid(Whisper *whisper, char *appid, size_t len);

/**
 * \~English
 * Get node identifier associated with the Whisper client.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      nodeid      [out] The buffer that will receive the identifier.
 *                        The buffer size should at least
 *                        (WHISPER_MAX_ID_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of nodeid.
 *
 * @return
 *      The nodeId string pointer, or NULL if buffer is too small.
 *
 * \~Chinese
 * 获取与Whisper客户端关联的节点标识。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      nodeid      [输出] 用来存储节点标识的存储空间。
 *                         存储空间至少要有WHISPER_MAX_ID_LEN+1个字节。
 * @param
 *      len         [输入] nodeid存储空间的长度。
 *
 * @return
 *      指向节点标识字符串的指针，如果提供的存储空间太小则返回空指针。
 */
WHISPER_API
char *whisper_get_nodeid(Whisper *whisper, char *nodeid, size_t len);

/**
 * \~English
 * Get user identifier associated with the Whisper client.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      userid      [out] The buffer that will receive the identifier.
 *                        The buffer size should at least
 *                        (WHISPER_MAX_ID_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of userid.
 *
 * @return
 *      The userId string pointer, or NULL if buffer is too small.
 *
 * \~Chinese
 * 获取与Whisper客户端关联的用户标识。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      userid      [输出] 用来存储节点标识的存储空间。
 *                         存储空间至少要有WHISPER_MAX_ID_LEN+1个字节。
 * @param
 *      len         [输入] nodeid存储空间的长度。
 *
 * @return
 *      指向用户标识字符串的指针，如果提供的存储空间太小则返回空指针。
 */
WHISPER_API
char *whisper_get_userid(Whisper *whisper, char *userid, size_t len);

/**
 * \~English
 * Get user login name associated with the Whisper client.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      login       [out] The buffer that will receive the login name.
 *                        The buffer size should at least
 *                        (WHISPER_MAX_LOGIN_LEN + 1) bytes.
 * @param
 *      len         [in] The buffer size of login name.
 *
 * @return
 *      The login string pointer, or NULL if buffer is too small.
 *
 * \~Chinese
 * 获取与客户端关联的登录名。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      login       [输出] 用来存储登录名的存储空间。
 *                         存储空间至少要有WHISPER_MAX_LOGIN_LEN+1个字节。
 * @param
 *      len         [输入] login存储空间的长度。
 *
 * @return
 *      指向登录名字符串的指针，如果提供的存储空间太小则返回空指针。
 */
WHISPER_API
char *whisper_get_login(Whisper *whisper, char *login, size_t len);

/******************************************************************************
 * Client information
 *****************************************************************************/

/**
 * \~English
 * Update self information.
 *
 * After self information changed, client will update self information
 * to server. then server will broadcast the change to all friends and
 * the other node that current user signed in.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      info        [in] The WhisperUserInfo pointer to the updated user info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 更新自己的信息。
 *
 * 在个人的信息修改后，客户端会将自己的信息更新到服务器。
 * 之后服务器就向该客户端的朋友和当前登录的节点广播更新后的信息。
 *
 * @param
 *      whisper     [输入] Whisper客户端实例的句柄。
 * @param
 *      info        [输入] 指向WhisperUserInfo结构体的新用户信息的指针。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_set_self_info(Whisper *whisper, const WhisperUserInfo *info);

/**
 * \~English
 * Get self information.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      info        [in] The WhisperUserInfo pointer to receive user info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 获取自己的信息。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      info        [输入] 用于接收用户信息的WhisperUserInfo类型的结构体的地址。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_get_self_info(Whisper *whisper, WhisperUserInfo *info);

/**
 * \~English
 * Set node information.
 *
 * After node information changed, client will update node information
 * to server.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      info        [in] The WhisperNodeInfo pointer to the updated node info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 设置节点信息。
 *
 * 在节点信息修改后，客户端会将自己的信息更新到服务器。
 *
 * @param
 *      whisper     [输入] 客户端实例句柄。
 * @param
 *      info        [输入] 新的WhisperNodeInfo结构体的地址。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_set_node_info(Whisper *whisper, const WhisperNodeInfo *info);

/**
 * \~English
 * Get node information.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      info        [in] The WhisperNodeInfo pointer to receive the node info.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 获取节点信息。
 *
 * @param
 *      whisper     [输入] 客户端实例句柄。
 * @param
 *      info        [输入] 用于保存节点信息的WhisperNodeInfo类型的结构体的地址。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_get_node_info(Whisper *whisper, WhisperNodeInfo *info);

/**
 * \~English
 * Check if whisper client instance is being ready.
 *
 * All whisper interactive APIs should be called only if whisper instace 
 * is being ready.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 *
 * @return
 *      true if the whisper client instance is ready, or false if not.
 *
 * \~Chinese
 * 检查Whisper客户端实例是否可以调用交互式Whisper接口。
 *
 * @param
 *      whisper     [输入] 客户端实例句柄。
 *
 * @return
 *      如果准备完成，则返回true。否则，返回false。
 */
WHISPER_API
bool whisper_is_ready(Whisper *whisper);

/******************************************************************************
 * Friend information
 *****************************************************************************/

 /**
 * \~English
 * Whisper client entrustment type to friend.
 *
 * \~Chinese
 * Whisper客户端相对于朋友的委托类型。
 */
typedef enum WhisperEntrustmentType {
    /**
     * \~English
     * Entrust the friend.
     *
     * \~Chinese
     * 委托。
     */
    WhisperEntrustmentType_Entrusted = 1,
    /**
     * \~English
     * No entrusted relationship with friend.
     *
     * \~Chinese
     * 无委托。
     */
    WhisperEntrustmentType_None = 0,
    /**
     * \~English
     * Entrusted by friend.
     *
     * \~Chinese
     * 被委托。
     */
    WhisperEntrustmentType_EntrustedBy = -1
} WhisperEntrustmentType;

/**
 * \~English
 * An application-defined function that iterate the each friends list item.
 *
 * WhisperFriendsIterateCallback is the callback function type.
 *
 * @param
 *      info        [in] A pointer to WhisperFriendInfo structure that
 *                       representing a friend
 * @param
 *      context     [in] The application defined context data.
 *
 * @return
 *      Return true to continue iterate next friend user info,
 *      false to stop iterate.
 *
 * \~Chinese
 * 应用程序定义的遍历每个朋友列表的函数。
 *
 * WhisperFriendsCallback为回调函数类型。
 *
 * @param
 *      info        [输入] 表示一个朋友的WhisperFriendInfo结构体类型的地址。
 * @param
 *      context     [输入] 应用程序定义的环境信息。
 *
 * @return
 *      如果继续迭代下一个朋友的信息，返回true。如果停止迭代，返回false。
 */
typedef bool WhisperFriendsIterateCallback(const WhisperFriendInfo *info,
                                           void *context);
/**
 * \~English
 * Get friends list. For each friend will call the application defined
 * iterate callback.
 *
 * @param
 *      whisper     [in] a handle to the Whisper client instance.
 * @param
 *      callback    [in] a pointer to WhisperFriendsIterateCallback function.
 * @param
 *      context     [in] the application defined context data.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 对于每一个朋友，都会调用应用程序定义的迭代回调函数。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      callback    [输入] WhisperFriendsIterateCallback类型的函数的地址。
 * @param
 *      context     [输入] 应用程序定义的环境信息。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_get_friends(Whisper *whisper,
                        WhisperFriendsIterateCallback *callback, void *context);

/**
 * \~English
 * Get friend information.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      friendid    [in] The friend's user id.
 * @param
 *      info        [in] The WhisperFriendInfo pointer to receive the friend
 *                       information.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 获取朋友信息。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      friendid    [输入] 朋友的用户ID。
 * @param
 *      info        [输入] 保存朋友信息的WhisperFriendInfo结构体的地址。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_get_friend_info(Whisper *whisper, const char *friendid,
                            WhisperFriendInfo *info);

/**
 * \~English
 * Set the label of the specified friend.
 *
 * If the value length is 0 or value is NULL, the attribute will be cleared.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      friendid    [in] The friend's user id.
 * @param
 *      label       [in] the new label of the specified friend.
 *
 * @return
 *      0 on success, or -1 if an error occurred. The specific error code
 *      can be retrieved by calling whisper_get_error().
 *
 * @remarks
 *      The label of a friend is a private alias named by yourself. It can be
 *      seen by yourself only, and has no impact to the target friend.
 *
 * \~Chinese
 * 设置指定朋友的标签。
 *
 * 如果值的长度为0或者值为空，朋友的标签会被清空。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      friendid    [输入] 朋友的用户ID。
 * @param
 *      label       [输入] 指定朋友的新标签。
 *
 * @return
 *      如果成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_set_friend_label(Whisper *whisper,
                             const char *friendid, const char *label);

/**
 * \~English
 * Check if the user ID is friend.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      userid      [in] The userid to check.
 *
 * @return
 *      true if the user id is friend, or false if not;
 *
 * \~Chinese
 * 检查用户ID是否为朋友。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      userid      [输入] 要检查的用户ID。
 *
 * @return
 *      如果ID对应的用户为朋友，返回true。否则，返回false。
 */
WHISPER_API
bool whisper_is_friend(Whisper* whisper, const char* userid);

/******************************************************************************
 * Friend requests & add & remove
 *****************************************************************************/

/**
 * \~English
 * Attempt to send a new friend request.
 *
 * This function will send a friend request to server, and the server
 * return the result within a friend response. The whisper client can
 * check whether the friend request be confirmed or refused in friend
 * response callback.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      userid      [in] The target user id.
 * @param
 *      hello       [in] PIN for target user, or any application defined
 *                       content.
 *
 * @return
 *      0 if the friend request successfully send to the server.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 尝试发送朋友请求。
 *
 * 此函数会发一个朋友请求到服务器，服务器将处理结果回复给客户端。
 * 客户端可以在服务器的回复中检查朋友请求是否被同意、拒绝。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      userid      [输入] 目标用户ID。
 * @param
 *      hello       [输入] 目标用户的密码，或者任何应用程序定义的内容。
 *
 * @return
 *      如果发送成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_friend_request(Whisper *whisper, const char *userid, const char *hello);

/**
 * \~English
 * Reply the friend request.
 *
 * This function will send a friend response to server.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      userid      [in] The user id who wants friend with self.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 *                       if success.
 * @param
 *      entrusted   [in] Entrust this friend or not.
 * @param
 *      expire      [in] Expire time with strict pattern "yyyy-MM-dd,hh:mm",
 *                       or NULL never expire.
 *
 * @return
 *      0 if the friend request successfully send to the server.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 回复朋友请求。
 *
 * 此函数会发一个朋友响应到服务器。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      userid      [输入] 想和自己成为朋友的用户的ID。
 * @param
 *      status      [输入] 响应的状态码。0表示成功，否则表示出错。
 * @param
 *      reason      [输入] 如果出错，表示错误信息。如果成功，为空。
 * @param
 *      entrusted   [输入] 是否接受朋友的委托。
 * @param
 *      expire      [输入] 超时时间，格式为"yyyy-MM-dd,hh:mm", 为空表示永不超时。
 *
 * @return
 *      如果发送成功，返回0。如果出错，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_reply_friend_request(Whisper *whisper, const char *userid,
                                 int status, const char *reason,
                                 bool entrusted,
                                 const char *expire);

/**
 * \~English
 * Remove a friend.
 *
 * This function will send a remove friend indicator to server.
 *
 * If all correct, the server will clean the friend relationship, and send
 * friend removed message to both.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      userid      [in] The target user id.
 *
 * @return
 *      0 if the indicator successfully send to the server.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 删除朋友
 *
 * 此函数会给服务器发送一个删除朋友的指示。
 *
 * 如果操作成功，服务器将会删除客户端和朋友之间的关系，并给双方发送
 * 删除朋友的消息。
 *
 * @param
 *      whisper     [输入] 客户端对象的句柄。
 * @param
 *      userid      [输入] 要删除的朋友ID。
 *
 * @return
 *      如果指示发送成功，返回0。否则，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_friend_remove(Whisper *whisper, const char *userid);

/******************************************************************************
 * Application transactions between friends
 *****************************************************************************/

/**
 * \~English
 * Send a message to a friend.
 *
 * The message length may not exceed WHISPER_MAX_APP_MESSAGE_LEN, and message
 * itself should be text-formatted. Larger messages must be split by application
 * and sent as separate messages. Other clients can reassemble the fragments.
 *
 * Message may not be empty or NULL.
 *
 * If the message is binary, it will encoded to text use base64.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      to          [in] The target id(userid or userid@nodeid).
 * @param
 *      msg         [in] The message content defined by application.
 * @param
 *      len         [in] The message length in bytes.
 *
 * @return
 *      0 if the text message successfully send to the server.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 给朋友发送消息。
 *
 * 消息长度不能超过WHISPER_MAX_APP_MESSAGE_LEN，消息本身必须是文本格式。
 * 当要发送较长的消息时，应用程序应当将长消息拆成短消息发送，其他的客户端
 * 收到后将短消息组装成长消息。
 *
 * 消息不能为空。
 *
 * 如果消息是二进制的，应当先按base64方式先编码再发送。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      to          [输入] 接收消息的用户ID(用户ID或者用户ID@节点ID)。
 * @param
 *      msg         [输入] 应用程序定义的消息内容。
 * @param
 *      len         [输入] 消息长度，以字节为单位。
 *
 * @return
 *      如果指示发送成功，返回0。否则，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_send_friend_message(Whisper *whisper, const char *to,
                                const char *msg, size_t len);

/**
 * \~English
 * An application-defined function that process the friend invite response.
 *
 * WhisperFriendInviteResponseCallback is the callback function type.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      from        [in] The target user id.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 * @param
 *      data        [in] The application defined data return by target user.
 * @param
 *      len         [in] The data length in bytes.
 * @param
 *      context      [in] The application defined context data.
 *
 * \~Chinese
 * 应用程序定义的处理朋友邀请响应的函数。
 *
 * WhisperFriendInviteResponseCallback为回调函数类型。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      from        [输入] 被邀请的朋友用户ID。
 * @param
 *      status      [输入] 响应的状态码，0表示成功，其他表示出错。
 * @param
 *      reason      [输入] 出错时的原因。
 * @param
 *      data        [输入] 应用程序定义的从被邀请的朋友那里发来的数据。
 * @param
 *      len         [输入] 以字节为单位的数据长度。
 * @param
 *      context     [输入] 应用程序定义的环境信息。
 */
typedef void WhisperFriendInviteResponseCallback(Whisper *whisper,
                                                 const char *from,
                                                 int status, const char *reason,
                                                 const char *data, size_t len,
                                                 void *context);

/**
 * \~English
 * Send invite request to a friend.
 *
 * Application can attach the application defined data within the invite
 * request, and the data will send to target friend.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      to          [in] The target id(userid or userid@nodeid).
 * @param
 *      data        [in] The application defined data send to target user.
 * @param
 *      len         [in] The data length in bytes.
 * @param
 *      callback    [in] A pointer to WhisperFriendInviteResponseCallback
 *                       function to receive the invite response.
 * @param
 *      context      [in] The application defined context data.
 *
 * @return
 *      0 if the invite request successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 给朋友发送邀请。
 *
 * 应用程序可以在邀请中附加应用程序定义的数据，这些数据会发给朋友。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      to          [输入] 接收邀请的用户ID(userid or userid@nodeid)。
 * @param
 *      data        [输入] 应用程序定义的发送给接收邀请用户的数据。
 * @param
 *      len         [输入] 以字节为单位的数据的长度。
 * @param
 *      callback    [输入] 接收邀请响应的WhisperFriendInviteResponseCallback
 *                         类型的回调函数的地址。
 * @param
 *      context     [输入] 应用程序定义的环境信息。
 *
 * @return
 *      如果邀请发送成功，返回0。否则，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_invite_friend(Whisper *whisper, const char *to,
                          const char *data, size_t len,
                          WhisperFriendInviteResponseCallback *callback,
                          void *context);

/**
 * \~English
 * Reply the friend invite request.
 *
 * This function will send a invite response to friend.
 *
 * @param
 *      whisper     [in] A handle to the Whisper client instance.
 * @param
 *      to          [in] The id(userid@nodeid) who send invite request.
 * @param
 *      status      [in] The status code of the response.
 *                       0 is success, otherwise is error.
 * @param
 *      reason      [in] The error message if status is error, or NULL
 *                       if success.
 * @param
 *      data        [in] The application defined data send to target user.
 *                       If the status is error, this will be ignored.
 * @param
 *      len         [in] The data length in bytes.
 *                       If the status is error, this will be ignored.
 *
 * @return
 *      0 if the invite response successfully send to the friend.
 *      Otherwise, return -1, and a specific error code can be
 *      retrieved by calling whisper_get_error().
 *
 * \~Chinese
 * 回复朋友的邀请。
 *
 * 此函数会给朋友发送一个邀请响应。
 *
 * @param
 *      whisper     [输入] Whisper客户端对象的句柄。
 * @param
 *      to          [输入] 发送邀请的朋友ID(userid@nodeid)。
 * @param
 *      status      [输入] 邀请响应的状态码，0表示成功，其他表示出错。
 * @param
 *      reason      [输入] 出错时的原因，成功时为空。
 * @param
 *      data        [输入] 应用程序定义的回复给发送邀请的朋友的数据。
 *                         此参数的值在出错时会被忽略。
 * @param
 *      len         [输入] 以字节为单位的数据的长度。
 *
 * @return
 *      如果邀请响应发送成功，返回0。否则，返回-1，并可以通过调用whisper_get_error()
 *      获取错误码。
 */
WHISPER_API
int whisper_reply_friend_invite(Whisper *whisper, const char *to,
                                int status, const char *reason,
                                const char *data, size_t len);

/******************************************************************************
 * Error handling
 *****************************************************************************/

#define WSUCCESS                                0

// Facility code
#define WF_GENERAL                              1
#define WF_SYS                                  2
#define WF_HTTP                                 3
#define WF_MQTT                                 4
#define WF_ICE                                  5


#define WERR_INVALID_ARGS                       1
#define WERR_OUT_OF_MEMORY                      2
#define WERR_BUFFER_TOO_SMALL                   3
#define WERR_BAD_PERSISTENT_DATA                4
#define WERR_INVALID_PERSISTENCE_FILE           5
#define WERR_INVALID_CONTROL_PACKET             6
#define WERR_INVALID_CREDENTIAL                 7
#define WERR_SERVER_FAILED                      8
#define WERR_ALREADY_RUN                        9
#define WERR_NOT_READY                          10
#define WERR_NOT_EXIST                          11
#define WERR_ALREADY_EXIST                      12
#define WERR_NO_MATCHED_REQUEST                 13
#define WERR_INVALID_USERID                     14
#define WERR_INVALID_NODEID                     15
#define WERR_INVALID_APPID                      16
#define WERR_INVALID_DESCRIPTOR                 17
#define WERR_WRONG_STATE                        18
#define WERR_BUSY                               19
#define WERR_LANGUAGE_BINDING                   20
#define WERR_ENCRYPT                            21
#define WERR_SDP_TOO_LONG                       22
#define WERR_INVALID_SDP                        23
#define WERR_NOT_IMPLEMENTED                    24
#define WEER_UNKNOWN                            25

#define W_MK_ERROR(facility, code)  (0x80000000 | ((facility) << 24) | \
                    ((((code) & 0x80000000) >> 8) | ((code) & 0x7FFFFFFF)))

#define W_GENERAL_ERROR(code)       W_MK_ERROR(WF_GENERAL, code)
#define W_SYS_ERROR(code)           W_MK_ERROR(WF_SYS, code)
#define W_HTTP_ERROR(code)          W_MK_ERROR(WF_HTTP, code)
#define W_MQTT_ERROR(code)          W_MK_ERROR(WF_MQTT, code)
#define W_ICE_ERROR(code)           W_MK_ERROR(WF_ICE, code)

/*
 * \~English
 * Retrieves the last-error code value. The last-error code is maintained on a
 * per-instance basis. Multiple instance do not overwrite each other's
 * last-error code.
 *
 * @return
 *      The return value is the last-error code.
 *
 * \~Chinese
 * 获取最后一次错误码值。每个对象包含一个错误码。各对象彼此不使用对方的最后一次错误码值。
 *
 * @return
 *      最后一次错误码值。
 */
WHISPER_API
int whisper_get_error(void);

/**
 * \~English
 * Clear the last-error code of a Whisper instance.
 *
 * \~Chinese
 * 清除Whisper对象的最后一次错误码。
 */
WHISPER_API
void whisper_clear_error(void);

#ifdef __cplusplus
}
#endif

#if defined(__APPLE__)
#pragma GCC diagnostic pop
#endif

#endif /* __WHISPER_H_ */
