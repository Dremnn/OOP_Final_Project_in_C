#ifndef ORDERMANAGER_H
#define ORDERMANAGER_H

#include <map>
#include <vector>
#include <string>
#include "../order/Order.h"
#include "../cart/CartItem.h"
#include "../exceptions/Exceptions.h"
#include "UserManager.h"

using namespace std;

// ============= ORDER MANAGER CLASS =============
class OrderManager {
private:
    map<string, Order*> orders;

public:
    ~OrderManager() {
        for (auto& pair : orders) {
            delete pair.second;
        }
    }

    Order* createOrder(string customerId, vector<CartItem*> items, OrderType type, 
                       string deliveryAddress, PaymentMethod paymentMethod) {
        if (items.empty()) {
            throw ValidationException("Cannot create order with empty cart");
        }
        
        Order* order = new Order(customerId, items, type, deliveryAddress);
        orders[order->getId()] = order;
        
        order->createPayment(paymentMethod);
        
        if (paymentMethod == CASH_ON_DELIVERY) {
            order->processPayment();
            order->updateStatus(CONFIRMED);
        }
        
        return order;
    }
    
    Order* getOrder(string orderId) {
        if (orders.find(orderId) == orders.end()) {
            throw ValidationException("Order not found: " + orderId);
        }
        return orders[orderId];
    }
    
    vector<Order*> getCustomerOrders(string customerId) {
        vector<Order*> result;
        for (auto& pair : orders) {
            if (pair.second->getCustomerId() == customerId) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
    
    vector<Order*> getAllOrders() {
        vector<Order*> result;
        for (auto& pair : orders) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    void updateOrderStatus(string orderId, OrderStatus newStatus, string sessionToken, UserManager* userManager) {
        if (!userManager->isAdmin(sessionToken)) {
            throw AuthorizationException("Only admin can update order status");
        }
        
        Order* order = getOrder(orderId);
        order->updateStatus(newStatus);
    }
    
    bool processPayment(string orderId, double amount) {
        Order* order = getOrder(orderId);
        return order->processPayment(amount);
    }
    
    void cancelOrder(string orderId) {
        Order* order = getOrder(orderId);
        
        // BR20: Validate cancellation is allowed
        OrderStatus currentStatus = order->getStatus();
        if (currentStatus == READY || currentStatus == DELIVERED) {
            throw ValidationException("Cannot cancel order that is READY or DELIVERED");
        }
        
        order->cancelOrder();
    }
};

#endif // ORDERMANAGER_H