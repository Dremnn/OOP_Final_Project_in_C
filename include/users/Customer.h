#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>
#include "User.h"
#include "../utils/Utils.h"

using namespace std;

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

#endif // CUSTOMER_H