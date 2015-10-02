CFLAGS = -D SELF_TEST -ggdb

all:scel_decoder.exe

scel_decoder.exe:../libsgscel.c ../main.c
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o *.exe

