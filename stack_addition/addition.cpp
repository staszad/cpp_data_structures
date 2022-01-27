#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// this is the algorithm to add two big numbers digit by digit
// it could be obviously done by just adding, but this is more like excuse to learn how stacks work in c++
// the program uses three stacks - two for storing numbers to add and one to store the rsult
// using stacks allows us to read numbers from right to left - like we do when we add them on paper

//stack element declaration
struct Stack{
    char value;
    Stack* previous;
};

//function that adds an element to the chosen stack
//@param val character stored in this element
//@param e reference to pointer to the stack. We do it so we can change the pointer itself not the elemnt which it points to
void push(char val, Stack *&e){
    Stack* tmp = new Stack();
    tmp->value = val;
    if((e)==NULL){
        e = tmp;
    } else {
        tmp->previous = e;
        e = tmp;
    }
}

//function that pops top elemnt from the chosen stack
//@param e referenc to pointer to the top of the chosen stack
//@return character stored in this element
char pop(Stack *&e){
    if(e==NULL)
        return 0;
    char tmp = e->value;
    e = e->previous;
    return tmp;
}

//pointers to top of the three stacks we will use
Stack*first;
Stack*second;
Stack*result;

int main(){
    //opening files
    ifstream in;
    ofstream out;
    in.open("we.txt");
    out.open("wy.txt");

    //reading values and saving them to the stacks
    string number;
    in >> number;
    int i=0;
    while(number[i]){
        push(number[i++], first);
    }
    i=0;
    in >> number;
    while(number[i]){
        push(number[i++], second);
    }

    //addition
    int over = 0; //if sum of two digits is bigger than 10 this is the place where we will store this extra digit
    while(true){
        int digit1 = pop(first);
        int digit2 = pop(second);
        if((digit1==0)&&(digit2==0)) //if both stacks are empty the loop should break
            break;
        if(!digit1)
            digit1 = '0';
        if(!digit2)
            digit2 = '0';
        int sum = digit1 - '0' + digit2 - '0' + over;
        over = 0;
        if(sum >= 10){ //removing the extra digit (if necessary)
            over = 1;
            sum -= 10;
        }
        push(sum+'0', result);
    }
    if(over){
        push(over + '0', result);
    }

    //result presentation
    while (char tmp = pop(result)){
        cout << tmp;
        out << tmp;
    }
    in.close();
    out.close();
}