#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "utilities.h"

#define MAXBUFFER 1024

int main(int argc, char** argv) {
    char* filename;
    int window_size;
    int buffer_size;
    int port;

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
            port = atoi(argv[4]);
        } else {
            printf("%s: wrong parameter(s)\nTry '%s --help' for more information.\n",argv[0],argv[0]);
            exit(0);
        }
    }

    printf("%s %d %d %d\n",filename,window_size,buffer_size,port);

    int sockfd;
    struct sockaddr_in server_address, client_address;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        perror("ERROR Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));
    memset(&client_address, 0, sizeof(client_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("ERROR Binding server information into socket failed");
        exit(EXIT_FAILURE);
    }

    struct packet_payload packets[buffer_size];

    int i = 0;
    for(;;){
        struct packet_payload temp_packet;
        int packet_size_read, dummy;

        packet_size_read = recvfrom(sockfd, (struct packet_payload *)&temp_packet, sizeof(temp_packet), MSG_WAITALL, (struct sockaddr *) &client_address, &dummy);
        memcpy((void *)&(packets[i]),(const void *)&temp_packet,sizeof(temp_packet));
        i++;

        if ((temp_packet.data_length < MAXBUFFER) || (i >= buffer_size)){
            break;
        }
    }

    i = 0;
    while((i < MAXBUFFER) && (packets[i].SOH != 0x00)){
        printf("SOH = %d\n",packets[i].SOH);
        printf("sequence_number = %d\n",packets[i].sequence_number);
        printf("data_length = %d\n",packets[i].data_length);
        for (int j = 0; j < packets[i].data_length; j++){
            printf("%c",packets[i].data[j]);
        }
        printf("\nchecksum = %x\n\n",packets[i].checksum);
        i++;
    }
    free(filename);
    return 0; 
} 
