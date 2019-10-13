#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define MBAP 7

//Enviar dados de um socket para o outro da ligação
int write (int fd,uint8_t *PDU, int PDUlen)
{
    ssize_t write, i;

    //Envia dados
    write = send(fd, PDU, PDUlen, 0);

    if(write == -1)
    {
        printf("\nError Sending Values\n");

        return -1;
    }

    else 
        return 0;

}

//Receber dados enviados do outro socket da ligação
int read (int fd, uint8_t *PDU_R, int PDU_Rlen)
{
    ssize_t read, i;

    //Receber dados
    read = recv(fd, PDU_R, PDU_Rlen, 0);

    if(read == -1)
    {
        printf("\nError Receiving Response\n");

        return -1;
    }

    else 
        return 0;
}

//Manda pedido ao servidor
int Send_Modbus_Request (char *address, unsigned short port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R)
{
    int sock, response, PDUlen, PDU_Rlen, i, n, connect_check, write_check;
    uint8_t *PDU, *PDU_R;
    uint8_t aux[2]={0};
	struct sockaddr_in serv;
	socklen_t addlen = sizeof(serv);

    //Verifica os argumentos da função
    if (address == NULL)
        return -1;

    if (port < 0)
        return -1;

    if (APDUlen <= 0)
        return -1;

    if (APDU ==  NULL)
        return -1;

    if (APDU_R == NULL)
        return -1;

    srand(time(NULL));  

    // Gera Transaction ID

    int TI = rand() % 99+ 1;

    aux[0]=TI;

    //Gera PDU

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
	
    //Criar socket do cliente
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(address, &serv.sin_addr);

    PDUlen = MBAP + APDUlen;

    printf("\n PDU: ");

    for(i=0;i<PDUlen;i++)
    {
        printf(" %d", PDU[i]);
    }

    PDU_Rlen = MBAP + 5;

    PDU_R = (uint8_t *) malloc(PDU_Rlen);

    APDU_R = (uint8_t *) realloc(APDU_R, PDU_Rlen-MBAP);

	//solicita uma conexão entre o servidor e o cliente
	connect_check = connect(sock, (struct sockaddr *)&serv, addlen);

    if(connect_check == -1)
        return -1;

    //envia PDU para o servidor
    write_check = write(sock, PDU, PDUlen);

    if (write_check == -1)
        return -1;

    //recebe PDU da resposta do servidor
    response = read(sock, PDU_R, PDU_Rlen);

    printf("\n PDU_R: ");

    for(i=0;i<PDU_Rlen;i++)
    {
        printf(" %d", PDU_R[i]);
    }

    //Verifica so o TI da resposta é igual ao do Pedido
    if((PDU_R[0]!=PDU[0]) || (PDU_R[1]!=PDU[1]))
        return -1;

    //Retira o APDU do PDU de resposta
    if (response == 0)
    {
        APDU_R = (uint8_t *) realloc(APDU_R, PDU_Rlen-MBAP);

        n=MBAP;

        for(i=0;i<(PDU_Rlen-MBAP);i++)
        {
            APDU_R[i] = PDU_R[n];

            n++;
        }

        return 0;
    }
    
    else 
        return -1;

}

//Recebe pedido do cliente
int Receive_Modbus_request (int fd, uint8_t *APDU, int APDUlen)
{
    int sd, read_MBAP_check, read_APDU_check, TI, i;
    uint8_t *MBAP1;

    struct sockaddr_in rem;
	socklen_t addlen = sizeof(rem);

    //Aceita a solicitação de conexão do cliente
    sd=accept(fd, (struct sockaddr *)&rem, &addlen);

    //Verifica se a conexão se realizou com sucesso
    if(sd==-1)
    {
        printf("\n Connection Failed");
        return -1;
    }

    MBAP1 = (uint8_t *) malloc(MBAP * sizeof(uint8_t));

    //Recebe o MBAP do pedido

    read_MBAP_check = read (sd, MBAP1, MBAP); 

    if(read_MBAP_check==-1)
    {
        printf("\nRead MBAP failed\n");

        return -1;
    }

    
    printf("\n MBAP: ");

    for(i=0;i<MBAP;i++)
    {
        printf(" %d", MBAP1[i]);
    }

    TI = MBAP1[1]; 
    
    APDUlen = MBAP1[5] + (MBAP1[4]<<8) - 1;

    //Recebe o APDU do pedido
    
    read_APDU_check = read (sd, APDU, APDUlen);

    if(read_APDU_check==-1)
    {
        printf("\nRead APDU failed\n");

        return -1;
    }

    
    printf("\n APDU: ");

    for(i=0;i<APDUlen;i++)
    {
        printf(" %d", APDU[i]);
    }
   
   //retorna o Transaction ID
    return TI;
}

//Envia a resposta ao cliente
int Send_Modbus_response (int fd, int TI, uint8_t *APDU_R, int APDU_Rlen)
{
    uint8_t *PDU, *aux, i, n;

    int PDUlen, write_check;

    PDUlen = APDU_Rlen+MBAP;

    PDU = (uint8_t *) malloc((PDUlen)*sizeof(uint8_t));

    aux = (uint8_t *) malloc(2*sizeof(uint8_t));

    aux[0] = aux[1] = 0;

    aux[0] = TI;

    //Gera PDU da resposta

    PDU[0] = aux[1];

    PDU[1] = aux[0];

    PDU[2] = PDU[3] = 0;

    aux[0] = aux[1] = 0;

    aux[0] = APDU_Rlen+1;

    PDU[4] = aux[1];

    PDU[5] = aux[0];

    PDU[6] = 1;

    n=7;

    for(i=0;i<(APDU_Rlen);i++)
    {
        PDU[n] = APDU_R[i];

        n++;
    }

    printf("\n PDU_R: ");

    for(i=0;i<PDUlen;i++)
    {
        printf(" %d", PDU[i]);
    }

    //Envia a resposta para o cliente

    write_check = write(fd, PDU, PDUlen);

    if(write_check ==-1)
    {
        printf("\nPDU_R write failed \n ");

        return -1;
    }

    return 0;

}

