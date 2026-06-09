#include <iostream>
using namespace std;

class ComplexNum{
private:
    float real;
    float img;
public:
//default constructor
    ComplexNum(){
        real = 0;
        img = 0;
    }
//constructor w params
    ComplexNum(int x, int y){
        real = x;
        img = y;
    }
//copy constructor
    ComplexNum (const ComplexNum &other){
        this->real = other.real;
        this->img = other.img;
    }
//overloaded fns
    ComplexNum operator+(const ComplexNum &other) const {
        ComplexNum result;
        result.real = real + other.real;
        result.img = img + other.img;
        return result;
    }
    bool operator==(const ComplexNum &other) const {
        return this->real == other.real && this->img == other.img;
    }
    ComplexNum& operator=(const ComplexNum &other){
        this->real = other.real;
        this->img = other.img;
        return *this;
    }
//misc
    void display() const {
        cout << real << " + " << img << "i\n";
    }
};

int main() {
    ComplexNum c1(2.5, 3.5);
    ComplexNum c2(1.0, 2.0);

    ComplexNum c3 = c1 + c2;
    c3.display();

    ComplexNum c4(c1);

    if (c1 == c4)
        cout << "Equal\n";

    c2 = c1;
}