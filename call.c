//
// Created by joy on 2020/8/19.
//

#include "call.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int callSystemCommand(char * downloadUrl,char * fileName){

    char * string = "curl https://codeload.github.com/junit-team/junit5/zip/main -o \"junit.zip\"";


    char * curl = (char *)malloc(strlen(downloadUrl)+strlen(fileName) + 12);


    sprintf(curl,"curl %s -o %s ",downloadUrl,fileName);

#ifdef __DEBUG__
    printf("%s\n",curl);
    return 0;
#else
    return system(curl);
#endif



}
