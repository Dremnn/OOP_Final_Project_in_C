#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <iostream>
#include "../enums/Enums.h"
#include "../utils/Utils.h"

using namespace std;

class Payment {
private:
    string id;
    string orderId;
    PaymentMethod method;
    PaymentStatus status;
    double amount;
    double paidAmount;

public:
    Payment(string orderId, double amount, PaymentMethod method) {
        this->id = generateId("PAY");
        this->orderId = orderId;
        this->amount = amount;
        this->method = method;
        this->paidAmount = 0;
        
        if (method == CASH_ON_DELIVERY) {
            this->status = PAID;
            this->paidAmount = amount;
        } else {
            this->status = UNPAID;
        }
    }

    string getId() { return id; }
    string getOrderId() { return orderId; }
    PaymentMethod getMethod() { return method; }
    PaymentStatus getStatus() { return status; }
    double getAmount() { return amount; }
    double getPaidAmount() { return paidAmount; }

    bool processPayment(double customerMoney) {
        if (status == PAID) {
            return true;
        }

        if (method == BANK_TRANSFER && customerMoney >= amount) {
            this->paidAmount = customerMoney;
            this->status = PAID;
            return true;
        }
        
        return false;
    }

    double getChange() {
        if (paidAmount > amount) {
            return paidAmount - amount;
        }
        return 0;
    }

    void refund() {
        if (status == PAID) {
            status = REFUNDED;
        }
    }

    bool isPaid() {
        return status == PAID;
    }
    
    string getMethodString() {
        return (method == BANK_TRANSFER) ? "Bank Transfer" : "Cash on Delivery";
    }
    
    string getStatusString() {
        if (status == UNPAID) return "Unpaid";
        if (status == PAID) return "Paid";
        return "Refunded";
    }
    
    void displayInfo() {
        cout << "Payment ID: " << id << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Method: " << getMethodString() << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Amount: " << formatPrice(amount) << endl;
        if (status == PAID) {
            cout << "Paid Amount: " << formatPrice(paidAmount) << endl;
            if (getChange() > 0) {
                cout << "Change: " << formatPrice(getChange()) << endl;
            }
        }
    }
};

#endif // PAYMENT_H