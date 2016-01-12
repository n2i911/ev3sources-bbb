#include <stdio.h>
#include <unistd.h>

#include "ev3test.h"

int main(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "m:")) != -1) {
        switch (opt) {
        case 'm':
            break;

        default:
            break;
        }
    }

    /* testing of d_ui */
    ui_init();

    while (1);

    return 0;
}
