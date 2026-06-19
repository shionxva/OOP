#include <iostream>
#include <math.h>
using namespace std;

float f1(float x){
    return (x/2) * (1+x+0.04*x*x*x);
}

float f2(float n){
    float res = 0;
    for(int i = 1; i <= n ; i++){
        res -= pow(0.5, i)/i;
    }
    return res;
}

int main(){
    cout << f1(0.5) << endl; //0.376250
    cout << f1(1.5) << endl; //1.976250
    cout << f2(10) << endl; //−0.693065
    cout << f2(100) << endl; //−0.693147
    return 0;
}