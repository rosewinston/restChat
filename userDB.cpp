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


void userDB::addEntry(string username,string email,string password,string color,string active){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
	
  	stmnt->executeQuery("INSERT INTO users(Username,Email,Password,Color,Active) VALUES ('"+username+"','"+email+"','"+password+"','"+color+"','"+active+"')");
}

string userDB::fetchColor(string username){

	string color;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT Color FROM users WHERE Username = '"+username+"'");
    
    while (rest->next()) {
    	color = res->getString("Color");
    }
    return color;
}

//takes in string email, and checks the database for any users that have that email. if no user has that email, return false, if a user has that email, return true 
bool userDB::checkEmail(string email) {

    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    bool res = stmnt->executeQuery("SELECT COUNT(1) FROM users WHERE Email = '%"+email+"%'");
	
    sql::ResultSet *resTest = stmnt->executeQuery("SELECT COUNT(1) FROM users WHERE Email = '%"+email+"%'");
   // cout<<"bool result: "<<res<<endl; 
    //cout<<"statement direct: "<<stmnt->executeQuery("SELECT COUNT(1) FROM users WHERE Email = '%"+email+"%'")<<endl;
    //cout<<"resTest: "<<resTest->next(); 
    //cout<<"restTest Type: " << typeid(resTest->next()).name()<<endl; 
	
    return res;
}

//looks for user in database, true if user exists, false if user no exist 
bool userDB::checkUser(string username) {
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    bool res = stmnt->executeQuery("SELECT COUNT(1) FROM users WHERE Username = '%"+username+"%'");
    return res;
}

//checks if given username is associated with given password (checks if username + password combination is valid) 
bool userDB::checkPassword(string username, string password){
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    bool res = stmnt->executeQuery("SELECT COUNT(1) FROM users WHERE Username = '%"+username+"%' AND Password = '%"+password+"%'" );
    return res;
}







void userDB::editEntry(string username,string email,string password, string color, string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

 
	
	//changed WHERE ID='"+idnum+"' --> WHERE Username='"+username+"'
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Email = '"+email+"', Password = '"+password+"', Color =' "+color+"', Active = '"+active+"' WHERE Username = '"+username+"'");
  	
}

void userDB::editStatus(string username, string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

 
	
	//changed WHERE ID='"+idnum+"' --> WHERE Username='"+username+"'
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Active = '"+active+"' WHERE Username = '"+username+"'");
  	
}











/*
void userDB::deleteEntry(string idnum){
  // Establish Connection
  std::unique_ptr<sql::Connection>  conn(driver->connect(db_url, properties));
    
  if (!conn) {
   	cerr << "Invalid database connection" << endl;
   	exit (EXIT_FAILURE);
  }

  std::auto_ptr<sql::Statement> stmt(conn->createStatement());

  //changed WHERE ID='"+idnum+"' --> WHERE Username='"+username+"'
  stmt->execute("DELETE FROM users WHERE Username='"+username+"'");
}
*/
