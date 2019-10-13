#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "ModbusTCP.h"

//Função para terminar a ligação com o cliente e que recebe os pedidos do cliente
int Get_request (int fd, int *op, int *st, int *n, uint8_t *val)
{   
    int TI, APDUlen, p, i;

    uint8_t *APDU;

    //Função para terminar a ligação com o cliente e que recebe os pedidos do cliente
    TI = Receive_Modbus_request(fd, APDU, APDUlen);

     printf("\n APDU: ");

    for(i=0;i<(2*(*n));i++)
    {
        printf(" %d", APDU[i]);
    }

    //Define op, st, n e val

    *op = APDU[0];

    *st = APDU[2] + (APDU[1]<<8);

    *n = APDU[4] + (APDU[3]<<8);

    val = (uint8_t *) malloc ((2*(*n))*sizeof(uint8_t));

    p=5;

    for(i=0;i<(2*(*n));i++)
    {
        val[i] = APDU[p];

        p++;
    }
    //retorna o Transaction ID
    return TI;
}

//Envia a resposta para o Cliente
int Send_response (int fd, int TI, int op, int st, int n, uint8_t *val)
{
    uint8_t *APDU_R, *aux;

    int APDU_Rlen, SMR_check, i;

    aux = (uint8_t *) malloc(2 * sizeof(uint8_t));

    aux[0] = aux[1] = 0;

    //Dependendo da função gera p APDU da resposta
    if(op==16)
    {
        APDU_Rlen = 5;

        APDU_R = (uint8_t *) malloc(APDU_Rlen * sizeof(uint8_t));

        APDU_R[0] = op;

        aux[0] = st;

        APDU_R[1]= aux[1];

        APDU_R[2] = aux[0];

        aux[0] = aux[1] = 0;

        aux[0] = n;

        APDU_R[3] = aux[1]; 

        APDU_R[4] = aux[0];
    }

     printf("\n APDU_R: ");

    for(i=0;i<APDU_Rlen;i++)
    {
        printf(" %d", APDU_R[i]);
    }
   
    //gera PDU e envia a resposta
    SMR_check = Send_Modbus_response (fd, TI, APDU_R, APDU_Rlen);
    
    if(SMR_check == -1)
        return -1;

    else
        return 0;
    
}

//Realiza a função de escrever registos no servidor
int Write_Multiple_Regs (int *fd, char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val)
{
    int i, n, response, APDU_len;
    
    uint8_t *APDU, *APDU_R, *aux;

    APDU_R = (uint8_t *) malloc(1 * sizeof(uint8_t));
    
    aux= (uint8_t *) malloc(2 * sizeof(uint8_t));

    aux[0]=aux[1]=0;

    APDU_len= 2*n_r + 6;

    APDU= (uint8_t *) malloc(APDU_len);


    //Verifica os argumentos da função
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

    val = (uint8_t *) malloc(2*n_r);

    printf("Values:");

    for(i=0;i<(2*n_r);i++)
    {
        scanf("%hd", &aux[0]);

        val[i]= aux[1];

        i++;

        val[i]=aux[0];

        aux[0]=aux[1]=0;
    }

    //Gera APDU

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

    for(i=0;i<((APDU_len)-6);i++)
    {
        APDU[n]=val[i];

        n++;
    }

    printf("\nAPDU: ");

    for(i=0;i<(2*n_r+6);i++)
    {
        printf("%hu ", APDU[i]);
    }
    //Envia o pedido ao servidor
    *fd= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

    if (response == -1)
        return -1;

     printf("\nAPDU_R: ");

    for(i=0;i<5;i++)
    {
        printf("%hu ", APDU_R[i]);
    }

    //Verifica se a resposta é válida tendo em conta o pedido 

    for(i=0;i<(APDU_len-2*n_r-1);i++)
    {
        if(APDU[i]!=APDU_R[i])
            return -1;
    }

    return n_r;

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