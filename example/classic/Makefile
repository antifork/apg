all: clean rebuild test-1

rebuild:
	apg -f -t template

clean:
	@rm -f apg.* *.o
	@rm -f classic 

test-1:
	gcc -g -c  test.c   
	gcc -g -c  -W -Wall apg.par.c  
	gcc -g test.o apg.par.o -o classic 

