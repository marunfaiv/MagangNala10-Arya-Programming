#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

    int err[3] = {0};
    int Kp=10,Ki=5,Kd=2;
    int output;
    float integral = 0, derivative = 0;

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
        int val = 10;
        inputError(err, val);
        
        integral = sumAv(err);
        derivative = err[0] - err[1];
        output = Kp*err[0] + Ki*integral + Kd*derivative;
        cout << output << endl; // check nilai output
    }

    return 0;
}
