/* |IMPLEMENTATION| User.cpp file */
  
#include "user.h" 
#include <iostream> 
  
struct User::Impl { 
  
    Impl(std::string name) 
        : name(name){}; 
  
    ~Impl() = default;
  
    void welcomeMessage() 
    { 
        std::cout << "Welcome, " << name << std::endl; 
    } 
  
    std::string name; 
    int salary = -1; 
}; 
  
// Constructor connected with our Impl structure 
User::User(std::string name) 
    : pimpl(new Impl(name)) 
{ 
    pimpl->welcomeMessage(); 
} 
  
// Default destrucotr 
User::~User() = default; 
  
// Assignment operator and Copy constructor 
  
User::User(const User& other) 
    : pimpl(new Impl(*other.pimpl)) 
{ 
} 
  
User& User::operator=(User rhs) 
{ 
    swap(pimpl, rhs.pimpl); 
    return *this; 
} 
  
// Getter and setter 
int User::getSalary() 
{ 
    return pimpl->salary; 
} 
  
void User::setSalary(int salary) 
{ 
    pimpl->salary = salary; 
    std::cout << "Salary set to "
         << salary << std::endl; 
}

//widget::widget(int n) : pImpl{std::make_unique<impl>(n)} {}
//widget::widget(widget&&) = default;
//widget::~widget() = default;
//widget& widget::operator=(widget&&) = default;