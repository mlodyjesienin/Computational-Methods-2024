#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;

float naive_sum(const float *array, int size);

float recursive_sum(float *array, int size);

float kahan_sum(float *array, int size);

int zadanie1_zadanie2();

float riemann_bottom_up(int n, float s);
float riemann_top_down(int n, float s);
float dirchlett_bottom_up(int n, float s);
float dirchlett_top_down(int n, float s);

double DP_riemann_bottom_up(int n, double s);
double DP_riemann_top_down(int n, double s);
double DP_dirchlett_bottom_up(int n, double s);
double DP_dirchlett_top_down(int n, double s);

int zadanie3();

int main() {
    zadanie1_zadanie2();
    zadanie3();

    return 0;
}
int zadanie1_zadanie2() {
    int size = 1e7;
    float *T = new float[size];
    float val = 0.53125;
    float result = val * size;

    for (int i = 0; i < 1e7; i++) {
        T[i] = val;
    }

    auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    start = high_resolution_clock::now();
    float sum_naive = naive_sum(T, 1e7);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    float error1 = abs(sum_naive - result);
    float error2 = error1 / result;

    cout << "blad bezwzgledny algorytmu naiwnego: " << error1 << endl;
    cout << "blad wzgledny algorytmu naiwnego: " << error2 << endl;
    cout << "czas wykonania algorytmu naiwnego: " << duration.count() << " microseconds" << endl;
    cout << endl;

    start = high_resolution_clock::now();
    float sum_recursive = recursive_sum(T, 1e7);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    error1 = abs(sum_recursive - result);
    error2 = error1 / result;

    cout << "blad bezwzgledny algorytmu rekurencyjnego: " << error1 << endl;
    cout << "blad wzgledny algorytmu rekurencyjnego: " << error2 << endl;
    cout << "czas wykonania algorytmu rekurencyjnego: " << duration.count() << " microseconds" << endl;
    cout << endl;


    start = high_resolution_clock::now();
    float sum_kahan = kahan_sum(T, 1e7);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    error1 = abs(sum_kahan - result);
    error2 = error1 / result;

    cout << "blad bezwzgledny algorytmu Kahana: " << error1 << endl;
    cout << "blad wzgledny algorytmu Kahana: " << error2 << endl;
    cout << "czas wykonania algorytmu Kahana: " << duration.count() << " microseconds" << endl;

    delete[] T;

    return 0;
}
float naive_sum(const float *array, int size) {
    float sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += array[i];
    }
    return sum;
}

float rec(float *array, int start, int end){
    if(start==end-1){
        return array[start];
    }
    int middle = (start + end) / 2;
    return rec(array, start, middle) + rec(array, middle, end);
}

float recursive_sum(float *array, int size) {
    return rec(array,0,size);
}

float kahan_sum(float *array, int size){
    float sum = 0.0f;
    float err = 0.0f;
    for(int i=0; i<size; i++){
        float y = array[i] - err;
        float temp = sum +y;
        err = (temp - sum) - y;
        sum = temp;
    }
    return sum;
}

int zadanie3(){
    float T1[5] = {2.0, 3.6667, 5, 7.2, 10};
    int T2[5] = {50,100,200,500,1000};
    double DP_T1[5] = {2.0, 3.6667, 5, 7.2, 10};
    for (int i = 0; i < 5; ++i) {
        for (int n : T2) {
            float s = T1[i];
            double DP_s = DP_T1[i];

            cout << "wartosc n: " << n << endl;
            cout << "wartosc s: " << s << endl;
            cout << endl;

            // Single precision calculations
            float riemann_bottom_up_result = riemann_bottom_up(n, s);
            float riemann_top_down_result = riemann_top_down(n, s);
            float dirchlett_bottom_up_result = dirchlett_bottom_up(n, s);
            float dirchlett_top_down_result = dirchlett_top_down(n, s);

            // Double precision calculations
            double DP_riemann_bottom_up_result = DP_riemann_bottom_up(n, DP_s);
            double DP_riemann_top_down_result = DP_riemann_top_down(n, DP_s);
            double DP_dirchlett_bottom_up_result = DP_dirchlett_bottom_up(n, DP_s);
            double DP_dirchlett_top_down_result = DP_dirchlett_top_down(n, DP_s);

            // Print differences between bottom-up and top-down results
            cout << "(DP) riemann difference:  " << DP_riemann_bottom_up_result - DP_riemann_top_down_result << endl;
            cout << "(DP) dirchlett difference: " << DP_dirchlett_bottom_up_result - DP_dirchlett_top_down_result << endl;
            cout << endl;
            cout << "(f) riemann difference:  " << riemann_bottom_up_result - riemann_top_down_result << endl;
            cout << "(f) dirchlett difference: " << dirchlett_bottom_up_result - dirchlett_top_down_result << endl;
            cout << "---    ---     ---     ---" << endl;
        }
    }

    return 0;
}

float riemann_bottom_up(int n, float s){
    float sum =0.0;
    for(int i=1; i<n;i++){
        sum+= pow(i,-s);
    }
    return sum;
}

float riemann_top_down(int n, float s){
    float sum =0.0f;
    for(int i=n-1; i>=1;i--){
        sum+= pow(i,-s);
    }
    return sum;
}

float dirchlett_bottom_up(int n, float s){
    float sum =0.0f;
    for(int i=1; i<n;i++){
        sum+= pow(-1,i-1)*1/pow(i,s);
    }
    return sum;
}

float dirchlett_top_down(int n, float s){
    float sum =0.0f;
    for(int i=n-1; i>=1;i--){
        sum+= pow(-1,i-1)*1/pow(i,s);
    }
    return sum;
}

double DP_riemann_bottom_up(int n, double s) {
    double sum = 0.0;
    for (int i = 1; i < n; i++) {
        sum += 1 / pow(i, s);
    }
    return sum;
}

double DP_riemann_top_down(int n, double s) {
    double sum = 0.0;
    for (int i = n - 1; i >= 1; i--) {
        sum += 1 / pow(i, s);
    }
    return sum;
}

double DP_dirchlett_bottom_up(int n, double s) {
    double sum = 0.0;
    for (int i = 1; i < n; i++) {
        sum += pow(-1, i - 1) * 1 / pow(i, s);
    }
    return sum;
}

double DP_dirchlett_top_down(int n, double s) {
    double sum = 0.0;
    for (int i = n - 1; i >= 1; i--) {
        sum += pow(-1, i - 1) * 1 / pow(i, s);
    }
    return sum;
}