all: debug release

build:
	mkdir -p build

debug: build
	mkdir -p build/debug
	cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_SHARED_LIBS=ON -GNinja ../..

release: build
	mkdir -p build/release
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -GNinja ../..

.PHONY:
clean:
	rm -rf debug
