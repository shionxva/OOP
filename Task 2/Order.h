#pragma once
#include "Cart.h"
#include <vector>
using namespace std;

class Order {
private:
    int orderId;
    vector<CartItem> items;
    double totalPrice;
    string date;

public:
    Order();
    Order(int id, const vector<CartItem>& items, double total, const string& date);

    void display();

    int getId() const;
    double getTotal() const;
    const string& getDate() const;
    const vector<CartItem>& getItems() const;
};