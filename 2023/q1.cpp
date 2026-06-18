#include <iostream>
#include <string>
using namespace std;

class Course{
private:
    int code;
    string cName;
public:
    Course(int courseCode, string courseName){
        code = courseCode;
        cName = courseName;
    }
    int getCourseCode(){
        return this->code;
    }
    string getCourseName(){
        return this->cName;
    }
    void describe(){
        cout<< "Course's code: "<< code<<endl;
        cout<< "Course's name: "<< cName<<endl;
    }
    
};

class Student{
private:
    string sname;
    int sID;
    Course* course; //dynamic memory
public:
    Student(string name, int studentID , int courseCode, string courseName)
    // : course(courseCode, courseName) "instead of using this"
    {
        sname = name;
        sID = studentID;
        course = new Course(courseCode, courseName); //dynamic memory "do this"
    }
    
    ~Student(){
        delete course; //delete
    }

    void describe(){
        cout<< "Student's name: "<< sname<<endl;
        cout<< "Student's ID: "<< sID<<endl;
        cout<< "Course's code: "<< course->getCourseCode()<<endl; //"pointer cannot use" course.getCourseCode()
        cout<< "Course's name: "<< course->getCourseName()<<endl;
    }
    
};

int main() {
    Student s1("Alice", 12345, 101, "Intro to Programming");
    s1.describe();
}