

FLAGS	= -Wall -DLINUX -g
CC	= g++
CFLAGS	= 

HDRS	= boggle_board.h wordtree.h common.h boggler.h
OTHERS	= Makefile
OBJS	= boggle_board.o wordtree.o common.o boggler.o

all:		boggler

boggler:	$(OBJS) $(HDRS)
		$(CC) $(CFLAGS) $(OBJS) -o boggler

.cc.o:		$*.c $(HDRS) $(OTHERS)
		$(CC) $(CFLAGS) $(FLAGS) -c -o $*.o $<

clean:		
		rm -f *.o *~ boggler




