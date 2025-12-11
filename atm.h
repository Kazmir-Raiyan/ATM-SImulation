#ifndef ATM_H_INCLUDED
#define ATM_H_INCLUDED
#include "accounts.h"

void atmheader();
int exitop();
long long checknum(char prompt[]);

void login(long long *id, long *pin, int *index, int *success);
void transact(int index, int amount, char mode);
void offers(int op);

int options();
int accinfo(int index);
int deposit(int index);
int withdraw(int index);
int transfer(int index);
int statement(int index);
int changepin(int index);
int more(int index);
int logout();

#endif
