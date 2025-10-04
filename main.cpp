#include <iostream>
#include "include/system/CoffeeShopSystem.h"

using namespace std;

// ============= MAIN FUNCTION WITH FOCUSED TEST CASES =============
int main() {
    cout << "========================================" << endl;
    cout << "COFFEE SHOP SYSTEM - TEST SUITE" << endl;
    cout << "========================================" << endl;
    
    int totalTests = 0;
    int passedTests = 0;
    int failedTests = 0;
    
    CoffeeShopSystem system;
    
    // Initialize system and add sample products
    system.initializeSystem();
    system.login("admin", "admin123");
    string drinkId1 = system.addDrink("Espresso", 35000, "M", true);
    string drinkId2 = system.addDrink("Cappuccino", 45000, "M", true);
    string foodId1 = system.addFood("Croissant", 30000, false);
    string foodId2 = system.addFood("Veggie Sandwich", 40000, true);
    system.logout();
    
    // ========================================
    // PART 1: INHERITANCE TESTS (3 tests)
    // Testing polymorphism and method overrides
    // ========================================
    cout << "\n========================================" << endl;
    cout << "PART 1: INHERITANCE & POLYMORPHISM TESTS" << endl;
    cout << "========================================" << endl;
    
    // TEST 1: Customer cannot modify products (inheritance override)
    cout << "\n[TEST 1] Inheritance: Customer.canModifyProduct() Override" << endl;
    totalTests++;
    try {
        Customer* customer = new Customer("testcust", "pass123", "0123456789");
        if (customer->canModifyProduct() == false) {
            cout << "PASSED: Customer.canModifyProduct() returns false" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Customer should not be able to modify products" << endl;
            failedTests++;
        }
        delete customer;
    } catch (...) {
        cout << "FAILED: Exception thrown" << endl;
        failedTests++;
    }
    
    // TEST 2: Admin can modify products (inheritance override)
    cout << "\n[TEST 2] Inheritance: Admin.canModifyProduct() Override" << endl;
    totalTests++;
    try {
        Admin* admin = new Admin("testadmin", "admin123", "0987654321");
        if (admin->canModifyProduct() == true) {
            cout << "PASSED: Admin.canModifyProduct() returns true" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Admin should be able to modify products" << endl;
            failedTests++;
        }
        delete admin;
    } catch (...) {
        cout << "FAILED: Exception thrown" << endl;
        failedTests++;
    }
    
    // TEST 3: Polymorphic behavior with base class pointer
    cout << "\n[TEST 3] Polymorphism: User Pointer to Derived Classes" << endl;
    totalTests++;
    try {
        User* userCustomer = new Customer("cust", "pass123", "0111111111");
        User* userAdmin = new Admin("adm", "admin123", "0222222222");
        
        if (userCustomer->canModifyProduct() == false && 
            userAdmin->canModifyProduct() == true) {
            cout << "PASSED: Polymorphic method calls work correctly" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Polymorphism not working as expected" << endl;
            failedTests++;
        }
        
        delete userCustomer;
        delete userAdmin;
    } catch (...) {
        cout << "FAILED: Exception thrown" << endl;
        failedTests++;
    }
    
    // ========================================
    // PART 2: EDGE CASES (7 tests)
    // Testing boundary conditions and invalid inputs
    // ========================================
    cout << "\n========================================" << endl;
    cout << "PART 2: EDGE CASES" << endl;
    cout << "========================================" << endl;
    
    // TEST 4: Empty username validation
    cout << "\n[TEST 4] Edge Case: Empty Username" << endl;
    totalTests++;
    try {
        system.registerCustomer("", "pass123", "0123456789");
        cout << "FAILED: Should reject empty username" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 5: Short password validation
    cout << "\n[TEST 5] Edge Case: Password Too Short" << endl;
    totalTests++;
    try {
        system.registerCustomer("user1", "12", "0123456789");
        cout << "FAILED: Should reject password < 6 characters" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 6: Empty phone number
    cout << "\n[TEST 6] Edge Case: Empty Phone Number" << endl;
    totalTests++;
    try {
        system.registerCustomer("user2", "pass123", "");
        cout << "FAILED: Should reject empty phone number" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 7: Negative product price
    cout << "\n[TEST 7] Edge Case: Negative Product Price" << endl;
    totalTests++;
    system.login("admin", "admin123");
    try {
        system.addDrink("Free Coffee", -10000, "M", true);
        cout << "FAILED: Should reject negative price" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 8: Zero product price
    cout << "\n[TEST 8] Edge Case: Zero Product Price" << endl;
    totalTests++;
    try {
        system.addFood("Free Food", 0, false);
        cout << "FAILED: Should reject zero price" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    system.logout();
    
    // TEST 9: Invalid cart item size
    cout << "\n[TEST 9] Edge Case: Invalid Size (not S/M/L)" << endl;
    totalTests++;
    try {
        CartItem item("PROD1", "CUST1", 1, 50000, "X");
        item.updateSize("Z");
        cout << "FAILED: Should reject invalid size" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 10: Negative cart quantity
    cout << "\n[TEST 10] Edge Case: Negative Quantity" << endl;
    totalTests++;
    try {
        CartItem item("PROD1", "CUST1", 1, 50000);
        item.updateQuantity(-5);
        cout << "FAILED: Should reject negative quantity" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // ========================================
    // PART 3: FUNCTIONAL REQUIREMENTS (5 tests)
    // ========================================
    cout << "\n========================================" << endl;
    cout << "PART 3: FUNCTIONAL REQUIREMENTS" << endl;
    cout << "========================================" << endl;
    
    // TEST 11: FR1 - Customer Registration
    cout << "\n[TEST 11] FR1: Customer Registration" << endl;
    totalTests++;
    try {
        string custId = system.registerCustomer("alice", "alice123", "0111111111");
        cout << "PASSED: Customer registered with ID: " << custId << endl;
        passedTests++;
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 12: FR2 - Customer Login
    cout << "\n[TEST 12] FR2: Customer Login" << endl;
    totalTests++;
    try {
        bool success = system.login("alice", "alice123");
        if (success && system.isLoggedIn()) {
            cout << "PASSED: Customer logged in successfully" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Login unsuccessful" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 13: FR7 - Add to Cart with Size Customization
    cout << "\n[TEST 13] FR7: Add Products to Cart with Size" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 2, "L");
        system.addToCart(foodId1, 1);
        
        vector<CartItem*> cart = system.viewCart();
        if (cart.size() == 2) {
            cout << "PASSED: Added 2 items to cart" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Expected 2 items in cart" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 14: FR10 - Place Order
    cout << "\n[TEST 14] FR10: Place Order" << endl;
    totalTests++;
    try {
        Order* order = system.checkout(REGULAR_ORDER, "123 Main Street", CASH_ON_DELIVERY);
        if (order != NULL && order->getStatus() == CONFIRMED) {
            cout << "PASSED: Order placed - ID: " << order->getId() << endl;
            passedTests++;
        } else {
            cout << "FAILED: Order not created properly" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 15: FR11 - View Order History
    cout << "\n[TEST 15] FR11: View Order History" << endl;
    totalTests++;
    try {
        vector<Order*> orders = system.viewMyOrders();
        if (orders.size() >= 1) {
            cout << "PASSED: Retrieved " << orders.size() << " order(s) from history" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Should have at least 1 order" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // ========================================
    // PART 4: BUSINESS RULES (10 tests)
    // ========================================
    cout << "\n========================================" << endl;
    cout << "PART 4: BUSINESS RULES" << endl;
    cout << "========================================" << endl;
    
    // TEST 16: BR1 - Only logged-in customers can add to cart
    cout << "\n[TEST 16] BR1: Authentication Required for Cart" << endl;
    totalTests++;
    system.logout();
    try {
        system.addToCart(drinkId1, 1);
        cout << "FAILED: Guest should not add to cart" << endl;
        failedTests++;
    } catch (AuthenticationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    system.login("alice", "alice123");
    
    // TEST 17: BR2 - Only admin can add products
    cout << "\n[TEST 17] BR2: Only Admin Can Add Products" << endl;
    totalTests++;
    try {
        system.addDrink("Unauthorized", 30000, "M", true);
        cout << "FAILED: Customer should not add products" << endl;
        failedTests++;
    } catch (AuthorizationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 18: BR4 - Password minimum 6 characters
    cout << "\n[TEST 18] BR4: Password Minimum Length (6 chars)" << endl;
    totalTests++;
    try {
        system.registerCustomer("bob", "123", "0222222222");
        cout << "FAILED: Should reject short password" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 19: BR5 - Username must be unique
    cout << "\n[TEST 19] BR5: Username Uniqueness" << endl;
    totalTests++;
    try {
        system.registerCustomer("alice", "alice456", "0333333333");
        cout << "FAILED: Should reject duplicate username" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 20: BR10 - Cart quantity must be positive
    cout << "\n[TEST 20] BR10: Positive Cart Quantity Required" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 0);
        cout << "FAILED: Should reject zero quantity" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 21: BR12 - Tax calculation (10%)
    cout << "\n[TEST 21] BR12: Tax Calculated at 10%" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId2, 1, "M");
        Order* order = system.checkout(REGULAR_ORDER, "456 Street", BANK_TRANSFER);
        
        double expectedTax = order->getSubtotal() * 0.1;
        if (order->getTax() == expectedTax) {
            cout << "PASSED: Tax = " << formatPrice(order->getTax()) 
                 << " (10% of " << formatPrice(order->getSubtotal()) << ")" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Tax calculation incorrect" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 22: BR13 - Delivery fee
    cout << "\n[TEST 22] BR13: Delivery Fee Calculation" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 1);
        Order* regularOrder = system.checkout(REGULAR_ORDER, "789 Ave", BANK_TRANSFER);
        
        system.addToCart(drinkId1, 1);
        Order* expressOrder = system.checkout(EXPRESS_ORDER, "789 Ave", BANK_TRANSFER);
        
        if (regularOrder->getDeliveryFee() == 25000 && 
            expressOrder->getDeliveryFee() == 50000) {
            cout << "PASSED: Regular=25,000 VND, Express=50,000 VND" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Delivery fee incorrect" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 23: BR15 - Cannot create order with empty cart
    cout << "\n[TEST 23] BR15: Cannot Order with Empty Cart" << endl;
    totalTests++;
    try {
        system.clearCart();
        system.checkout(REGULAR_ORDER, "999 Road", CASH_ON_DELIVERY);
        cout << "FAILED: Should reject empty cart" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    // TEST 24: BR18 - COD auto-payment
    cout << "\n[TEST 24] BR18: COD Auto-Payment & Confirmation" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 1);
        Order* codOrder = system.checkout(REGULAR_ORDER, "111 Lane", CASH_ON_DELIVERY);
        
        if (codOrder->isPaid() && codOrder->getStatus() == CONFIRMED) {
            cout << "PASSED: COD order is PAID and CONFIRMED automatically" << endl;
            passedTests++;
        } else {
            cout << "FAILED: COD should be auto-paid and confirmed" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 25: BR22 - Payment amount validation
    cout << "\n[TEST 25] BR22: Payment Amount Must Be Sufficient" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId2, 1);
        Order* bankOrder = system.checkout(REGULAR_ORDER, "222 Blvd", BANK_TRANSFER);
        
        bool success = system.processPayment(bankOrder->getId(), 1000);
        
        if (!success && !bankOrder->isPaid()) {
            cout << "PASSED: Insufficient payment rejected" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Should reject insufficient amount" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // TEST 26-28: BR20 - Order cancellation rules
    cout << "\n[TEST 26] BR20: Cannot Cancel READY Orders" << endl;
    totalTests++;
    system.logout();
    system.login("admin", "admin123");
    try {
        system.logout();
        system.login("alice", "alice123");
        system.addToCart(drinkId1, 1);
        Order* testOrder = system.checkout(REGULAR_ORDER, "Test Address", CASH_ON_DELIVERY);
        string orderId = testOrder->getId();
        
        system.logout();
        system.login("admin", "admin123");
        system.updateOrderStatus(orderId, READY);
        
        system.logout();
        system.login("alice", "alice123");
        system.cancelOrder(orderId);
        
        cout << "FAILED: Should not cancel READY orders" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    cout << "\n[TEST 27] BR20: Cannot Cancel DELIVERED Orders" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 1);
        Order* testOrder2 = system.checkout(REGULAR_ORDER, "Test Address 2", CASH_ON_DELIVERY);
        string orderId2 = testOrder2->getId();
        
        system.logout();
        system.login("admin", "admin123");
        system.updateOrderStatus(orderId2, PREPARING);
        system.updateOrderStatus(orderId2, READY);
        system.updateOrderStatus(orderId2, DELIVERED);
        
        system.logout();
        system.login("alice", "alice123");
        system.cancelOrder(orderId2);
        
        cout << "FAILED: Should not cancel DELIVERED orders" << endl;
        failedTests++;
    } catch (ValidationException& e) {
        cout << "PASSED: " << e.what() << endl;
        passedTests++;
    }
    
    cout << "\n[TEST 28] BR20: Can Cancel PENDING Orders" << endl;
    totalTests++;
    try {
        system.addToCart(drinkId1, 1);
        Order* testOrder3 = system.checkout(REGULAR_ORDER, "Test Address 3", BANK_TRANSFER);
        string orderId3 = testOrder3->getId();
        
        system.cancelOrder(orderId3);
        
        Order* cancelled = system.getOrder(orderId3);
        if (cancelled->getStatus() == CANCELLED) {
            cout << "PASSED: PENDING order cancelled successfully" << endl;
            passedTests++;
        } else {
            cout << "FAILED: Order should be CANCELLED" << endl;
            failedTests++;
        }
    } catch (CoffeeShopException& e) {
        cout << "FAILED: " << e.what() << endl;
        failedTests++;
    }
    
    // SUMMARY
    cout << "\n========================================" << endl;
    cout << "TEST SUMMARY" << endl;
    cout << "========================================" << endl;
    cout << "Total Tests: " << totalTests << endl;
    cout << "Passed: " << passedTests << endl;
    cout << "Failed: " << failedTests << endl;
    cout << "Success Rate: " << (passedTests * 100.0 / totalTests) << "%" << endl;
    
    return 0;
}