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

    read = recv(sd, buf, BUFFER, 0);

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
    int sock, response;
	struct sockaddr_in serv;
	socklen_t addlen = sizeof(serv);
	char buf[BUFFER];
	
	sock = socket(PF_INET, SOCK_STREAM, 0);
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(address, &serv.sin_addr);
	
	connect(sock, (struct sockaddr *)&serv, addlen);

    write(sock, PDU, PDUlen);

    response = read(sock, PDU_R, PDU_Rlen);

    close(serv);

    return 0;
}
