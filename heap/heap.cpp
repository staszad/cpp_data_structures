#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//--------STACK--------
//Simple stack structure that I needed for one function (FILO queue)
//Its properties are char value and pointer to the previous element
struct StackEl{
    char value;
    StackEl * previous;
    StackEl(char v);
};
StackEl * top = NULL;

//Constructor of the structure
//@param v single character - value of an element
StackEl::StackEl(char v){
    value = v;
    previous = NULL;
}

//function that pushes element with given value to the top of the stach
//@param v single character - value of an element
void push(char v){
    StackEl * tmp = new StackEl(v);
    tmp->previous = top;
    top = tmp;
}

//function that pops out value of the top element, deletes the top element and puts next on top
//@return value of the popped element
char pop(){
    if(!top)
        return 0;
    char c = top->value;
    StackEl * tmp = top;
    top = top->previous;
    delete tmp;
    tmp = NULL;
    return c;
}

//--------HEAP--------
//Structure of single node from the heap. It contains of value, obviously, and some other things.
//Because it is based on an array this struct has its index and pointers to the next and previous element in the array.
//Because it's also a binary tree, the structure has methods to calculate parent and children of an element
struct Node{
    int value;
    //array properties
    int index;
    Node*next;
    Node*prev;
    
    //constructor
    Node(int v){
        value = v;
        next = prev = NULL;
    }

    //tree methods
    Node* parent();
    Node* left();
    Node* right();
};

Node *first, *last; //pointers to the first and the last element of the array
ofstream out; //output file

//Function that returns pointer to an element at given index. Almost all other functions use it.
//@param index index of an element in the array
//@return pointer to the element at given index
Node*find(int index){
    Node * tmp = first;
    while(tmp->index != index)
        tmp = tmp-> next;
        if(!tmp)
            return NULL;
    return tmp;
}

//method that calculates where is the father of a Node and returns a pointer to it
//@return pointer to the father
Node* Node::parent(){
    if(index == 0)
        return NULL;
    else
        return find((index-1)/2);
}

//method that calculates where is the left child of a Node and returns a pointer to it
//@return pointer to the left child
Node* Node::left(){
    int l = index*2+1;
    if(l<=last->index)
        return find(l);
    else
        return NULL;
}

//method that calculates where is the right child of a Node and returns a pointer to it
//@return pointer to the right child
Node* Node::right(){
    int r = index*2+2;
    if(r<=last->index)
        return find(r);
    else
        return NULL;
}

//because I wasn't allowed to use bulit-in functions I had to write this one for calculating power of a number
//@param base - base of the calculation
//@param exponent - exponent of the calculation
int power (int base, int exponent){
    int result = 1;
    for(int i = 0; i<exponent; i++){
        result *= base;
    }
    return result;
}

//function that checks if an element at given index is the las one at its level. Based on power calculations
//@return bool that tells you if it is the last element or not
bool isNewLevel(int num){
    if(num == 0)
        return true;
    int i = 1;
    int pow = 0;
    while(num>=pow){
        if(num == pow)
            return true;
        pow += power(2, i++);
    }
    return false;
}

//Function that swaps two values in the heap (fun fact: it swaps only values, not the whole nodes. The nodes stay at their place)
//@param i1 First node to swap from
//@param i2 Second node to swap from
void swap(Node*i1, Node*i2){
    Node*first = i1;
    Node*second = i2;
    int tmpVal = first->value;
    first->value = second->value;
    second->value = tmpVal;
}

//Function that adds new node with given value to the heap.
//First it adds it as the new last node and then puts it up until it's on the right place (its parent isn't smaller than it)
//@param val value of the element we want to add to the heap.
void add(int val){
    Node * toAdd = new Node(val);
    //adding the element to the end
    if(!first){
            toAdd->index = 0;
            first = toAdd;
            last = toAdd;
        } else {
            toAdd->index = last->index + 1;
            toAdd->prev = last;
            last->next = toAdd;
            last = toAdd;
        }
    
    //putting the element in the right place
    while(toAdd->parent()){
        if(toAdd->value > toAdd->parent()->value){
            swap(toAdd, toAdd->parent());
            toAdd = toAdd->parent();
        } else 
            break;
    }
}

//Function that visualizes the heap level by level
//This is where I needed to check if we are on the end of the level
void display(){
    Node * tmp = first;
    cout << "\nTree visualised:\n";
    out << "\nTree visualised:\n";
    while(tmp){
        cout << tmp->value;
        out << tmp->value;
        if(isNewLevel(tmp->index)){
            cout << "\n";
            out << "\n";
        }else if(tmp != last){
            cout << ", ";
            out << ", ";
        }
        tmp = tmp->next;
    }
    cout << "\n";
    out << "\n";
}

//Recursive function that displays all values in the tree "in order" (left, visit, right). 
//Displayed values are now sorted
//@param tmp - single node
void showTreeInOrder(Node * tmp){
    if(tmp->left() != NULL)
        showTreeInOrder(tmp->left());
    cout << tmp->value << " ";
    out << tmp->value << " ";
    if(tmp->right() != NULL)
        showTreeInOrder(tmp->right());
}

//Recursive function that displays all values in the tree "pre order" (visit, left, right).
//@param tmp - single node
void showTreePreOrder(Node * tmp){
    cout << tmp->value << " ";
    out << tmp->value << " ";
    if(tmp->left() != NULL)
        showTreePreOrder(tmp->left());
    if(tmp->right() != NULL)
        showTreePreOrder(tmp->right());
}

//Recursive function that displays all values in the tree "post order" (left, right, visit).
//@param tmp - single node
void showTreePostOrder(Node * tmp){
    if(tmp->left() != NULL)
        showTreePostOrder(tmp->left());
    if(tmp->right() != NULL)
        showTreePostOrder(tmp->right());
    cout << tmp->value << " ";
    out << tmp->value << " ";
}

//Function that finds a Node at given index, deletes it and fills the left space.
//@param i index of a Node we want to delete
void deleteNodeByIndex(int i){
    //finding the element
    Node * tmp = find(i);
    //swapping it with the last element in the array
    swap(tmp, last);
    //deleting the last element
    last = last->prev;
    delete last->next;
    last->next = NULL;

    //finding the right place for the swapped element
    while(tmp){
        int l, r;
        if(tmp->left())
            l = tmp->left()->value;
        else
            l = 0;
        if(tmp->right())
            r = tmp->right()->value;
        else
            r = 0;

        if(r > l){
            if(r > tmp->value){
                swap(tmp, tmp->right());
                tmp = tmp->right();
            } else break;
        }else{
            if(l > tmp->value){
                swap(tmp, tmp->left());
                tmp = tmp->left();
            } else break;
        }
    }
    cout << "successfully deleted node from index " << i << "\n";
    out << "successfully deleted node from index " << i << "\n";
}

//Function that finds a Node with given value, deletes it and fills the left space.
//@param i value of a Node we want to delete
void deleteNodeByValue(int val){
    //finding the element
    Node * tmp = first;
    while(tmp){
        if(tmp->value == val)
            break;
        tmp = tmp->next;
    }
    if(tmp){
        //swapping it with the last element in the array
        swap(tmp, last);
        //deleting the last element
        last = last->prev;
        delete last->next;
        last->next = NULL;

        //finding the right place for the swapped element
        while(true){
            if(!tmp)
                break;
            int l, r;
            if(tmp->left())
                l = tmp->left()->value;
            else
                l = 0;
            if(tmp->right())
                r = tmp->right()->value;
            else
                r = 0;

            if(r > l){
                if(r > tmp->value){
                    swap(tmp, tmp->right());
                    tmp = tmp->right();
                } else break;
            }else{
                if(l > tmp->value){
                    swap(tmp, tmp->left());
                    tmp = tmp->left();
                } else break;
            }
        }
        cout << "successfully deleted node storing value " << val << "\n";
        out << "successfully deleted node storing value " << val << "\n";
    } else {
        cout << "Value not found, cannot be deleted\n";
        out << "Value not found, cannot be deleted\n";
    }
}

//recursive function that ereases the whole tree form the computer's memory
//@param tmp - reference to pointer to the node we want to delete
void deleteTree(Node *& tmp){
    if(tmp->next)
        deleteTree(tmp->next);  
    delete tmp;
    tmp = NULL;
}

//Function that finds path to the element with given value.
//First it finds an element, then it goes up in "family tree" and pushes directions to the stack and at the end if pos every step needed to get to the element.
//@param val Value of the element we want to find.
void findPath(int val){
    Node * tmp = first;
    while(tmp){
        if(tmp->value == val)
            break;
        tmp = tmp->next;
    }

    if(tmp){
        Node * tmpCopy = tmp;
        while(tmp->parent()){
            //if to get to the child you need to go left - 'l' is pushed to the stack
            if(tmp->parent()->left() == tmp){
                push('l');
            //if you have to go right - 'r' is pushed
            } else {
                push('r');
            }
            tmp = tmp->parent();
        }
        cout << "path to " << val << " ('l' for left and 'r' for right):\n  ";
        out << "path to " << val << " ('l' for left and 'r' for right):\n  ";
        //emptying the stack
        char direction;
        while(direction=pop()){
            cout << direction << " -> ";
            out << direction << " -> ";
        }
        cout << "found " << val << " at index " << tmpCopy->index << " and position " << tmpCopy << "\n";
        out << "found " << val << " at index " << tmpCopy->index << " and position " << tmpCopy << "\n";
    } else {
        cout << "Value not found\n";
        out << "Value not found\n";
    }
}

int main(){
    ifstream in;
    in.open("we.txt");
    out.open("wy.txt");
    string num;
    while(in >> num){
        add(stoi(num));
    }

    display();
    //displaying values from the tree in three ways
    cout << "\nLVR: ";
    out << "\nLVR: ";
    showTreeInOrder(first);
    cout << "\nVLR: ";
    out << "\nVLR: ";
    showTreePreOrder(first);
    cout << "\nLRV: ";
    out << "\nLRV: ";
    showTreePostOrder(first);
    cout << "\n";
    out << "\n";

    deleteNodeByIndex(1);
    display();

    deleteNodeByValue(9);
    display();

    findPath(5);

    deleteTree(first);
    display();

    in.close();
    out.close();
}