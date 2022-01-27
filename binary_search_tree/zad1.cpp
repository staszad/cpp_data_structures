#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*
This is my implementation of the binary search tree in c++
Using written here functions you can:
    -add and delete nodes to the tree
    -display values from the tree in three ways
    -erease the whole tree from memory
    -find path to the specified node
    -display the tree level by level, line by line
*/

//structure of a single node of a binary search tree
struct Node{
    int value;
    bool farRight; // bool that tells you if the node is rightmost at its level
    Node *left; //smaller numbers
    Node *right; //bigger numbers
    Node *parent;

    //constructor
    //@param v - value of the node
    Node(int v){ 
        farRight = false;
        value = v;
        left = right = parent = NULL;
    }
};

//global variables
Node * root = NULL; //root of the bst 
bool endReached = false; //bool that tells if the end of the tree was reached while performing some operations
ofstream out; //output file

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

//function that adds value to the tree
//@param v - value we want to add
void addToTree(int v){
    Node *tmp = new Node(v);
    //in case no element is located in the tree
    if(root == NULL){
        tmp->farRight = true;
        root = tmp;
    }

    //the loop sends bigger elements to the right and smaller to the left, until it finds some free space
    else{
        Node * current = root;
        while(true){
            if(tmp->value >= current->value){
                if(current->right == NULL){
                    //adding node at specific place to the tree
                    if(current->farRight)   //"inheritance" of farRight property
                        tmp->farRight = true;
                    current->right = tmp;
                    tmp->parent = current;
                    break;
                }
                else
                    current = current->right;
            }
            if(tmp->value < current->value){
                if(current->left == NULL){
                    //adding node at specific place to the tree
                    current->left = tmp;
                    tmp->parent = current;
                    break;
                }
                else
                    current = current->left;
            } 
        }
    }
}

//Function that checks if there are any elemnts at specified level in the tree. 
//This function is based on recursion (so it can chack EVERY node)
//@param cur - current level
//@param dst - the destination, level we want to check
//@param tmp - single node
bool isEndReached(int cur, int dst, Node * tmp){
    if(cur < dst){
        bool l, r;
        l = r = true;
        if(tmp->left){
            l = isEndReached(cur+1, dst, tmp->left);
        }
        if(tmp->right){
            r = isEndReached(cur+1, dst, tmp->right);
        }
        return (l && r);
    }
    else
        return false;
}

//Function that finds and deletes one node specified by value.
//@param val - value based on which the node will be found
void deleteNode(int val){
    Node * tmp = root;
    int i = 0;

    //searching loop - it makes comparisons and finds path to the element 
    while(true){
        char direction; //variable needed for deleting the parent's pointer to deleted node
        if(tmp->value > val){
            tmp = tmp->left;
            direction = 'l';
        }
        if(tmp->value < val){
            tmp = tmp->right;
            direction = 'r';
        }
        if(tmp->value == val){
            if(tmp->parent){
                if(direction == 'l'){
                    delete tmp->parent->left;
                    tmp->parent->left = NULL;
                }
                if(direction == 'r'){
                    delete tmp->parent->right;
                    tmp->parent->right = NULL;
                }
            }else{
                tmp = NULL;
            }
            break;
        }
        if(isEndReached(0, i, root)){
            break;
        }
        i++;
    }
}

//Recursive function that displays all values in the tree "in order" (left, visit, right). 
//Displayed values are now sorted
//@param tmp - single node
void showTreeInOrder(Node * tmp){
    if(tmp->left != NULL)
        showTreeInOrder(tmp->left);
    cout << tmp->value << " ";
    out << tmp->value << " ";
    if(tmp->right != NULL)
        showTreeInOrder(tmp->right);
}

//Recursive function that displays all values in the tree "pre order" (visit, left, right).
//@param tmp - single node
void showTreePreOrder(Node * tmp){
    cout << tmp->value << " ";
    out << tmp->value << " ";
    if(tmp->left != NULL)
        showTreePreOrder(tmp->left);
    if(tmp->right != NULL)
        showTreePreOrder(tmp->right);
}

//Recursive function that displays all values in the tree "post order" (left, right, visit).
//@param tmp - single node
void showTreePostOrder(Node * tmp){
    if(tmp->left != NULL)
        showTreePostOrder(tmp->left);
    if(tmp->right != NULL)
        showTreePostOrder(tmp->right);
    cout << tmp->value << " ";
    out << tmp->value << " ";
}

//Function, based on recursion, meant to display all values in one level of the tree
//@param cur - current level
//@param dst - the destination, level we want to display
//@param tmp - single node
void displayLevel(int cur, int dst, Node * tmp){
    if(!isEndReached(0, dst, root)){
        //if we are on the wrong level the function is called one level lower twice (one for left and one for right child)
        if(cur < dst){
            if(tmp->left){
                displayLevel(cur+1, dst, tmp->left);
            } else{
                //if there are no more nodes and the destination level isn't reached this loop displays right number of free spaces
                for(int i = 0; i< power(2,(dst-cur-1)); i++){
                   cout << " ,"; 
                   out << " ,"; 
                }
            }
            if(tmp->right){
                displayLevel(cur+1, dst, tmp->right);
            } else {
                if(tmp->farRight){
                    //same as above, but if the element is the rightmost we want to display one comma less (for aesthetic purposes) 
                    for(int i = 1; i< power(2,(dst-cur-1)); i++){
                        cout << " ,"; 
                        out << " ,"; 
                    }
                } else {
                    for(int i = 0; i< power(2,(dst-cur-1)); i++){
                        cout << " ,"; 
                        out << " ,"; 
                    }
                }
            }
            return;
        }

        //when we're on the right level we display the value and a comma (except farRight case)
        else{
            cout << tmp->value;
            out << tmp->value;
            if(!tmp->farRight){ 
                cout << ",";
                out << ",";
            }
            endReached = false;
            return;
        }
    }
    return;
}

//function that displays whole structure of the tree, level by level
void drawTree(){
    endReached = false;
    int i = 0;
    cout << "\nTree visualised:\n";
    out << "\nTree visualised:\n";
    while (!endReached){
        endReached = true;
        displayLevel(0, i, root);
        i++;
        cout << "\n";
        out << "\n";
    }
}

//function that checks if the specified value has its place in the tree
//@param val - value we want to find
bool isInTree(int val){
    Node * tmp = root;
    int i = 0;
    //searching process is similar to the one in deleteNode() function
    while(true){
        if(tmp->value > val){
            if(tmp->left)
                tmp = tmp->left;
            else
                return false;
        }
        if(tmp->value < val){
            if(tmp->right)
                tmp = tmp->right;
            else
                return false;
        }
        if(tmp->value == val){
            return true;
        }
        if(isEndReached(0, i, root)){
            return false;
        }
        i++;
    }
}

//function that finds and displays path to the specified value and address of its node
//@param val - value we want to find the path to
void findInTree(int val){
    if(isInTree(val)){
        cout << "path to " << val << " ('l' for left and 'r' for right):\n  ";
        out << "path to " << val << " ('l' for left and 'r' for right):\n  ";
        Node * tmp = root;
        int i = 0;
        //similar searching process as above - comparisons
        while(true){
            if(tmp->value > val){
                tmp = tmp->left;
                cout << "l -> ";
                out << "l -> ";
            }
            if(tmp->value < val){
                tmp = tmp->right;
                cout << "r -> ";
                out << "r -> ";
            }
            if(tmp->value == val){
                cout << "found " << tmp->value << " at position " << tmp << "\n";
                out << "found " << tmp->value << " at position " << tmp << "\n";
                break;
            }
            if(isEndReached(0, i, root)){
                break;
            }
            i++;
        }
    } else {
        cout << val << " is not located in the tree\n";
        out << val << " is not located in the tree\n";
    }
}

//recursive function that ereases the whole tree form the computer's memory
//@param tmp - reference to pointer we want to delete
void deleteTree(Node *& tmp){
    if(tmp->left)
        deleteTree(tmp->left);  
    if(tmp->right)
        deleteTree(tmp->right);
    delete tmp;
    tmp = NULL;
}

int main(int argc, char const *argv[]){
    //creating variables, opening files  
    ifstream in;
    in.open("we.txt");
    out.open("wy.txt");
    string toAdd;
    int toAddVal;

    //adding values to the tree
    while(in >> toAdd){
        toAddVal = stoi(toAdd);
        addToTree(toAddVal);
    }
    /*cout << "How many numbers you want to add to the tree? ";
    string size;
    cin >> size;
    for(int i=0; i<stoi(size); i++){
        cin >> toAdd;
        toAddVal = stoi(toAdd);
        addToTree(toAddVal);
    }*/

    //displaying values from the tree in three ways
    cout << "LVR: ";
    out << "LVR: ";
    showTreeInOrder(root);
    cout << "\nVLR: ";
    out << "\nVLR: ";
    showTreePreOrder(root);
    cout << "\nLRV: ";
    out << "\nLRV: ";
    showTreePostOrder(root);
    cout << "\n";
    out << "\n";

    //presenting results of other functions
    drawTree();
    findInTree(6);
    deleteNode(-5);
    cout << "\nAfter delition of node:\n";
    out << "\nAfter delition of node:\n";
    drawTree();
    deleteTree(root);
    return 0;
}