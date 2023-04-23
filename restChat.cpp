//
//  namesAPI.cc - a microservice demo program
//
// Rose, Michelle, Sebi
// Kenyon College, 2023
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <string>
#include "httplib.h"
#include "user.h"
#include "userDB.h"
#include "userEntry.h" 

using namespace httplib;
using namespace std;

const int port = 5005;

// Takes a username, message, message map, color
// and constructs a JSON-formatted string containing the user's information and the message,
// and then this JSON string to the message list of all users in the message map.
void addMessage(string username, string message, map<string,vector<string>> &messageMap, string color) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\",\"color\":\""+color+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

// Function for when a user joins, generating an automatic token for them for use in the program instead of the username
// this token is in a map associated with the username for easy association betweeen the two
string generateToken(string username, map<string,string> &tokenMap) {
	int tokenInt = rand() % 89999999 + 10000000;
	string token=to_string(tokenInt);
	tokenMap[token] = username;
	return token;
}

// verify the token by returning the username
string verifyToken(string token, map<string,string> &tokenMap) {
	string username = tokenMap.find(token)->second;
	return username;
}

// Takes a username, message map, masterUserList, activeUserList, and user map
// Returns a JSON-formatted string with the messages for the given user, masterUserList, and activeUserList
string getMessagesJSON(string username, map<string,vector<string>> &messageMap,vector<string> &masterUserList,vector<string> &activeUserList, userDB &usrDB) {
	/* retrieve json list of messages for this user along with a userlist*/
	bool first = true;
	string user;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
	result+="],";
  messageMap[username].clear();
  first = true;
  result += "\"userlist\":[";
  for (int i=0; i<masterUserList.size(); i++) {
    if (not first) result += ",";
	user = masterUserList[i];
    result += "{\"name\":""\"";
    result += user +"\"}";
    first = false;
  }
  result+= "],";
  string activeList = "\"activelist\":[";
  for (int i = masterUserList.begin(); i != masterUserList.end(); ++i){
	username = masterUserList[i];
	if ((usrDB.fetchStatus(username)== "true") && (find(activeUserList.begin(),activeUserList.end(), activeuser) == activeUserList.end()) { //check this line//
		activeUserList.push_back(username);
	}
  }
  first = true;
  for (int i=0; i<activeUserList.size(); i++) {
	  if (not first) activeList += ",";
	  user = activeUserList[i];
	  activeList += "{\"name\":""\"";
	  activeList += user +"\"}";
	  first = false;
  }
	activeUserList.clear();
	result+=activeList;
	result += "]}";
	return result;
}

void addUser(string username, string email, string password, string color, userDB &usrDB, vector<string> &masterUserList) {
	usrDB.addEntry(username, email, password, color, "false");
	masterUserList.push_back(username);
        cout << "user created" << endl;
}

string verifyUser(string username, string email, string password, string color, userDB &usrDB, vector<string> &masterUserList) {
	string result;
	bool emailExists = usrDB.checkEmail(email);
	if (usrDB.checkUser(username) || emailExists) {
    		cout << "exists" << endl;
		result = "{\"status\":\"exists\"}";
		return result;
    } 	else {
    		addUser(username,email,password,color,usrDB,masterUserList);
		result = "{\"status\":\"success\",\"user\":\""+username+"\",\"email\":\""+email+"\",\"password\":\""+password+"\",\"color\":\""+color+"\"}";
		return result;
    }
}

// Defines various endpoints for operations
int main(void) {
  Server svr;
  int nextUser=0;
  userDB usrDB; 
  map<string,vector<string>> messageMap;
  vector<string> masterUserList = usrDB.findUsernames();
  vector<string> activeUserList;
  map<string, user> userMap;
  map<string,string> tokenMap;  
  	  
  cout << usrDB.checkEmail("winston1@kenyon.edu") << endl << usrDB.checkEmail("hitest125847") << endl;

  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


  // Registers a new user with the given username, email, password, and color
  // Got rid of: bool active = true;
  svr.Get(R"(/chat/register/(.*)/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string email = req.matches[2];
    string password = req.matches[3];
    string color = req.matches[4];
    string result;
    cout << username << email << password << color << endl;
    result = verifyUser(username,email,password,color,usrDB,masterUserList);
	res.set_content(result, "text/json");
  });

  // Logs in a user with the given username and password
  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string password = req.matches[2];
    string result;
    vector<string> empty;
    // Check if username and password matches with registered user
    cout<<"username: "<<username<<" "<<"password: "<<password<<endl; 
    cout<<usrDB.checkPassword(username,password)<<endl; 
    if (usrDB.checkPassword(username, password)) {
        messageMap[username]=empty;
		string token=generateToken(username, tokenMap);
		result = "{\"status\":\"success\",\"user\":\"" + token + "\"}";
	cout << username << " joins" << endl;
	usrDB.editStatus(username, "true");
    }
    else {
      result = "{\"status\":\"exists\"}";
    }
	
    res.set_content(result, "text/json");
  });

   // Sends a message from the given username's user
   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string token = req.matches[1];
	string message = req.matches[2];
	string result; 
	string username = verifyToken(token,tokenMap);
    if (!messageMap.count(username)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		string userColor = usrDB.fetchColor(username);
		addMessage(username,message,messageMap,userColor);
		result = "{\"status\":\"success\"}";
	}
    res.set_content(result, "text/json");
  });
  
   //Fetches messages and user lists for the given username's user.
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string token = req.matches[1];
	res.set_header("Access-Control-Allow-Origin","*");
    string username = verifyToken(token,tokenMap);
    string resultJSON = getMessagesJSON(username,messageMap,masterUserList,activeUserList,usrDB);
    res.set_content(resultJSON, "text/json");  
    });

  // Returns a JSON-formatted string with a status of "success" and the given name
  svr.Get(R"(/chat/hello/(.*))", [&](const Request& req, Response& res) {
    string name = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = "{\"status\":\"success\",\"name\":\""+name+"\"}";
    res.set_content(resultJSON, "text/json");
  });

  // Logs out the user with the given name.
  svr.Get(R"(/chat/logout/(.*))", [&](const Request& req, Response& res) {
    string token = req.matches[1];
    string name = verifyToken(token,tokenMap);
    res.set_header("Access-Control-Allow-Origin","*");
	usrDB.editStatus(name, "false");
    string resultJSON = "{\"status\":\"success\",\"name\":\""+name+"\"}"; 
	res.set_content(resultJSON, "text/json");
  });

  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
