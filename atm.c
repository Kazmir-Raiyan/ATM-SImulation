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

        for(int i = 0; i < total; i++)
        {
            if(accounts[i].id == enteredid)
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

        if(accounts[foundindex].pin == enteredpin)
        {
            printf("\nWelcome! %s!", accounts[foundindex].name);
            break;
        }
        else if(attempts == 0)
        {
            printf("\nToo many unsuccessful attempts. Try again later.\n");
            exit(1);
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
        case 'B':
            strcpy(action, "Bonus");
            sign = '+';
            break;
    }
    fprintf(fp, "%lld|%02d:%02d|%s|%d%c\n", accounts[index].id, tm_ptr.tm_hour, tm_ptr.tm_min, action, amount, sign);
    fclose(fp);
}
void offers(int op)
{
    switch(op)
    {
        case 1:
            printf("\nDeposit 4000+ for a 20 taka bonus!");
            break;
        case 2:
            printf("\nWithdraw 2000+ and get 20 taka cashback!");
            break;
        case 3:
            printf("\nTransfer 5000+ and get 50 taka cashback!");
    }
}

int options()
{
    while(1)
    {
        int op = checknum("\n1. Account Information\n2. Withdraw\n3. Deposit\n4. Transfer\n5. Mini Statement\n6. Change Pin\n7. More\n8. Log Out\n9. Quit\nEnter your selection: ");
        if(op == 1 || op == 2 || op == 3 || op == 4 || op == 5 || op == 6 || op == 7 || op == 8)
        {
            return op;
            break;
        }
        else if(op == 9)
        {
            return 0;
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
    printf("\nAccount number: %lld\nAccount Name: %s\nAvailable Balance: %ld Tk", accounts[index].id, accounts[index].name, accounts[index].balance);
    nav = exitop();
    return nav;
}
int deposit(int index)
{
    int nav;
    long input;
    printf("\nAvailable Balance: %ld\n", accounts[index].balance);
    while(1)
    {
        input = checknum("\nEnter the amount to Deposit (0 to cancel): ");
        if(input < 0)
        {
            printf("\nInvalid Deposit. Try again");
            continue;
        }
        else if(input == 0)
        {
            printf("Process Terminated\n");
            return 1;
            break;
        }
        else
        {
            accounts[index].balance += input;
            transact(index, input, 'd');
            printf("\nSuccess! Your account has been credited by %ld Tk", input);
            if(input >= 4000)
            {
                accounts[index].balance += 20;
                transact(index, 20, 'B');
                printf("\nYou got 20 taka bonus!");
            }
            save();
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
    printf("\nAvailable Balance: %ld\n", accounts[index].balance);
    while(1)
    {
        input = checknum("\nEnter the amount to Withdraw (0 to cancel): ");
        if(input > accounts[index].balance)
        {
            printf("\nInsufficient Balance. Try again");
            continue;
        }
        else if(input == 0)
        {
            printf("Process Terminated\n");
            return 1;
            break;
        }
        else
        {
            accounts[index].balance -= input;
            transact(index, input, 'w');
            printf("\nSuccess! Your account has been debited by %ld Tk", input);
            if(input >= 2000)
            {
                accounts[index].balance += 20;
                transact(index, 20, 'b');
                printf("\nYou got 20 taka cashback!");
            }
            save();
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
        check = checknum("\nEnter the recipient Account no. (0 to cancel): ");
        if(check == 0)
        {
            printf("Process Terminated.\n");
            return 1;
            break;
        }
        for(int i = 0; i < total; i++)
        {
            if(check == accounts[i].id)
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
        if(accounts[index2].id == accounts[index].id)
        {
            printf("\nYou cannot enter your own Id. Try again.");
            continue;
        }
        while(1)
        {
            amount = checknum("Enter amount (0 to cancel): ");
            if(amount == 0)
            {
                printf("Process Terminated.\n");
                return 1;
                break;
            }
            if(amount > accounts[index].balance)
            {
                printf("\nInsufficient balance. Try again.\n");
                continue;
            }
            printf("Are you sure you want to transfer %d to %s? (y/n) : ", amount, accounts[index2].name);
            gets(confirm);
            if(strcmp(confirm, "y") == 0)
            {
                accounts[index].balance -= amount;
                accounts[index2].balance += amount;
                transact(index, amount, 'o');
                transact(index2, amount, 'i');
                printf("Fund transfer successful!");
                if(amount > 5000)
                {
                    accounts[index].balance += 50;
                    transact(index, 50, 'B');
                    printf("\nYou got 50 taka cashback!");
                }
                save();
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
    printf("\nMINI-STATEMENT\tID: %lld\n", accounts[index].id);
    printf("\nTime\tType\t      Amount");
    printf("\n----------------------------\n");

    long long id;
    char time[6], type[20];
    char sign;
    int amount;

    while((fscanf(fp, "%lld|%5[^|]|%19[^|]|%d%c", &id, time, type, &amount, &sign)) == 5);
    {
        if(id == accounts[index].id)
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
    printf("\nAvailable Balance:\tTk.%ld", accounts[index].balance);
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
        pin = checknum("\nEnter current PIN (0 to cancel): ");
        if(pin == accounts[index].pin)
        {
            while(1)
            {
                newpin = checknum("Enter new PIN (0 to cancel): ");
                if(newpin <= 9999 && newpin >= 0001)
                {
                    printf("Are you sure you want %d to be your new PIN? (y/n): ", newpin);
                    gets(verify);
                    if(strcmp(verify, "y") == 0)
                    {
                        accounts[index].pin = newpin;
                        save();
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
                else if(newpin == 0)
                {
                    printf("Process Terminated.\n");
                    return 1;
                }
                else
                {
                    printf("Please enter a 4-Digit PIN.\n");
                    continue;
                }
            }
            return nav;
        }
        else if(pin == 0)
        {
            printf("Process Terminated.\n");
            return 1;
        }
        else
        {
            printf("Incorrect PIN. Try again\n");
            continue;
        }
    }
}
int more(int index)
{
    int op;
    printf("\n1. Bill Payment\n2. Offers\n3. Vouchers\n");
    printf("Enter your selection: ");
    scanf("%d", &op);

    switch(op)
    {
        case 2:
            printf("- 4000+ taka deposit: 20 taka bonus.\n" );
            printf("- 2000+ taka withdrawal: 20 taka cashback.\n");
            printf("- 5000+ taka transfer: 50 taka cashback.\n");
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
            return -1;
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
