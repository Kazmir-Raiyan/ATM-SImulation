#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "students.h"
#include "atm.h"

int main()
{
    srand(time(0));
    loadstudents();
    atmheader();

    long long id;
    long pin;
    int index, navigate, success;

    while(1)
    {
        login(&id, &pin, &index, &success);

        while(success > 0)
        {
            int offerindex = (rand() % 2) + 1;
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
                    navigate = upgrade(index);
                    break;
                case 8:
                    navigate = logout();
                    break;
            }

            if(navigate == 0) break;

            offers(offerindex);
        }
    }
}
