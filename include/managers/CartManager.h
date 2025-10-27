#ifndef CARTMANAGER_H
#define CARTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include "../cart/CartItem.h"
#include "../exceptions/Exceptions.h"
#include "../enums/Enums.h"  

using namespace std;

class CartManager {
private:
    map<string, vector<CartItem*>> userCarts;

public:
    ~CartManager() {
        for (auto& pair : userCarts) {
            for (CartItem* item : pair.second) {
                delete item;
            }
        }
    }

    // Cập nhật hàm addToCart để nhận ProductType
    CartItem* addToCart(string customerId, string productId, int quantity, double unitPrice, ProductType productType, string size = "M") {
        if (quantity <= 0) {
            throw ValidationException("Quantity must be positive");
        }
        
        // Truyền productType vào constructor của CartItem
        CartItem* item = new CartItem(productId, customerId, quantity, unitPrice, productType, size);
        userCarts[customerId].push_back(item);
        return item;
    }

    vector<CartItem*> getCart(string customerId) {
        if (userCarts.find(customerId) != userCarts.end()) {
            return userCarts[customerId];
        }
        return vector<CartItem*>();
    }
    
    void updateCartItem(string customerId, string itemId, int newQuantity) {
        if (userCarts.find(customerId) != userCarts.end()) {
            vector<CartItem*>& cart = userCarts[customerId];
            for (int i = 0; i < cart.size(); i++) {
                if (cart[i]->getId() == itemId) {
                    if (newQuantity <= 0) {
                        delete cart[i];
                        cart.erase(cart.begin() + i);
                    } else {
                        cart[i]->updateQuantity(newQuantity);
                    }
                    return;
                }
            }
            throw ValidationException("Cart item not found: " + itemId);
        }
    }
    
    void updateCartItemSize(string customerId, string itemId, string newSize) {
        if (userCarts.find(customerId) != userCarts.end()) {
            vector<CartItem*>& cart = userCarts[customerId];
            for (CartItem* item : cart) {
                if (item->getId() == itemId) {
                    // Có thể thêm validation: chỉ cho phép update size cho DRINK
                    if (item->getProductType() != DRINK) {
                        throw ValidationException("Cannot update size for non-drink items");
                    }
                    item->updateSize(newSize);
                    return;
                }
            }
            throw ValidationException("Cart item not found: " + itemId);
        }
    }

    void clearCart(string customerId) {
        if (userCarts.find(customerId) != userCarts.end()) {
            userCarts[customerId].clear();
        }
    }
};

#endif // CARTMANAGER_H