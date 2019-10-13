#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include "ModbusAP.h"

#define ADDR_LEN 25


//Função que escreve os registers diretamente no servidor
int W_regs (int st, int n, uint8_t *val, uint8_t *addr)
{
    int i, b=0, count=0;

    for(i=st;i<st+n;i++)
    {
        addr[i] = val[b+1]+(val[b]<<8);

        b+=2;

        count++;
    }
    //retorna erro se fahar ou o número de registers que foram escritos no servidor
    if(count!=n)
        return -1;
    
    else
        return count;
}

//Função para iniciar uma conexão entre sockets
int sConnect (char *server_add, unsigned int port)
{

    int so, bind_check, listen_check;
	struct sockaddr_in loc;
	socklen_t addlen = sizeof(loc);

    //Criar socket do servidor
	so = socket(PF_INET, SOCK_STREAM, 0);
	
	loc.sin_family = AF_INET;
	loc.sin_port = htons(port);
	
	inet_aton(server_add,&loc.sin_addr);

    //Dar assign do endereço ao socket
	bind_check = bind(so, (struct sockaddr *) &loc, addlen);

    if(bind_check == -1)
    {
        printf("\n Bind failed\n");
        return -1;
    }
	

    //esperar por request de conexão de algum cliente
	listen_check = listen(so, 10);
    
    if(listen_check == -1)
    {
        printf("\n Listen failed\n");
        return -1;
    }

    return so;
}

int main()
{
    int b, fd =0, TI, SR_check;
    int *op, *st, *n;
    unsigned short port;

    //addr é o vetor com os valores escritos dentro do servidor
    uint8_t *val, *addr;
    char address[ADDR_LEN];

    //val é o vetor com os valores para escrever ou que vai buscar os valores lidos
     val=(uint8_t *) malloc(1);

    //op tem o nr da função, st é o byte pelo qual se começa a função e n é o numero de valores
    op = (int *) malloc(1*sizeof(int));

    st = (int *) malloc(1*sizeof(int));

    n = (int *) malloc(1*sizeof(int));

    //configuração do servidor

    printf("Endereco do servidor: ");

    scanf("%s", address);

    printf("\nPort:");

    scanf("%hu", &port);

    //Quantity é o nr de endereços de memoria de Addr

    printf("\nQuantity:");

    scanf("%d", &b);

    printf("\n");

    addr = (uint8_t *) malloc (b*sizeof(uint8_t));

    while(fd!=-1)
    {
        //Conexão com o cliente
        fd=sConnect(address, port);

        //Terminar conexão com o cliente e esperar a função do cliente
        TI = Get_request(fd, op, st, n, val);

        //escrever registers no servidor
        *n=W_regs(*st, *n, val, addr);

        //Enviar resposta para o Cliente
        SR_check = Send_response (fd, TI, *op, *st, *n, val);

        if(SR_check ==-1)
        {
            printf("\n Failed to send response \n");

            return 0;
        }

    }

    return 0;

}