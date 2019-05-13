
gcc -fpic -c -I/usr/include/python2.7 -I/usr/lib/python2.7/config -I./ -DBUILD_LIB *.c
gcc -shared -o uwpflash.so *.o
