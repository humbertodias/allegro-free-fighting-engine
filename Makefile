MAIN=kof2003
SO=LINUX
#SO=OSX

.PHONY: cmake
cmake:	so-inc
	mkdir -p build && cd build && cmake -DSO=${SO} .. && make

compile:	so-inc
	gcc src/*.c -g -o ${MAIN} -Iinc \
	`allegro-config --cflags --libs --static` \
	-Wl,-rpath,'/usr/local/lib'

so-inc:
	echo "#define SO ${SO}" > inc/so.inc

clean:
	rm -rf ${MAIN}*	build/* cmake-build-debug

replace-data:
	rm -rf data \
	&& curl -L -o k91v12s.zip https://archive.org/download/k91v12s/k91v12s.zip \
	&& unzip k91v12s.zip -d data \
	&& rm k91v12s.zip