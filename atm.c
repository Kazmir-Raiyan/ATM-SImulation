#include "atm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void atmheader()
{
    printf("====================================\n");
    printf("           CSE115 ATM                 \n");
    printf("====================================\n\n");
}
int exitop()
{
    int nav;
    while(1)
    {
        nav = checknum("\nPress 1 to go back or 0 to quit: ");
        if(nav == 1 || nav ==0)
        {
            return nav;
            break;
        }
        else
        {
            printf("\nInvalid input. Try Again.");
            continue;
        }
    }
}
long long checknum(char *prompt)
{
    int success = 0;
    long long value;
    char term;

    while (1)
    {
        printf("%s", prompt);
        success = scanf("%lld%c", &value, &term);
        if ( success != 2 || success == 0 || term != '\n')
        {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n');
            success = 0;
            continue;
        }
        return value;
    }
}

void login(long long *id, long *pin, int *index, int *success)
{
    long long enteredid;
    long enteredpin;
    int foundindex = -1;

    while(1)
    {
        enteredid = checknum("Enter your Account no.: ");

        for(int i = 0; i < total_students; i++)
        {
            if(students[i].id == enteredid)
            {
                foundindex = i;
                break;
            }
        }
        if(foundindex == -1)
        {
            printf("Invalid ID. Try again.\n");
            continue;
        }
        break;
    }
    int attempts = 3;
    while(1)
    {
        enteredpin = checknum("Enter your 4-digit pin: ");

        if(students[foundindex].pin == enteredpin)
        {
            printf("\nWelcome! %s!", students[foundindex].name);
            break;
        }
        else if(attempts == 0)
        {
            printf("Too many unsuccessful attempts. Try again later.\n");
            break;
        }
        else
        {
            if(attempts == 1)
                printf("Incorrect Pin. You have %d attempt remaining. Try again.\n", attempts--);
            else
                printf("Incorrect Pin. You have %d attempts remaining. Try again.\n", attempts--);
                continue;
        }
    }
    *success = attempts;
    *id = enteredid;
    *pin = enteredpin;
    *index = foundindex;
}
void transact(int index, int amount, char mode)
{
    FILE *fp = fopen("transactions.txt", "a");
    if(!fp)
    {
        printf("No transactions found.\n");
    }

    time_t t = time(NULL);
    struct tm tm_ptr = *localtime(&t);

    char action[100];
    char sign;

    switch(mode)
    {
        case 'w':
            strcpy(action, "Withdraw");
            sign = '-';
            break;
        case 'd':
            strcpy(action, "Deposit");
            sign = '+';
            break;
        case 'i':
            strcpy(action, "Inflow");
            sign = '+';
            break;
        case 'o':
            strcpy(action, "Outflow");
            sign = '-';
            break;
        case 'b':
            strcpy(action, "Cashback");
            sign = '+';
            break;
    }
    fprintf(fp, "%lld|%02d:%02d|%s|%d%c\n", students[index].id, tm_ptr.tm_hour, tm_ptr.tm_min, action, amount, sign);
    fclose(fp);
}
void offers(int op)
{
    switch(op)
    {
        case 1:
            printf("\nGet 5%% cashback with our platinum debit card!");
            break;
        case 2:
            printf("\nWithdraw 2000+ and get 20 taka cashback!");
            break;
    }
}

int options()
{
    while(1)
    {
        int op = checknum("\n1. Check Balance\n2. Withdraw\n3. Deposit\n4. Transfer\n5. Mini Statement\n6. Change Pin\n7. Card Upgrade\n8. Log Out\nEnter your selection: ");
        if(op == 1 || op == 2 || op == 3 || op == 4 || op == 5 || op == 6 || op == 7 || op == 8)
        {
            return op;
            break;
        }
        else
        {
            printf("Invalid input. Please try again.");
            continue;
        }
    }
}
int accinfo(int index)
{
    int nav;
    printf("\nAccount number: %lld\nAccount Name: %s\nCard Type: %s\nAvailable Balance: %ld Tk", students[index].id, students[index].name, students[index].card, students[index].balance);
    nav = exitop();
    return nav;
}
int deposit(int index)
{
    int nav;
    long input;
    printf("\nAvailable Balance: %ld\n", students[index].balance);
    while(1)
    {
        input = checknum("\nEnter the amount to Deposit: ");
        if(input <= 0)
        {
            printf("\nInvalid Deposit. Try again");
            continue;
        }
        else
        {
            students[index].balance += input;
            transact(index, input, 'd');
            savestudents();
            printf("\nSuccess! Your account has been credited by %ld Tk", input);
            nav = exitop();
            break;
        }
    }
    return nav;
}
int withdraw(int index)
{
    int nav;
    long input;
    printf("\nAvailable Balance: %ld\n", students[index].balance);
    while(1)
    {
        input = checknum("\nEnter the amount to Withdraw: ");
        if(input > students[index].balance)
        {
            printf("\nInsufficient Balance. Try again");
            continue;
        }
        else
        {
            students[index].balance -= input;
            transact(index, input, 'w');
            printf("\nSuccess! Your account has been debited by %ld Tk", input);
            if(input >= 2000)
            {
                students[index].balance += 20;
                transact(index, 20, 'b');
                printf("\nYou got 20 taka cashback!");
            }
            savestudents();
            nav = exitop();
            break;
        }
    }
    return nav;
}
int transfer(int index)
{
    char confirm[50];
    int nav, amount, index2 = -1;
    long long check;
    while(1)
    {
        check = checknum("\nEnter the recipient Account no.: ");
        for(int i = 0; i < total_students; i++)
        {
            if(check == students[i].id)
            {
                index2 = i;
                break;
            }
        }
        if(index2 == -1)
        {
            printf("Invalid ID. Try again.\n");
            continue;
        }
        if(students[index2].id == students[index].id)
        {
            printf("\nYou cannot enter your own Id. Try again.");
            continue;
        }
        while(1)
        {
            amount = checknum("Enter amount: ");
            if(amount > students[index].balance)
            {
                printf("\nInsufficient balance. Try again.\n");
                continue;
            }
            printf("Are you sure you want to transfer %d to %s? (y/n) : ", amount, students[index2].name);
            gets(confirm);
            if(strcmp(confirm, "y") == 0)
            {
                students[index].balance -= amount;
                students[index2].balance += amount;
                transact(index, amount, 'o');
                transact(index2, amount, 'i');
                savestudents();
                printf("Fund transfer successful!");
                nav = exitop();
                break;
            }
            else if(strcmp(confirm, "n") == 0)
            {
                printf("Transfer terminated.");
                nav = exitop();
                break;
            }
            else
            {
                printf("Invalid Input, Try again.");
                continue;
            }
        }
        return nav;
        break;
    }
}
int statement(int index)
{
    int nav, count = 0;
    FILE *fp = fopen("transactions.txt", "r");
    if(!fp)
    {
        printf("error");
    }
    char line[100];
    printf("\nMINI-STATEMENT\tID: %lld\n", students[index].id);
    printf("\nTime\tType\t      Amount");
    printf("\n----------------------------\n");

    while(fgets(line, sizeof(line), fp) != NULL)
    {
        long long id;
        char time[6], type[20];
        char sign;
        int amount;

        sscanf(line, "%lld|%5[^|]|%19[^|]|%d%c", &id, time, type, &amount, &sign);

        if(id == students[index].id)
        {
            printf("%s\t%-9s\t%d%c\n", time, type, amount, sign);
            count++;
        }
    }
    if(count == 0)
    {
        printf("\nNo recent transactions.\n");
    }
    printf("----------------------------");
    printf("\nAvailable Balance:\tTk.%ld", students[index].balance);
    printf("\nThank You!\n");
    nav = exitop();
    return nav;
}
int changepin(int index)
{
    int newpin, nav, pin;
    char verify[50];
    while(1)
    {
        pin = checknum("\nEnter current PIN: ");
        if(pin == students[index].pin)
        {
            while(1)
            {
                newpin = checknum("Enter new PIN: ");
                if(newpin <= 9999 && newpin >= 0001)
                {
                    printf("Are you sure you want %d to be your new PIN? (y/n): ", newpin);
                    gets(verify);
                    if(strcmp(verify, "y") == 0)
                    {
                        students[index].pin = newpin;
                        savestudents();
                        printf("PIN reset successful!");
                        nav = exitop();
                        break;
                    }
                    else if(strcmp(verify, "n") == 0)
                    {
                        continue;
                    }
                    else
                    {
                        printf("Invalid Input, Try again.");
                        continue;
                    }
                }
                else
                {
                    printf("Please enter a 4-Digit PIN.\n");
                    continue;
                }
            }
            return nav;
        }
        else
        {
            printf("Incorrect PIN. Try again\n");
            continue;
        }
    }
}
int upgrade(int index)
{
    while(1)
    {
        printf("\n\033[1mCurrent Plan: \033[0m");
        printf("%s\n", students[index].card);
        printf("\033[1mAvailable Offers: \033[0m\n");
        printf("\n\033[1mStandard: \033[0m\n");
        printf("1. Visa Classic\n");
        printf("2. Mastarcard Standard\n");
        printf("3. Unionpay Classic\n");
        printf("\n\033[1mPremium: \033[0m\n");
        printf("4. Visa Platinum\n");
        printf("5. Visa Signature\n");
        printf("6. Visa Infinite\n");
        printf("\n\033[1mExclusive: \033[0m\n");
        printf("7. Mastercard World\n");
        printf("8. American Express Gold\n");
        printf("9. American Express Platinum\n");
        break;
    }
}
int logout()
{
    char confirm[50];
    while(1)
    {
        printf("\nAre you sure you want to log out? (y/n): ");
        gets(confirm);
        if(strcmp(confirm, "y") == 0)
        {
            printf("\nLogging out...\n");
            return 0;
            break;
        }
        else if(strcmp(confirm, "n") == 0)
        {
            printf("\nProcess Terminated.");
            return 1;
            break;
        }
        else
        {
            printf("Invalid input. Try again.");
            continue;
        }
    }
}
