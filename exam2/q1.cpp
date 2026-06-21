#include <iostream> 
#include <string>
#include <vector>
using namespace std;

class Media{
    double price;
public:
    Media(double p=0) : price(p){}
    double getPrice(){
        return price;
    }
    virtual void describe() = 0;
};

class Book : public Media{
private:
    string name;
public:
    Book(string name="", double price = 0) : name(name), Media(price) {}
    void describe() override {
        cout << "Book name: " <<name << ", price: " << this->getPrice() <<endl;
    }
};

class CD : public Media{
private:
    int ID;
public:
    CD(int id=0, double price = 0) : ID(id), Media(price) {}
    void describe() override {
        cout << "CD id: " <<ID << ", price: " <<this->getPrice()<<endl;
    }
};

class Library{
private:
    string name;
    vector<Media*> mList;
public:
    Library(string n ="", vector<Media*> list ={}) : name(n), mList(list) {}

    void describe(){
        cout<< "Library | name: " << name << endl << "list:" <<endl;
        for(Media* m: mList){
            m->describe();
        }
         
    }

    void addMedia(Media* media){
        mList.push_back(media);
    }
};



int main(){
    Book book1("C++", 20.0);
    book1.describe();
    CD cd1(12345, 5.52);
    cd1.describe();

    cout<<endl;
    Library lib("Lib1");
    Book book2("C#", 21.0);
    lib.addMedia(&book1);
    lib.addMedia(&book2);
    lib.addMedia(&cd1);
    lib.describe();

    return 0;
}