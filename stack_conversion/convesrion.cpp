#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//this is algorithm that converts a mathematical expression from normal, infix notation to the reversed polish one
//the algorithm is based on a stack data structure and so called "priority table"
//don't ask me about rpn - I just followed an algorithm and learned how to use stacks

//stack element declaration
struct Stack{
    char value;
    Stack* previous;
};

//pointer to the top of the stack 
Stack* top = NULL;

//function that checks if the stack is empty
bool isEmpty(){
    if(top == NULL)
        return true;
    else
        return false;
}

//function that adds an element to the top of the stack
//@param val character stored in this element
void push(char val){
    Stack* tmp = new Stack();
    tmp->value = val;
    if(isEmpty()){
        tmp->previous = NULL;
        top = tmp;
    } else {
        tmp->previous = top;
        top = tmp;
    }
}

//function that pops the top elemnt from the chosen stack
//@return character stored in this element
char pop(){
    char tmp = top->value;
    top = top->previous;
    return tmp;
}

//function that checks if the given character is alphanumeric
//@param tmp character we want to check
//@return bool value
bool isAlphNum(char tmp){
    if((tmp >= '0')&&(tmp <= '9')||(tmp >= 'a')&&(tmp <= 'z'))
        return true;
    return false;
}

//funckja that checks the priority of the given character
//@param tmp character we want to check
//@return character's priority
int checkPriority(char c){
    if((c=='+')||(c=='-'))
        return 1;
    if((c=='*')||(c=='/')||(c=='%'))
        return 2;
    else
        return 0;
}

int main(){
    //opening files, reading values
    ifstream in;
    ofstream out;
    in.open("we.txt");
    out.open("wy.txt");
    string read;
    getline(in, read);

    //a loop that reads input char by char
    int i=0;
    while(read[i]){
        if(read[i]!=' '){
            //if the character is alphanumeric it goes straight to the output
            if(isAlphNum(read[i])){
                cout << read[i];
                out << read[i];
                //space management
                if((!isAlphNum(read[i+1]))||(!read[i+1])){
                    cout << " ";
                    out << " ";
                }
            }
            //if the character is an opening parenthesis - push it to the stack
            else if(read[i] == '('){
                push('(');
            //if it's a closing one - pop to output every operator to the opening parenthesis
            } else if(read[i] == ')'){
                char tmp;
                while((tmp = pop()) != '('){
                    cout << tmp << " ";
                    out << tmp << " ";
                }
            //if the read character is an operator follow the priority table
            } else {
                int priority = checkPriority(read[i]);
                char tmp;
                while((!isEmpty())&&(checkPriority(tmp=pop())>=priority)){
                    cout << tmp << " ";
                    out << tmp << " ";
                }
                //push back an operator if necessary
                if(checkPriority(tmp)<priority)
                    push(tmp);
                push(read[i]);
            }
        }
        i++;
    }

    //empty the stack and close the files
    while(char tmp = pop()){
        cout << tmp << "\n";
        out << tmp << " ";
    }
    in.close();
    out.close();
}