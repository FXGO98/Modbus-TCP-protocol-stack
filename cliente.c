#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include "ModbusAP.h"

#define ADDR_LEN 25

//Função para quebrar ligação entre o socket do cliente e do servidor
int sDisconnect(int fd)
{
    shutdown(fd, SHUT_RDWR);

    return 0;
}

int main()
{
    int function, num;
    int *fd;
    char c='s';
    unsigned short port;
    unsigned int st_r, st_c, n_r, n_c;

    uint8_t *val;
    char address[ADDR_LEN];

    val=(uint8_t *) malloc(1);

    fd= (int *) malloc(1*sizeof(int));

    //Configuração do Endereço IP, do Port e da função a usar e seus valores

    printf("Endereco do servidor: ");

    scanf("%s", address);

    printf("\nPort:");

    scanf("%hu", &port);

    while((c=='s') || (c=='S'))
    {

        printf("\nReference Number: ");

        scanf("%d", &st_r);

        st_c=st_r;

        printf("\nNumber of Values: ");

        scanf("%d", &n_r);

        n_c=n_r;

        printf("\nQual a função: \nWrite Multiple Registers (1)\nWrite Multiple Coils (2)\nRead Multiple Registers (3)\nRead Multiple Coils (4)\n");

        scanf("%d", &function);

        //switch para escolha da função a partir do input
        switch (function)
        {
            case 1:
            {
                num=Write_Multiple_Regs (fd, address, port, st_r, n_r, val);

                //Caso a função falhe
                if(num<0)
                {
                    printf("\nWritting Error\n");
                }

                //Case a função funcione com sucesso
                else
                {
                    printf("\n%d Register(s) was/were written", num);
                }   


            }
            break;

    // FUNÇÕES POR DEFINIR
    /*
            case 2:
            {
                num=Write_Multiple_Coils (address, port, st_c, n_c, val);

                if(num<0)
                {
                    printf("\nWritting Error\n");

                    return 0;
                }

                else
                    printf("\n%d Coil(s) was/were written", num);
            }
            break;

            case 3:
            {
                num=Read_h_regs (address, port, st_r, n_r, val);

                if(num<0)
                {
                    printf("\nReading Error\n");

                    return 0;
                }
            
                else
                    printf("\n%d Register(s) was/were read", num);
            }
            break;

            case 4:
            {
                num=Read_coils (address, port, st_c, n_c, val);

                if(num<0)
                {
                    printf("\nReading Error\n");

                    return 0;
                }

                else
                    printf("\n%d Coil(s) was/were read", num);
            }
            break;
*/
            default:
            break;
        }

        //Possibilidade de realizar mais funções nesta sessão ou fechar sessão
        printf("\nQuer realizar mais funções? (S/N): ");

        scanf(" %c", &c);

        printf("\n");
    }


    //Fechar sessão quebrando a ligação de sockets
    sDisconnect(*fd);

    return 0;

}