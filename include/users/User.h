#ifndef USER_H
#define USER_H

#include <string>
#include "../enums/Enums.h"

using namespace std;

// ============= USER BASE CLASS =============
class User {
protected:
    string username;
    string password;
    string phoneNumber;
    UserRole role;

public:
    User(string username, string password, string phoneNumber, UserRole role) {
        this->username = username;
        this->password = password;
        this->phoneNumber = phoneNumber;
        this->role = role;
    }

    virtual ~User() {}

    string getUsername() { return username; }
    string getPassword() { return password; }
    string getPhoneNumber() { return phoneNumber; }
    UserRole getRole() { return role; }

    void setPhoneNumber(string phone) { phoneNumber = phone; }
    
    virtual bool canModifyProduct() = 0;
};

#endif // USER_H