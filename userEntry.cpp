#include "userEntry.h"

userEntry::userEntry()
{
}


userEntry::userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString c, sql::SQLString a)
{
	username = u;
	email = e;
	password = p;
	color = c;
	active = a;
}

string userEntry::text()
{
	string result = username + ". ";
	result += email + " ";
	result += password + " ";
	result += color + " ";
	result += active;
	return result;
}

string userEntry::json()
{
	string result = "{\"username\":\"" + username + "\",";
	result += "\"email\":\"" + email + "\",";
	result += "\"password\":\"" + password + "\",";
	result += "\"color\":\"" + color + "\",";
	result += "\"active\":\"" + active + "\"}";

	return result;
}
