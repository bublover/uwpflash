#ifndef _DOWNLOAD_H_


extern int dl_init(void);
extern int dl_flash_fdl(unsigned char *chip, unsigned int addr);
extern int dl_flash_file(char *fname, unsigned int addr);

#endif
