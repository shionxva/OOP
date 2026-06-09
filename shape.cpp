#include <iostream>
#include <math.h>
using namespace std;

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
    void print() const {
        cout << "C point: (" << x << ", " << y << ")"<<endl;
    }
    
};

class TwoDShape{
protected:
    Point center;
public:
    TwoDShape(float x, float y) : center(x,y){
    }
    
    float distance(const TwoDShape &other) {
        float x1 = this->center.getX(); float y1 = this->center.getY();
        float x2 = other.center.getX(); float y2 = other.center.getY();
        float x = x1-x2; float y = y1-y2;
        return sqrt(x*x + y*y);
    }
};

class Circle : public TwoDShape {
private:
    float radius;
public:
    Circle (float x, float y, float r) : TwoDShape(x, y){
        radius = r;
    }
    
    void print(){
        center.print();
        cout << "radius: " << radius <<endl;
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
        center.print();
        cout << "width: " << width <<endl;
        cout << "height: " << height <<endl;
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
        center.print();
        cout << "side: " << side <<endl;
    }
};

int main() {
    Circle c(1, 2, 5);
    Rectangle r(3, 4, 10, 6);
    Square s(5, 5, 4);

    c.print();
    cout << endl;

    r.print();
    cout << endl;

    s.print();

    return 0;
}