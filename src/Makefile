PKG_CONFIG_PATH=/usr/X11/lib/pkgconfig:/usr/local/opt/libffi/lib/pkgconfig:/usr/local/Cellar/cario/1.16.0/lib/pkgconfig

main: main.cc
	gcc main.cc -o main `pkg-config --cflags --libs gtk+-3.0`

.PHONY:	clean
clean:
	rm main
