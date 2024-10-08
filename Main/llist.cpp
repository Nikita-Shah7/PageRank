// Linked list implementation

#include <iostream>
#include <string>
#include "llist.h"

using namespace std;

// Constructor - initialize front, rear to null and count to 0
// this does not create any node. new list is empty.
llist::llist() {
    Front = NULL;
    Rear = NULL;
    Count = 0;
} // end constructor

// PURPOSE : Removes all nodes of linked list to destruct it
// Destructor - while list not empty, call deleteFront to delete all nodes
llist::~llist() {
    int dummy; // take in what is deleted from the front
    while (Count != 0) {
        deleteFront(dummy);
    } // end while
} // end destructor

// PURPOSE : Determines if Linked List is empty
// Returns true if the following 3 conditions are met :
// front and rear both point to NULL, count is 0
bool llist::isEmpty() {
    // definition of empty: front, rear point null and count is 0
    if (Front == NULL && Rear == NULL && Count == 0)
        return true;
    return false;
} // end isEmpty

// PURPOSE : Displays all the elements in the linked list
// beginning from front
void llist::displayAll() {
    if (isEmpty()) // if empty, display [ empty ]
        cout << "[ empty ]" << endl;
    else { // loop until our pointer gets to the rear
        Node *box = Front; // create traversal node
        for (int i = 0; i < Count; i++) {
            cout << box->Elem << " ";
            box = box->Next;
        } // end for (i)
        cout << endl;
    } // end else
} // end displayAll

// PURPOSE : Prints out the elements for a csv file
string llist::csvPrint() {
    string output = "";
    if (isEmpty())
        return output;
    Node *box = Front;
    for (int i = 0; i < Count; i++) {
        output += to_string(box->Elem) + ",";
        box = box->Next;
    }
    return output;
}

// PURPOSE : Adds new element into sorted linked list
// PARAMETER : New value to be inserted
bool llist::add(el_t newNum) {
    // create new node to add; fill it as needed
    Node *x;
    x = new Node;
    x->Elem = newNum;
    x->Next = NULL;
    bool output = true;
if (isEmpty() || newNum < Front->Elem) {
    addFront(newNum);
    return true;
} else if (newNum > Rear->Elem) {
    addRear(newNum);
    return true;
} else {
    output = false;
    Node *N1, *N2;
    N1 = Front;
    N2 = N1->Next;
    while (N1 != NULL) {
        if (newNum == N1->Elem)
            return false;
        else if (newNum < N2->Elem && newNum > N1->Elem) {
            N1->Next = x;
            x->Next = N2;
            output = true;
            Count++;
            return true;
        } // end else if
        N1 = N2;
        N2 = N1->Next;
    } // end while
} // end else
return true;
}

// PURPOSE : Add an element to the front of the linked list
// PARAMETER : Element to be added
void llist::addFront(el_t newNum) {
    // create new node to add to the front; fill it as needed
    Node *x;
    x = new Node;
    x->Elem = newNum;
    x->Next = NULL;

    // if the linked list is empty, this will be our very first entry
    // thus, make the rear point to it
    if (isEmpty())
        Rear = x;
    // if the linked list has elements, make the new node
    // point to the front instead
    else
        x->Next = Front;

    // now make the front point to this new node to
    // finalize that it’s the front
    Front = x;
    // update counter to show we have 1 more element in linked list
    Count++;
}

// PURPOSE : Add an element to the rear of the linked list
// PARAMETER : Element to be added
void llist::addRear(el_t newNum) {
    // create new node to add to the rear; fill it as needed
    Node *x;
    x = new Node;
    x->Elem = newNum;
    x->Next = NULL;

    // if the linked list is empty, this will be our very first entry
    // thus, make the front point to it
    if (isEmpty())
        Front = x;
    // if the linked list has elements,
    // make the old rear point to this new node
    else
        Rear->Next = x;

    // now make the rear point to this new node
    // to finalize that it’s the rear
    Rear = x;
    // update the counter to show we have 1 more
    // element in the linked list
    Count++;
}

// PURPOSE : Inserts new node into linked list making
// it the new ith node
// PARAMETER : Element to be added
void llist::addbeforeIth(int i, el_t newNum) {
    // error case - i is an illegal value:
    // >Count +1 or <1 - throw outofrange
    if (i > Count + 1 || i < 1)
        throw OutOfRange();
    // special cases - call other functions we already made
    // for i=1 or i=Count + 1
    else if (i == 1)
        addFront(newNum);
    else if (i == Count + 1)
        addRear(newNum);
    // regular case
    // add right before ith node where i begins at 1 and update count
    else {
        Node *iNode, *priorNode, *newNode;
        // pointers that will hold i-1, i, i+1 entries after traversal
        newNode = new Node;
        priorNode = Front;
        for (int j = 1; j < i - 1; j++)
            priorNode = priorNode->Next; // obtain i-1 entry
        iNode = priorNode->Next;
// obtain i entry - will become i+1 entry after insertion
priorNode->Next = newNode; // place new node in i spot
newNode->Next = iNode;
// have old i entry follow this new node - making it the i+1
newNode->Elem = newNum;
// insert value into new ith entry of linked list
Count++;
} // end else: regular case
}

// PURPOSE : Deletes the entry in the front of the linked list
// PARAMETER : Holds the value that used to be in the front
// prior to deletion
void llist::deleteFront(el_t &oldNum) {
    if (isEmpty())
        throw Underflow();
    else if (Count == 1) {
        oldNum = Front->Elem;
        Front = NULL;
        Rear = NULL;
        Count = 0;
    } else {
        oldNum = Front->Elem;
        Node *newFront;
        newFront = Front->Next;
        delete Front;
        Front = newFront;
        Count--;
    }
}

// PURPOSE : Outputs the size of the linked list
int llist::getSize() {
    return Count;
}