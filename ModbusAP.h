int Write_Multiple_Regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint16_t *val);

int Write_Multiple_Coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint16_t *val);

int Read_h_regs (char *address, unsigned short port, unsigned int st_r, unsigned int n_r, uint16_t *val);

int Read_coils (char *address, unsigned short port, unsigned int st_c, unsigned int n_c, uint16_t *val);

