#include "students.h"
#include <stdlib.h>
#include <string.h>

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
            sscanf(line, "%lld|%49[^|]|%ld|%ld|%99[^\n]",
                   &students[i].id, students[i].name,
                   &students[i].pin, &students[i].balance,
                   students[i].card);
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
        fprintf(fp, "%lld|%s|%ld|%ld|%s\n",
                students[i].id, students[i].name,
                students[i].pin, students[i].balance,
                students[i].card);
    }
    fclose(fp);
}
