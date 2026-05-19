#pragma once
#include <string>
using namespace std;

class User {
protected:
    int id;
    string username;
    string password;

public:
    virtual void menu() = 0;

    const string& getUsername() const { return username; }

    virtual ~User() {}
};