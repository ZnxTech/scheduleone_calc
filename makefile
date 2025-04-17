all: ./build/socalc

./build/socalc: socalc.c
	@mkdir ./build -p
	gcc socalc.c -o ./build/socalc -s -O3 -Wall