all: clean test rebuild test-1

rebuild:
	apg -a -f -t template

clean:
	@rm -f apg.* *.o
	@rm -f robust
	@rm -f test 
test:
	./make_test
test-1:
	gcc -g -O3 -c test.c     
	gcc -g -march=pentium -O9 -c apg.par.c  -DAPG_PERF -pg  # -fomit-frame-pointer 
	gcc -g test.o apg.par.o -o robust    -pg 

