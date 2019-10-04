 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "ModbusAP.h"

#define ADDR_LEN 25

int main()
{
    int function, num;

    unsigned short port;
    unsigned int st_r, st_c, n_r, n_c;

    uint8_t *val;
    char address[ADDR_LEN];

    val=(uint8_t *) malloc(1);

    printf("Endereco do servidor: ");

    scanf("%s", &address);

    printf("\nPort:");

    scanf("%d", &port);

    printf("\nReference Number: ");

    scanf("%d", &st_r);

    st_c=st_r;

    printf("\nNumber of Values: ");

    scanf("%d", &n_r);

    n_c=n_r;

    printf("\nQual a função: \nWrite Multiple Registers (1)\nWrite Multiple Coils (2)\nRead Multiple Registers (3)\nRead Multiple Coils (4)\n");

    scanf("%d", &function);

    switch (function)
    {
        case 1:
        {
            num=Write_Multiple_Regs (address, port, st_r, n_r, val);

            printf("\n%d Register(s) was/were written", num);

        }
        break;

        case 2:
        {
            num=Write_Multiple_Coils (address, port, st_c, n_c, val);

            printf("\n%d Coil(s) was/were written", num);
        }
        break;

        case 3:
        {
            num=Read_h_regs (address, port, st_r, n_r, val);

            printf("\n%d Register(s) was/were read", num);
        }
        break;

        case 4:
        {
            num=Read_coils (address, port, st_c, n_c, val);

            printf("\n%d Coil(s) was/were read", num);
        }
        break;

        default:
        break;
    }

    return 0;

}