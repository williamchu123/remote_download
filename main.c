

#include <stdio.h>
#include "conc.h"
#include "call.h"
#include "utils.h"
#include <unistd.h>
#include <string.h>


int main() {



    printf("remote_download server starting......!\n");


//    pid_t pid = getpid();
//
//
//    printf("%d\n",pid);


//    utils_set_daemon();



//    char * url = "https://codeload.github.com/junit-team/junit5/zip/main";
//
//    char * fileName = "junit.zip";
//
//    callSystemCommand(url,fileName);


    openConnection(8001);



    return 0;
}
