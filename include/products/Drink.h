#ifndef DRINK_H
#define DRINK_H

#include <string>
#include <iostream>
#include "Product.h"

using namespace std;

class Drink : public Product {
private:
    string size;
    bool isHot;

public:
    Drink(string name, double price, string size, bool isHot)
        : Product(name, price, DRINK) {
        this->size = size;
        this->isHot = isHot;
    }

    string getSize() { return size; }
    bool getIsHot() { return isHot; }
    
    void setSize(string s) { size = s; }
    void setIsHot(bool hot) { isHot = hot; }
    
    void displayInfo() override {
        Product::displayInfo();
        cout << "Size: " << size << endl;
        cout << "Temperature: " << (isHot ? "Hot" : "Cold") << endl;
    }
};

#endif // DRINK_H