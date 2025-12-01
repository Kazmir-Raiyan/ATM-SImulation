#ifndef STUDENTS_H_INCLUDED
#define STUDENTS_H_INCLUDED
#include <stdio.h>

#define total_students 39

typedef struct
{
    long long id;
    char name[50];
    long pin;
    long balance;
    char card[100];
} data;

extern data students[total_students];

void loadstudents();
void savestudents();

#endif
