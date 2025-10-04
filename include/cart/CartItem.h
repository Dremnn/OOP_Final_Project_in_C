#ifndef CARTITEM_H
#define CARTITEM_H

#include <string>
#include <iostream>
#include "../utils/Utils.h"
#include "../exceptions/Exceptions.h"
#include "../enums/Enums.h"  

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
    ProductType productType;  

public:
    CartItem(string productId, string customerId, int quantity, double unitPrice, ProductType productType, string size = "M") {
        this->id = generateId("ITEM");
        this->productId = productId;
        this->customerId = customerId;
        this->quantity = quantity;
        this->unitPrice = unitPrice;
        this->size = size;
        this->productType = productType;  
    }
    
    string getId() { return id; }
    string getProductId() { return productId; }
    string getCustomerId() { return customerId; }
    int getQuantity() { return quantity; }
    double getUnitPrice() { return unitPrice; }
    string getSize() { return size; }
    ProductType getProductType() { return productType; }  
    
    double getTotalPrice() {
        double multiplier = 1.0;
        
        // CHỈ áp dụng size multiplier cho DRINK
        if (productType == DRINK) {
            if (size == "S") multiplier = 0.8;
            else if (size == "L") multiplier = 1.3;
        }
        // FOOD luôn có multiplier = 1.0 (không phụ thuộc size)
        
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
        if (productType == DRINK) {  
            cout << "  Size: " << size << endl;
        }
        cout << "  Unit Price: " << formatPrice(unitPrice) << endl;
        cout << "  Total: " << formatPrice(getTotalPrice()) << endl;
    }
};

#endif // CARTITEM_H