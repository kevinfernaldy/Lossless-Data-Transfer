//Implementation file of procedures and function from utilites.h
#include <string.h>
#include "utilities.h"

// Checks if the parameter is a number or not. Flag is used for special case in which "." / ASCII 0x2E is included
int checkIfNumbers(char* param,int ip_flag){
    int check = 1;

    for (int i = 0; i < strlen(param); i++){
        if (ip_flag){
            if (((param[i] < 0x30) || (param[i] > 0x39)) && (param[i] != 0x2E)){
                check = 0;
                break;
            }
        } else {
            if ((param[i] < 0x30) || (param[i] > 0x39)){
                check = 0;
                break;
            }
        }
    }
    return check;
}