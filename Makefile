build: cmake
	cd build && make

test: cmake build
	./build/test/tinybooter_test

cmake: CMakeLists.txt
	cmake -G "Unix Makefiles" -B build -DCMAKE_BUILD_TYPE=Debug

clean:
	rm -rf build

.PHONY: build test cmake clean
