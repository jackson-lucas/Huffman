#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <queue>

// Disciplina: Projeto e Análise de Algoritmo
// Feito por: Jackson Lima - 21351221

using namespace std;

class Column  {
  public:
    string code;
    char symbol;
    Column(char symbol, string code);
};

Column::Column(char symbol, string code) {
  this->symbol = symbol;
  this->code = code;
}

class Node {
  public:
    int frequency;
    bool hasSymbol;
    char symbol;
    vector<Node> adjacents;
    vector<Column> codes;
    string compactedData;
    Node(int frequency, char symbol);
    Node(char symbol);
    Node();
    Node(int frequency, vector<Node> adjacents);
    void print();
    void getTreeFromFile();
    void getCodesFromFile();
    //void printOne();
    //void plusOneFrequency();
    //vector<Column> getCodes(string code);
   //void printCodes();
   //string treeAsString();
   //string getCodeBySymbol(char symbol);
   //void generateCode(vector<char> data);
   //void printCompactedData();
};

Node::Node(int frequency, char symbol) {
  this->frequency = frequency;
  this->hasSymbol = true;
  this->symbol = symbol;
}

Node::Node(char symbol) {
  this->symbol = symbol;
  this->hasSymbol = true;
}

Node::Node() {
  this->hasSymbol = false;
}

Node::Node(int frequency, vector<Node> adjacents) {
  this->frequency = frequency;
  this->hasSymbol = false;
  this->adjacents = adjacents;
}

void Node::print() {
  cout << " ( ";
  // the lesser to the left
  if(adjacents.size() > 0) {
    adjacents[0].print();
  }

  cout << symbol << ": " << frequency;

  if(adjacents.size() > 1) {
    adjacents[1].print();
  }
  cout << " ) ";
}

void Node::getCodesFromFile() {
  std::ifstream infile("code", std::ios::binary);
  vector<Column> codes;

  std::vector<char> data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  string treeData(data.begin(), data.end());

  vector<string> textCode;
  int nextIndex;

  while((nextIndex = treeData.find('\n'))) {
    textCode.push_back(treeData.substr(0, nextIndex));
    treeData = treeData.substr(nextIndex+1);
  }
  //printVector(textCode);

  for(int index = 0; index<textCode.size(); index++) {
    char symbol = textCode[index][0];
    string code = textCode[index].substr(2, textCode[index].size()-2);
    //cout << code << "<CODE" <<  endl;
    codes.push_back(Column (symbol, code));
  }
}

class Tree {
  public:
    vector<char> data;
    vector<Node> nodes;
    vector<Node> prefixNodes;
    vector<Node> infixNodes;
    string descompressedData;
    Tree();
    void descompress();
    void getTreeFromFile();
    void printNodes();
};

int getIndexByNode(vector<Node> nodes, Node node) {
  for(int index=0; index<nodes.size(); index++) {
    if(nodes[index].symbol == node.symbol) {

    }
  }
}

vector<Node> getLeftAndRightFromInfix(vector<Node> infix, Node root) {
  vector<Node> infixLefRight = vector<Node>(2);

  int rootPosition = infix.find(root);

  infixLefRight[0] = infix.substr(0, rootPosition);
  infixLefRight[1] = infix.substr(rootPosition+1);

  return infixLefRight;
}

vector<Node> Tree::getTree() {
  vector<Node> leftRight;
  vector<Node> left = vector<Node>(3);
  vector<Node> right = vector<Node>(3);
  Node root;
  Node leftResult, rightResult;

  if(prefixNodes.size() && infixNodes.size()) {

    root = prefixNodes[0];
    leftRight = getLeftAndRightFromInfix(infixNodes, root);

    left[1] = leftRight[0];
    right[1] = leftRight[1];

    left[0] = prefixNodes.substr(1, left[1].size());
    right[0] = prefixNodes.substr(left[1].size()+1);

    leftResult = getTree(left);
    rightResult = getTree(right);

    //return leftResult + rightResult + root;
  }

  //return "";
}

Tree::Tree() {}

void Tree::getTreeFromFile() {
  std::ifstream infile("tree", std::ios::binary);
  std::vector<char> data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  string treeData1(data.begin(), data.end());
  string treeData2 = treeData1.substr(treeData1.find('\n')+1);

  int nextIndex;
  while(treeData1.size()) {
    if(treeData1[0] == ']') {
      prefixNodes.push_back(Node());
    } else {
      prefixNodes.push_back(Node(treeData1[nextIndex]));
    }

    treeData1 = treeData1.substr(nextIndex+1);
  }

  while(treeData2.size()) {
    if(treeData2[0] == ']') {
      infixNodes.push_back(Node());
    } else {
      infixNodes.push_back(Node(treeData2[nextIndex]));
    }

    treeData2 = treeData2.substr(nextIndex+1);
  }
}

void Tree::printNodes() {
  for(int index=0; index<prefixNodes.size(); index++) {
    cout << prefixNodes[index].symbol << endl;
  }
  cout << endl;
  cout << "INFIX" << endl;
  for(int index=0; index<infixNodes.size(); index++) {
    cout << infixNodes[index].symbol << endl;
  }
}

void Tree::descompress() {
  for(int index=0; index<data.size(); index++) {


    if(data[index] == 0) {

    } else {

    }
  }
}

void printVector(vector<string> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << endl;
  }
  cout << endl;
}


int main(int argc, char** argv) {
  Tree tree;

  std::ifstream infile(argv[1], std::ios::binary);

  tree.data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  tree.getTreeFromFile();
  tree.printNodes();
  return 0;
}