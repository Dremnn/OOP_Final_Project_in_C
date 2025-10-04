#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <iostream>
#include "Product.h"

using namespace std;

// ============= FOOD CLASS =============
class Food : public Product {
private:
    bool isVegetarian;

public:
    Food(string name, double price, bool isVegetarian)
        : Product(name, price, FOOD) {
        this->isVegetarian = isVegetarian;
    }

    bool getIsVegetarian() { return isVegetarian; }
    void setVegetarian(bool veg) { isVegetarian = veg; }
    
    void displayInfo() {
        Product::displayInfo();
        cout << "Vegetarian: " << (isVegetarian ? "Yes" : "No") << endl;
    }
};

#endif // FOOD_H