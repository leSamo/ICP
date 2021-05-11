all:
	g++ receive.cpp -lmosquitto -o receive
	g++ send.cpp -lmosquitto -o send

clean:
	rm -f receive send example.zip

pack: clean
	zip example.zip *