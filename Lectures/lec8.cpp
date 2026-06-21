#include <iostream>
#include <string>
using namespace std;

class Shape{
private:
    string color;
public:
    Shape(string color ="") : color(color) {}

    virtual void describe() {
        cout << "color: " << color;
    }

    string getColor() {
        return color;
    }
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle(string color = "", double r = 0) : Shape(color), radius(r) {}

    void describe() override {
        cout<< "color: " << this->getColor() << endl;
        cout<< "radius: " << radius << endl;
    }
};

class Rectangle : public Shape {
private:
    double width;
    double height;
public:
    Rectangle(string color = "", double w = 0, double h=0) 
    : Shape(color), width(w), height(h) {}

    void describe() override {
        cout<< "color: " << this->getColor() << endl;
        cout<< "width: " << width <<", height: " << height << endl;
    }
};

int main(){
    Circle c1("red",5 );
    Rectangle r1("blue", 3, 4 );
    c1.describe();
    r1.describe();
    return 0;
}