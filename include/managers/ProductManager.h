#ifndef PRODUCTMANAGER_H
#define PRODUCTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include "../products/Product.h"
#include "../products/Drink.h"
#include "../products/Food.h"
#include "../exceptions/Exceptions.h"
#include "UserManager.h"

using namespace std;

class ProductManager {
private:
    map<string, Product*> products;

    void validateProductInput(string name, double price) {
        if (name.empty()) {
            throw ValidationException("Product name cannot be empty");
        }
        if (price <= 0) {
            throw ValidationException("Product price must be positive");
        }
    }

public:
    ~ProductManager() {
        for (auto& pair : products) {
            delete pair.second;
        }
    }

    string addDrink(string name, double price, string size, bool isHot, string sessionToken, UserManager* userManager) {
        if (!userManager->isAdmin(sessionToken)) {
            throw AuthorizationException("Only admin can add products");
        }
        
        validateProductInput(name, price);
        
        Drink* drink = new Drink(name, price, size, isHot);
        products[drink->getId()] = drink;
        return drink->getId();
    }
    
    string addFood(string name, double price, bool isVegetarian, string sessionToken, UserManager* userManager) {
        if (!userManager->isAdmin(sessionToken)) {
            throw AuthorizationException("Only admin can add products");
        }
        
        validateProductInput(name, price);
        
        Food* food = new Food(name, price, isVegetarian);
        products[food->getId()] = food;
        return food->getId();
    }

    Product* getProduct(string productId) {
        if (products.find(productId) == products.end()) {
            throw ValidationException("Product not found: " + productId);
        }
        return products[productId];
    }
    
    void updateProduct(string productId, string name, double price, bool available, string sessionToken, UserManager* userManager) {
        if (!userManager->isAdmin(sessionToken)) {
            throw AuthorizationException("Only admin can update products");
        }
        
        Product* product = getProduct(productId);
        validateProductInput(name, price);
        
        product->setName(name);
        product->setPrice(price);
        product->setAvailable(available);
    }
    
    void deleteProduct(string productId, string sessionToken, UserManager* userManager) {
        if (!userManager->isAdmin(sessionToken)) {
            throw AuthorizationException("Only admin can delete products");
        }
        
        if (products.find(productId) == products.end()) {
            throw ValidationException("Product not found: " + productId);
        }
        
        delete products[productId];
        products.erase(productId);
    }

    vector<Product*> getAllProducts() {
        vector<Product*> result;
        for (auto& pair : products) {
            if (pair.second->getIsAvailable()) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
    
    vector<Product*> getProductsByType(ProductType type) {
        vector<Product*> result;
        for (auto& pair : products) {
            if (pair.second->getType() == type && pair.second->getIsAvailable()) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
};

#endif // PRODUCTMANAGER_H