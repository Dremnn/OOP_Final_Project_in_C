#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// ============= UTILITY FUNCTIONS =============
string generateId(string prefix) {
    static int counter = 1000;
    counter++;
    return prefix + to_string(counter);
}

string formatPrice(double price) {
    stringstream ss;
    ss << fixed << setprecision(0) << price << " VND";
    return ss.str();
}

#endif // UTILS_H