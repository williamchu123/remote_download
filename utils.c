//
// Created by joy on 2020/8/20.
//
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

/* Run current process in background mode (daemon, evil Monkey >:) */
int utils_set_daemon() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("%s\n", "Error: Failed creating to switch to daemon mode(fork failed)");

        fflush(stdout);

        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }




    /* set files mask */
    umask(0);

    /* Create new session */
    setsid();

    if (chdir("/") < 0) { /* make sure we can unmount the inherited filesystem */
        printf("%s\n", "Error: Unable to unmount the inherited filesystem in the daemon process");
        exit(EXIT_FAILURE);
    }

    /* Our last STDOUT messages */

    printf("%s\n", "Background mode ON");


    fclose(stderr);
    fclose(stdout);

    return 0;
}


void rtrim(char * pstr,char trim){

    if(pstr == NULL ){
        return;
    }
    size_t len = strlen(pstr);

    for (int i = len; i > 0; --i) {
        if(*(pstr+i) == trim){
            *(pstr+i) = 0;
        }
    }
}




