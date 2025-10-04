#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include "User.h"

using namespace std;

// ============= ADMIN CLASS =============
class Admin : public User {
public:
    Admin(string username, string password, string phoneNumber)
        : User(username, password, phoneNumber, ADMIN) {}

    bool canModifyProduct() { return true; }
};

#endif // ADMIN_H