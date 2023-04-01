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
