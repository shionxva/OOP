#include <iostream>
#include <math.h>
#include <vector> //store object and sort
#include <algorithm> //sort
using namespace std;
const double PI = 3.14159;

class Point{
private:
    float x;
    float y;
public:    
    Point(float x=0, float y=0){
        this->x = x;
        this->y = y;
    }
    float getX() const {
        return x;
    }
    void setX(float n) {
        x = n;
    }
    float getY() const {
        return y;
    }
    void setY(float n) {
        y = n;
    }
    
};

class TwoDShape{
protected:
    Point center;
public:
    TwoDShape(float x, float y) : center(x,y){
    }

    virtual void print() = 0;

    float distance(const TwoDShape &other) {
        float x1 = this->center.getX(); float y1 = this->center.getY();
        float x2 = other.center.getX(); float y2 = other.center.getY();
        float x = x1-x2; float y = y1-y2;
        return sqrt(x*x + y*y);
    }

    virtual double perimeter() = 0;
    virtual double area() = 0;
};

class Circle : public TwoDShape {
private:
    float radius;
public:
    Circle (float x, float y, float r) : TwoDShape(x, y){
        radius = r;
    }
    
    void print(){
        cout << "C point: (" << center.getX() << ", " << center.getY() << ")"<<endl;
        cout << "radius: " << radius <<endl;
        cout << "perimeter: " << perimeter() <<endl;
        cout << "area: " << area() <<endl;
    }
    
    double perimeter() override {
        return 2*radius*PI;
    }
    
    double area() override {
        return PI*radius*radius;
    }
};

class Rectangle : public TwoDShape {
private:
    float width;
    float height;
public:
    Rectangle (float x, float y, float w, float h) : TwoDShape(x,y){
        width = w;
        height = h;
    }
    
    void print(){
        cout << "C point: (" << center.getX() << ", " << center.getY() << ")"<<endl;
        cout << "width: " << width <<endl;
        cout << "height: " << height <<endl;
        cout << "perimeter: " << perimeter() <<endl;
        cout << "area: " << area() <<endl;
    }
    
    double perimeter() override {
        return 2*(width + height);
    }
    
    double area() override {
        return width * height;
    }
};

class Square : public TwoDShape {
private:
    float side;
public:
    Square (float x, float y, float s) : TwoDShape(x, y){
        side = s;
    }

    void print(){
        cout << "C point: (" << center.getX() << ", " << center.getY() << ")"<<endl;
        cout << "side: " << side <<endl;
        cout << "perimeter: " << perimeter() <<endl;
        cout << "area: " << area() <<endl;
    }
    
    double perimeter() override {
        return 4*side;
    }
    double area() override {
        return side*side;
    }
};

int main() {
    Circle c(1, 2, 5);
    Rectangle r(3, 4, 10, 6);
    Square s(5, 5, 4);

    cout<< "circle info: "<<endl;
    c.print();
    cout << endl;

    cout<< "rectangle info: "<<endl;
    r.print();
    cout << endl;

    cout<< "square info: "<<endl;
    s.print();

    //vector
    vector<TwoDShape*> shapes;

    shapes.push_back(new Circle(1, 2, 5));
    shapes.push_back(new Rectangle(3, 4, 10, 6));
    shapes.push_back(new Square(5, 5, 4));
    sort(shapes.begin(), shapes.end(), [](TwoDShape* a, TwoDShape* b) {
        return a->area() < b->area();
    });

    cout << "\nSorted by area:\n";

    for (TwoDShape* shape : shapes) {
        shape->print();
        cout << endl;
    }

    double totalArea = 0;
    for (TwoDShape* shape : shapes) {
        totalArea += shape->area();
    }
    cout << "Total area: " << totalArea << endl;

    cout << "\nSorted by decreasing perimeter:\n";

    sort(shapes.begin(), shapes.end(), [](TwoDShape* a, TwoDShape* b) {
        return a->perimeter() > b->perimeter();
    });

    for (TwoDShape* shape : shapes) {
        shape->print();
        cout << endl;
    }
    return 0;
}