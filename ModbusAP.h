int Write_Multiple_Regs (int *fd, char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val);

int Write_Multiple_Coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint8_t *val);

int Read_h_regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint8_t *val);

int Read_coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint8_t *val);

int Get_request (int fd, int *op, int *st, int *n, uint8_t *val);

int Send_response (int fd, int TI, int op, int st, int n, uint8_t *val);



