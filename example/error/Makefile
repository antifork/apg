all: clean rebuild test-1

rebuild:
	apg -t template

clean:
	@rm -f apg.* *.o
	@rm -f error 

test-1:
	gcc -c test.c #-include /usr/include/almepro.h
	gcc -c apg.par.c #-include /usr/include/almepro.h
	gcc test.o apg.par.o -o error  #-lalmepro

