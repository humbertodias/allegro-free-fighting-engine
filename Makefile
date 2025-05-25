.PHONY: build
build:
	cmake -S . -B build
	cmake --build build

run:	data/get build
	cp build/src/koflinux data
	cd data && ./koflinux

clean:
	rm -rf build data

data/get:
	if ! test -f k91v12s.zip; then curl -L -o k91v12s.zip https://archive.org/download/k91v12s/k91v12s.zip ; fi
	if ! test -d data ; then unzip k91v12s.zip -d data && $(MAKE) data/lowercase ; fi

data/lowercase:
	find ./data -maxdepth 5 -type f | xargs -I % bash -c 'mv "%" `echo "%" | tr "[:upper:]" "[:lower:]"`' || true
