# include "stacko.h"

    void stacko::push(double num){
        stacker.push_back(num);
    }

    double stacko::pop(){
        double value = peek();
        stacker.pop_back();    
        return value;
    }

    double stacko::peek(){
        if(!isEmpty()) 
            return stacker.back();
        else {
            std::cout << "Stack is empty.";
            return -1;
        }
    }   

    bool stacko::isEmpty(){
        return stacker.empty(); 
    }

    void stacko::clear(){
        while (!isEmpty()){
            stacker.pop_back();
        }
    }
    void stacko::display(){
        for (double num : stacker){
            std::cout << num << " ";
        }
    };
