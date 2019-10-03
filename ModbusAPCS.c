#include <stdio.h>
#include <stdint.h>
#include "ModbusTCPCS.h"
#define BUFFER 50


int Write_Multiple_Regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val)
{
    int i, response, APDU_len;

    char *buf;
    
    uint8_t *APDU, *APDU_R;

    buf = (char *) malloc(BUFFER);

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
        printf("Invalid number of Registers");

        return 0;
    }

    val = (uint8_t *) realloc(val, 2*n_r); //this needs fixing

    printf("Values:");

/*  APDU generation
    for (i=0;i<n_r;i++)
    {
        scanf("%s", &buf);


    }

*/
    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

}

int Write_Multiple_Coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint8_t *val)
{
    int i, response, APDU_len;

    char *buf;
    
    uint8_t *APDU, *APDU_R;

    buf = (char *) malloc(BUFFER);

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

    /*  APDU generation
    for (i=0;i<n_r;i++)
    {
        scanf("%s", &buf);


    }

 */

    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);

}

int Read_h_regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val)
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

    if((n_r>121) || (n_r==0))
    {
        printf("Invalid number of Registers");

        return 0;
    }

   
/*  APDU generation
    for (i=0;i<n_r;i++)
    {
        scanf("%s", &buf);


    }

*/
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

   
/*  APDU generation
    for (i=0;i<n_r;i++)
    {
        scanf("%s", &buf);


    }

*/
    response= Send_Modbus_Request(address, port, APDU, APDU_len, APDU_R);
}
