# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -std=c++14  -Wno-deprecated-declarations

RM= /bin/rm -f


all: PutHTML restChat

PutHTML:
	cp restChat.html /var/www/html/restChat/
	cp restChat.css /var/www/html/restChat/
	cp restChat.js /var/www/html/restChat/
	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/restChat/

userEntry.o: userEntry.cpp userEntry.h
	$(CC) -c $(CFLAGS) userEntry.cpp

userDB.o: userDB.cpp userDB.h
	$(CC) -c $(CFLAGS) -I/usr/include/cppconn userDB.cpp

restChat.o: restChat.cpp userEntry.h userDB.h httplib.h
	$(CC) -c $(CFLAGS) restChat.cpp
	
restChat: restChat.o userDB.o userEntry.o
	$(CC) restChat.o userDB.o userEntry.o -L/usr/lib -o restChat -L/usr/local/lib -lmariadbcpp


clean:
	rm -f *.o  restChat






