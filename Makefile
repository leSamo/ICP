all:
	qmake -makefile src -o src/ ||qmake-qt5 -makefile src -o src/
	cd src/ && make

run: all
	./src/ICP

doxygen:
	doxygen Doxyfile	

clean:
	rm -rf doc/ src/*.o

pack: clean
	zip -r 1-xoleks00-xfindr00.zip *