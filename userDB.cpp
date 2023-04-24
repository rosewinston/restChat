#include <vector>
#include <iostream>
#include <regex>
#include <mariadb/conncpp.hpp>
#include "userDB.h"
#include "userEntry.h"

userDB::userDB() {
  	// Instantiate Driver
  	driver = sql::mariadb::get_driver_instance();
 	// Configure Connection
  	// The URL or TCP connection string format is
  	// ``jdbc:mariadb://host:port/database``.
  	sql::SQLString url(db_url);
	// Use a properties map for the other connection options
  	sql::Properties my_properties({{"user", user}, {"password",pass}});
  	// Save properties in object
  	properties = my_properties;
    	// Establish Connection
  	std::unique_ptr<sql::Connection> my_conn(driver->connect(db_url, properties));
    // Check success
    if (!my_conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
   	// Save connection in object
   	conn = std::move(my_conn);
}

// add user entry with username, email, password, color and active
void userDB::addEntry(string username,string email,string password,string color,string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
	}
  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
  	stmnt->executeQuery("INSERT INTO users(Username,Email,Password,Color,Active) VALUES ('"+username+"','"+email+"','"+password+"','"+color+"','"+active+"')");
}

//create a vector of all users in the database
vector<string> userDB::findUsernames() {
    vector<string> list;
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
    std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT Username FROM users");
    // Loop through and print results
    while (res->next()) {
    	string entry(res->getString("Username"));
	    list.push_back(entry);
    }
    return list;
}

//create a vector of all active users in the database
vector<string> userDB::findActiveUsers() {

	vector<string> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
    std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    string userActive = "true"; 
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT Username FROM users WHERE Active = '"+userActive+"'");
    // Loop through and print results
    while (res->next()) {
    	string entry(res->getString("Username"));
	    list.push_back(entry);
    }
    return list;
}

//return the color of username from the database
string userDB::fetchColor(string username){
    string color;	
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}
  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
    sql::ResultSet *res = stmnt->executeQuery("SELECT Color FROM users WHERE Username = '"+username+"'");
    while (res->next()) {
    	color = res->getString("Color");
    }
    return color;
}

//return the status of user from the database
string userDB::fetchStatus(string username){
    string status;	
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}
  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
    sql::ResultSet *res = stmnt->executeQuery("SELECT Active FROM users WHERE Username = '"+username+"'");
    while (res->next()) {
    	status = res->getString("Active");
    }
    return status;
}

//takes in string email, and checks the database for any users that have that email. if no user has that email, return false, if a user has that email, return true 
bool userDB::checkEmail(string email) {
    bool status;
   // vector<string> list;
    vector<userEntry> list; 
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Email = '"+email+"'");
	if (res->next()) {
		status = true;
	}
	else {status = false;}
	
//     while (res->next()) {
//     	userEntry entry(res->getString("Username"),res->getString("Email"),
// 			res->getString("Password"),res->getString("Color"),
// 	    		res->getString("Active"));
//     	list.push_back(entry);
//     }
//     if (list.size()>0){
// 	    status = true;
//     }else{status=  false;}
    return status;	
}

//looks for user in database, true if user exists, false if user no exist 
bool userDB::checkUser(string username) {
    bool status;
    vector<userEntry> list; 
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username = '"+username+"'");

	if (res->next()) {
		status = true;
	}
	else {status = false;}
	
//     while (res->next()) {
//     	userEntry entry(res->getString("Username"),res->getString("Email"),
// 			res->getString("Password"),res->getString("Color"),
// 	    		res->getString("Active"));
// 	list.push_back(entry);
//     }
//     if (list.size()>0){
// 	    status = true;
//     }else{status=  false;}
    return status;	
}

//checks if given username is associated with given password (checks if username + password combination is valid) 
bool userDB::checkPassword(string username, string password){
    bool status;
    vector<userEntry> list; 
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username = '"+username+"' AND Password = '"+password+"'");
	
	if (res->next()) {
		status = true;
	}
	else {status = false;}
//     while (res->next()) {
//     	userEntry entry(res->getString("Username"),res->getString("Email"),
// 			res->getString("Password"),res->getString("Color"),
// 	    	res->getString("Active"));
	    	
// 	    list.push_back(entry);
//     }
//     if (list.size()>0){
// 	status = true;
//     }else{status=  false;}
	return status;		
}

void userDB::editEntry(string username,string email,string password, string color, string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}
  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Email = '"+email+"', Password = '"+password+"', Color =' "+color+"', Active = '"+active+"' WHERE Username = '"+username+"'");	
}

void userDB::editStatus(string username, string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}
  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Active = '"+active+"' WHERE Username = '"+username+"'");
  	
}
