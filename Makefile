all:
	qmake -makefile src -o src/ ||qmake-qt5 -makefile src -o src/
	cd src/ && make

run: all
	./src/ICP

doxygen:
	doxygen Doxyfile	

clean:
	rm -rf doc/ build/

pack: clean
	zip 1-xoleks00-xfindr00.zip *