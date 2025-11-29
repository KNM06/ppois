#include "vocabulary.h"
#include "tree.h"

Vocabulary::Vocabulary() : root_(nullptr){}

Vocabulary::Vocabulary(const Vocabulary& copiedVocabulary) 
    : root_(copiedVocabulary.root_ ? new Tree(*copiedVocabulary.root_) : nullptr) {}

Vocabulary::~Vocabulary(){
    delete root_;
}

Tree* Vocabulary::getRoot() const {
    return root_;
}
 
void Vocabulary::setRoot(Tree* root){
    root_ = root;
}

Vocabulary& Vocabulary::operator=(const Vocabulary& other){
    if(this != &other) {
        delete getRoot();
        setRoot(other.getRoot() ? new Tree(*other.getRoot()) : nullptr);
    }
    return *this;
}

Vocabulary& Vocabulary::operator+=(const pair<string, string>& pairWords){
    if(!getRoot()) setRoot(new Tree(pairWords.first, pairWords.second));
    else getRoot()->addNodeTree(pairWords.first, pairWords.second);
    return *this;
}

Vocabulary& Vocabulary::operator-=(const string& english){ 
    if(getRoot()) setRoot(getRoot()->deleteNodeTree(english));
    return *this;
}

string Vocabulary::operator[](const string& english) const{
    if(getRoot()) {
        Tree* foundNode=getRoot()->findNode(english);
        if(foundNode) return foundNode->getRussian();
    }
    return "";
}

string& Vocabulary::operator[](const string& english) {
   Tree* foundNode = getRoot() ? getRoot()->findNode(english) : nullptr;
   if(!foundNode) {
        *this += {english, ""};
        foundNode = getRoot()->findNode(english);
   }
   string russian = foundNode->getRussian();
   string* word =&russian;
   return *word;
}

bool Vocabulary::operator==(const Vocabulary& other) const{
    if (getRoot() == nullptr && other.getRoot() == nullptr) return true;
    if (getRoot() == nullptr || other.getRoot() == nullptr) return false;
    return *getRoot() == *other.getRoot();  
}

bool Vocabulary::operator!=(const Vocabulary& other) const{
        return !(*this == other);
}

ostream& operator<<(ostream& out, const Vocabulary& vocabulary) {
    vocabulary.printVocabulary(out);
    return out;
}

istream& operator>>(istream& in, Vocabulary& vocabulary) {
    string english, russian;
    while (in >> english >> russian) {
        vocabulary += make_pair(english, russian);
    }
    return in;
}

void Vocabulary::printVocabulary(ostream& out) const{
    out << "Англо-русский словарь:\n";
    if (getRoot()) {
        getRoot()->printTree(out);
    } else {
        out << "Словарь пуст\n";
    }
}

bool Vocabulary::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    delete getRoot();
    setRoot(nullptr);
    
    string english, russian;
    while (file >> english >> russian) {
        operator+=(make_pair(english, russian));
    }
    
    file.close();
    return true;
}

bool Vocabulary::saveToFile(const string& filename) const {
    if (!getRoot()) {
        return false;
    }
    return getRoot()->saveToFile(filename);
}
