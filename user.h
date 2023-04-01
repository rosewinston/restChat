#ifndef user_h
#define user_h
#include <iostream>
#include <fstream> 
#include <map>
#include <algorithm>

using namespace std; 


class user {
public: 
  user(); 
  user(string username, string email, string password, string color);
  string getEmail(); 
  bool emailUnique(map<string, User> userMap);
  bool nameUnique(map<string, User> userMap);
  
private: 
  string username;
  string email; 
  string password; 
  string color; 
  
};

#endif 
