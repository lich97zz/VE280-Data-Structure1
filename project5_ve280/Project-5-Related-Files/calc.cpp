#include <iostream>
#include "dlist.h"
#include <cstdlib>
using namespace std;

//exception type
class zero_division_err{};
class operand_err{};
class bad_input_err{};

bool is_num(string str){
//EFFECT: check if the string is expressing an integer(eg."-1", "6")
    if((str[0]<'0' || str[0]>'9') && str[0]!='-')
        return false;
    if(str.length()!=1)
        for(unsigned int i=1; i<str.length();i++)
            if(str[i]<'0' || str[i]>'9')
                return false;
    if(str == "-")
        return false;
    return true;
}


void clear(Dlist<int> &storage){
//EFFECT: clear the stack
//MODIFIES: stack
    while(!storage.isEmpty()){
        int* ptr = storage.removeBack();
        delete ptr;
    }
}

void print_top(Dlist<int> storage){
//EFFECT: print the top of the stack, if the stack is empty, throw exception
    if(storage.isEmpty()){
        operand_err err;
        throw err;
    }
    int* top = storage.removeBack();
    cout<<*top<<endl;
    delete top;
}

void print_all(Dlist<int> storage){
//EFFECT: print all elements of the stack in one line, seperated by space
    while(!storage.isEmpty()){
        int* temp = storage.removeBack();
        cout<<*temp<<" ";
        delete temp;
    }
    cout<<endl;
}

bool enough_operands(Dlist<int> stack){
//EFFECT: check if the stack has two or more than two operands
    if(stack.isEmpty())
        return false;
    int* temp = stack.removeBack();
    delete temp;
    if(stack.isEmpty())
        return false;
    return true;
}

void binary_operation(Dlist<int> &stack, string operation){
//EFFECTS: do binary operation, change the stack accordingly
//MODIFIES: stack
    if(!enough_operands(stack)){
        operand_err err;
        throw err;
    }
    int* int_2 = stack.removeBack();
    int* int_1 = stack.removeBack();
    if(operation == "plus"){
        int result = *int_1 + *int_2;
        int* ptr_result = new int(result);
        stack.insertBack(ptr_result);
    }
    else if(operation == "minus"){
        int result = *int_1 - *int_2;
        int* ptr_result = new int(result);
        stack.insertBack(ptr_result);
    }
    else if(operation == "multiply"){
        int result = *int_1 * *int_2;
        int* ptr_result = new int(result);
        stack.insertBack(ptr_result);
    }
    else if(operation == "divide"){
        if(*int_2 == 0){
            stack.insertBack(int_1);
            stack.insertBack(int_2);
            zero_division_err err;
            throw err;
        }
        int result = *int_1 / *int_2;
        int* ptr_result = new int(result);
        stack.insertBack(ptr_result);
    }
    else if(operation == "reverse"){
        int* result1 = new int(*int_1);
        int* result2 = new int(*int_2);
        stack.insertBack(result2);
        stack.insertBack(result1);
    }
    delete int_1;
    delete int_2;
}

void unary_operation(Dlist<int> &stack, string operation){
//EFFECTS: do unary operation, change the stack accordingly
//MODIFIES: stack
    if(stack.isEmpty()){
        operand_err err;
        throw err;
    }
    int* int_1 = stack.removeBack();
    if(operation == "negate"){
        int result = (-1) * *int_1;
        int* ptr_result = new int(result);
        stack.insertBack(ptr_result);
    }
    else if(operation == "duplicate"){
        int* result1 = new int(*int_1);
        int* result2 = new int(*int_1);
        stack.insertBack(result1);
        stack.insertBack(result2);
    }
    delete int_1;
}

void one_round(Dlist<int> &storage, string input){
//EFFECTS: each time a valid input is given, do calculation accordingly
//MODIFIES: stack
    if(is_num(input)){
        int num = atoi(input.c_str());
        int* ptr_num = new int(num);
        storage.insertBack(ptr_num);
        return;
    }
    if(input.length() > 1){
        bad_input_err err;
        throw err;
    }
    char choice = input[0];
    switch (choice){
        case 'p': print_top(storage); break;
        case 'a': print_all(storage); break;
        case 'c': clear(storage); break;
        case '+': binary_operation(storage, "plus"); break;
        case '-': binary_operation(storage, "minus"); break;
        case '*': binary_operation(storage, "multiply"); break;
        case '/': binary_operation(storage, "divide"); break;
        case 'r': binary_operation(storage, "reverse"); break;
        case 'n': unary_operation(storage, "negate"); break;
        case 'd': unary_operation(storage, "duplicate"); break;
        default: bad_input_err err; throw err; break;
    }
}

void calculator(){
//EFFECTS: this is the calculator
    Dlist<int> storage;
    string input;
    while(1){
        cin >> input;
        if(input == "q")
            break;
        try{
            one_round(storage, input);
        }
        catch(bad_input_err e){cout<<"Bad input"<<endl;}
        catch(operand_err e){cout<<"Not enough operands"<<endl;}
        catch(zero_division_err e){cout<<"Divide by zero"<<endl;}
    }    
}


int main(int argc, char *argv[])
{
    calculator();
}
