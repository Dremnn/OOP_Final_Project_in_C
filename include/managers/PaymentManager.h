#ifndef PAYMENTMANAGER_H
#define PAYMENTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include "../payment/Payment.h"
#include "../exceptions/Exceptions.h"

using namespace std;

// ============= PAYMENT MANAGER CLASS =============
class PaymentManager {
private:
    map<string, Payment*> payments;

public:
    ~PaymentManager() {
        // Payments được quản lý bởi Orders, không delete ở đây
    }
    
    void trackPayment(Payment* payment) {
        if (payment != NULL) {
            payments[payment->getId()] = payment;
        }
    }
    
    Payment* getPayment(string paymentId) {
        if (payments.find(paymentId) == payments.end()) {
            throw ValidationException("Payment not found: " + paymentId);
        }
        return payments[paymentId];
    }
    
    Payment* getPaymentByOrderId(string orderId) {
        for (auto& pair : payments) {
            if (pair.second->getOrderId() == orderId) {
                return pair.second;
            }
        }
        throw ValidationException("Payment not found for order: " + orderId);
    }
    
    vector<Payment*> getAllPayments() {
        vector<Payment*> result;
        for (auto& pair : payments) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    vector<Payment*> getPaidPayments() {
        vector<Payment*> result;
        for (auto& pair : payments) {
            if (pair.second->isPaid()) {
                result.push_back(pair.second);
            }
        }
        return result;
    }
    
    double getTotalRevenue() {
        double total = 0;
        for (auto& pair : payments) {
            if (pair.second->getStatus() == PAID) {
                total += pair.second->getAmount();
            }
        }
        return total;
    }
};

#endif // PAYMENTMANAGER_H