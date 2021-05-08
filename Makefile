all:
	# build project

doxygen:
	doxygen Doxyfile	

clean:
	rm -rf doc/ build/

pack: clean
	zip 1-xoleks00-xfindr00.zip *