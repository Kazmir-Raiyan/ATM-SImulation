#ifndef ACCOUNTS_H_INCLUDED
#define ACCOUNTS_H_INCLUDED
#include <stdio.h>

#define total 39

typedef struct
{
    long long id;
    char name[50];
    long pin;
    long balance;
} data;

extern data accounts[total];

void load();
void save();

#endif
