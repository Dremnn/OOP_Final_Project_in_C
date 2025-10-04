#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>
#include "../enums/Enums.h"
#include "../utils/Utils.h"

using namespace std;

// ============= PRODUCT BASE CLASS =============
class Product {
protected:
    string id;
    string name;
    double price;
    bool isAvailable;
    ProductType type;

public:
    Product(string name, double price, ProductType type) {
        this->id = generateId("PROD");
        this->name = name;
        this->price = price;
        this->type = type;
        this->isAvailable = true;
    }

    virtual ~Product() {}

    string getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }
    bool getIsAvailable() { return isAvailable; }
    ProductType getType() { return type; }
    
    void setName(string n) { name = n; }
    void setPrice(double p) { price = p; }
    void setAvailable(bool available) { isAvailable = available; }
    
    virtual void displayInfo() {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Price: " << formatPrice(price) << endl;
        cout << "Type: " << (type == DRINK ? "DRINK" : "FOOD") << endl;
        cout << "Available: " << (isAvailable ? "Yes" : "No") << endl;
    }
};

#endif // PRODUCT_H