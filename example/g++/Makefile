all: test-1

rebuild:
	apg -f -t template

clean:
	@rm -f apg.* *.o
	@rm -f plusplus 

test-1:
	g++ -g -c  test.c   
	g++ -g -c  -W -Wall apg.par.c  
	g++ -g test.o apg.par.o -o plusplus 

