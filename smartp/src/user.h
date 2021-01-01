#pragma once 
#include <memory> // PImpl 
#include <string> 
  
class User { 
public: 
    // Constructor and Destructors 
  
    ~User(); 
    User(std::string name); 
  
    // Asssignment Operator and Copy Constructor 
  
    User(const User& other); 
    User& operator=(User rhs); 
  
    // Getter 
    int getSalary(); 
  
    // Setter 
    void setSalary(int); 
  
private: 
    // Internal implementation class 
    class Impl; 
  
    // Pointer to the internal implementation 
    std::unique_ptr<Impl> pimpl; 
}; 