#pragma once
#include "User.h"
#include <vector>
using namespace std;

class Admin : public User {
public:
    void addProduct();
    void editProduct();
    void deleteProduct();
    void viewStatistics();

    void menu() override;
};