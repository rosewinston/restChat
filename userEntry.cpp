#include "userEntry.h"

userEntry::userEntry()
{
}

// creates userEntry object with given parameters 
userEntry::userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString c, sql::SQLString a)
{
	username = u;
	email = e;
	password = p;
	color = c;
	active = a;
}

// creates string result containing all attributes of userEntry object 
string userEntry::text()
{
	string result = username + ". ";
	result += email + " ";
	result += password + " ";
	result += color + " ";
	result += active;
	return result;
}

//formats userEntry object's attributes and information into json 
string userEntry::json()
{
	string result = "{\"username\":\"" + username + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\",";
	result += "\"color\":\"" + color + "\",";
	result += "\"active\":\"" + active + "\"}";

	return result;
}
