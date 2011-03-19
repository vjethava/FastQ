CC=g++
LD=ld
WN_ROOT=/usr/local/WordNet-3.0
DAI_ROOT=${HOME}/Code/libdai

CCINC=-I$(DAI_ROOT)/include -I$(WN_ROOT)/include
CCLIB=-L$(DAI_ROOT)/lib -L$(WN_ROOT)/lib

MATLAB_ROOT=/chalmers/sw/sup/matlab-2009b
LD_LIBRARY_PATH:=${MATLAB_ROOT}/bin/glnxa64:${LD_LIBRARY_PATH}
MINC=-I$(MATLAB_ROOT)/extern/include
# MLIB=-L$(MATLAB_ROOT)/bin/glnxa64
## glibc dependency screwing up stuff. do it in matlab separately.
MLIB=

DAIFLAGS=-W -Wextra -fpic -O3 -g -DDAI_DEBUG -DDAI_WITH_BP -DDAI_WITH_FBP -DDAI_WITH_TRWBP -DDAI_WITH_MF -DDAI_WITH_HAK -DDAI_WITH_LC -DDAI_WITH_TREEEP -DDAI_WITH_JTREE -DDAI_WITH_MR -DDAI_WITH_GIBBS -DDAI_WITH_CBP
DEBUGFLAGS=-DDEBUG_INF
## when dependency with matlab 
# LIB=-ldai `mysql_config --libs` -leng -lmx


LIB=-ldai `mysql_config5 --libs`

CCFLAGS=-Wno-deprecated -g `mysql_config5 --cflags`

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

arki: 
	scp -r mylegend.txt T.txt P.txt P2.txt runs.log result jethava@arki.ce.chalmers.se:./current

all: main 


resclean:
	rm runs.log P.txt P2.txt T.txt mylegend.txt
	rm -rf result/*

clean: 
	rm -f $(OBJS)
	rm -f ./main


