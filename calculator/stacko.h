#ifndef STACKO_H
#define STACKO_H

#include <vector>
#include <iostream>

class stacko {
public:
    std::vector<double> stacker;

    void push(double num);
    double pop();
    double peek();
    bool isEmpty();
    void display();
    void clear();
};
#endif