#pragma once
#include "Product.h"
#include <vector>
using namespace std;

class CartItem {
public:
    Product product;
    int quantity;

    CartItem(Product p, int qty)
        : product(p), quantity(qty) {}
};

class Cart {
private:
    vector<CartItem> items;

public:
    void addItem(Product p, int qty);
    void removeItem(int productId);
    void updateQuantity(int productId, int qty);

    double calculateTotal();

    vector<CartItem>& getItems();
};