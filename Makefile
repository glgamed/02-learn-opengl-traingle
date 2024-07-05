.PHONY: clean build
.ONESHELL:

clean:
	rm -rf build

build:
	cmake -GNinja -Bbuild
	cd build
	ninja -v
