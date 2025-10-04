#ifndef CARTITEM_H
#define CARTITEM_H

#include <string>
#include <iostream>
#include "../utils/Utils.h"
#include "../exceptions/Exceptions.h"

using namespace std;

// ============= CART ITEM CLASS =============
class CartItem {
private:
    string id;
    string productId;
    string customerId;
    int quantity;
    double unitPrice;
    string size;

public:
    CartItem(string productId, string customerId, int quantity, double unitPrice, string size = "M") {
        this->id = generateId("ITEM");
        this->productId = productId;
        this->customerId = customerId;
        this->quantity = quantity;
        this->unitPrice = unitPrice;
        this->size = size;
    }
    
    string getId() { return id; }
    string getProductId() { return productId; }
    string getCustomerId() { return customerId; }
    int getQuantity() { return quantity; }
    double getUnitPrice() { return unitPrice; }
    string getSize() { return size; }
    
    double getTotalPrice() {
        double multiplier = 1.0;
        if (size == "S") multiplier = 0.8;
        else if (size == "L") multiplier = 1.3;
        return quantity * unitPrice * multiplier;
    }

    void updateQuantity(int newQuantity) {
        if (newQuantity < 0) {
            throw ValidationException("Quantity cannot be negative");
        }
        quantity = newQuantity;
    }

    void updateSize(string newSize) {
        if (newSize != "S" && newSize != "M" && newSize != "L") {
            throw ValidationException("Invalid size. Must be S, M, or L");
        }
        size = newSize;
    }
    
    void displayInfo() {
        cout << "  Item ID: " << id << endl;
        cout << "  Product ID: " << productId << endl;
        cout << "  Quantity: " << quantity << endl;
        cout << "  Size: " << size << endl;
        cout << "  Unit Price: " << formatPrice(unitPrice) << endl;
        cout << "  Total: " << formatPrice(getTotalPrice()) << endl;
    }
};

#endif // CARTITEM_H