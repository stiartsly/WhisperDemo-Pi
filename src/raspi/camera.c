#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char data[8*1024];

static const char *host = "127.0.0.1";
static int stream_port = 12300;
static int width = 320;
static int height = 240;
static int bitrate = 150000;
static int framerate = 30;
static int profile = 0;

static bool running = false;
static pthread_t camera_thread;

static void (*streamFwdCb)(void *, int, void *) = NULL;
static void* streamFwdArgv = NULL;

void camera_set_callbacks(void *streamCb, void *context)
{
    streamFwdCb = (void (*)(void *, int, void *))streamCb;
    streamFwdArgv = context;
}

void camera_set_port(int port)
{
    stream_port = port;
}

void camera_set_parameters(int w, int h, int br, int fps, int pf)
{
    width = w;
    height = h;
    bitrate = br;
    framerate = fps;
    profile = pf;
}

static pid_t raspivid_pid = -1;

static int raspivid_open(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("fork: ");
        return -1;
    } else if (pid == 0) {
        int rc  = 0;
        int off = 0;
        int i = 0;
        char buf[1024] = {0};
        char *argv[32] = { NULL };

        rc = sprintf(buf + off, "raspivid");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-v");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-o");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "udp://127.0.0.1:%d", stream_port);
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-w");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "%d", width);
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-h");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "%d", height);
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-pf");
        argv[i++] = buf + off;
        off += rc + 1;

        if (profile == 2)
            rc = sprintf(buf + off, "high");
        else if (profile == 1)
            rc = sprintf(buf + off, "main");
        else
            rc = sprintf(buf + off, "baseline");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-b");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "%d", bitrate);
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-fps");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "%d", framerate);
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-t");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "0");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-ih");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-fl");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-g");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "10");
        argv[i++] = buf + off;
        off += rc + 1;

        rc = sprintf(buf + off, "-n");
        argv[i++] = buf + off;
        off += rc + 1;

#if 0
        for (i = 0; argv[i] != NULL; i++)
            printf("argv[%d]: %s\n",i, argv[i]);
#endif

        rc = execvp(argv[0], argv);
        if (rc < 0) {
            perror("execv:");
            return -1;
        }
        exit(0);

    } else {
        raspivid_pid = pid;
    }

    return 0;
}

static void raspivid_close(void)
{
    if (raspivid_pid > 0)
        kill(raspivid_pid, SIGKILL);
}

static
void *camera_forwarding_routine(void *argv)
{
    int sock = (int)argv;
    int rc;

    running = true;

    while(running) {
        fd_set rfds;

        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);

        rc = select(sock + 1, &rfds, NULL, NULL, NULL);
        if (rc < 0) {
            printf("select error: \n");
            close(sock);
            return NULL;
        }

        if (FD_ISSET(sock, &rfds)) {
            struct sockaddr_in raddr;
            socklen_t addrlen = sizeof(raddr);

            //memset(&data, 0, sizeof(data));
            memset(&raddr,0, sizeof(raddr));

            rc = recvfrom(sock, data, sizeof(data), 0, (struct sockaddr *)&raddr, &addrlen);
            if (rc < 0) {
                printf("recvfrom error: %d\n", errno);
                close(sock);
                return NULL;
            }

            if (streamFwdCb)
                streamFwdCb(data, rc, streamFwdArgv);
        }
    }

    close(sock);
    return NULL;
}

int camera_open(void)
{
    int sock;

    if (raspivid_open() < 0) {
        printf("open raspivid error");
        return -1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket error: %d\n", errno);
        raspivid_close();
        return -1;
    }

    int tmp = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(stream_port);

    int rc;
    rc = inet_aton(host, &saddr.sin_addr);
    if (rc == 0) {
        printf("inet_aton error\n");
        close(sock);
        raspivid_close();
        return -1;
    }

    rc = bind(sock, (struct sockaddr *)&saddr, sizeof(saddr));
    if (rc == -1) {
        printf("bind error: %d\n", errno);
        close(sock);
        raspivid_close();
        return -1;
    }

    printf("binded to stream port:%s:%d successfully\n", host, stream_port);

    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    rc = pthread_create(&camera_thread, &attr, camera_forwarding_routine, (void*)sock);
    if (rc < 0) {
        printf("create camera forwarding thread error :%d\n", rc);
        close(sock);
        raspivid_close();
        return -1;
    }

    return 0;
}

void camera_close(void)
{
    running = false;
    raspivid_close();
}

int camera_flip(void)
{
    //TODO;
    return 0;
}
