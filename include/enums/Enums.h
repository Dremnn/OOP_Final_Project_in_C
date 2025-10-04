#ifndef ENUMS_H
#define ENUMS_H

// ============= ENUMS =============
enum UserRole {
    CUSTOMER,
    ADMIN
};

enum OrderStatus {
    PENDING,
    CONFIRMED, 
    PREPARING,
    READY,
    DELIVERED,
    CANCELLED
};

enum ProductType {
    DRINK,
    FOOD
};

enum OrderType {
    REGULAR_ORDER,
    EXPRESS_ORDER
};

enum PaymentMethod { 
    BANK_TRANSFER,
    CASH_ON_DELIVERY
};

enum PaymentStatus { 
    UNPAID,
    PAID,
    REFUNDED
};

#endif // ENUMS_H