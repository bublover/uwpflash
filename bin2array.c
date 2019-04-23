#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#define FILE_IN  "fdl.bin"
#define FILE_OUT "fdl.h"
#define BUF_SIZE 128

static int fd_in;
static int fd_out;
char str_header[] = "#ifndef _FDL_%s_H__\nchar fdl_%s[] = {\n";
char str_tail[] = "};\n#endif\n";
char *str_chip = "NONE";
char buf[BUF_SIZE];

int main(int argc, char **argv)
{
	int i;
	int ret;

	if (argc > 1)
		str_chip = argv[1];

	fd_in = open(FILE_IN, O_RDONLY);
	if (fd_in < 0) {
		perror("file");
		return -1;
	}

	fd_out = open(FILE_OUT, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0) {
		perror("file");
		return -1;
	}

	memset(buf, 0, BUF_SIZE);
	snprintf(buf, BUF_SIZE, str_header, str_chip, str_chip);

	write(fd_out, buf, strlen(buf));

	unsigned char c;
	unsigned int len;
	i = 0;
	while (1) {
		len = read(fd_in, &c, 1);

		if (len != 1) {
			printf("finish.\n");
			break;
		}

		snprintf(buf, BUF_SIZE, "0x%02x, ", c);
		
		write(fd_out, buf, 6);
		i++;

		if (i % 16 == 0) {
			write(fd_out, "\n", 1);
		}
	}
	write(fd_out, str_tail, strlen(str_tail));

	close(fd_in);
	close(fd_out);
}
