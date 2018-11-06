#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "utilities.h"

int main(int argc, char** argv) {
    char* filename;
    int window_size;
    int buffer_size;
    char* destination_ip;
    int destination_port;

    if (argc == 2){
        if ((strcmp(argv[1],"--help") == 0)){
            printf("Usage: %s <PARAMETERS>\n",argv[0]);
            printf("\n  Where PARAMETERS are:\n");
            printf("    <filename>\tThe name of file to be sent\n");
            printf("    <windowsize>\tThe size of window for sliding window protocol\n");
            printf("    <buffersize>\tThe size of buffer available for holding the file data\n");
            printf("    <destination_ip>\tThe IP address of the receiver\n");
            printf("    <destination_port>\tThe port of connection\n");
            printf("All parameters are mandatory for the application.\n");
        } else {
            printf("%s: invalid option %s\nTry '%s --help' for more information.\n",argv[0],argv[1],argv[0]);
        }
        exit(0);
    } else if (argc != 6){
        printf("%s: missing parameter\nTry '%s --help' for more information.\n",argv[0],argv[0]);
        exit(0);
    } else {
        if(checkIfNumbers(argv[2],0) && checkIfNumbers(argv[3],0) && checkIfNumbers(argv[4],1) && checkIfNumbers(argv[5],0)){
            filename = malloc(sizeof(argv[1]));
            strcpy(filename,argv[1]);
            window_size = atoi(argv[2]);
            buffer_size = atoi(argv[3]);
            destination_ip = malloc(sizeof(argv[4]));
            strcpy(destination_ip,argv[4]);
            destination_port = atoi(argv[5]);
        } else {
            printf("%s: wrong parameter(s)\nTry '%s --help' for more information.\n",argv[0],argv[0]);
            exit(0);
        }
    }

    printf("%s %d %d %s %d\n",filename,window_size,buffer_size,destination_ip,destination_port);

    free(filename);
    free(destination_ip);
    return 0; 
} 
