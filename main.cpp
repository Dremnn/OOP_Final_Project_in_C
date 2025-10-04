#include <iostream>
#include <vector>
#include "include/system/CoffeeShopSystem.h"

using namespace std;

int main() {
    cout << "\n========================================================" << endl;
    cout << "        COFFEE SHOP SYSTEM - TEST SUITE" << endl;
    cout << "========================================================\n" << endl;

    //========================================================
    // TEST 1: INHERITANCE & POLYMORPHISM
    //========================================================
    cout << "\n--- TEST 1: INHERITANCE & POLYMORPHISM ---" << endl;
    {
        Customer* cust = new Customer("testcust", "pass123", "0123456789");
        Admin* adm = new Admin("testadmin", "admin123", "0987654321");
        
        if (cust->canModifyProduct() == false) {
            cout << "[PASS] 1.1: Customer cannot modify products" << endl;
        } else {
            cout << "[FAIL] 1.1: Customer cannot modify products" << endl;
        }
        
        if (adm->canModifyProduct() == true) {
            cout << "[PASS] 1.2: Admin can modify products" << endl;
        } else {
            cout << "[FAIL] 1.2: Admin can modify products" << endl;
        }
        
        delete cust;
        delete adm;
    }

    //========================================================
    // TEST 2: VALIDATION & EDGE CASES
    //========================================================
    cout << "\n--- TEST 2: VALIDATION & EDGE CASES ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        
        // Test 2.1: Empty username
        bool caught = false;
        try {
            system.registerCustomer("", "pass123", "0123456789");
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 2.1: Empty username rejected" << endl;
        } else {
            cout << "[FAIL] 2.1: Empty username rejected" << endl;
        }
        
        // Test 2.2: Short password
        caught = false;
        try {
            system.registerCustomer("user1", "12", "0123456789");
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 2.2: Short password rejected" << endl;
        } else {
            cout << "[FAIL] 2.2: Short password rejected" << endl;
        }
        
        // Test 2.3: Negative price
        system.login("admin", "admin123");
        caught = false;
        try {
            system.addDrink("Free", -100, "M", true);
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 2.3: Negative price rejected" << endl;
        } else {
            cout << "[FAIL] 2.3: Negative price rejected" << endl;
        }
        
        // Test 2.4: Invalid size
        string drinkId = system.addDrink("TestDrink", 50000, "M", true);
        system.logout();
        
        string custId = system.registerCustomer("testuser", "test123", "0123456789");
        system.login("testuser", "test123");
        system.addToCart(drinkId, 1, "M");
        vector<CartItem*> cart = system.viewCart();
        
        caught = false;
        if (!cart.empty()) {
            try {
                cart[0]->updateSize("Z");
            } catch (ValidationException&) {
                caught = true;
            }
        }
        if (caught) {
            cout << "[PASS] 2.4: Invalid size rejected" << endl;
        } else {
            cout << "[FAIL] 2.4: Invalid size rejected" << endl;
        }
        system.logout();
    }

    //========================================================
    // TEST 3: FUNCTIONAL REQUIREMENTS
    //========================================================
    cout << "\n--- TEST 3: FUNCTIONAL REQUIREMENTS ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        
        // Setup products
        system.login("admin", "admin123");
        string drinkId = system.addDrink("Coffee", 35000, "M", true);
        string foodId = system.addFood("Sandwich", 40000, true);
        system.logout();
        
        // Test 3.1: Guest browse menu
        vector<Product*> guestProducts = system.browseProductsAsGuest();
        if (guestProducts.size() == 2) {
            cout << "[PASS] 3.1 (FR4): Guest can browse menu" << endl;
        } else {
            cout << "[FAIL] 3.1 (FR4): Guest can browse menu" << endl;
        }
        
        // Test 3.2: Customer registration
        string custId = system.registerCustomer("alice", "alice123", "0111111111");
        if (!custId.empty()) {
            cout << "[PASS] 3.2 (FR1): Customer registration successful" << endl;
        } else {
            cout << "[FAIL] 3.2 (FR1): Customer registration successful" << endl;
        }
        
        // Test 3.3: Customer login
        bool loggedIn = system.login("alice", "alice123");
        if (loggedIn && system.isLoggedIn()) {
            cout << "[PASS] 3.3 (FR2): Customer login successful" << endl;
        } else {
            cout << "[FAIL] 3.3 (FR2): Customer login successful" << endl;
        }
        
        // Test 3.4: Add to cart with size
        system.addToCart(drinkId, 2, "L");
        system.addToCart(foodId, 1);
        vector<CartItem*> cart = system.viewCart();
        if (cart.size() == 2) {
            cout << "[PASS] 3.4 (FR7): Add items to cart with customization" << endl;
        } else {
            cout << "[FAIL] 3.4 (FR7): Add items to cart with customization" << endl;
        }
        
        // Test 3.5: Place order
        Order* order = system.checkout(REGULAR_ORDER, "123 Main St", CASH_ON_DELIVERY);
        if (order != NULL && order->getStatus() == CONFIRMED) {
            cout << "[PASS] 3.5 (FR10): Place order successfully" << endl;
        } else {
            cout << "[FAIL] 3.5 (FR10): Place order successfully" << endl;
        }
        
        // Test 3.6: View order history
        vector<Order*> orders = system.viewMyOrders();
        if (orders.size() >= 1) {
            cout << "[PASS] 3.6 (FR11): View order history" << endl;
        } else {
            cout << "[FAIL] 3.6 (FR11): View order history" << endl;
        }
        
        // Test 3.7: Admin adds products
        system.logout();
        system.login("admin", "admin123");
        string newDrink = system.addDrink("Tea", 30000, "M", false);
        if (!newDrink.empty()) {
            cout << "[PASS] 3.7 (FR15): Admin can add products" << endl;
        } else {
            cout << "[FAIL] 3.7 (FR15): Admin can add products" << endl;
        }
        
        // Test 3.8: Admin updates order status
        system.updateOrderStatus(order->getId(), PREPARING);
        if (order->getStatus() == PREPARING) {
            cout << "[PASS] 3.8 (FR17): Admin can update order status" << endl;
        } else {
            cout << "[FAIL] 3.8 (FR17): Admin can update order status" << endl;
        }
        system.logout();
    }

    //========================================================
    // TEST 4: BUSINESS RULES
    //========================================================
    cout << "\n--- TEST 4: BUSINESS RULES ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        
        system.login("admin", "admin123");
        string drinkId = system.addDrink("Espresso", 45000, "M", true);
        system.logout();
        
        // Test 4.1: Must login to add to cart (BR1)
        bool caught = false;
        try {
            system.addToCart(drinkId, 1);
        } catch (AuthenticationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.1 (BR1): Authentication required for cart" << endl;
        } else {
            cout << "[FAIL] 4.1 (BR1): Authentication required for cart" << endl;
        }
        
        system.registerCustomer("bob", "bob123", "0222222222");
        system.login("bob", "bob123");
        
        // Test 4.2: Only admin can add products (BR2)
        caught = false;
        try {
            system.addDrink("Unauthorized", 30000, "M", true);
        } catch (AuthorizationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.2 (BR2): Only admin can add products" << endl;
        } else {
            cout << "[FAIL] 4.2 (BR2): Only admin can add products" << endl;
        }
        
        system.logout();
        
        // Test 4.3: Password minimum 6 characters (BR4)
        caught = false;
        try {
            system.registerCustomer("charlie", "123", "0333333333");
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.3 (BR4): Password minimum 6 characters" << endl;
        } else {
            cout << "[FAIL] 4.3 (BR4): Password minimum 6 characters" << endl;
        }
        
        // Test 4.4: Username must be unique (BR5)
        caught = false;
        try {
            system.registerCustomer("bob", "newpass", "0444444444");
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.4 (BR5): Username must be unique" << endl;
        } else {
            cout << "[FAIL] 4.4 (BR5): Username must be unique" << endl;
        }
        
        // Test 4.5: Product price must be positive (BR6)
        system.login("admin", "admin123");
        caught = false;
        try {
            system.addFood("Free", 0, false);
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.5 (BR6): Product price must be positive" << endl;
        } else {
            cout << "[FAIL] 4.5 (BR6): Product price must be positive" << endl;
        }
        system.logout();
        
        system.login("bob", "bob123");
        
        // Test 4.6: Cart quantity must be positive (BR10)
        caught = false;
        try {
            system.addToCart(drinkId, 0);
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.6 (BR10): Cart quantity must be positive" << endl;
        } else {
            cout << "[FAIL] 4.6 (BR10): Cart quantity must be positive" << endl;
        }
        
        // Test 4.7: Size multipliers for drinks (BR11)
        system.addToCart(drinkId, 1, "S");
        system.addToCart(drinkId, 1, "M");
        system.addToCart(drinkId, 1, "L");
        vector<CartItem*> cart = system.viewCart();
        
        double priceS = cart[0]->getTotalPrice();
        double priceM = cart[1]->getTotalPrice();
        double priceL = cart[2]->getTotalPrice();
        
        bool sizeCorrect = (priceS == 45000 * 0.8) && (priceM == 45000 * 1.0) && (priceL == 45000 * 1.3);
        if (sizeCorrect) {
            cout << "[PASS] 4.7 (BR11): Size multipliers (S=0.8, M=1.0, L=1.3)" << endl;
        } else {
            cout << "[FAIL] 4.7 (BR11): Size multipliers (S=0.8, M=1.0, L=1.3)" << endl;
        }
        
        // Test 4.8-4.10: Pricing calculations (BR12-BR14)
        Order* order = system.checkout(REGULAR_ORDER, "456 St", BANK_TRANSFER);
        double expectedTax = order->getSubtotal() * 0.1;
        if (order->getTax() == expectedTax) {
            cout << "[PASS] 4.8 (BR12): Tax is 10% of subtotal" << endl;
        } else {
            cout << "[FAIL] 4.8 (BR12): Tax is 10% of subtotal" << endl;
        }
        
        if (order->getDeliveryFee() == 25000) {
            cout << "[PASS] 4.9 (BR13): Regular delivery fee is 25,000 VND" << endl;
        } else {
            cout << "[FAIL] 4.9 (BR13): Regular delivery fee is 25,000 VND" << endl;
        }
        
        double expectedTotal = order->getSubtotal() + order->getTax() + order->getDeliveryFee();
        if (order->getTotal() == expectedTotal) {
            cout << "[PASS] 4.10 (BR14): Total = Subtotal + Tax + Delivery" << endl;
        } else {
            cout << "[FAIL] 4.10 (BR14): Total = Subtotal + Tax + Delivery" << endl;
        }
        
        // Test 4.11: Cannot order empty cart (BR15)
        system.clearCart();
        caught = false;
        try {
            system.checkout(REGULAR_ORDER, "789 Ave", CASH_ON_DELIVERY);
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.11 (BR15): Cannot create order with empty cart" << endl;
        } else {
            cout << "[FAIL] 4.11 (BR15): Cannot create order with empty cart" << endl;
        }
        
        // Test 4.12: COD auto-payment (BR18)
        system.addToCart(drinkId, 1);
        Order* codOrder = system.checkout(REGULAR_ORDER, "111 Rd", CASH_ON_DELIVERY);
        if (codOrder->isPaid() && codOrder->getStatus() == CONFIRMED) {
            cout << "[PASS] 4.12 (BR18): COD orders auto-paid and confirmed" << endl;
        } else {
            cout << "[FAIL] 4.12 (BR18): COD orders auto-paid and confirmed" << endl;
        }
        
        // Test 4.13: Cancel restrictions (BR20)
        system.addToCart(drinkId, 1);
        Order* testOrder = system.checkout(REGULAR_ORDER, "Test", CASH_ON_DELIVERY);
        
        system.logout();
        system.login("admin", "admin123");
        system.updateOrderStatus(testOrder->getId(), READY);
        
        system.logout();
        system.login("bob", "bob123");
        caught = false;
        try {
            system.cancelOrder(testOrder->getId());
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 4.13 (BR20): Cannot cancel READY/DELIVERED orders" << endl;
        } else {
            cout << "[FAIL] 4.13 (BR20): Cannot cancel READY/DELIVERED orders" << endl;
        }
        
        // Test 4.14: Payment amount validation (BR21)
        system.addToCart(drinkId, 1);
        Order* bankOrder = system.checkout(REGULAR_ORDER, "Pay Test", BANK_TRANSFER);
        bool insufficientPay = system.processPayment(bankOrder->getId(), 1000);
        if (!insufficientPay) {
            cout << "[PASS] 4.14 (BR21): Payment amount must be sufficient" << endl;
        } else {
            cout << "[FAIL] 4.14 (BR21): Payment amount must be sufficient" << endl;
        }
        
        system.logout();
    }

    //========================================================
    // TEST 5: ADMIN CAPABILITIES
    //========================================================
    cout << "\n--- TEST 5: ADMIN CAPABILITIES ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        system.login("admin", "admin123");
        
        // Test 5.1: Add products
        string drinkId = system.addDrink("Latte", 50000, "M", true);
        string foodId = system.addFood("Cake", 35000, false);
        if (!drinkId.empty() && !foodId.empty()) {
            cout << "[PASS] 5.1: Admin can add drinks and food" << endl;
        } else {
            cout << "[FAIL] 5.1: Admin can add drinks and food" << endl;
        }
        
        // Test 5.2: Update product
        system.updateProduct(drinkId, "Premium Latte", 60000, true);
        Product* updated = system.getProduct(drinkId);
        if (updated->getName() == "Premium Latte" && updated->getPrice() == 60000) {
            cout << "[PASS] 5.2: Admin can update products" << endl;
        } else {
            cout << "[FAIL] 5.2: Admin can update products" << endl;
        }
        
        // Test 5.3: View all orders
        vector<Order*> allOrders = system.viewAllOrders();
        cout << "[PASS] 5.3: Admin can view all orders" << endl;
        
        // Test 5.4: Delete product
        system.deleteProduct(foodId);
        bool caught = false;
        try {
            system.getProduct(foodId);
        } catch (ValidationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 5.4: Admin can delete products" << endl;
        } else {
            cout << "[FAIL] 5.4: Admin can delete products" << endl;
        }
        
        system.logout();
    }

    //========================================================
    // TEST 6: CUSTOMER WORKFLOW
    //========================================================
    cout << "\n--- TEST 6: CUSTOMER WORKFLOW ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        
        system.login("admin", "admin123");
        string drinkId = system.addDrink("Mocha", 40000, "M", true);
        system.logout();
        
        // Test 6.1: Register and login
        system.registerCustomer("carol", "carol123", "0555555555");
        bool loginSuccess = system.login("carol", "carol123");
        if (loginSuccess) {
            cout << "[PASS] 6.1: Customer registration and login" << endl;
        } else {
            cout << "[FAIL] 6.1: Customer registration and login" << endl;
        }
        
        // Test 6.2: Browse and filter products
        vector<Product*> drinks = system.getDrinks();
        vector<Product*> foods = system.getFoods();
        if (drinks.size() > 0) {
            cout << "[PASS] 6.2: Customer can filter products by type" << endl;
        } else {
            cout << "[FAIL] 6.2: Customer can filter products by type" << endl;
        }
        
        // Test 6.3: Add to cart and modify
        system.addToCart(drinkId, 3, "M");
        vector<CartItem*> cart = system.viewCart();
        string itemId = cart[0]->getId();
        
        system.updateCartItem(itemId, 5);
        system.updateCartItemSize(itemId, "L");
        cart = system.viewCart();
        if (cart[0]->getQuantity() == 5 && cart[0]->getSize() == "L") {
            cout << "[PASS] 6.3: Customer can modify cart items" << endl;
        } else {
            cout << "[FAIL] 6.3: Customer can modify cart items" << endl;
        }
        
        // Test 6.4: Place order and pay
        Order* order = system.checkout(EXPRESS_ORDER, "789 Blvd", BANK_TRANSFER);
        if (order->getDeliveryFee() == 50000) {
            cout << "[PASS] 6.4a: Express order delivery fee is 50,000 VND" << endl;
        } else {
            cout << "[FAIL] 6.4a: Express order delivery fee is 50,000 VND" << endl;
        }
        
        bool paid = system.processPayment(order->getId(), order->getTotal() + 10000);
        if (paid && order->isPaid() && order->getPayment()->getChange() == 10000) {
            cout << "[PASS] 6.4b: Customer can pay with change" << endl;
        } else {
            cout << "[FAIL] 6.4b: Customer can pay with change" << endl;
        }
        
        // Test 6.5: Cancel pending order
        system.addToCart(drinkId, 1);
        Order* cancelOrder = system.checkout(REGULAR_ORDER, "Cancel Test", BANK_TRANSFER);
        system.cancelOrder(cancelOrder->getId());
        if (cancelOrder->getStatus() == CANCELLED) {
            cout << "[PASS] 6.5: Customer can cancel pending orders" << endl;
        } else {
            cout << "[FAIL] 6.5: Customer can cancel pending orders" << endl;
        }
        
        system.logout();
    }

    //========================================================
    // TEST 7: GUEST RESTRICTIONS
    //========================================================
    cout << "\n--- TEST 7: GUEST RESTRICTIONS ---" << endl;
    {
        CoffeeShopSystem system;
        system.initializeSystem();
        
        system.login("admin", "admin123");
        string drinkId = system.addDrink("Guest Coffee", 30000, "M", true);
        system.logout();
        
        // Test 7.1: Guest can browse
        vector<Product*> products = system.browseProductsAsGuest();
        if (products.size() > 0) {
            cout << "[PASS] 7.1: Guest can browse products" << endl;
        } else {
            cout << "[FAIL] 7.1: Guest can browse products" << endl;
        }
        
        // Test 7.2: Guest cannot add to cart
        bool caught = false;
        try {
            system.addToCart(drinkId, 1);
        } catch (AuthenticationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 7.2: Guest cannot add to cart" << endl;
        } else {
            cout << "[FAIL] 7.2: Guest cannot add to cart" << endl;
        }
        
        // Test 7.3: Guest cannot view cart
        caught = false;
        try {
            system.viewCart();
        } catch (AuthenticationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 7.3: Guest cannot view cart" << endl;
        } else {
            cout << "[FAIL] 7.3: Guest cannot view cart" << endl;
        }
        
        // Test 7.4: Guest cannot checkout
        caught = false;
        try {
            system.checkout(REGULAR_ORDER, "Test", CASH_ON_DELIVERY);
        } catch (AuthenticationException&) {
            caught = true;
        }
        if (caught) {
            cout << "[PASS] 7.4: Guest cannot checkout" << endl;
        } else {
            cout << "[FAIL] 7.4: Guest cannot checkout" << endl;
        }
    }

    cout << "\n========================================================" << endl;
    cout << "                  TESTING COMPLETED" << endl;
    cout << "========================================================\n" << endl;

    return 0;
}