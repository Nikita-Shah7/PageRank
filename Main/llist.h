#pragma once  // Prevents multiple inclusions

// linked list header file

// ----- Globally setting up the alias and struct ----------------
typedef int el_t; // elements will be integers

struct Node {
    el_t Elem; // elem is the element stored
    Node *Next; // next is the pointer to the next node
};
// ---------------------------------------------------------

class llist {
private:
    Node *Front; // pointer to the front node
    Node *Rear; // pointer to the rear node
    int Count; // counter for the number of nodes

public:
    // Exception handling classes
    class Underflow {};
    class OutOfRange {}; // when the specified Node is out of range

    llist(); // empty constructor to create a list object
    ~llist(); // destructor to destroy all nodes

    // PURPOSE : Determines if the linked list is empty
    bool isEmpty();

    // PURPOSE : Prints all elements in linked list beginning from
    // front node all the way until to the rear node
    void displayAll();

    // PURPOSE : Prints out the linked list elements for a csv file
    std::string csvPrint();

    // PURPOSE : Adds new element into sorted linked list
    // PARAMETER : New value to be inserted
    bool add(el_t);

    // PURPOSE : Inserts new element into the front of linked list
    // PARAMETER : New value to be inserted
    void addFront(el_t);

    // PURPOSE : Inserts new element into the rear of linked list
    // PARAMETER : New value to be inserted
    void addRear(el_t);

    // PURPOSE : Inserts new element into the linked list,
    // making the new ith entry
    // PARAMETER : New value to be inserted
    void addbeforeIth(int, el_t);

    // PURPOSE : Deletes the entry in the front of the linked list
    // PARAMETER : Holds the value that used to be in the front
    // prior to deletion
    void deleteFront(el_t &oldNum);

    // PURPOSE : Outputs the size of the linked list
    int getSize();
};