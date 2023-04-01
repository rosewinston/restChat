#include <iostream>
#include <fstream> 
#include <map>
#include <algorithm>


#include "user.h" 
using namespace std; 

user::user(){
  username = "";
  email = "";
  password = "";
  color = "";
  
}

user::user(string username, string email, string password, string color){
  this->username = username; 
  this->email = email;
  this->password = password;
  this->color = color; 
}

string user::getEmail(){
  return email;
}

bool emailUnique(map<string, user> userMap, string email){
  for (auto pair : userMap){
      if (email==pair.second.getEmail()){
        return false;
      }
  }
  return true; 
}

bool nameUnique(map<string, user> userMap, string username){
  if (userMap.count(username)){
    return true; 
  }else{
    return false;
  }
}
