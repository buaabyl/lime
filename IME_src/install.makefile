CFLAGS 		= -Wall -DUNICODE -D_UNICODE -static-libgcc
LFLAGS		= -municode -static-libgcc
LIBS		= -lshlwapi -limm32
LFLAGS_IME	= -shared -Wl,--out-implib,libmessage.a -Wl,--kill-at -Wl,--subsystem,windows

all:imeinstall.exe

imeinstall.exe:install.c
	gcc $(CFLAGS) $< $(LFLAGS) $(LIBS) -o $@

wbus.dll:wbus.c wbus.h
	windres wbus.rc wbus_rc.o
	gcc $(CFLAGS) $< -c
	gcc $(LFLAGS) $(LFLAGS_IME) wbus.o wbus_rc.o $(LIBS) -o $@

clean:
	rm -rf *.exe
	rm -rf *.dll
	rm -rf *.ime

