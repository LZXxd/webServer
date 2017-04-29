CC = g++

LIB = -pthread

all: webserver

webserver: webServer.cpp locker.h thread_pool.h task.h
	$(CC) -o webserver webServer.cpp locker.h thread_pool.h task.h $(LIB)

clean:
	rm -f *.o *~
