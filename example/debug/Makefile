all: clean rebuild x 

rebuild:
	apg -t template

clean:
	@rm -f apg.* *.o
	@rm -f debug 

x:
	gcc -g -c test.c -include /usr/include/almepro.h
	gcc -g -c apg.par.c  -include /usr/include/almepro.h
	gcc -g test.o apg.par.o -o debug  -lalmepro

