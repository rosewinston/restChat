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

//CHANGE LINE 20 BELOW TO FUTURE userEntry.h 
#include "userEntry.h"

#ifndef USERDB_H
#define USERDB_H

//NOTE FOR LATER change user, change pass,
#define DB_URL "jdbc:mariadb://localhost:3306/RestChatUsers"
#define USER "root"
#define PASS "ubuntu"

using namespace std;

class userDB {
public:
    userDB();
  
 //edit and look over everything else later 
    vector<contactEntry> find(string search);
    vector<contactEntry> findByFirst(string first);
    vector<contactEntry> findByLast(string last);
    vector<contactEntry> findByType(string type);
    contactEntry fetchEntry(string id);
    void addEntry(string first,string last,string phone,string type,string email,string address);
    void editEntry(string idnum,string first,string last,string phone,string type,string email,string address);
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
