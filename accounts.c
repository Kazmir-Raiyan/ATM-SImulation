#include "accounts.h"
#include <stdlib.h>
#include <string.h>

data accounts[total];

void load()
{
    FILE *fp = fopen("info.txt", "r");

    if(!fp)
    {
        printf("Error loading data.\n");
        exit(1);
    }

    for(int i = 0; i < total; i++)
    {
        fscanf(fp, "%lld\t%[^\t]\t%ld\t%ld",
                &accounts[i].id, accounts[i].name,
                &accounts[i].pin, &accounts[i].balance);
    }
    fclose(fp);
}

void save()
{
    FILE *fp = fopen("info.txt", "w");
    if(!fp)
    {
        printf("Error saving data.\n");
        exit(1);
    }

    for(int i = 0; i < total; i++)
    {
        fprintf(fp, "%lld\t%s\t%ld\t%ld\n",
                accounts[i].id, accounts[i].name,
                accounts[i].pin, accounts[i].balance);
    }
    fclose(fp);
}
