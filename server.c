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
    int destination_port;

    if (argc == 2){
        if ((strcmp(argv[1],"--help") == 0)){
            printf("Usage: %s <PARAMETERS>\n",argv[0]);
            printf("\n  Where PARAMETERS are:\n");
            printf("    <filename>\tThe name of file to be written\n");
            printf("    <windowsize>\tThe size of window for sliding window protocol\n");
            printf("    <buffersize>\tThe size of buffer available for holding the data from transfer\n");
            printf("    <port>\tThe port of connection\n");
            printf("All parameters are mandatory for the application.\n");
        } else {
            printf("%s: invalid option %s\nTry '%s --help' for more information.\n",argv[0],argv[1],argv[0]);
        }
        exit(0);
    } else if (argc != 5){
        printf("%s: missing parameter\nTry '%s --help' for more information.\n",argv[0],argv[0]);
        exit(0);
    } else {
        if(checkIfNumbers(argv[2],0) && checkIfNumbers(argv[3],0) && checkIfNumbers(argv[4],1) && checkIfNumbers(argv[4],0)){
            filename = malloc(sizeof(argv[1]));
            strcpy(filename,argv[1]);
            window_size = atoi(argv[2]);
            buffer_size = atoi(argv[3]);
            destination_port = atoi(argv[4]);
        } else {
            printf("%s: wrong parameter(s)\nTry '%s --help' for more information.\n",argv[0],argv[0]);
            exit(0);
        }
    }

    printf("%s %d %d %d\n",filename,window_size,buffer_size,destination_port);

    free(filename);
    return 0; 
} 
