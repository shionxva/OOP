#include <iostream>
#include <vector>
using namespace std;

bool even(int val){
    return !(val % 2); //0 is false otw true
}

void fun(vector<vector<int>> matrix, vector<int>& result){

    for(int i = 0; i < matrix.size(); i++){
        for(int j=0; j < matrix[i].size(); j++){
            if(even(i+j)){
                result[i] += matrix[i][j];
            }
        }
    }
}

int fun2(vector<vector<int>> matrix){
    int res = 0;
    for(int i = 0; i < matrix.size(); i++){
        for(int j=0; j < matrix[i].size(); j++){
            if(i == j){
                res += matrix[i][j]*matrix[i][j];
            }
        }
    }
    return res;
}

int main(){
    vector<vector<int>> mat{{1,4,7},
                            {2,5,8},
                            {3,6,9}};
    vector<vector<int>> mat2{{1,2,3},
                            {4,5,6},
                            {7,8,9},
                            {10,11,12}};

    vector<int> res(mat.size());
    cout<< "q1: ";
    fun(mat,res);
    for(int x : res){
        cout << x << " ";
    }
    cout<<endl;
    cout<< "q2: ";
    int result = fun2(mat);
    cout << result;

    cout<<endl;
    vector<int> res2(mat2.size());
    cout<< "q1: ";
    fun(mat2,res2);
    for(int x : res2){
        cout << x << " ";
    }
    cout<<endl;
    cout<< "q2: ";
    int result2 = fun2(mat2);
    cout << result2;
    return 0;
}