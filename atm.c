#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#define total_students 39

typedef struct
{
  long long id;
  char name[50];
  long pin;
  long balance;
}data;

data students[total_students];

void loadstudents()
{
    FILE *fp = fopen("info.txt", "r");

    if(fp == NULL)
    {
        printf("Error loading data.\n");
        exit(1);
    }

    for(int i = 0; i < total_students; i++)
    {
        char line[200];
        if(fgets(line, sizeof(line), fp) != NULL)
        {
            sscanf(line, "%lld|%49[^|]|%ld|%ld", &students[i].id, students[i].name, &students[i].pin, &students[i].balance);
        }
    }
    fclose(fp);
}
void savestudents()
{
    FILE *fp = fopen("info.txt", "w");
    if(fp == NULL)
    {
        printf("Error saving data.\n");
        exit(1);
    }

    for(int i = 0; i < total_students; i++)
    {
        fprintf(fp, "%lld|%s|%ld|%ld\n", students[i].id, students[i].name, students[i].pin, students[i].balance);
    }
    fclose(fp);
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
    }
    fprintf(fp, "%lld|%02d:%02d|%s|%d%c\n", students[index].id, tm_ptr.tm_hour, tm_ptr.tm_min, action, amount, sign);
    fclose(fp);
}
void atmheader()
{
    printf("====================================\n");
    printf("           CSE115 ATM                 \n");
    printf("====================================\n\n");
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
long long login(long long *id, long *pin, int *index, int *success)
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
int options()
{
    while(1)
    {
        int op = checknum("\n1. Check Balance\n2. Withdraw\n3. Deposit\n4. Transfer\n5. Mini Statement\n6. Change Pin\n7. Log Out\nEnter your selection: ");
        if(op == 1 || op == 2 || op == 3 || op == 4 || op == 5 || op == 6 || op == 7)
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
int balance(int index)
{
    int nav;
    printf("\nAccount number: %lld\nAccount Name: %s\nAvailable Balance: %ld Tk", students[index].id, students[index].name, students[index].balance);
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
            savestudents();
            printf("\nSuccess! Your account has been debited by %ld Tk", input);
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
int main()
{
    srand(time(0));
    loadstudents();
    atmheader();

    long long id;
    long pin;
    int index, select, success, navigate;

    while(1)
    {
        login(&id, &pin, &index, &success);
        while(success != 0)
        {
            int offerindex = (rand() % 2) + 1;
            int op = options();
            switch(op)
            {
                case 1:
                    navigate = balance(index);
                    break;
                case 2:
                    navigate = withdraw(index);
                    break;
                case 3:
                    navigate = deposit(index);
                    break;
                case 4:
                    navigate = transfer(index);
                    break;
                case 5:
                    navigate = statement(index);
                    break;
                case 6:
                    navigate = changepin(index);
                    break;
                case 7:
                    navigate = logout();
                    break;
            }
            if(navigate == 0)
                break;
            else
            {
                offers(offerindex);
                continue;
            }
        }
    }
}

