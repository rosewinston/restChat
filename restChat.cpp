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

string getMessagesJSON(string username, map<string,vector<string>> &messageMap) {
	/* retrieve json list of messages for this user along with a userlist*/
	bool first = true;
	string result = "{\"messages\":[";
	for (string message :  messageMap[username]) {
		if (not first) result += ",";
		result += message;
		first = false;
	}
  result += "],";
  messageMap[username].clear();
  bool loopOne = true;
  string userList = "\"userlist\":[";
  for (const auto &[user, messages]: messageMap) {
    if (not loopOne) userList += ",";
    userList += "{\"name\":""\"";
    userList += user +"\"}";
    loopOne = false;
  }
  result+=userList;
	result += "]}";
	return result;
}

int main(void) {
  Server svr;
  int nextUser=0;
  map<string,vector<string>> messageMap;
	vector<string> userList;

  /* "/" just returnsAPI name */
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("Chat API", "text/plain");
  });


  svr.Get(R"(/chat/join/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string username = req.matches[1];
    string result;
    vector<string> empty;
    cout << username << " joins" << endl;
    
    // Check if user with this name exists
    if (messageMap.count(username)) { 
    	result = "{\"status\":\"exists\"}";
    } else {
    	// Add user to messages map
      userList.push_back(username);
    	messageMap[username]=empty;
    	result = "{\"status\":\"success\",\"user\":\"" + username + "\"}";
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
    string resultJSON = getMessagesJSON(username,messageMap);
    res.set_content(resultJSON, "text/json");
  });
  
  cout << "Server listening on port " << port << endl;
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
  svr.listen("0.0.0.0", port);

}
