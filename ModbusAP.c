#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ModbusTCP.h"


int Write_Multiple_Regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val)
{
    int i, n, response, APDU_len, s;
    
    uint8_t *APDU, *APDU_R, *aux;
    
    aux= (uint8_t *) malloc(2 * sizeof(uint8_t));

    aux[0]=aux[1]=0;

    s=2*n_r + 6;

    printf("\ns = %d\n", s);

    APDU= (uint8_t *) malloc(s);

    APDU_len= s;


    if (address==NULL)
    {
        printf("Server Adress missing\n");

        return -1;
    }

    if (port < 0)
    {
        printf("Port missing\n");

        return -1;
    }

    if((n_r>121) || (n_r==0))
    {
        printf("Invalid number of Registers\n");

        return -1;
    }

    APDU_R = (uint8_t *) malloc(1 * sizeof(uint8_t));

    val = (uint8_t *) realloc(val, 2*n_r);

    printf("Values:");

    for(i=0;i<(2*n_r);i++)
    {
        scanf("%hd", &aux[0]);

        val[i]= aux[1];

        i++;

        val[i]=aux[0];

        aux[0]=aux[1]=0;
    }


    APDU[0]=16;

    aux[0]=st_r;

    APDU[1]=aux[1];

    APDU[2]=aux[0];

    aux[0]=aux[1]=0;

    aux[0]=n_r;

    APDU[3]=aux[1];

    APDU[4]=aux[0];

    APDU[5]=n_r*2;

    n=6;

    printf("\nAPDU START\n");

    printf("\nAPDU_len = %d\n", APDU_len);

    for(i=0;i<((APDU_len)-6);i++)
    {
        APDU[n]=val[i];

        n++;
    }

    printf("\nAPDU FINISHED\n");

    printf("\nAPDU: ");

    for(i=0;i<(2*n_r+6);i++)
    {
        printf("%hu ", APDU[i]);
    }

     printf("\nAPDU_len = %d\n", APDU_len);

    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

    printf("\n response = %d \n", response);

    if (response == -1)
        return -1;

    for(i=0;i<(APDU_len-2*n_r-1);i++)
    {
        if(APDU[i]!=APDU_R[i])
            return -1;
    }

    return n_r;

    /*if((APDU_R[(sizeof(APDU_R-1))]==(APDU[4])) && (APDU_R[(sizeof(APDU_R-2))]==(APDU[3])))
        return n_r;

    else 
        return -1;*/

}

/*
int Write_Multiple_Coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint8_t *val)
{
    int i, response, APDU_len;

    char *buf;
    
    uint8_t *APDU, *APDU_R;


    if (address==NULL)
    {
        printf("Server Adress missing\n");

        return 0;
    }

    if (port==NULL)
    {
        printf("Port missing\n");

        return 0;
    }

    if((n_c>121) || (n_c==0))
    {
        printf("Invalid number of Registers");

        return 0;
    }

    val = (uint8_t *) realloc(val, 2*n_c); // this needs fixing

    printf("Values:");

   

    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

}

int Read_h_regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val)
{ 
   int i, n, response, APDU_len;
    
    uint8_t *APDU, *APDU_R, aux[2]={0};

    if (address==NULL)
    {
        printf("Server Adress missing\n");

        return 0;
    }

    if (port==NULL)
    {
        printf("Port missing\n");

        return 0;
    }

    if((n_r>121) || (n_r==0))
    {
        printf("Invalid number of Registers\n");

        return 0;
    }

    APDU_R = (uint8_t *) malloc(1);

    APDU= (uint8_t *) malloc(6);

    APDU[0]=16;

    aux[0]=st_r;

    APDU[1]=aux[1];

    APDU[2]=aux[0];

    aux[0]=aux[1]=0;

    aux[0]=n_r;

    APDU[3]=aux[1];

    APDU[4]=aux[0];

    APDU[5]=n_r*2;

    APDU_len= sizeof(APDU);

    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

    
}

int Read_coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint8_t *val)
{
    int i, response, APDU_len;

    uint8_t *APDU, *APDU_R;


    if (address==NULL)
    {
        printf("Server Adress missing\n");

        return 0;
    }

    if (port==NULL)
    {
        printf("Port missing\n");

        return 0;
    }

    if((n_c>121) || (n_c==0))
    {
        printf("Invalid number of Registers");

        return 0;
    }

   
    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);
}
*/