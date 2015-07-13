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

class Descompressor {
  public:
    string filename;
    vector<char> data;
    vector<Column> codes;
    string descompressedData;
    Descompressor();
    void descompress();
    void getCodesFromFile();
    void printNodes();
    void printCodes();
    char getSymbol(string);
};

char Descompressor::getSymbol(string code) {
  for(int index=0; index<codes.size(); index++) {
    if(code == codes[index].code) {
      //cout << codes[index].symbol << endl;
      return codes[index].symbol;

    }
  }
  return '\0';
}

void Descompressor::printCodes() {
  for(int index=0; index<codes.size(); index++) {
    cout << codes[index].symbol << ": " << codes[index].code << endl;
  }
}

void Descompressor::getCodesFromFile() {
  std::ifstream infile("codes", std::ios::binary);

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

Descompressor::Descompressor() {}

void Descompressor::descompress() {
  string toDescompress;
  descompressedData.clear();
  char symbol;
  for(int index=0; index<data.size(); index++) {
    toDescompress += data[index];
    symbol = getSymbol(toDescompress);

    if(symbol != '\0') {
      descompressedData += symbol;
      toDescompress.clear();
    }
  }

  std::ofstream file (filename);
  file << descompressedData;
  file.close();
}

void printVector(vector<string> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << endl;
  }
  cout << endl;
}


int main(int argc, char** argv) {
  Descompressor descompressor;

  std::ifstream infile(argv[1], std::ios::binary);
  descompressor.filename = string(argv[1]);
  descompressor.filename = descompressor.filename.substr
    (0, descompressor.filename.find(".comp"));
  //cout << "FILE " << descompressor.filename << endl;
  descompressor.data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  descompressor.getCodesFromFile();
  //descompressor.printCodes();
  descompressor.descompress();
  //cout << descompressor.descompressedData << endl;
  return 0;
}