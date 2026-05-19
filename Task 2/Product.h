#pragma once
#include <string>
using namespace std;

class Product {
private:
    int id;
    string name;
    string category;
    double price;
    int stock;

    int viewCount;
    int purchaseCount;

public:
    Product();

    Product(int id, string name,
            string category,
            double price,
            int stock);

    void display() const;

    void increaseView();
    void increasePurchase(int qty);

    int getId() const;
    const string& getName() const;
    const string& getCategory() const;
    double getPrice() const;
    int getStock() const;
    int getViewCount() const;
    int getPurchaseCount() const;

    void setName(const string& value);
    void setCategory(const string& value);
    void setPrice(double value);
    void setStock(int value);
    void setViewCount(int value);
    void setPurchaseCount(int value);
};