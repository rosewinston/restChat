#include "userEntry.h"

userEntry::userEntry() {
}

//COME BACK AND EDIT SQL DATATYPES BELOW 
userEntry::userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString c, sql::SQLString a, sql::SQLString t) {
    username = u;
    email = e;
    password = p;
    color = c;
    active = a; 
    token = t; 
   
}

string userEntry::text() {
	string result = username + ". ";
	result += email + " ";
	result += password + " ";
	result += color + " ";
	result += active + " ";
	result += token;
	return result;

}

string userEntry::json() {
	string result = "{\"username\":\"" + username + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\",";
	result += "\"color\":\"" + color + "\",";
	result += "\"active\":\"" + active + "\",";
	result += "\"token\":\"" + token + "\"}";
	
	return result;

}
