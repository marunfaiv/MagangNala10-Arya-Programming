#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

    int err[3] = {0};
    int e_accum = 0;
    int Kp=10,Ki=5,Kd=2;
    int output;

int sumAv(int arr[]){
    int sum;

    sum = (arr[0] + arr[1] + arr[2]);
    return sum;
}

void inputError(int *arr, int value){
    arr[2] = arr[1];
    arr[1] = arr[0];
    arr[0] = value;
}

int main(){


    while(true){
        int val = rand();
        inputError(err, val);
        e_accum = sumAv(err);
        output = Kp*err[0] + Ki*e_accum + Kd*err[1];
        cout << output << endl; // check nilai output
    }

    return 0;
}
