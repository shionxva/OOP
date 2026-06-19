#include <iostream>
#include <vector>
using namespace std;

void fun(int min, int max, vector<int>& arr){
    for(int i = 0; i < arr.size(); i++){
        if (arr[i] < min){
            arr[i] = min;
        }
        else if (arr[i] > max){
            arr[i] = max;
        }
    }
}

void fun2(vector<int> arr, vector<int>& diff, vector<int>& sign){
    int val = 0;
    for(int i = 0; i < arr.size()-1; i++){
        val = arr[i+1] - arr[i];
        if(val < 0){
            sign[i] = -1;
        }
        diff[i] = val; sign[i] = 1;
    }
}


int main(){
    //manual input
    // int n = 0; cout << "n = "; cin >> n ;
    // vector<int> arr(n);
    // for(int i = 0; i < n; i++){
    //     int input = 0;
    //     cin >> input ;
    //     arr[i] = input;
    // }

    //1)
    cout<< "first question" << endl;
    vector<int> arr = {1,2,3,6,7};
    int minVal = 3; int maxVal = 6; int n = 5;

    fun(minVal,maxVal,arr);

    for(int i = 0; i < n; i++){
        cout << arr[i] << " ";
    }
    cout<<endl;

    cout<< "second question" << endl;
    vector<int> diff(arr.size()-1);
    vector<int> sign(arr.size()-1);
    fun2(arr, diff, sign);
    for(int i = 0; i < diff.size(); i++){
        cout << diff[i] << " ";
    }
    cout<<endl;;
    for(int i = 0; i < sign.size(); i++){
        cout << sign[i] << " ";
    }

    return 0;

}