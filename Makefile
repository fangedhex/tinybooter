build: conan cmake
	cd build && make

conan: conanfile.txt
	conan install -if build --build=missing .

cmake: CMakeLists.txt
	cmake -G "Unix Makefiles" -S . -B build

clean:
	rm -rf build
