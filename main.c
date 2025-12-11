#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "accounts.h"
#include "atm.h"

int main()
{
    srand(time(0));
    load();
    atmheader();

    long long id;
    long pin;
    int index, navigate, success;

    while(1)
    {
        login(&id, &pin, &index, &success);

        while(success > 0)
        {
            int offerindex = (rand() % 3) + 1;
            int op = options();

            switch(op)
            {
                case 1:
                    navigate = accinfo(index);
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
                    navigate = more(index);
                    break;
                case 8:
                    navigate = logout();
                    break;
                case 9:
                    return navigate = 0;
                    break;
            }

            if(navigate == 0)
            {
                printf("\nHave a nice day!");
                exit(1);
            }
            if(navigate == -1)
                break;
            offers(offerindex);
        }
    }
}
