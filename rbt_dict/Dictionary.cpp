// Joshua Revilla
// jtrevill
// pa8

#include "Dictionary.h"

#define black 0
#define red   1

// Private Constructor---------------------------

Dictionary::Node::Node(keyType k, valType v) {
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    color = black;
}

// HELPER NODE FUNCTIONS-----------------

void Dictionary::inOrderString(std::string &s, Node *R) const {
    if (R && R != nil) {
        inOrderString(s, R->left);
        s += R->key;
        s += " : ";
        s += std::to_string(R->val);
        s += "\n";
        inOrderString(s, R->right);
    }
    return;
}

void Dictionary::preOrderString(std::string &s, Node *R) const {
    if (R && R != nil) {
        s += R->key;
        s += "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
    return;
}

void Dictionary::preOrderCopy(Node *R, Node *N) {
    if (R && R != N) {
        this->setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
    return;
}

void Dictionary::postOrderDelete(Node *R) {
    if (R && R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
    return;
}

void Dictionary::attach(Node *N) {
    N->left = nil;
    N->right = nil;
    N->parent = nil;
}

Dictionary::Node *Dictionary::search(Node *R, keyType k) const {
    if (R->key == k || R == nil) {
        return R;
    } else if (R->key > k) {
        return search(R->left, k);
    } else {
        return search(R->right, k);
    }
}

Dictionary::Node *Dictionary::findMin(Node *R) {
    while (R->left != nil) {
        R = R->left;
    }
    return R;
}

Dictionary::Node *Dictionary::findMax(Node *R) {
    while (R->right != nil) {
        R = R->right;
    }
    return R;
}

Dictionary::Node *Dictionary::findNext(Node *N) {
    if (N == nil) {
        return nil;
    }
    if (N->right != nil) {
        return findMin(N->right);
    }
    Node *Y = N->parent;
    while (Y != nil && N == Y->right) {
        N = Y;
        Y = Y->parent;
    }
    return Y;
}

Dictionary::Node *Dictionary::findPrev(Node *N) {
    if (N == nil) {
        return nil;
    }
    if (N->left != nil) {
        return findMax(N->left);
    }
    Node *Y = N->parent;
    while (Y != nil && N == Y->left) {
        N = Y;
        Y = Y->parent;
    }
    return Y;
}

// RBT Helper Functions-------------------------

void Dictionary::LeftRotate(Node *N) {
    Node *Y = N->right;

    N->right = Y->left;
    if (Y->left != nil) {
        Y->left->parent = N;
    }

    Y->parent = N->parent;
    if (N->parent == nil) {
        root = Y;
    } else if (N == N->parent->left) {
        N->parent->left = Y;
    } else {
        N->parent->right = Y;
    }

    Y->left = N;
    N->parent = Y;
    return;
}

void Dictionary::RightRotate(Node *N) {
    Node *Y = N->left;

    N->left = Y->right;
    if (Y->right != nil) {
        Y->right->parent = N;
    }

    Y->parent = N->parent;
    if (N->parent == nil) {
        root = Y;
    } else if (N == N->parent->right) {
        N->parent->right = Y;
    } else {
        N->parent->left = Y;
    }

    Y->right = N;
    N->parent = Y;
    return;
}

void Dictionary::RB_InsertFixUp(Node *N) {
    while (N->parent->color == red) {
        if (N->parent == N->parent->parent->left) {
            Node *Y = N->parent->parent->right;
            if (Y->color == red) {
                N->parent->color = black;
                Y->color = black;
                N->parent->parent->color = red;
                N = N->parent->parent;
            } else {
                if (N == N->parent->right) {
                    N = N->parent;
                    LeftRotate(N);
                }
                N->parent->color = black;
                N->parent->parent->color = red;
                RightRotate(N->parent->parent);
            }
        } else {
            Node *Y = N->parent->parent->left;
            if (Y->color == red) {
                N->parent->color = black;
                Y->color = black;
                N->parent->parent->color = red;
                N = N->parent->parent;
            } else {
                if (N == N->parent->left) {
                    N = N->parent;
                    RightRotate(N);
                }
                N->parent->color = black;
                N->parent->parent->color = red;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = black;
    return;
}

void Dictionary::RB_Transplant(Node *u, Node *v) {
    if (u->parent == nil) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
    return;
}

void Dictionary::RB_DeleteFixUp(Node *N) {
    while (N != root && N->color == black) {
        if (N == N->parent->left) {
            Node *w = N->parent->right;
            if (w->color == red) {
                w->color = black;
                N->parent->color = red;
                LeftRotate(N->parent);
                w = N->parent->right;
            }
            if (w->left->color == black && w->right->color == black) {
                w->color = red;
                N = N->parent;
            } else {
                if (w->right->color == black) {
                    w->left->color = black;
                    w->color = red;
                    RightRotate(w);
                    w = N->parent->right;
                }
                w->color = N->parent->color;
                N->parent->color = black;
                w->right->color = black;
                LeftRotate(N->parent);
                N = root;
            }
        } else {
            Node *w = N->parent->left;
            if (w->color == red) {
                w->color = black;
                N->parent->color = red;
                RightRotate(N->parent);
                w = N->parent->left;
            }
            if (w->right->color == black && w->left->color == black) {
                w->color = red;
                N = N->parent;
            } else {
                if (w->left->color == black) {
                    w->right->color = black;
                    w->color = red;
                    LeftRotate(w);
                    w = N->parent->left;
                }
                w->color = N->parent->color;
                N->parent->color = black;
                w->left->color = black;
                RightRotate(N->parent);
                N = root;
            }
        }
    }
    N->color = black;
    return;
}

void Dictionary::RB_Delete(Node *N) {
    Node *Y = N;
    Node *X;
    int Y_original_color = Y->color;
    if (N->left == nil) {
        X = N->right;
        RB_Transplant(N, N->right);
    } else if (N->right == nil) {
        X = N->left;
        RB_Transplant(N, N->left);
    } else {
        Y = findMin(N->right);
        Y_original_color = Y->color;
        X = Y->right;
        if (Y->parent == N) {
            X->parent = Y;
        } else {
            RB_Transplant(Y, Y->right);
            Y->right = N->right;
            Y->right->parent = Y;
        }
        RB_Transplant(N, Y);
        Y->left = N->left;
        Y->left->parent = Y;
        Y->color = N->color;
    }
    if (Y_original_color == black) {
        RB_DeleteFixUp(X);
    }
    return;
}

// Dictionary Constructors---------------------------

Dictionary::Dictionary() {
    nil = new Node("NIL", -1);
    root = nil;
    current = nil;
    num_pairs = 0;
    attach(nil);
}

Dictionary::Dictionary(const Dictionary &D) {
    nil = new Node("NIL", -1);
    root = nil;
    current = nil;
    this->preOrderCopy(D.root, D.nil);
    this->num_pairs = D.num_pairs;
}

Dictionary::~Dictionary() {
    current = nullptr;
    postOrderDelete(root);
    delete nil;
}

// Access Functions----------------

int Dictionary::size() const {
    return num_pairs;
}

bool Dictionary::contains(keyType k) const {
    if (search(root, k) != nil) {
        return true;
    }
    return false;
}

valType &Dictionary::getValue(keyType k) const {
    Node *x = search(root, k);
    if (x == nil) {
        std::string error = "Dictionary: getValue(): key \"" + k + "\" does not exist";
        throw std::logic_error(error);
    }
    return x->val;
}

bool Dictionary::hasCurrent() const {
    if (current != nil) {
        return true;
    }
    return false;
}

keyType Dictionary::currentKey() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentKey(): cursor undefined");
    }
    return current->key;
}

valType &Dictionary::currentVal() const {
    if (!hasCurrent()) {
        throw std::logic_error("Dictionary: currentVal(): cursor undefined");
    }
    return current->val;
}

// Manipulation Procedures-------------------------

void Dictionary::clear() {
    postOrderDelete(root);
    num_pairs = 0;
    current = nil;
    root = nil;
    return;
}

void Dictionary::setValue(keyType k, valType v) {
    Node *y = nil;
    Node *x = root;
    while (x != nil) {
        y = x;
        if (k < x->key) {
            x = x->left;
        } else if (k > x->key) {
            x = x->right;
        } else {
            x->val = v;
            return;
        }
    }
    // Key was not in Dictionary
    Node *N = new Node(k, v);
    attach(N);
    num_pairs++;
    if (y == nil) { // tree was empty
        root = N;
    } else if (N->key < y->key) {
        y->left = N;
        N->parent = y;
    } else {
        y->right = N;
        N->parent = y;
    }
    N->color = red;
    RB_InsertFixUp(N);
    return;
}

void Dictionary::remove(keyType k) {
    Node *N = search(root, k);
    if (current == N) {
        current = nil;
    }
    if (N == nil) {
        std::string error = "Dictionary: remove(): key \"" + k + "\" does not exist";
        throw std::logic_error(error);
    }
    num_pairs--;
    RB_Delete(N);
    delete N;
}

void Dictionary::begin() {
    if (num_pairs == 0) {
        return;
    }
    current = findMin(root);
    return;
}

void Dictionary::end() {
    if (num_pairs == 0) {
        return;
    }
    current = findMax(root);
    return;
}

void Dictionary::next() {
    if (current == nil) {
        throw std::logic_error("Dictionary: next(): current undefined");
    }
    if (current == findMax(root)) {
        current = nil;
        return;
    }
    current = findNext(current);
    return;
}

void Dictionary::prev() {
    if (current == nil) {
        throw std::logic_error("Dictionary: prev(): current undefined");
    }
    if (current == findMin(root)) {
        current = nil;
        return;
    }
    current = findPrev(current);
    return;
}

// Other Functions-----------------------

std::string Dictionary::to_string() const {
    std::string returnString = "";
    inOrderString(returnString, root);
    return returnString;
}

std::string Dictionary::pre_string() const {
    std::string returnString = "";
    preOrderString(returnString, root);
    return returnString;
}

bool Dictionary::equals(const Dictionary &D) const {
    std::string compareString1 = this->pre_string();
    std::string compareString2 = D.pre_string();
    std::string compareString3 = this->to_string();
    std::string compareString4 = D.to_string();
    return ((compareString1 == compareString2) && (compareString3 == compareString4));
}

// Overloaded Operators------------------

std::ostream &operator<<(std::ostream &stream, Dictionary &D) {
    return stream << D.Dictionary::to_string();
}

bool operator==(const Dictionary &A, const Dictionary &B) {
    return A.equals(B);
}

Dictionary &Dictionary::operator=(const Dictionary &D) {
    if (this != &D) { // not self assignment
        Dictionary temp = Dictionary(D);
        this->preOrderCopy(temp.root, temp.nil);
    }
    return *this;
}
