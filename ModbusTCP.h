int Send_Modbus_Request (char *address, unsigned short port, uint8_t *APDU, int APDUlen, uint8_t *APDU_R);

int write (int fd,uint8_t *PDU, int PDUlen);

int read (int fd, uint8_t *PDU_R, int PDU_Rlen);

int Receive_Modbus_request (int fd, uint8_t * APDU, int APDUlen);

int Send_Modbus_response (int fd, int TI, uint8_t *APDU_R, int APDU_Rlen);


