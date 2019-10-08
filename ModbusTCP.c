#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define MBAP 7

int write (int fd,uint8_t *PDU, int PDUlen)
{
    ssize_t write;

    write = send(fd, PDU, PDUlen,0);

    if(write == -1)
    {
        printf("\nError Sending Values\n");

        return -1;
    }

    else 
        return 0;

}

int read (int fd, uint8_t *PDU_R, int PDU_Rlen)
{
    ssize_t read;

    read = recv(fd, PDU_R, PDU_Rlen, 0);

    if(read == -1)
    {
        printf("\nError Receiving Response\n");

        return -1;
    }

    else 
        return 0;
}

int Send_Modbus_Request (char *address, unsigned short port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R)
{
    int sock, response, PDUlen, PDU_Rlen, i, n, connect_check, write_check;
    uint8_t *PDU, *PDU_R;
    uint8_t aux[2]={0};
	struct sockaddr_in serv;
	socklen_t addlen = sizeof(serv);

    if (address == NULL)
        return -1;

    printf("\n Adress ok \n");

    if (port < 0)
        return -1;

    printf("\n Port ok \n");

    if (APDUlen <= 0)

        return -1;

    printf("\n APDUlen ok \n");

    if (APDU ==  NULL)
        return -1;

    printf("\n APDU ok \n");

    if (APDU_R == NULL)
        return -1;

    printf("\n APDU_R ok \n");

    srand(time(NULL));  

    int TI = rand() % 99+ 1;

    printf("TI = %d", TI);

    aux[0]=TI;

    printf("\nPDULEN= %d \n",APDUlen + MBAP);

    PDU= (uint8_t *) malloc(MBAP+APDUlen);

    PDU[0] = aux[1];

    PDU[1] = aux[0];

    aux[0]=aux[1]=0;

    PDU[2] = PDU[3] = 0;

    aux[0]= APDUlen + 1;

    PDU[4] = aux[1];

    PDU[5] = aux[0];

    aux[0]=aux[1]=0;

    PDU[6] = 1;

    n=7;

    for(i=0;i<APDUlen;i++)
    {
        PDU[n]=APDU[i];

        n++;
    }
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(address, &serv.sin_addr);

    PDUlen = MBAP + APDUlen;

    printf("\nPDU: ");

    for(i=0;i< (MBAP+APDUlen);i++)
    {
        printf("%hu ", PDU[i]);
    }

    PDU_Rlen = MBAP + 5;

    PDU_R = (uint8_t *) malloc(PDU_Rlen);
	
	connect_check = connect(sock, (struct sockaddr *)&serv, addlen);

    if(connect_check == -1)
        return -1;

    printf("\n Connect ok \n");

    write_check = write(sock, PDU, PDUlen);

    if (write_check == -1)
        return -1;
    printf("\n Write ok \n");

    response = read(sock, PDU_R, PDU_Rlen);

    printf("\n Read ok \n");

    printf("\nPDU_R: ");

    for(i=0;i< PDU_Rlen;i++)
    {
        printf("%hu ", PDU_R[i]);
    }

    shutdown(sock, SHUT_RDWR);

    printf("response1 = %d", response);

    if((PDU_R[0]!=PDU[0]) || (PDU_R[1]!=PDU[1]))
        return -1;

    if (response == 0)
    {
        APDU_R = (uint8_t *) realloc(APDU_R, PDU_Rlen-MBAP);

        n=MBAP;

        printf("\nMBAP+1 = %d\n", n);

        printf("\nPDU_Rlen-MBAP = %d", PDU_Rlen - MBAP);

        for(i=0;i<(PDU_Rlen-MBAP);i++)
        {
            APDU_R[i] = PDU_R[n];

            n++;
        }

         printf("\nAPDU_R: ");

        for(i=0;i < ((PDU_R[5])-1);i++)
        {
            printf("%hu ", APDU_R[i]);
        }


        return 0;
    }
    
    else 
        return -1;

}
