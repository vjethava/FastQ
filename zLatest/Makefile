CC=g++
CCINC=-I/home/vjethava/CODE/libDAI-0.2.4/include
CCLIB=-L/home/vjethava/CODE/libDAI-0.2.4/lib
DAIFLAGS=-W -Wextra -fpic -O3 -g -DDAI_DEBUG -DDAI_WITH_BP -DDAI_WITH_FBP -DDAI_WITH_TRWBP -DDAI_WITH_MF -DDAI_WITH_HAK -DDAI_WITH_LC -DDAI_WITH_TREEEP -DDAI_WITH_JTREE -DDAI_WITH_MR -DDAI_WITH_GIBBS -DDAI_WITH_CBP
LIB=-ldai 
CCFLAGS=-Wno-deprecated -g 
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
INCS=$(wildcard *.h)

SRCDIR=src
INCDIR=inc
OBJDIR=obj


%.cpp : $(INCS)

%.o : %.cpp
	$(CC) $(CCFLAGS) $(CCINC) -c $< -o $@

main: $(OBJS)
	$(CC) $(CCLIB)  -o $@  $(OBJS) $(LIB)

all: main

clean:
	rm -f $(OBJS)
	rm -f main



