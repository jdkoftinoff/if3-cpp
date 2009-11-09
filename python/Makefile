
.PHONY : all clean install

all : clean
	python setup.py build

install : clean
	python setup.py install --user --optimize 1

clean :
	rm -r -f build

