#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Node{
    int value;
    int index;
    Node*next;
    Node*prev;
    
    Node(int v){
        value = v;
        next = prev = NULL;
    }
    Node* parent();
    Node* left();
    Node* right();
};

Node *first, *last;
ofstream out;

Node*find(int index){
    Node * tmp = first;
    while(tmp->index != index)
        tmp = tmp-> next;
        if(!tmp)
            return NULL;
    return tmp;
}

Node* Node::parent(){
    if(index == 0)
        return NULL;
    else
        return find((index-1)/2);
}

Node* Node::left(){
    int l = index*2+1;
    if(l<=last->index)
        return find(l);
    else
        return NULL;
}

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

void swap(Node*i1, Node*i2){
    Node*first = i1;
    Node*second = i2;
    int tmpVal = first->value;
    first->value = second->value;
    second->value = tmpVal;
}

void add(int val){
    Node * toAdd = new Node(val);
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
    
    while(toAdd->parent()){
        if(toAdd->value > toAdd->parent()->value){
            swap(toAdd, toAdd->parent());
            toAdd = toAdd->parent();
        } else 
            break;
    }
}

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

void deleteNodeByIndex(int i){
    Node * tmp = find(i);
    swap(tmp, last);
    last = last->prev;
    delete last->next;
    last->next = NULL;

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

void deleteNodeByValue(int val){
    Node * tmp = first;
    while(tmp){
        if(tmp->value == val)
            break;
        tmp = tmp->next;
    }
    if(tmp){
        swap(tmp, last);
        last = last->prev;
        delete last->next;
        last->next = NULL;

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
//@param tmp - reference to pointer we want to delete
void deleteTree(Node *& tmp){
    if(tmp->next)
        deleteTree(tmp->next);  
    delete tmp;
    tmp = NULL;
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

    deleteTree(first);
    display();

    in.close();
    out.close();
}