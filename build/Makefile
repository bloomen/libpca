PROG = libpca.so

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
CXX = clang++ -stdlib=libc++
else
CXX = g++
endif

FLAGS = -O2 -Wall -std=c++0x -pthread -shared -fPIC

INCS = -I"../include"
LIBS = -larmadillo 
SRCS = ../src/*.cpp

RM = rm -f

all :
	$(CXX) $(FLAGS) $(INCS) $(SRCS) $(LIBS) -o $(PROG)

clean :
	$(RM) $(PROG)
