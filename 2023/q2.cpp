#include <iostream>
using namespace std;

class Product{
private:
    float price;
    float weight;
    float discount;

public:
//default constructor
    Product(){
        price = 0;
        weight = 0;
        discount = 0;
    }
//constructor w/ params
    Product(float p, float w, float d){
        price = p;
        weight = w;
        discount = d;
    }
//copy constructor
    Product(Product &other){
        price = other.price;
        weight = other.weight;
        discount = other.discount;
    }
//overloaded plus op
    Product operator + (const Product &other) const {
        Product res;
        res.price = price + other.price;
        res.weight = weight + other.weight;
        res.discount = discount + other.discount;
        return res;
    }
//overloaded assign op "note that assign operator return a reference"
    Product& operator = (const Product &other) {
        price = other.price;
        weight = other.weight;
        discount = other.discount;
    }
//overloaded compare op "return bool"
    bool operator == (const Product &other) const {
        return price == other.price && weight == other.weight && discount == other.discount;
    }
//describe
    void describe(){
        cout << "Price: " << price <<endl;
        cout << "Weight: " << weight <<endl;
        cout << "Discount: " << discount <<endl;
    }
};



int main(){
    //try dynamic memory allocated object
    // Product m0; "this is the equivalent"
    Product* m0;
    m0 = new Product;
    cout << "m0 with default constructor" <<endl;
    m0->describe(); //dont use m0.describe()
    cout<<endl;

    cout << "m1" <<endl;
    Product m1(10.5, 2.3, 1.2);
    m1.describe();
    cout<<endl;
    
    cout << "m2" <<endl;
    Product m2(15, 2.2, 0.5);
    m2.describe();
    cout<<endl;

    cout << "m3" <<endl;
    Product m3(m1);
    m3.describe();
    cout<<endl;

    cout<<"m3 = m1 + m2"<<endl;
    m3 = m1 + m2;
    m3.describe();
    cout << "Is m3 = m1? " << ( m3 == m1 ) << endl ;
    cout<<endl;
    
    cout<<"m3 = m1"<<endl;
    m3 = m1;
    m3.describe ();
    cout << "Is m3 = m1? " << ( m3 == m1 ) << endl ;
    return 0;
}