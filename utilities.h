#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#define ASCII_SOH 0x01
#define ASCII_ACK 0x06
#define ASCII_NAK 0x15

struct packet_payload {
    char SOH;
    int sequence_number;
    int data_length;
    char data[1024];
    unsigned char checksum;
};

struct packet_ack {
    char ACK;
    int next_sequence_number;
    unsigned char checksum;
};

int checkIfNumbers(char* param,int ip_flag);

#endif