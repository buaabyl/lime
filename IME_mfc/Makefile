CFLAGS 		= -DUNICODE -D_UNICODE
LDFLAGS 	= -municode -static-libgcc
LDFLAGS_DLL	= -shared -Wl,--enable-stdcall-fixup
LDFLAGS_GUI	= -Wl,-subsystem,windows 

all:
	gcc $(CFLAGS) -c load.c main.c libwmstr.c
	gcc $(LDFLAGS) load.o
	windres -i main.rc -o main.res.o
	gcc $(LDFLAGS) $(LDFLAGS_DLL) $(LDFLAGS_GUI) main.o main.res.o libwmstr.o gplime.def -o gplime.ime

