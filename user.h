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
  user(string username, string email, string password, string color, bool active);
  string getEmail(); 
  string getPassword();
  bool setActive(bool status);
  bool getActive();

  
private: 
  string username;
  string email; 
  string password; 
  string color; 
  bool active; 
  
};

#endif 
