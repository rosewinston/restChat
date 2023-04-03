
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
  active; 
  
}

user::user(string username, string email, string password, string color, bool active){
  this->username = username; 
  this->email = email;
  this->password = password;
  this->color = color; 
  this->active = true;
}

string user::getEmail(){
  return email;
}

string user::getPassword(){
  return password;
}
