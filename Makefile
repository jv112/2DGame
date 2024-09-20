.PHONY: all run

all:
	cmake -S . -B build
	cmake --build build --config Release

run:
	./build/bin/release/main.exe

clean:
	rm -rf build