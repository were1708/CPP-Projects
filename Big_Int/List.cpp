// Joshua Revilla
// jtrevill
// pa6

#include "List.h"
#include <iostream>
#include <string>

// private constructor------------------------

List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Class Constructors & Destructors-----------

// Create new empty list
// we're gonna start the cursor at position 0
List::List() {
    Node *front = new Node(-1);
    frontDummy = front;
    Node *back = new Node(-1);
    backDummy = back;
    pos_cursor = 0;
    num_elements = 0;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
}

// Copy Function
List::List(const List &L) {

    Node *front = new Node(-1);
    frontDummy = front;
    Node *back = new Node(-1);
    backDummy = back;
    pos_cursor = 0;
    num_elements = 0;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    Node *N = L.frontDummy->next;
    while (N != L.backDummy) {
        insertBefore(N->data);
        N = N->next;
    }
    num_elements = L.num_elements;
    moveFront();
    while (pos_cursor != L.pos_cursor) { // we need to copy the cursor position
        moveNext();
    }
}

// Destructor
List::~List() {
    moveFront();
    while (num_elements != 0) {
        eraseAfter();
    }
    delete frontDummy;
    delete backDummy;
}

// Access Functions----------------------------

int List::length() const {
    return num_elements;
}

ListElement List::front() const {
    if (num_elements == 0) {
        throw std::length_error("List: front(): empty list");
    }
    return frontDummy->next->data;
}

ListElement List::back() const {
    if (num_elements == 0) {
        throw std::length_error("List: back(): empty list");
    }
    return backDummy->prev->data;
}

int List::position() const {
    return pos_cursor;
}

ListElement List::peekNext() const {
    if (pos_cursor == num_elements) {
        throw std::range_error("List: peekNext(): cursor at back");
    }
    return afterCursor->data;
}

ListElement List::peekPrev() const {
    if (pos_cursor == 0) {
        throw std::range_error("List: peekPrev(): cursor at front");
    }
    return beforeCursor->data;
}

// Manipulation Functions--------------------------

void List::clear() {
    moveFront();
    while (num_elements != 0) {
        eraseAfter();
    }
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    return;
}

void List::moveFront() {
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    return;
}

void List::moveBack() {
    pos_cursor = num_elements;
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    return;
}

ListElement List::moveNext() {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: moveNext(): cursor at back");
    }
    pos_cursor++;
    afterCursor = afterCursor->next;
    beforeCursor = beforeCursor->next;
    return beforeCursor->data;
}

ListElement List::movePrev() {
    if (pos_cursor <= 0) {
        throw std::range_error("List: movePrev(): cursor at front");
    }
    pos_cursor--;
    beforeCursor = beforeCursor->prev; // we can assume this is not NULL!
    afterCursor = afterCursor->prev;
    return afterCursor->data;
}

void List::insertAfter(ListElement x) { // we do not need to change the cursor position
    Node *new_node = new Node(x);
    beforeCursor->next = new_node;
    afterCursor->prev = new_node;
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;
    afterCursor = new_node;
    num_elements++;
    return;
}

void List::insertBefore(ListElement x) { // we are going to need to change the cursor position
    Node *new_node = new Node(x);
    beforeCursor->next = new_node;
    afterCursor->prev = new_node;
    new_node->next = afterCursor;
    new_node->prev = beforeCursor;
    beforeCursor = new_node;
    pos_cursor++;
    num_elements++;

    return;
}

void List::setAfter(ListElement x) {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: setAfter(): cursor at back");
    }
    afterCursor->data = x;
    return;
}

void List::setBefore(ListElement x) {
    if (pos_cursor <= 0) {
        throw std::range_error("List: setBefore(): cursor at front");
    }
    beforeCursor->data = x;
    return;
}

void List::eraseAfter() {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
    beforeCursor->next = afterCursor->next;
    afterCursor->next->prev = beforeCursor;
    delete afterCursor;
    afterCursor = beforeCursor->next;
    num_elements--;
    return;
}

void List::eraseBefore() {
    if (pos_cursor <= 0) {
        throw std::range_error("List: eraseBefore(): cursor at front");
    }
    afterCursor->prev = beforeCursor->prev;
    beforeCursor->prev->next = afterCursor;
    delete beforeCursor;
    beforeCursor = afterCursor->prev;
    num_elements--;
    pos_cursor--;
    return;
}

// Other Functions--------------------

int List::findNext(ListElement x) {
    while (pos_cursor != num_elements) {
        if (this->moveNext() == x) {
            return pos_cursor;
        }
    }
    return -1;
}

int List::findPrev(ListElement x) {
    while (pos_cursor != 0) {
        if (movePrev() == x) {
            return pos_cursor;
        }
    }
    return -1;
}

void List::cleanup() {
    Node *J = frontDummy->next;
    Node *R = J;
    Node *temp = nullptr;
    int posJ = 0;
    int posR = 0;
    bool deleted = false;
    while (J != backDummy->prev && J) { // if at backDummy->prev there are no repeats
        // std::cout << R->data;
        // std::cout << "\n";
        R = J->next;
        posR = posJ + 1;
        while (R != backDummy && R) {
            deleted = false;
            if (J->data == R->data) {
                if (R == beforeCursor) {
                    beforeCursor = R->prev;
                    pos_cursor--;
                } else if (R == afterCursor) {
                    afterCursor = R->next;
                } else if (posR < pos_cursor) {
                    pos_cursor--;
                }

                R->prev->next = R->next;
                R->next->prev = R->prev;
                temp = R->next;
                delete R;
                deleted = true;
                R = temp;
                num_elements--;
            }
            if (!deleted) {
                R = R->next;
            }
            posR += 1;
        }
        J = J->next;
        posJ += 1;
    }

    return;
}

List List::concat(const List &L) const {
    List returnList;
    Node *J = this->frontDummy->next; // points to first node;
    Node *R = L.frontDummy->next; // points to first node;
    while (J != this->backDummy) {
        returnList.insertBefore(J->data);
        J = J->next;
    }

    while (R != L.backDummy) {
        returnList.insertBefore(R->data);
        R = R->next;
    }
    returnList.moveFront();
    return returnList;
}

std::string List::to_string() const {
    Node *N = this->frontDummy->next;
    std::string s = "(";
    while (N != this->backDummy) {
        if (N->next == backDummy) { // last element
            s += std::to_string(N->data);
            break;
        }
        s += std::to_string(N->data) + ", ";
        N = N->next;
    }
    s += ")";
    return s;
}

bool List::equals(const List &R) const {
    if (this->num_elements != R.num_elements) {
        return false;
    }
    Node *J = this->frontDummy->next;
    Node *K = R.frontDummy->next;
    while (J != this->backDummy || K != R.backDummy) {
        if (J->data != K->data) {
            return false;
        }
        J = J->next;
        K = K->next;
    }
    return true;
}

// Overridden Operators----------------

std::ostream &operator<<(std::ostream &stream, const List &L) {
    return stream << L.List::to_string();
}

bool operator==(const List &A, const List &B) {
    return A.List::equals(B);
}

List &List::operator=(const List &L) {
    if (this != &L) { // not self assignment
        List temp = L;
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }
    return *this;
}
