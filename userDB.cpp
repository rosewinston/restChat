#include <vector>
#include <iostream>
#include <regex>
#include <mariadb/conncpp.hpp>
#include "userDB.h"
#include "userEntry.h"

// Changed "contact" --> "user"
// Changed "First" --> "Username"
// Changed "Last" --> "Email"
// Changed "Phone" --> "Password"
// Changed "Type" --> "Color"
// Changed "Email" --> "Active"


/*bool isValid(const string& email)
{
  
    // Regular expression definition
    const regex pattern(
        "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
  
    // Match the string pattern
    // with regular expression
    return regex_match(email, pattern);
}*/


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

vector<userEntry> userDB::find(string search) {

	vector<userEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery(
			"SELECT * FROM users WHERE Email like '%"+search+"%' OR "+
	    	 + "Username like '%"+search+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Email"),
			res->getString("Password"),res->getString("Color"),
	    	res->getString("Active"));
	    	
	    list.push_back(entry);

    }
    return list;

}

void userDB::addEntry(string username,string email,string password,string color,string active){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());
	
  	stmnt->executeQuery("INSERT INTO users(Username,Email,Password,Color,Active) VALUES ('"+username+"','"+email+"','"+password+"','"+color+"',"+active+")");
}

// string "id" --> string "username"
userEntry userDB::fetchEntry(string username){

	userEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    //changed WHERE ID='"+idnum+"' --> WHERE Username='"+username+"'
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username = '"+username+"'");
    
    // Get username entry
    if (res->next()) {
    	entry = userEntry(res->getString("Username"),res->getString("Email"),
			res->getString("Password"),res->getString("Color"),
	    	res->getString("Active"));
    }
    return entry;
}

//takes in string email, and checks the database for any users that have that email. if no user has that email, return false, if a user has that email, return true 
boolean userDB::checkEmail(string email) {

    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Email = '%"+email+"%'");

 
	
    if (res->rowCount() == 0){
       return false; 
    }else{
       return true; 
	
    }
}

//looks for user in database, true if user exists, false if user no exist 
boolean userDB::checkUser(string username) {
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username = '%"+username+"%'");

 
	
    if (res->rowCount() == 0){
       return false; 
    }else{
       return true; 
	
    }
}

//checks if given username is associated with given password (checks if username + password combination is valid) 
boolean userDB::checkPassword(string username, string password){
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());

    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username = '%"+username+"%' AND Password = '%"+password+"%'" );

 
	
    if (res->rowCount() == 0){
       return false; 
    }else{
       return true; 
	
    }
}







void userDB::editEntry(string username,string email,string password, string color, string active){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	/*if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
	 if (!isValid(email))
         {
             email=" ";
         }  */
	
	//changed WHERE ID='"+idnum+"' --> WHERE Username='"+username+"'
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Email ='"+email+"', Password ='"+password+"', Color ='"+color+"', Active = '"+active+"' WHERE Username='"+username+"'");
  	
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
