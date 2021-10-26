
.PHONY = all clean

all:
	gcc rsa.c -o rsa
clean:
	rm rsa
