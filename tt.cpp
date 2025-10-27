#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "include/utils/Utils.h"
#include "include/enums/Enums.h"
#include "include/exceptions/Exceptions.h"

using namespace std;

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

    void setPhoneNumber(string phone) { 
        phoneNumber = phone; 
    }
    
    virtual bool canModifyProduct() = 0;
};

class Customer : public User {
private:
    string id;
    string address;
    vector<string> orderHistory;

public:
    Customer(string username, string password, string phoneNumber) 
        : User(username, password, phoneNumber, CUSTOMER) {
        this->id = generateId("CUST");
        this->address = "";
    }

    string getId() { return id; }
    string getAddress() { return address; }
    
    void setAddress(string addr) { address = addr; }
    
    bool canModifyProduct() override { 
        return false; 
    }
    
    void addOrderToHistory(string orderId) {
        orderHistory.push_back(orderId);
    }
    
    vector<string> getOrderHistory() { return orderHistory; }
};

class Admin : public User {
public:
    Admin(string username, string password, string phoneNumber)
        : User(username, password, phoneNumber, ADMIN) {}

    bool canModifyProduct() override { 
        return true; 
    }
};

class UserManager {
private:
    map<string, User*> users;
    map<string, string> sessions; // sessionToken -> userId

    void validateUserInput(string username, string password, string phoneNumber) {
        if (username.empty()) 
            throw ValidationException("Username cannot be empty");
        if (password.length() < 6) 
            throw ValidationException("Password must be at least 6 characters");
        if (phoneNumber.empty()) 
            throw ValidationException("Phone number cannot be empty");
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

class CoffeeShopSystem {
private:
    UserManager* userManager;
    
    string currentSessionToken;
    bool isInitialized;

public:
    CoffeeShopSystem() {
        userManager = new UserManager();
        currentSessionToken = "";
        isInitialized = false;
    }
    
    ~CoffeeShopSystem() {
        delete userManager;
    }
    
    // ===== USER OPERATIONS =====
    void initializeSystem() {
        if (!isInitialized) {
                userManager->registerAdmin("admin", "admin123", "0000000000");
                cout << "System initialized with default admin account" << endl;
                isInitialized = true;
        }
    }
    
    string registerCustomer(string username, string password, string phoneNumber) {
        return userManager->registerCustomer(username, password, phoneNumber);
    }
    
    void registerAdmin(string username, string password, string phoneNumber) {
        userManager->registerAdmin(username, password, phoneNumber);
    }
    
    bool login(string username, string password) {
        try {
            currentSessionToken = userManager->login(username, password);
            return true;
        } catch (AuthenticationException& e) {
            cout << e.what() << endl;
            return false;
        }
    }
    
    void logout() {
        if (!currentSessionToken.empty()) {
            userManager->logout(currentSessionToken);
            currentSessionToken = "";
        }
    }
    
    bool isLoggedIn() {
        return !currentSessionToken.empty();
    }
    
    bool isCurrentUserAdmin() {
        if (!isLoggedIn()) return false;
        return userManager->isAdmin(currentSessionToken);
    }
    
    Customer* getCurrentCustomer() {
        return userManager->getCurrentCustomer(currentSessionToken);
    }
};

int main(){
    CoffeeShopSystem system;
    system.initializeSystem(); 

    //  admin
    cout << "-Admin Flow-" << endl;
    string adminUsername = "admin";
    string adminPassword = "admin123";

    system.login(adminUsername, adminPassword);
    cout << "Logged in successfully as Admin: " << adminUsername << endl;
    
    cout << "Is current user Admin? " << (system.isCurrentUserAdmin() ? "YES" : "NO") << endl;
    
    system.logout();
    cout << "Logged out successfully." << endl;

    string customerUsername = "user_khang";
    string customerPassword = "khangpassword";

    // Customer  
    cout << "-Customer Flow-" << endl;
    string custId = system.registerCustomer(customerUsername, customerPassword, "0912345678");
    cout << "Registered Customer: " << customerUsername << " (ID: " << custId << ")" << endl;

    system.login(customerUsername, customerPassword);
    cout << "Logged in successfully as Customer: " << customerUsername << endl;
    
    cout << "Is current user Admin? " << (system.isCurrentUserAdmin() ? "YES" : "NO") << endl;
    

    Customer* currentCustomer = system.getCurrentCustomer();
    currentCustomer->setAddress("50 Le Loi St.");
    currentCustomer->addOrderToHistory("ORD_001A");
    cout << "Customer Address set to: " << currentCustomer->getAddress() << endl;
    
    system.logout();
    cout << "Logged out successfully." << endl;
    

    return 0;
}