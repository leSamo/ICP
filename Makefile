all:
	qmake-qt5 -makefile src -o build/
	cd build/; make

run: all
	./build/ICP

doxygen:
	doxygen Doxyfile	

clean:
	rm -rf doc/ build/

pack: clean
	zip 1-xoleks00-xfindr00.zip *