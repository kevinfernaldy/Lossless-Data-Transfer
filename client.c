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

void initialize(struct packet_payload *packets, int buffer_size){
    for (int i = 0; i < buffer_size; i++){
        packets[i].SOH = 0x00;
    }
}

void cleanBuffer(char* buffer, int buffer_size){
    for (int i = 0; i < buffer_size; i++){
        buffer[i] = '\0';
    }
}

unsigned char generateChecksum(char* data, int buffer_size){
    unsigned int total = 0;
    for (int i = 0; i < buffer_size; i++){
        total += data[i];
    }

    return (total ^ 0xFF);
}

void importData(struct packet_payload *packets, int buffer_size, char* filename){
    char buffer[MAXBUFFER];
    size_t actual_reads;
    FILE * fo = fopen(filename,"rb");

    initialize(packets,buffer_size);
    for (int i = 0; i < buffer_size; i++){
        cleanBuffer(buffer,MAXBUFFER);
        actual_reads = fread(buffer,1,MAXBUFFER,fo);
        packets[i].SOH = ASCII_SOH;
        packets[i].sequence_number = i+1;
        packets[i].data_length = actual_reads;
        memcpy((void *)&(packets[i].data),(const void *)buffer,MAXBUFFER);
        packets[i].checksum = generateChecksum(buffer,MAXBUFFER);

        if (actual_reads < MAXBUFFER){
            break;
        }
    }

    fclose(fo);
}

int main(int argc, char** argv) {
    char* filename;
    int window_size;
    int buffer_size;
    char* destination_ip;
    int destination_port;

    // All parameters passed checking goes here
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

    int sockfd;
    struct packet_payload packets[buffer_size];
    struct sockaddr_in server_address;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
        perror("ERROR Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(destination_port);
    server_address.sin_addr.s_addr = inet_addr(destination_ip);

    importData(packets,buffer_size,filename);

    int i = 0;
    while((i < MAXBUFFER) && (packets[i].SOH != 0x00)){
        sendto(sockfd, (const struct packet_payload *)packets[i], sizeof(packets), MSG_CONFIRM, (const struct sockaddr *) &server_address, sizeof(server_address));
        printf("Packet %d sent.\n",packets[i].sequence_number);
        i++;
    }
    // int i = 0;
    // while((i < MAXBUFFER) && (packets[i].SOH != 0x00)){
    //     printf("SOH = %d\n",packets[i].SOH);
    //     printf("sequence_number = %d\n",packets[i].sequence_number);
    //     printf("data_length = %d\n",packets[i].data_length);
    //     for (int j = 0; j < packets[i].data_length; j++){
    //         printf("%c",packets[i].data[j]);
    //     }
    //     printf("\nchecksum = %x\n\n",packets[i].checksum);
    //     i++;
    // }

    free(filename);
    free(destination_ip);
    return 0; 
} 
