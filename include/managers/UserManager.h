#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <map>
#include <string>
#include "../users/User.h"
#include "../users/Customer.h"
#include "../users/Admin.h"
#include "../exceptions/Exceptions.h"
#include "../utils/Utils.h"

using namespace std;

class UserManager {
private:
    map<string, User*> users;
    map<string, string> sessions; // sessionToken -> userId

    void validateUserInput(string username, string password, string phoneNumber) {
        if (username.empty()) {
            throw ValidationException("Username cannot be empty");
        }
        if (password.length() < 6) {
            throw ValidationException("Password must be at least 6 characters");
        }
        if (phoneNumber.empty()) {
            throw ValidationException("Phone number cannot be empty");
        }
    }

    bool userExists(string username) {
        for (auto& pair : users) {
            if (pair.second->getUsername() == username) {
                return true;
            }
        }
        return false;
    }

public:
    ~UserManager() {
        for (auto& pair : users) {
            delete pair.second;
        }
    }

    string registerCustomer(string username, string password, string phoneNumber) {
        validateUserInput(username, password, phoneNumber);
        
        if (userExists(username)) {
            throw ValidationException("Username already exists: " + username);
        }
        
        Customer* customer = new Customer(username, password, phoneNumber);
        users[customer->getId()] = customer;
        return customer->getId();
    }
    
    void registerAdmin(string username, string password, string phoneNumber) {
        validateUserInput(username, password, phoneNumber);
        
        if (userExists(username)) {
            throw ValidationException("Username already exists: " + username);
        }
        
        Admin* admin = new Admin(username, password, phoneNumber);
        users[username] = admin;
    }

    string login(string username, string password) {
        User* user = NULL;
        string userId = "";
        
        for (auto& pair : users) {
            if (pair.second->getUsername() == username) {
                user = pair.second;
                userId = pair.first;
                break;
            }
        }
        
        if (user == NULL) {
            throw AuthenticationException("User not found: " + username);
        }
        
        if (user->getPassword() != password) {
            throw AuthenticationException("Invalid password for user: " + username);
        }
        
        string sessionToken = generateId("TOKEN");
        sessions[sessionToken] = userId;
        return sessionToken;
    }

    void logout(string sessionToken) {
        if (sessions.find(sessionToken) == sessions.end()) {
            throw AuthenticationException("Invalid session token");
        }
        sessions.erase(sessionToken);
    }

    User* getCurrentUser(string sessionToken) {
        if (sessions.find(sessionToken) == sessions.end()) {
            throw AuthenticationException("Session not found or expired");
        }
        
        string userId = sessions[sessionToken];
        if (users.find(userId) == users.end()) {
            throw AuthenticationException("User not found for this session");
        }
        
        return users[userId];
    }
    
    Customer* getCurrentCustomer(string sessionToken) {
        User* user = getCurrentUser(sessionToken);
        if (user->getRole() != CUSTOMER) {
            throw AuthorizationException("Current user is not a customer");
        }
        return (Customer*)user;
    }

    bool isAdmin(string sessionToken) {
        try {
            User* user = getCurrentUser(sessionToken);
            return (user != NULL && user->getRole() == ADMIN);
        } catch (AuthenticationException& e) {
            return false;
        }
    }
};

#endif // USERMAN