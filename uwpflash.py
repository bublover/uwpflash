#! /usr/bin/env python

import uwpflash as uf
import getopt
import sys

intf_type = "UART"
dev = "/dev/ttyUSB0"

def main(argv):
	print("UNISOC uwpflash tool.")
	uf.about()

	pair = {}

	opts,args = getopt.getopt(sys.argv[1:],'t:d:vc:f:a:hD',['type','dev','version','chip','file','addr','help','debug'])
	for opt_name,opt_value in opts:
		if opt_name in ('-h','--help'):
			uf.help()
			exit()
		if opt_name in ('-v','--version'):
			uf.about()
			exit()
		if opt_name in ('-f','--file'):
			fileName = opt_value
		if opt_name in ('-t','--type'):
			global intf_type
			intf_type = opt_value
		if opt_name in ('-d','--dev'):
			global dev
			dev = opt_value
		if opt_name in ('-c','--chip'):
			chip = opt_value
		if opt_name in ('-a','--addr'):
			addr = opt_value
			pair[fileName] = int(addr, 16)
	
	print("Interface: " + intf_type);
	print("Device: %s\n" % dev);

	if not pair:
		print("None file(s) need to flash.\n")
		uf.help()
		

	ret = uf.intf_init(intf_type, dev)
	if (ret):
		print("initf_init failed")
		exit()

	ret = uf.cmd_init()
	if (ret):
		print("cmd_init failed")
		exit()

	ret = uf.dl_init()
	if (ret):
		print("dl_init failed")
		exit()

	ret = uf.dl_flash_fdl(chip, 0x100000)
	if (ret):
		print("dl_flash_fdl failed")
		exit()

	for fname, addr in pair.items():
		ret = uf.dl_flash_file(fname, addr)
		if (ret):
			print("download file %s failed." % fname)
			exit()

if __name__ == '__main__':
	main(sys.argv);
