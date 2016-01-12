#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>

#include "ev3test.h"

#define BUFLEN 32

static struct UiInstance UiInstance;

static void *button_thread_start(void *arg)
{
    UI *pUi = (UI*) arg;

    while (1) {

        if (pUi->Pressed[BUT0] == 1) {
            printf("%s(): Button UP is pressed\n", __func__);
        }

        if (pUi->Pressed[BUT1] == 1) {
            printf("%s(): Button ENTER is pressed\n", __func__);
        }

        if (pUi->Pressed[BUT2] == 1) {
            printf("%s(): Button DOWN is pressed\n", __func__);
        }

        if (pUi->Pressed[BUT3] == 1) {
            printf("%s(): Button RIGHT is pressed\n", __func__);
        }

        if (pUi->Pressed[BUT4] == 1) {
            printf("%s(): Button LEFT is pressed\n", __func__);
        }

        if (pUi->Pressed[BUT5] == 1) {
            printf("%s(): Button BACK is pressed\n", __func__);
        }

        usleep(500);
    }

    return NULL;
}

static void *led_thread_start(void *arg)
{
    int state;
    char buf[2];

    while (1) {

        for (state = LED_BLACK; state < LEDPATTERNS; state++) {

            printf("%s(): state: %d\n", __func__, state);

            UiInstance.ledstate = state;

            if (UiInstance.warnlight) {

                if ((state == LED_GREEN_FLASH) || (state == LED_RED_FLASH) || (state == LED_ORANGE_FLASH)) {

                    buf[0] = LED_ORANGE_FLASH + '0';
                } else {

                    if ((state == LED_GREEN_PULSE) || (state == LED_RED_PULSE) || (state == LED_ORANGE_PULSE)) {

                        buf[0] = LED_ORANGE_PULSE + '0';
                    } else {

                        buf[0] = LED_ORANGE + '0';
                    }
                }
            } else {

                buf[0] = UiInstance.ledstate + '0';
            }

            buf[1] = 0;

            write(UiInstance.fd, buf, 2);

            sleep(5);
        }
    }

    return NULL;
}

int ui_init(void)
{
    int fd;
    int n, ret;
    char buf[BUFLEN];
    pthread_t button_thread, led_thread;

    UI *pUi = NULL;

    if ((fd = open(UI_DEVICE_NAME, O_RDWR | O_SYNC)) == -1) {

        fprintf(stderr, "open() failed\n");
        return -1;
    }

    UiInstance.fd = fd;

    if ((n = read(fd, buf, BUFLEN)) > 0) {
        printf("hardware version: %s\n", buf);
    }

    if ((pUi = (UI*) mmap(NULL, sizeof(UI), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {

        fprintf(stderr, "mmap() failed\n");
        return -1;

    }

    ret = pthread_create(&button_thread, NULL, button_thread_start, pUi);
    if (ret) {
        fprintf(stderr, "pthread_create() failed\n");
        return -1;
    }

    ret = pthread_create(&led_thread, NULL, led_thread_start, NULL);
    if (ret) {
        fprintf(stderr, "pthread_create() failed\n");
        return -1;
    }

    return 0;
}
