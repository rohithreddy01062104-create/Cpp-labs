#include<iostream>
using namespace std;
class Tracer{
    int id;
public:
    Tracer(int i):id(i){
        cout<<"Construct #"<<id<<endl;
~Tracer(){
        cout<<"Destruct #"<<id<<endl;
    }