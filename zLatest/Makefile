CC=g++
CCINC=-I/home/vjethava/CODE/libDAI-0.2.4/include
CCLIB=-L/home/vjethava/CODE/libDAI-0.2.4/lib

MINC=-I/opt/matlab7/extern/include
MLIB=-L'/opt/matlab7/bin/glnxa64'

DAIFLAGS=-W -Wextra -fpic -O3 -g -DDAI_DEBUG -DDAI_WITH_BP -DDAI_WITH_FBP -DDAI_WITH_TRWBP -DDAI_WITH_MF -DDAI_WITH_HAK -DDAI_WITH_LC -DDAI_WITH_TREEEP -DDAI_WITH_JTREE -DDAI_WITH_MR -DDAI_WITH_GIBBS -DDAI_WITH_CBP
DEBUGFLAGS=-DDEBUG_INF

LIB=-ldai `mysql_config --libs` -leng -lmx

CCFLAGS=-Wno-deprecated -g `mysql_config --cflags`

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
INCS=$(wildcard *.h)

USERDEFS=
SRCDIR=src
INCDIR=inc
OBJDIR=obj

%.cpp : $(INCS)

%.o : %.cpp
	$(CC) $(USERDEFS) $(CCFLAGS) $(CCINC) $(MINC) -c $< -o $@

main: $(OBJS)
	$(CC) $(CCLIB) $(MLIB) -o $@  $(OBJS) $(LIB)

all: main 

clean:
	rm -f $(OBJS)
	rm -f ./main
