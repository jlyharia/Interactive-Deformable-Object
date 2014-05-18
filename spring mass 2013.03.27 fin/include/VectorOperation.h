#include <iostream>
#include <vector>
using namespace std;

vector<float> vec_cross(const vector<float>&, const vector<float>&);


template <typename T>
T vec_dot(const vector<T>&, const vector<T>&);

vector<float> vec_subtraction(const vector<float>&, const vector<float>&);

template <typename T>
vector<T> vec_addition(const vector<T>&, const vector<T>&);


template <class T>
T vec_length(const vector<T> &input);


vector<float> vec_normalize(const vector<float> &input);

vector<float>vec_dim2to1(const vector< vector<float>> &);
