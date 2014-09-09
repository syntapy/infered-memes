CC=colorgcc
CFLAGS=-ggdb3 -Isrc/ -I./ -lm

all: main.c
	rm -f core
	ctags -R *
	$(CC) -o resolve main.c $(CFLAGS)

clean:
	rm -f core resolve out.txt err.txt record.gdb

dbg:
	cgdb ./resolve -c ./core

run:
	/resolve

memcheck:
	valgrind --tool=memcheck --leak-check=full --read-var-info=yes --vgdb=yes --vgdb-error=0 ./resolve

dev:
	vim main.c src/tester.h src/kb.h src/parse.h src/_read.h src/_list.h src/_tree_synth.h src/prps.h src/_prps_err.h src/_tree_gen.h src/hash.h src/_hash_alloc.h src/_hash_check.h

tdev:
	vim src/ops.h src/err.h src/include.h

input:
	vim KB.txt PRPS.txt ALPHA.txt

tags:
	ctags -R *
