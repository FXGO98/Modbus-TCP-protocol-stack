#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdint.h>

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
    int sock, response, PDUlen, PDU_Rlen;
    uint8_t *PDU, *PDU_R;
	struct sockaddr_in serv;
	socklen_t addlen = sizeof(serv);
	char buf[BUFFER];

    // generates TI (trans.ID  sequence number)
    // assembles PDU = APDU SDU ) + MBAP
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(address, &serv.sin_addr);



    PDUlen = sizeof(PDU);

    PDU_R = (uint8_t *) malloc(1);

    PDU_Rlen = sizeof(PDU_R);
	
	connect(sock, (struct sockaddr *)&serv, addlen);

    write(sock, PDU, PDUlen);

    response = read(sock, PDU_R, PDU_Rlen);

    // if response, remove MBAP , PDU_R  APDU_R

    close(serv);

    // if response, remove MBAP , PDU_R  APDU_R

    return 0;
}
