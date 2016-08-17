default:
	gcc -o objs/src/core/tms.o -c src/core/tms.c
	gcc -o objs/tms objs/src/core/tms.o
