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
	gcc -O3 -c test.c     # -fprofile-arcs -ftest-coverage  # -include /usr/include/almepro.h
	gcc -march=pentium -O9 -c apg.par.c  -DAPG_PERF -fomit-frame-pointer # -pg # -fprofile-arcs -ftest-coverage # -include /usr/include/almepro.h
	gcc test.o apg.par.o -o robust   # -pg # -fprofile-arcs -ftest-coverage #-lalmepro

