//
// Created by joy on 2020/8/20.
//


#include "conc.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "call.h"


void openConnection(int port) {

    int server_fd;

    struct sockaddr_in server_addr_in;



    //
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() error. Failed to initiate a socket");
    }



    //进行初始化服务器端配置

    memset(&server_addr_in, 0, sizeof(struct sockaddr_in));//全部初始化为0

    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_port = htons(port);
    server_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
//    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));



    if (bind(server_fd, (struct sockaddr *) &server_addr_in, sizeof(server_addr_in)) == -1) {
        perror("bind() failed");
    }

    printf("the server started...\n");
    printf("listen on port:%d\n", port);
    printf("waiting for client...\n");


    if (listen(server_fd, 128) == -1) {
        perror("listen failed");
    }



//    struct sockaddr_in client_sock_in;



    while (1){
        struct sockaddr_in * pSockAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));

        socklen_t socket_id = sizeof(struct sockaddr_in);

        int client_fd = accept(server_fd, (struct sockaddr *) pSockAddr, &socket_id);

        if(client_fd == -1){
            goto close;
        }


        int * p_client_fd = (int *)malloc(sizeof(int));
        *p_client_fd = client_fd;


        pthread_t * ppthread = (pthread_t *)malloc(sizeof(pthread_t));

        if(pthread_create(ppthread, NULL, handle, p_client_fd) != 0){
            perror("创建线程失败！！！");
            goto close;
        }


    }


    //进行资源的释放
    close:
    close(server_fd);

}


void *handle(void *arg) {

    int client_fd = *((int*)arg);

    printf("当前fd:%d",client_fd);

    char buf[1024];

    int count = 0;
    while (1){
        if((count = read(client_fd,buf,1024)) == -1){
            goto close;
        }


        buf[count] = '\0';

        printf("开始下载%s",buf);

        //https://codeload.github.com/junit-team/junit5/zip/main junit5.zip

        //

        char * s_buf = (char *)malloc(sizeof(char)*(count+1));


        strncpy(s_buf,buf,count);


        char * p1  = strsep(&s_buf," ");

        size_t sdl1 = strlen(p1);

        char *download_url =  (char *)malloc(sizeof(char)*(sdl1+1));



        strcpy(download_url,p1);


        char * p2 = strsep(&s_buf,"");

        size_t sdl2 = strlen(p2);

        char *download_file_name =  (char *)malloc(sizeof(char)*(sdl2+1));

        strcpy(download_file_name,p2);


        printf("\ndownload_url:%s download_file_name:%s\n",download_url,download_file_name);

        callSystemCommand(download_url,download_file_name);


        write(client_fd,buf,count);
    }

    close:
    close(client_fd);

    return NULL;

}



