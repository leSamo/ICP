all:
	gcc receive.c -lmosquitto -o receive
	gcc send.c -lmosquitto -o send

clean:
	rm -f receive send mqqt-testing.zip

pack:
	zip mqqt-testing.zip Makefile receive.c send.c