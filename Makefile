MAIN=kof2003
UNAME := $(shell uname)

SO=LINUX
ifeq ($(UNAME), Darwin)
	SO=OSX
endif
ifeq ($(UNAME), Windows)
	SO=DOS
endif

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

clean-config:
	rm -rf fonts.dat game.ini title.*

cp-config:
	cd data/k91v12s && cp fonts.dat game.ini title.* ../..

replace-data:
	rm -rf data \
	&& curl -L -o k91v12s.zip https://archive.org/download/k91v12s/k91v12s.zip \
	&& unzip k91v12s.zip -d data \
	&& rm k91v12s.zip
	make cp-config

