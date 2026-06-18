#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Member{
private:
    string name;
    int year;
public:
    Member(string name = "", int year=0){
        this->name = name;
        this->year = year;
    }
    virtual void describe() const{
        cout << "Name: " << name << endl;
        cout << "Year of birth: " << year << endl;
    }

    virtual bool isCoordinator() {
        return false;
    }

    virtual bool isMentor() {
        return false;
    }

    int getYear() const {
        return year;
    }

    string getName() const {
        return name;
    }

};

class Society{
private:
    string sName;
    vector<Member*> memberList; 
    //store pointer instead of plain vector<Member> because it copies only the Member part into the vector.
public:
    Society(string name = "", vector<Member*> list ={}) : sName(name), memberList(list) {}

    void describe() const{
        cout << "Society name: " << sName << endl;
        cout << "Member list: " << endl;
        for(const Member* m : memberList){
            m->describe();
        }
    }

    void addMember(Member* m){
        memberList.push_back(m);
    }

    int countCoordinator(){
        int count = 0;
        for(Member* m : memberList){
            if(m->isCoordinator()){
                count++;
            }
        }
        return count;
    }

    void sortByYear(){
        sort(memberList.begin(), memberList.end(),[](Member* a, Member* b){return a->getYear() < b->getYear();});
    }

    float aveMentorYearOfBirth(){
        int count = 0;
        int sum = 0;
        for(Member* m : memberList){
            if(m->isMentor()){
                sum += m->getYear();
                count++;
            }
        }
        return sum/count;
    }
};

class Participant : public Member {
private:
    string field;
public:
    Participant(string name ="", int year =0, string field =""): Member(name, year), field(field){}

    virtual void describe() const{
        cout << "Name: " << this->getName() << endl;
        cout << "Year of birth: " << this->getYear() << endl;
        cout << "Field: " << field << endl;
    }
};

class Mentor : public Member {
private:
    string subject;
public:
    Mentor(string name ="", int year =0, string subject =""): Member(name, year), subject(subject){}

    virtual void describe() const{
        cout << "Name: " << this->getName() << endl;
        cout << "Year of birth: " << this->getYear() << endl;
        cout << "Sub: " << subject << endl;
    }

    virtual bool isMentor() {
        return true;
    }
};

class Coordinator : public Member {
private:
    string division;
public:
    Coordinator(string name ="", int year =0, string division ="") : Member(name, year), division(division){}

    virtual void describe() const{
        cout << "Name: " << this->getName() << endl;
        cout << "Year of birth: " << this->getYear() << endl;
        cout << "Div: " << division << endl;
    }

    virtual bool isCoordinator(){
        return true;
    }
};



int main(){
    cout<< "Create participant Alice" <<endl; 
    Participant participant(" Alice ", 1995 , "Art") ;
    cout<< endl;

    cout<< "Create 2 mentors Bob and Carol" <<endl; 
    Mentor mentor1 (" Bob", 1980 , " Math ") ;
    Mentor mentor2 (" Carol ", 1970 , " Science ") ;
    cout<< endl;

    cout<< "Create 2 coordinator David and Thomas" <<endl; 
    Coordinator coordinator1 (" David ", 1985 , " Events ") ;
    Coordinator coordinator2 (" Thomas ", 1980 , " Logistics ") ;
    cout<< endl;

    cout << "create Society 1 with 1 participant , 2 mentor , 2 coordinator" <<endl;
    Society society (" society1 ") ;
    society.addMember (& participant ) ;
    society.addMember (& mentor1 ) ;
    society.addMember (& mentor2 ) ;
    society.addMember (& coordinator1 ) ;
    society.addMember (& coordinator2 ) ;
    society.describe () ;
    cout<< endl;

    int numberOfCoordinators = society.countCoordinator () ;
    cout << "numberOfCoordinators : " << numberOfCoordinators << endl ;

    cout << "sort society by age" <<endl;
    society.sortByYear();
    society.describe () ;
    cout<< endl;

    int aveMentorYearOfBirth = society.aveMentorYearOfBirth();
    cout << "Average year of birth ( Mentors ): " << aveMentorYearOfBirth << endl ;

    return 0;
}