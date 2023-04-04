//
//  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "httplib.h"
#include "user.h"

using namespace httplib;
using namespace std;

const int port = 5005;

void addMessage(string username, string message, map<string,vector<string>> &messageMap) {
	/* iterate through users adding message to each */
	string jsonMessage = "{\"user\":\""+username+"\",\"message\":\""+message+"\"}";
	for (auto userMessagePair : messageMap) {
		username = userMessagePair.first;
		messageMap[username].push_back(jsonMessage);
	}
}

string getMessagesJSON(string username, map<string,vector<string>> &messageMap,vector<string> &masterUserList,vector<string> &activeUserList, map<string,user> &userMap) {
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
  for (auto pair : userMap){
	string activeuser = pair.first;
	if (pair.second.getActive() && find(activeUserList.begin(),activeUserList.end(), activeuser) == activeUserList.end()) {
		activeUserList.push_back(pair.first);
		cout << pair.first << " added!";
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
	result+=activeList;
	result += "]}";
	return result;
}

int main(void) {
  Server svr;
  int nextUser=0;
  map<string,vector<string>> messageMap;
  vector<string> masterUserList;
  vector<string> activeUserList;
  map<string, user> userMap;
	
  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


  svr.Get(R"(/chat/register/(.*)/(.*)/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string email = req.matches[2];
    string password = req.matches[3];
    string color = req.matches[4];
    bool active = true;
    string result;
    cout << username << email << password << color << endl;
    bool emailUnique = true;
    for (auto pair : userMap){
	   if (email == pair.second.getEmail()){
		   emailUnique = false;
       cout << "email not unique" << endl;
	   } 
    }
    if (userMap.count(username) || !emailUnique) {
    	cout << "exists" << endl;
      result = "{\"status\":\"exists\"}";
    } else {
    	// Add user to messages map
        user newUser(username, email, password, color, active);
        cout << "user created" << endl;
        userMap[username]=newUser;
        masterUserList.push_back(username);
	result = "{\"status\":\"success\",\"user\":\""+username+"\",\"email\":\""+email+"\",\"password\":\""+password+"\",\"color\":\""+color+"\"}";
    }
	res.set_content(result, "text/json");
  });
  svr.Get(R"(/chat/join/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string password = req.matches[2];
    string result;
    vector<string> empty;
    cout << username << " joins" << endl;
    
    // Check if username and password matches with registered user
    if (userMap.count(username)) {
      if (password==userMap[username].getPassword()) {
        messageMap[username]=empty;
		result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
      }
    }
    else {
      result = "{\"status\":\"exists\"}";
    }
    res.set_content(result, "text/json");
  });

   svr.Get(R"(/chat/send/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
	string username = req.matches[1];
	string message = req.matches[2];
	string result; 
	
    if (!messageMap.count(username)) {
    	result = "{\"status\":\"baduser\"}";
	} else {
		addMessage(username,message,messageMap);
		result = "{\"status\":\"success\"}";
	}
    res.set_content(result, "text/json");
  });
  
   svr.Get(R"(/chat/fetch/(.*))", [&](const Request& req, Response& res) {
    string username = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = getMessagesJSON(username,messageMap,masterUserList,activeUserList,userMap);
    res.set_content(resultJSON, "text/json");  
    });
  svr.Get(R"(/chat/hello/(.*))", [&](const Request& req, Response& res) {
    string name = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = "{\"status\":\"success\",\"name\":\""+name+"\"}";
    res.set_content(resultJSON, "text/json");
  });

  svr.Get(R"(/chat/logout/(.*))", [&](const Request& req, Response& res) {
    string name = req.matches[1];
    res.set_header("Access-Control-Allow-Origin","*");
    string resultJSON = "{\"status\":\"success\",\"name\":\""+name+"\"logged out\"}"; 
  });

  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}