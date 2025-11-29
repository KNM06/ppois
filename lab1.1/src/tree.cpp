#include "tree.h"

Tree::Tree(const string& english, const string& russian)
:english_(english), russian_(russian),left_(nullptr),right_(nullptr){}

Tree::Tree(const Tree& copiedTree)
:english_(copiedTree.english_), russian_(copiedTree.russian_),
left_(copiedTree.left_ ? new Tree(*copiedTree.left_) : nullptr),
right_(copiedTree.right_ ? new Tree(*copiedTree.right_) : nullptr){}

Tree::~Tree(){
    delete left_;
    delete right_;
}

string Tree::getEnglish() const{
    return english_;
}

string Tree::getRussian() const{
    return russian_;
}

Tree* Tree::getLeft() const{
    return left_;
}

Tree* Tree::getRight() const{
    return right_;
}

void Tree::setEnglish(const string& english){
    english_ = english;
}

void Tree::setRussian(const string& russian){
    russian_ = russian;
}

void Tree::setLeft(Tree* left){
    left_ = left;
}

void Tree::setRight(Tree* right){
    right_ = right;
}

bool Tree::addNodeTree(const string& english, const string& russian){
    if(english < getEnglish()){
        if(!getLeft()){
            Tree* left = new Tree(english,russian);
            setLeft(left);
            return true;
        } else {
            return getLeft()->addNodeTree(english,russian);
        }
    } else {
        if(!getRight()){
            Tree* right = new Tree(english, russian);
            setRight(right);
            return true;
        } else {
            return getRight()->addNodeTree(english,russian);
        }
    }
    return false;   
}

Tree* Tree::deleteNodeTree(const string& english){
    if(english<getEnglish()){
        if(getLeft()) setLeft(getLeft()->deleteNodeTree(english));
    } else {
        if(english>getEnglish()){
            if(getRight()) setRight(getRight()->deleteNodeTree(english));
        } else {
            if(!getLeft()){
                Tree* rightChild = getRight();
                setRight(nullptr);
                delete this;
                return rightChild;
            } else {
                if(!getRight()){
                Tree* leftChild = getLeft();
                setLeft(nullptr);
                delete this;
                return leftChild;
                } else {
                    Tree* minNode = getRight();
                    while(minNode->getLeft()) minNode = minNode->getLeft();
                    string en = minNode->getEnglish();
                    setEnglish(en);
                    string ru = minNode->getRussian();
                    setRussian(ru);
                    setRight(getRight()->deleteNodeTree(en));
                }
            }
        }
    }
    return this;
}
Tree* Tree::findNode(const string& english){
    if(getEnglish() == english) return this;
    if(english < getEnglish()) return getLeft() ? getLeft()->findNode(english) : nullptr;
    else return getRight() ? getRight()->findNode(english) : nullptr;
}

const Tree* Tree::findNode(const string& english) const {
    if(getEnglish() == english) return this;
    if(english < getEnglish()) return getLeft() ? getLeft()->findNode(english) : nullptr;
    else return getRight() ? getRight()->findNode(english) : nullptr;
}

int Tree::countNodeTree() const{
    int countLeft = getLeft() ? getLeft()->countNodeTree() : 0;
    int countRight = getRight() ? getRight()->countNodeTree() : 0;
    return 1 + countLeft + countRight;
}

void Tree::printTree(ostream& out) const {
    if(getLeft()) getLeft()->printTree(out);
    out << getEnglish() << " - " << getRussian() << "\n";
    if(getRight()) getRight()->printTree(out);

} 

bool Tree::operator==(const Tree& other) const{
    if(getRussian() != other.getRussian() || getEnglish() != other.getEnglish()) return false;
    bool leftTree = (getLeft() == nullptr && other.getLeft() == nullptr) ||
                    (getLeft() != nullptr && other.getLeft() != nullptr && *getLeft() == *other.getLeft());
    bool rightTree = (getRight() == nullptr && other.getRight() == nullptr) ||
                     (getRight() != nullptr && other.getRight() != nullptr && *getRight() == *other.getRight());
    return leftTree && rightTree;
}

bool Tree::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    saveToOut(file);
    file.close();
    return true;
}

void Tree::saveToOut(ostream& out) const {
    if (getLeft()) {
        getLeft()->saveToOut(out);
    }
    out << getEnglish() << " " << getRussian() << endl;
    if (getRight()) {
        getRight()->saveToOut(out);
    }
}
