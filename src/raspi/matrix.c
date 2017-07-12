#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

void display(uint8_t *buf)
{
    uint8_t data[4] = {0x0,0x0,0x0,0x0};
    int j;

    for (j=0; j<8; j++) {
        data[0] = ~buf[j];
        data[2] = 0xff;
        data[1] = 0xff;
        data[3] = 0x01 << j ;

        wiringPiSPIDataRW(0,data,sizeof(data));
        delay(2);
    }
}

void turnon(void)
{
    uint8_t heart[8] = {0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18};
    display(heart);
}

void turnoff(void)
{
    uint8_t heart[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    display(heart);
}

static bool matrix_on = false;
static bool running = false;
static pthread_t matrix_thread;

void *matrix_routine(void *argv)
{
    running = true;

    while (running) {
        if (matrix_on)
            turnon();
        else
            turnoff();
    }

    turnoff();
    return NULL;
}

int matrix_open(void)
{
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int rc;

    rc = pthread_create(&matrix_thread, &attr, matrix_routine, NULL);
    pthread_attr_destroy(&attr);

    if (rc != 0) {
        printf("create matrix thread error (%d)\n", rc);
        return -1;
    }
}

int matrix_flip(void)
{
    matrix_on = !matrix_on;
    return 0;
}

void matrix_close(void)
{
    matrix_on = false;
    sleep(1);
    running = false;
}
