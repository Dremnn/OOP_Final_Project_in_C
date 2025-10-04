#ifndef COFFEESHOPSYSTEM_H
#define COFFEESHOPSYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include "../managers/UserManager.h"
#include "../managers/ProductManager.h"
#include "../managers/CartManager.h"
#include "../managers/OrderManager.h"
#include "../managers/PaymentManager.h"
#include "../users/Customer.h"
#include "../products/Product.h"
#include "../cart/CartItem.h"
#include "../order/Order.h"
#include "../payment/Payment.h"
#include "../exceptions/Exceptions.h"

using namespace std;

// ============= SYSTEM CLASS =============
class CoffeeShopSystem {
private:
    UserManager* userManager;
    ProductManager* productManager;
    CartManager* cartManager;
    OrderManager* orderManager;
    PaymentManager* paymentManager;
    
    string currentSessionToken;
    bool isInitialized;

public:
    CoffeeShopSystem() {
        userManager = new UserManager();
        productManager = new ProductManager();
        cartManager = new CartManager();
        orderManager = new OrderManager();
        paymentManager = new PaymentManager();
        currentSessionToken = "";
        isInitialized = false;
    }
    
    ~CoffeeShopSystem() {
        delete userManager;
        delete productManager;
        delete cartManager;
        delete orderManager;
        delete paymentManager;
    }
    
    // ===== USER OPERATIONS =====
    void initializeSystem() {
        if (!isInitialized) {
            try {
                userManager->registerAdmin("admin", "admin123", "0000000000");
                cout << "System initialized with default admin account" << endl;
                isInitialized = true;
            } catch (ValidationException& e) {
                cout << "Admin already exists" << endl;
            }
        }
    }
    
    string registerCustomer(string username, string password, string phoneNumber) {
        return userManager->registerCustomer(username, password, phoneNumber);
    }
    
    void registerAdmin(string username, string password, string phoneNumber) {
        userManager->registerAdmin(username, password, phoneNumber);
    }
    
    bool login(string username, string password) {
        try {
            currentSessionToken = userManager->login(username, password);
            return true;
        } catch (AuthenticationException& e) {
            cout << e.what() << endl;
            return false;
        }
    }
    
    void logout() {
        if (!currentSessionToken.empty()) {
            userManager->logout(currentSessionToken);
            currentSessionToken = "";
        }
    }
    
    bool isLoggedIn() {
        return !currentSessionToken.empty();
    }
    
    bool isCurrentUserAdmin() {
        if (!isLoggedIn()) return false;
        return userManager->isAdmin(currentSessionToken);
    }
    
    Customer* getCurrentCustomer() {
        return userManager->getCurrentCustomer(currentSessionToken);
    }
    
    // ===== PRODUCT OPERATIONS =====
    string addDrink(string name, double price, string size, bool isHot) {
        return productManager->addDrink(name, price, size, isHot, currentSessionToken, userManager);
    }
    
    string addFood(string name, double price, bool isVegetarian) {
        return productManager->addFood(name, price, isVegetarian, currentSessionToken, userManager);
    }
    
    void updateProduct(string productId, string name, double price, bool available) {
        productManager->updateProduct(productId, name, price, available, currentSessionToken, userManager);
    }
    
    void deleteProduct(string productId) {
        productManager->deleteProduct(productId, currentSessionToken, userManager);
    }
    
    vector<Product*> getAllProducts() {
        return productManager->getAllProducts();
    }
    
    vector<Product*> getDrinks() {
        return productManager->getProductsByType(DRINK);
    }
    
    vector<Product*> getFoods() {
        return productManager->getProductsByType(FOOD);
    }
    
    Product* getProduct(string productId) {
        return productManager->getProduct(productId);
    }
    
    // ===== CART OPERATIONS =====
    void addToCart(string productId, int quantity, string size = "M") {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in to add to cart");
        }
        
        Customer* customer = getCurrentCustomer();
        Product* product = productManager->getProduct(productId);
        
        if (!product->getIsAvailable()) {
            throw ValidationException("Product is not available");
        }
        
        // Truyền thêm product type vào hàm addToCart của CartManager
        cartManager->addToCart(customer->getId(), productId, quantity, product->getPrice(), product->getType(), size);
    }
    
    vector<CartItem*> viewCart() {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in to view cart");
        }
        
        Customer* customer = getCurrentCustomer();
        return cartManager->getCart(customer->getId());
    }
    
    void updateCartItem(string itemId, int newQuantity) {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in");
        }
        
        Customer* customer = getCurrentCustomer();
        cartManager->updateCartItem(customer->getId(), itemId, newQuantity);
    }
    
    void updateCartItemSize(string itemId, string newSize) {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in");
        }
        
        Customer* customer = getCurrentCustomer();
        cartManager->updateCartItemSize(customer->getId(), itemId, newSize);
    }
    
    void clearCart() {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in");
        }
        
        Customer* customer = getCurrentCustomer();
        cartManager->clearCart(customer->getId());
    }
    
    // ===== ORDER OPERATIONS =====
    Order* checkout(OrderType orderType, string deliveryAddress, PaymentMethod paymentMethod) {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in to checkout");
        }
        
        Customer* customer = getCurrentCustomer();
        vector<CartItem*> items = cartManager->getCart(customer->getId());
        
        if (items.empty()) {
            throw ValidationException("Cart is empty");
        }
        
        if (deliveryAddress.empty()) {
            deliveryAddress = customer->getAddress();
            if (deliveryAddress.empty()) {
                throw ValidationException("Delivery address is required");
            }
        }
        
        Order* order = orderManager->createOrder(customer->getId(), items, orderType, deliveryAddress, paymentMethod);
        
        if (order->getPayment() != NULL) {
            paymentManager->trackPayment(order->getPayment());
        }
        
        customer->addOrderToHistory(order->getId());
        cartManager->clearCart(customer->getId());
        
        return order;
    }
    
    vector<Order*> viewMyOrders() {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in");
        }
        
        Customer* customer = getCurrentCustomer();
        return orderManager->getCustomerOrders(customer->getId());
    }
    
    vector<Order*> viewAllOrders() {
        if (!isCurrentUserAdmin()) {
            throw AuthorizationException("Only admin can view all orders");
        }
        
        return orderManager->getAllOrders();
    }
    
    Order* getOrder(string orderId) {
        return orderManager->getOrder(orderId);
    }
    
    void updateOrderStatus(string orderId, OrderStatus newStatus) {
        orderManager->updateOrderStatus(orderId, newStatus, currentSessionToken, userManager);
    }
    
    void cancelOrder(string orderId) {
        if (!isLoggedIn()) {
            throw AuthenticationException("Must be logged in");
        }
        
        Order* order = orderManager->getOrder(orderId);
        
        Customer* customer = getCurrentCustomer();
        if (order->getCustomerId() != customer->getId() && !isCurrentUserAdmin()) {
            throw AuthorizationException("Cannot cancel other customer's order");
        }
        
        OrderStatus status = order->getStatus();
        if (status == READY || status == DELIVERED) {
            throw ValidationException("Cannot cancel order that is ready or delivered");
        }
        
        orderManager->cancelOrder(orderId);
    }
    
    // ===== PAYMENT OPERATIONS =====
    bool processPayment(string orderId, double amount) {
        Order* order = orderManager->getOrder(orderId);
        
        if (!isCurrentUserAdmin()) {
            Customer* customer = getCurrentCustomer();
            if (order->getCustomerId() != customer->getId()) {
                throw AuthorizationException("Cannot pay for other customer's order");
            }
        }
        
        bool success = orderManager->processPayment(orderId, amount);
        
        if (success) {
            order->updateStatus(CONFIRMED);
        }
        
        return success;
    }
    
    double getTotalRevenue() {
        if (!isCurrentUserAdmin()) {
            throw AuthorizationException("Only admin can view revenue");
        }
        
        return paymentManager->getTotalRevenue();
    }
    
    vector<Payment*> getAllPayments() {
        if (!isCurrentUserAdmin()) {
            throw AuthorizationException("Only admin can view all payments");
        }
        
        return paymentManager->getAllPayments();
    }
    
    // ===== DISPLAY OPERATIONS =====
    void displayAllProducts() {
        vector<Product*> products = getAllProducts();
        
        cout << "\n=== ALL PRODUCTS ===" << endl;
        if (products.empty()) {
            cout << "No products available" << endl;
            return;
        }
        
        for (int i = 0; i < products.size(); i++) {
            cout << "\n--- Product " << (i + 1) << " ---" << endl;
            products[i]->displayInfo();
        }
    }
    
    void displayCart() {
        vector<CartItem*> items = viewCart();
        
        cout << "\n=== MY CART ===" << endl;
        if (items.empty()) {
            cout << "Cart is empty" << endl;
            return;
        }
        
        double total = 0;
        for (int i = 0; i < items.size(); i++) {
            cout << "\n--- Item " << (i + 1) << " ---" << endl;
            items[i]->displayInfo();
            total += items[i]->getTotalPrice();
        }
        
        cout << "\n--- Cart Total ---" << endl;
        cout << "Subtotal: " << formatPrice(total) << endl;
    }
    
    void displayMyOrders() {
        vector<Order*> orders = viewMyOrders();
        
        cout << "\n=== MY ORDER HISTORY ===" << endl;
        if (orders.empty()) {
            cout << "No orders yet" << endl;
            return;
        }
        
        for (int i = 0; i < orders.size(); i++) {
            orders[i]->displayInfo();
        }
    }

    // ===== GUEST OPERATIONS =====
    vector<Product*> browseProductsAsGuest() {
        return productManager->getAllProducts();
    }

    vector<Product*> browseDrinksAsGuest() {
        return productManager->getProductsByType(DRINK);
    }

    vector<Product*> browseFoodsAsGuest() {
        return productManager->getProductsByType(FOOD);
    }

    void displayProductsForGuest() {
        vector<Product*> products = browseProductsAsGuest();
        
        cout << "\n=== MENU (Guest View) ===" << endl;
        if (products.empty()) {
            cout << "No products available" << endl;
            return;
        }
        
        for (int i = 0; i < products.size(); i++) {
            cout << "\n--- Product " << (i + 1) << " ---" << endl;
            products[i]->displayInfo();
        }
        
        cout << "\n[Note] Please login or register to place orders" << endl;
    }

    bool isGuest() {
        return !isLoggedIn();
    }
};

#endif // COFFEESHOPSYSTEM_H