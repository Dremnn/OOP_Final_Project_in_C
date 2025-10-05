#include <iostream>
#include <vector>
#include "include/system/CoffeeShopSystem.h"

using namespace std;

void printSeparator() {
    cout << "\n========================================================\n";
}

void printHeader(string title) {
    printSeparator();
    cout << "  " << title << endl;
    printSeparator();
}

int main() {
    printHeader("COFFEE SHOP SYSTEM - DEMO");
    
    CoffeeShopSystem system;
    
    try {
        //========================================================
        // PHASE 1: SYSTEM INITIALIZATION
        //========================================================
        printHeader("PHASE 1: SYSTEM INITIALIZATION");
        
        cout << "\n[*] Initializing system with default admin account..." << endl;
        system.initializeSystem();
        cout << "[+] System initialized successfully!" << endl;
        
        //========================================================
        // PHASE 2: ADMIN SETUP - ADD PRODUCTS
        //========================================================
        printHeader("PHASE 2: ADMIN SETUP - ADDING PRODUCTS");
        
        cout << "\n[*] Admin logging in..." << endl;
        system.login("admin", "admin123");
        cout << "[+] Admin logged in successfully!" << endl;
        
        cout << "\n[*] Adding drinks to menu..." << endl;
        string espressoId = system.addDrink("Espresso", 45000, "M", true);
        string latteId = system.addDrink("Latte", 50000, "M", true);
        string icedCoffeeId = system.addDrink("Iced Coffee", 40000, "M", false);
        cout << "[+] Added 3 drinks to menu" << endl;
        
        cout << "\n[*] Adding food items to menu..." << endl;
        string croissantId = system.addFood("Croissant", 35000, false);
        string sandwichId = system.addFood("Vegetarian Sandwich", 45000, true);
        cout << "[+] Added 2 food items to menu" << endl;
        
        system.logout();
        cout << "\n[+] Admin logged out" << endl;
        
        //========================================================
        // PHASE 3: GUEST BROWSING
        //========================================================
        printHeader("PHASE 3: GUEST BROWSING MENU");
        
        cout << "\n[*] Guest viewing available products..." << endl;
        system.displayProductsForGuest();
        
        //========================================================
        // PHASE 4: CUSTOMER REGISTRATION & LOGIN
        //========================================================
        printHeader("PHASE 4: CUSTOMER REGISTRATION & LOGIN");
        
        cout << "\n[*] Registering new customer 'Alice'..." << endl;
        string aliceId = system.registerCustomer("alice", "alice123", "0901234567");
        cout << "[+] Customer registered with ID: " << aliceId << endl;
        
        cout << "\n[*] Alice logging in..." << endl;
        system.login("alice", "alice123");
        cout << "[+] Alice logged in successfully!" << endl;
        
        //========================================================
        // PHASE 5: BROWSING & ADDING TO CART
        //========================================================
        printHeader("PHASE 5: BROWSING & SHOPPING");
        
        cout << "\n[*] Viewing all available products..." << endl;
        system.displayAllProducts();
        
        cout << "\n[*] Adding items to cart..." << endl;
        system.addToCart(latteId, 2, "L");
        cout << "[+] Added 2 Large Lattes to cart" << endl;
        
        system.addToCart(icedCoffeeId, 1, "M");
        cout << "[+] Added 1 Medium Iced Coffee to cart" << endl;
        
        system.addToCart(croissantId, 3);
        cout << "[+] Added 3 Croissants to cart" << endl;
        
        cout << "\n[*] Viewing cart contents..." << endl;
        system.displayCart();
        
        //========================================================
        // PHASE 6: CHECKOUT & PAYMENT (BANK TRANSFER)
        //========================================================
        printHeader("PHASE 6: CHECKOUT - BANK TRANSFER ORDER");
        
        cout << "\n[*] Proceeding to checkout..." << endl;
        Order* order1 = system.checkout(REGULAR_ORDER, "123 Nguyen Hue, District 1, HCMC", BANK_TRANSFER);
        cout << "[+] Order created successfully!" << endl;
        order1->displayInfo();
        
        cout << "\n[*] Processing bank transfer payment..." << endl;
        bool paid = system.processPayment(order1->getId(), order1->getTotal());
        if (paid) {
            cout << "[+] Payment successful! Order confirmed." << endl;
        }
        
        //========================================================
        // PHASE 7: SECOND ORDER (CASH ON DELIVERY)
        //========================================================
        printHeader("PHASE 7: SECOND ORDER - CASH ON DELIVERY");
        
        cout << "\n[*] Adding more items to cart..." << endl;
        system.addToCart(espressoId, 1, "S");
        cout << "[+] Added 1 Small Espresso" << endl;
        
        system.addToCart(sandwichId, 2);
        cout << "[+] Added 2 Vegetarian Sandwiches" << endl;
        
        cout << "\n[*] Viewing cart..." << endl;
        system.displayCart();
        
        cout << "\n[*] Checkout with Express delivery and COD..." << endl;
        Order* order2 = system.checkout(EXPRESS_ORDER, "456 Le Loi, District 1, HCMC", CASH_ON_DELIVERY);
        cout << "[+] Express order created and auto-confirmed (COD)!" << endl;
        order2->displayInfo();
        
        //========================================================
        // PHASE 8: VIEW ORDER HISTORY
        //========================================================
        printHeader("PHASE 8: CUSTOMER ORDER HISTORY");
        
        cout << "\n[*] Alice viewing order history..." << endl;
        system.displayMyOrders();
        
        system.logout();
        cout << "\n[+] Alice logged out" << endl;
        
        //========================================================
        // PHASE 9: ADMIN OPERATIONS
        //========================================================
        printHeader("PHASE 9: ADMIN ORDER MANAGEMENT");
        
        cout << "\n[*] Admin logging in..." << endl;
        system.login("admin", "admin123");
        cout << "[+] Admin logged in" << endl;
        
        cout << "\n[*] Viewing all orders..." << endl;
        vector<Order*> allOrders = system.viewAllOrders();
        cout << "[+] Total orders in system: " << allOrders.size() << endl;
        
        cout << "\n[*] Updating order status to PREPARING..." << endl;
        system.updateOrderStatus(order1->getId(), PREPARING);
        cout << "[+] Order " << order1->getId() << " status updated to PREPARING" << endl;
        
        cout << "\n[*] Updating order status to READY..." << endl;
        system.updateOrderStatus(order1->getId(), READY);
        cout << "[+] Order " << order1->getId() << " status updated to READY" << endl;
        
        cout << "\n[*] Checking total revenue..." << endl;
        double revenue = system.getTotalRevenue();
        cout << "[+] Total Revenue: " << formatPrice(revenue) << endl;
        
        system.logout();
        cout << "\n[+] Admin logged out" << endl;
        
        //========================================================
        // PHASE 10: NEW CUSTOMER - ORDER CANCELLATION
        //========================================================
        printHeader("PHASE 10: ORDER CANCELLATION DEMO");
        
        cout << "\n[*] Registering new customer 'Bob'..." << endl;
        system.registerCustomer("bob", "bob12345", "0909876543");
        system.login("bob", "bob12345");
        cout << "[+] Bob logged in" << endl;
        
        cout << "\n[*] Bob adding items and placing order..." << endl;
        system.addToCart(latteId, 1, "M");
        Order* bobOrder = system.checkout(REGULAR_ORDER, "789 Tran Hung Dao, District 5, HCMC", BANK_TRANSFER);
        cout << "[+] Order created: " << bobOrder->getId() << endl;
        
        cout << "\n[*] Bob cancelling the order..." << endl;
        system.cancelOrder(bobOrder->getId());
        cout << "[+] Order cancelled successfully!" << endl;
        cout << "    Status: " << bobOrder->getStatusString() << endl;
        
        system.logout();
        cout << "\n[+] Bob logged out" << endl;
        
        //========================================================
        // DEMO COMPLETED
        //========================================================
        printHeader("DEMO COMPLETED SUCCESSFULLY");
        
        cout << "\n[SUMMARY]" << endl;
        cout << "- Admin initialized and added 5 products" << endl;
        cout << "- Guest browsed menu without login" << endl;
        cout << "- Customer Alice registered, placed 2 orders" << endl;
        cout << "- Admin managed orders and checked revenue" << endl;
        cout << "- Customer Bob placed and cancelled an order" << endl;
        cout << "\n[+] All core features demonstrated!" << endl;
        
    } catch (CoffeeShopException& e) {
        cout << "\n[ERROR] " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cout << "\n[SYSTEM ERROR] " << e.what() << endl;
        return 1;
    }
    
    printSeparator();
    cout << "\nPress Enter to exit...";
    cin.get();
    
    return 0;
}