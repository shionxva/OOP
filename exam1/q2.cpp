#include <iostream>
#include <string>
using namespace std;

class Beverage{
private:
    float price;
    float toppingPrice;
    float condimentPrice;
public:
//default constructor
    Beverage(){
        this->price = 0;
        this->toppingPrice = 0;
        this->condimentPrice = 0;
    }
//constructor with parameters
    Beverage(float p, float tp, float cp){
        this->price = p;
        this->toppingPrice = tp;
        this->condimentPrice = cp;
    }
//copy constructor
    Beverage(const Beverage& other){
        this->price = other.price;
        this->toppingPrice = other.price;
        this->condimentPrice = other.condimentPrice;
    }
//overloaded op+
    Beverage operator+(const Beverage& other){
        Beverage result;
        result.price = price + other.price;
        result.toppingPrice = toppingPrice + other.toppingPrice;
        result.condimentPrice = condimentPrice + other.condimentPrice;
        return result;
    }
//overloaded op==
    bool operator==(const Beverage& other){
        return this->price == other.price 
        && this->toppingPrice == other.toppingPrice 
        && this->condimentPrice == other.condimentPrice;
    }

    void describe(){
        cout<< "price: " << price <<endl;
        cout<< "topping price: " << toppingPrice <<endl;
        cout<< "condiment price: " << condimentPrice <<endl;
    }
};


int main(){
    cout << "test default con" << endl;
    Beverage b0;
    b0.describe();
    cout<<endl;

    cout << "test param con" << endl;
    Beverage b1(10.5, 2.3, 1.2);
    b1.describe();
    cout<<endl;
    Beverage b2(15, 2.2, 0.5);
    b2.describe();
    cout<<endl;

    cout << "test copy con" << endl;
    Beverage b3(b1);
    b3.describe();
    cout<<endl;

    cout << "test plus op" << endl;
    b3 = b1 + b2;
    b3.describe();
    cout<<endl;

    cout<<"Is b3 = b1? " << (b3==b1) <<endl;
    cout<<"Assign b3 = b1" <<endl;
    b3 = b1;
    b3.describe();
    cout<<"Is b3 = b1? " << (b3==b1) <<endl;

    return 0;
}