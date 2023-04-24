#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef USERENTRY_H
#define USERENTRY_H

class userEntry
{
public:
   userEntry();
   // CHECK THE LINE BELOW AGAIN
   userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString c, sql::SQLString a);
   string text();
   string json();

   string username;
   string email;
   string password;
   string color;
   string active;

private:
};

#endif /* USERENTRY_H */
