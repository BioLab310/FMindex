CXX = g++

LD = ld

LDFLAGS = -r

CFLAGS = -O3 -msse4.2 -I

ALL=FMindex

OBJECT=build.o read.o B+tree.o basic.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
endif
ifeq ($(UNAME_S),Darwin)
	CFLAGS += -I/opt/local/include
endif

$(ALL):$(OBJECT) Main.cpp
	$(CXX) $(CFLAG) $^ -o $@ -pthread
	
.PHONY : clean

clean:
	rm *.o $(OBJECT)
