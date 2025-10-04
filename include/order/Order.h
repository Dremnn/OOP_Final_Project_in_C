#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <vector>
#include <iostream>
#include "../cart/CartItem.h"
#include "../payment/Payment.h"
#include "../enums/Enums.h"
#include "../utils/Utils.h"
#include "../exceptions/Exceptions.h"

using namespace std;

class Order {
protected:
    string id;
    string customerId;
    vector<CartItem*> items;
    double subtotal;
    double tax;
    double deliveryFee;
    double total;
    OrderStatus status;
    OrderType orderType;
    string deliveryAddress;
    Payment* payment;

public:
    Order(string customerId, vector<CartItem*> items, OrderType orderType, string deliveryAddress) {
        if (items.empty()) {
            throw ValidationException("Cannot create order with empty cart");
        }
        if (deliveryAddress.empty()) {
            throw ValidationException("Delivery address is required");
        }
        
        this->id = generateId("ORD");
        this->customerId = customerId;
        this->items = items;
        this->orderType = orderType;
        this->deliveryAddress = deliveryAddress;
        this->status = PENDING;
        this->payment = NULL;
        
        if (orderType == EXPRESS_ORDER) {
            this->deliveryFee = 50000;
        } else {
            this->deliveryFee = 25000;
        }
        
        calculateTotal();
    }
    
    ~Order() {
        if (payment != NULL) {
            delete payment;
        }
    }

    void calculateTotal() {
        subtotal = 0;
        for (int i = 0; i < items.size(); i++) {
            subtotal += items[i]->getTotalPrice();
        }
        tax = subtotal * 0.1;
        total = subtotal + tax + deliveryFee;
    }

    string getId() { return id; }
    string getCustomerId() { return customerId; }
    OrderStatus getStatus() { return status; }
    double getTotal() { return total; }
    double getSubtotal() { return subtotal; }
    double getTax() { return tax; }
    double getDeliveryFee() { return deliveryFee; }
    OrderType getOrderType() { return orderType; }
    Payment* getPayment() { return payment; }

    void updateStatus(OrderStatus newStatus) {
        status = newStatus;
    }
    
    Payment* createPayment(PaymentMethod method) {
        if (payment == NULL) {
            payment = new Payment(id, total, method);
        }
        return payment;
    }
    
    bool processPayment(double customerMoney = 0) {
        if (payment == NULL) {
            throw ValidationException("Payment not initialized for this order");
        }
        return payment->processPayment(customerMoney);
    }
    
    bool isPaid() {
        if (payment == NULL) {
            return false;
        }
        return payment->isPaid();
    }
    
    void cancelOrder() {
        status = CANCELLED;
        if (payment != NULL && payment->isPaid()) {
            payment->refund();
        }
    }
    
    string getStatusString() {
        switch(status) {
            case PENDING: return "PENDING";
            case CONFIRMED: return "CONFIRMED";
            case PREPARING: return "PREPARING";
            case READY: return "READY";
            case DELIVERED: return "DELIVERED";
            case CANCELLED: return "CANCELLED";
            default: return "UNKNOWN";
        }
    }
    
    void displayInfo() {
        cout << "\n=== ORDER DETAILS ===" << endl;
        cout << "Order ID: " << id << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Type: " << (orderType == EXPRESS_ORDER ? "EXPRESS" : "REGULAR") << endl;
        cout << "Delivery Address: " << deliveryAddress << endl;
        
        cout << "\nItems (" << items.size() << "):" << endl;
        for (int i = 0; i < items.size(); i++) {
            items[i]->displayInfo();
        }
        
        cout << "\n--- Pricing ---" << endl;
        cout << "Subtotal: " << formatPrice(subtotal) << endl;
        cout << "Tax (10%): " << formatPrice(tax) << endl;
        cout << "Delivery Fee: " << formatPrice(deliveryFee) << endl;
        cout << "Total: " << formatPrice(total) << endl;
        
        if (payment != NULL) {
            cout << "\n--- Payment Info ---" << endl;
            payment->displayInfo();
        }
    }
};

#endif // ORDER_H