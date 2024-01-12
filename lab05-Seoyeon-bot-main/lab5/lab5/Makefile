TARGET = retlib  # change this name to newretlib for task3 attack2

all: ${TARGET}

N = 75
retlib: retlib.c # change this name to newretlib for task3 attack2
	gcc -m32 -DBUF_SIZE=${N} -fno-stack-protector -z noexecstack -o $@ $@.c
	sudo chown root $@ && sudo chmod 4755 $@

clean:
	rm -f *.o *.out ${TARGET} badfile

