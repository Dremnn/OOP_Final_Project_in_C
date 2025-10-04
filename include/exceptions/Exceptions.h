#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

using namespace std;

// ============= CUSTOM EXCEPTIONS =============
class CoffeeShopException : public runtime_error {
public:
    CoffeeShopException(const string& msg) : runtime_error(msg) {}
};

class AuthenticationException : public CoffeeShopException {
public:
    AuthenticationException(const string& msg) 
        : CoffeeShopException("[AUTH ERROR] " + msg) {}
};

class AuthorizationException : public CoffeeShopException {
public:
    AuthorizationException(const string& msg) 
        : CoffeeShopException("[PERMISSION DENIED] " + msg) {}
};

class ValidationException : public CoffeeShopException {
public:
    ValidationException(const string& msg) 
        : CoffeeShopException("[VALIDATION ERROR] " + msg) {}
};

#endif // EXCEPTIONS_H