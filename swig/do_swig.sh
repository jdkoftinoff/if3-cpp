

swig -DIF2K_CONFIG_POSIX=1 -Wall -classic -I../include -python -c++ -module if2kkernel  -MF if2k_swig.d -o python/if2k_swig_wrap.cpp -outdir python if2k_swig.i
swig -DIF2K_CONFIG_POSIX=1 -Wall -I../include -java -c++ -module if2kkernel  -MF if2k_swig.d -o java/if2k_swig_wrap.cpp -outdir java if2k_swig.i
swig -DIF2K_CONFIG_POSIX=1 -Wall -I../include -lua -c++ -module if2kkernel -o lua/if2k_swig_wrap.cpp -outdir lua if2k_swig.i

#mv -f if2k_swig_wrap.cxx if2k_swig_wrap.cpp

#mac osx:
#all : sp.o
#	swig -python -classic -c++ -module sp sp.h
#	g++ -I /usr/include/python2.5 -c sp_wrap.cxx -o sp_wrap.o
#	ld -bundle -flat_namespace -undefined suppress -o _sp.so sp_wrap.o sp.o -lstdc++-static



