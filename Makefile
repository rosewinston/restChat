# MakeFile for restChat
# server using C++ Microservice
# sudo mkdir /var/www/html/restChat
# sudo chown ubuntu /var/www/html/restChat


#
#all: PutHTML restChat

#PutHTML:
#	cp restChat.html /var/www/html/restChat/
#	cp restChat.css /var/www/html/restChat/
#	cp restChat.js /var/www/html/restChat/
#	echo "Current contents of your HTML directory: "
#	ls -l /var/www/html/restChat/

#restChat : restChat.cpp httplib.h user.cpp user.h userDB.cpp userDB.h
#	$(CXX) -o restChat $(CXXFLAGS) restChat.cpp user.cpp userDB.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) 
	
#userEntry.o: userEntry.cpp userEntry.h
#	$(CC) -c $(CFLAGS) userEntry.cpp

#userDB.o: userDB.cpp userDB.h
#	$(CC) -c $(CFLAGS) -I/usr/include/cppconn userDB.cpp


	


#clean:
#	rm restChat *.o
#*/
