#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
/* uncomment for applications that use vectors */
#include <vector>

//#include "mysql_connection.h"
//#include "mysql_driver.h"
#include <mariadb/conncpp.hpp>

//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>

#include "userEntry.h"

#ifndef USERDB_H
#define USERDB_H


#define DB_URL "jdbc:mariadb://localhost:3306/RestChatUsers"
#define USER "root"
#define PASS "0606"

using namespace std;

class userDB {
public:
    userDB();
    vector<contactEntry> find(string search);
    userEntry fetchEntry(string user);
    void addEntry(string user,string email,string password,string color, bool active);
    void editEntry(string user,string email,string password,string color, bool active);
    void deleteEntry(string idnum);
private:
    const string db_url=DB_URL;
    const string user=USER;
    const string pass=PASS;
    sql::Driver* driver;
    sql::Properties properties;
    std::unique_ptr<sql::Connection> conn;

};

#endif /* contactDB_H */
