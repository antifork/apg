all: clean rebuild test-1

rebuild:
	apg -f -t template

clean:
	@rm -f apg.* *.o
	@rm -f ansi 

test-1:
	gcc  -c test.c    #-ansi -pedantic 
	gcc  -Wall -c apg.par.c -ansi -pedantic 
	gcc test.o apg.par.o -o ansi 

