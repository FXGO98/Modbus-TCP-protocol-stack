int Send_Modbus_Request (char *address, unsigned short port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R);

int write (int fd,uint8_t *PDU, int PDUlen);

int read (int fd, uint8_t *PDU_R, int PDU_Rlen);

Receive_Modbus_request (fd, APDU, APDUlen);

Send_Modbus_response (TI, APDU_R, APDU_Rlen);

