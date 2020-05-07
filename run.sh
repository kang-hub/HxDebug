#! /bin/bash
#arm-linux-gnueabihf-gcc -o main main.c lib_print.c -I ./include -L ./lib-standard -lcjson -lpthread -lmosquitto -lcares -licudata -llzma -lsg -lxml2 -lcrypto -licuuc -lssl -lz
arm-linux-gnueabihf-gcc -o test test.c debug.c -I ./include -L./lib -lmosquitto -lpthread -lcares -lssl -lcjson -lcrypto

