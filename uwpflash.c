#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "command.h"
#include "interface.h"
#include "download.h"
#include "fdl_5661.h"
#include "fdl_5662.h"

#define VERSION	"v0.0.4"
#define AUTHOR "Dong Xiang <dong.xiang@unisoc.com>"

struct fobject {
	char *fname;
	unsigned int addr;
	struct fobject *next;
};

struct fobject * fobj_alloc(void)
{
	struct fobject *fobj;

	fobj = (struct fobject *)malloc(sizeof(*fobj));
	if (fobj == NULL) {
		printf("out of memory.\n");
		return NULL;
	}

	memset((void *)fobj, 0, sizeof(*fobj));
	fobj->next = NULL;

	return fobj;
}

int fobj_insert(struct fobject **flist, struct fobject *fobj)
{
	fobj->next = (*flist);
	(*flist) = fobj;

	return 0;
}

struct params {
	char *dev;
	char *intf_type;
	char *chip;
	unsigned char *fdl;
	unsigned int fdl_len;
	struct fobject *flist;
	unsigned char debug_mode;
};

static struct params params = {
	.intf_type = "UART",
	.dev = "/dev/ttyUSB0",
	.chip = "5661",
	.flist = NULL,
};

static struct fobject *fobj;

void help(void) {
	printf("-h\t\t show this message.\n");
	printf("-D\t\t Debug mode.\n");
	printf("-t <type>\t interface type.\n");
	printf("\t\t default: UART\n");
	printf("-d <dev> \t device name.\n");
	printf("\t\t default: /dev/ttyUSB0\n");
	printf("-c <chip>\t chip name.\n");
	printf("\t\t default: 5661\n");
	printf("-f <file>\t image file name.\n");
	printf("-a <address>\t flash address.\n");

	exit(0);
}

unsigned char debug_mode(void)
{
	return params.debug_mode;
}

int main(int argc,char **argv)
{
	struct params *p = &params;
	int opt;
	int ret;
	opterr = -1;

	printf("UNISOC uwpflash tool.\n");
	printf("Version: " VERSION "\n");

	while ((opt = getopt (argc, argv, "t:d:vc:f:a:h:D")) != -1) {
		switch (opt) {
			case 'h':
				help();
				break;
			case 'D':
				p->debug_mode = 1;
				break;
			case 't':
				p->intf_type = optarg;
				break;
			case 'v':
				printf("UNISOC uwpflash tool v" VERSION "\n");
				break;
			case 'c':
				p->chip = optarg;
				break;
			case 'd':
				p->dev = optarg;
				break;
			case 'f':
				if (fobj == NULL) {
					fobj = fobj_alloc();
					if (fobj == NULL) {
						return -1;
					}
				}
				fobj->fname = optarg;
				break;
			case 'a':
				if (fobj == NULL)
					continue;
				sscanf(optarg,"0x%x", &fobj->addr);
				fobj_insert(&(p->flist), fobj);
				fobj = NULL;
				break;
			default:
				help();
		}
	}

	printf("Interface: %s\n", p->intf_type);
	printf("Device: %s\n", p->dev);
	printf("\n");

	if (p->flist == NULL) {
		printf("None file(s) need to flash.\n\n");
		help();
	}

	if (strcmp(p->chip, "5661") == 0) {
		p->fdl = fdl_5661;
		p->fdl_len = sizeof(fdl_5661);
	} else if (strcmp(p->chip, "5662") == 0) {
		p->fdl = fdl_5662;
		p->fdl_len = sizeof(fdl_5662);
	}

	ret = intf_init(p->intf_type, p->dev);
	if (ret < 0) {
		perror("init interface failed");
		return ret;
	}

	ret = cmd_init();
	if (ret < 0) {
		printf("command init failed.\n");
		return ret;
	}


	ret = dl_init();
	if (ret < 0) {
		printf("download init failed.\n");
		return ret;
	}
	

	ret = dl_flash_fdl(p->fdl, p->fdl_len, 0x100000);
	if (ret < 0) {
		printf("download file failed.\n");
		return ret;
	}

	for (fobj = p->flist; fobj != NULL; fobj = fobj->next) {
		ret = dl_flash_file(fobj->fname, fobj->addr);
		if (ret < 0) {
			printf("download file %s failed.\n", fobj->fname);
			return ret;
		}
	}

	return 0;
}
