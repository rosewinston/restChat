#include <vector>
#include <iostream>
#include <regex>
#include <mariadb/conncpp.hpp>
#include "userDB.h"
#include "userEntry.h"

// Changed "contact" --> "user"
// Changed "First" --> "Username"
// Changed "Last" --> "Password"
// Changed "Phone" --> "Color"
// Changed "Type" --> "Active"


bool isValid(const string& email)
{
  
    // Regular expression definition
    const regex pattern(
        "(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
  
    // Match the string pattern
    // with regular expression
    return regex_match(email, pattern);
}


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
			"SELECT * FROM users WHERE Password like '%"+search+"%' OR "+
    		 + "Username like '%"+search+"%' OR " +
    		 + "Type like '%"+search+"%' OR " +
	    	 + "Email like '%"+search+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"), res->getString("Age"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<userEntry> userDB::findByPassword(string password) {

	vector<userEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Password like '%"+password+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"),res->getString("Age"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<userEntry> userDB::findByUsername(string username) {

	vector<userEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Username like '%"+username+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"),res->getString("Age"));
	    	
	    list.push_back(entry);

    }
    return list;
}

vector<userEntry> userDB::findByType(string type) {
	vector<userEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Type like '%"+type+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"),res->getString("Age"));
	    	
	    list.push_back(entry);

    }
    return list;

}

//HERE!
vector<userEntry> userDB::findByEmail(string email) {
	vector<userEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE Type like '%"+email+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	userEntry entry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"),res->getString("Age"));
	    	
	    list.push_back(entry);

    }
    return list;

}

void userDB::addEntry(string username,string password,string color, string type, string email, string age){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
	
	  if (!isValid(email))
          {
              email=" ";
          }   
	
	
  	stmnt->executeQuery("INSERT INTO users(Username,Password,Color,Type,Email,Age) VALUES ('"+username+"','"+password+"','"+color+"','"+type+"','"+email+"','"+age+"')");
}

userEntry userDB::fetchEntry(string id){

	userEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM users WHERE ID = '"+id+"'");
    
    // Get username entry
    if (res->next()) {
    	entry = userEntry(res->getString("Username"),res->getString("Password"),
			res->getString("Color"),res->getString("Type"),
	    	res->getString("ID"),res->getString("Email"),res->getString("Age"));
    }
    return entry;
}

void userDB::editEntry(string idnum,string username,string password,string color, string type, string email, string age){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
	 if (!isValid(email))
         {
             email=" ";
         }  
	
  	stmnt->executeQuery("UPDATE users SET Username = '"+username+"', Password ='"+password+"', Color ='"+color+"', Type ='"+type+"', Email = '"+email+"', Age='"+age+"' WHERE ID='"+idnum+"'");
  	
}


void userDB::deleteEntry(string idnum){
  // Establish Connection
  std::unique_ptr<sql::Connection>  conn(driver->connect(db_url, properties));
    
  if (!conn) {
   	cerr << "Invalid database connection" << endl;
   	exit (EXIT_FAILURE);
  }

  std::auto_ptr<sql::Statement> stmt(conn->createStatement());

  stmt->execute("DELETE FROM users WHERE ID='"+idnum+"'");
}
