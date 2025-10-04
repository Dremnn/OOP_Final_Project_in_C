#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "User.h"

using namespace std;

class Admin : public User {
public:
    Admin(string username, string password, string phoneNumber)
        : User(username, password, phoneNumber, ADMIN) {}

    bool canModifyProduct() override { 
        return true; 
    }
};

#endif // ADMIN_H