#include <iostream>
#include <string>
using namespace std;

class Manufacturer{
private:
    int id;
    string location;
public:
    Manufacturer(int id, string location) : id(id), location(location) {}

    void describe(){
        cout << "Manufacturer| id: " << id << ", location: " <<location<<endl;
    }
};

class Device{
private:
    string name;
    double price;
    Manufacturer* manufacturer; //use pointer for b)
public:
    Device(string name, double price, int id, string location) 
    : name(name), price(price) {    //cannot use manufacturer(id, location)
        manufacturer = new Manufacturer(id, location);
    }

    ~Device(){
        delete manufacturer;
    }

    void describe(){
        cout << "Device| name: " << name << ", price: " <<price<<endl;
        manufacturer->describe(); //notice pointer use -> not .
    }
};

int main(){
    Device mouse("mouse", 2.5, 9725, "Vietnam");
    mouse.describe();

    return 0;
}