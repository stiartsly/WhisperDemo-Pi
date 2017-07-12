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

char buff[8*1024];

static const char *host = "127.0.0.1";
static int port = 12300;
static bool running = false;
static pthread_t camera_thread;

static void (*streamFwdCb)(void *, int, void *) = NULL;
static void* streamFwdArgv = NULL;

void camera_set_callbacks(void *streamCb, void *context)
{
    streamFwdCb = (void (*)(void *, int, void *))streamCb;
    streamFwdArgv = context;
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

            memset(&buff, 0, sizeof(buff));
            memset(&raddr,0, sizeof(raddr));

            rc = recvfrom(sock, buff, sizeof(buff), 0, (struct sockaddr *)&raddr, &addrlen);
            if (rc < 0) {
                printf("recvfrom error: %d\n", errno);
                close(sock);
                return NULL;
            }

            //printf("received data length: %d from %s\n", rc, inet_ntoa(raddr.sin_addr));
            if (streamFwdCb)
                streamFwdCb(buff, rc, streamFwdArgv);
        }
    }

    close(sock);
    return NULL;
}

int camera_open(void)
{
    int sock;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        printf("socket error: %d\n", errno);
        return -1;
    }

    int tmp = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tmp, sizeof(int));
    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);

    int rc;
    rc = inet_aton(host, &saddr.sin_addr);
    if (rc == 0) {
        printf("inet_aton error\n");
        close(sock);
        return -1;
    }

    rc = bind(sock, (struct sockaddr *)&saddr, sizeof(saddr));
    if (rc == -1) {
        printf("bind error: %d\n", errno);
        close(sock);
        return -1;
    }

    printf("binded to stream port:%s:%d successfully\n", host, port);

    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    rc = pthread_create(&camera_thread, &attr, camera_forwarding_routine, (void*)sock);
    if (rc < 0) {
        printf("create camera forwarding thread error :%d\n", rc);
        close(sock);
        return -1;
    }

    return 0;
}

void camera_close(void)
{
    running = false;
}

int camera_flip(void)
{
    //TODO;
    return 0;
}
