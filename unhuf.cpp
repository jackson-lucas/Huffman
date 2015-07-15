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
    string descompressedData;
    string compressedData;
    Node(int frequency, char symbol);
    Node(char symbol);
    Node();
    Node(int frequency, vector<Node> adjacents);
    void print();
    void getTreeFromFile();
    void getCodesFromFile();
    void generateDescompressedData();
    //void printOne();
    //void plusOneFrequency();
    //vector<Column> getCodes(string code);
   //void printCodes();
   //string treeAsString();
   //string getCodeBySymbol(char symbol);
   //void generateCode(vector<char> data);
   //void printCompactedData();
};

void Node::generateDescompressedData() {
  Node* actualNode = this;
  for(int index=0; index<compressedData.size(); index++) {
    if(compressedData[index] == '1') {
      actualNode = &(actualNode->adjacents[0]);
    } else {
      actualNode = &(actualNode->adjacents[1]);
    }

    if(actualNode->hasSymbol) {
      descompressedData += actualNode->symbol;
      actualNode = this;
    }
  }
}

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


class Graph {
  public:
    vector<Node> nodes;
    void getNodes(string);
    Graph(string);
    void printNodes();
    int getLesserFrequency();
    Node getTree();
};

void Graph::getNodes(string data) {
  int separator, doublePoint, frequency;
  char symbol;

  while(data.size() > 1) {
    separator = data.find(';');
    string manipulate = data.substr(0, separator);
    data = data.substr(separator+1);
    //cout << manipulate << endl;

    //doublePoint = data.find(':');
    symbol = manipulate[0];
    frequency = stoi(manipulate.substr(2));
    nodes.push_back(Node(frequency, symbol));

  }
}

Graph::Graph(string nodes) {
  getNodes(nodes);
}

void Graph::printNodes() {
  //cout << nodes.size() << endl;
  for(int index=0; index<nodes.size(); index++) {
    cout << nodes[index].symbol << ": " << nodes[index].frequency << endl;
  }
  cout << endl;
}

int Graph::getLesserFrequency() {
  int lesserFrequency = nodes[0].frequency;
  int symbolIndex = 0;

  for(int index = 1; index < nodes.size(); index++) {
    if(nodes[index].frequency < lesserFrequency) {
      lesserFrequency = nodes[index].frequency;
      symbolIndex = index;
    }
  }

  return symbolIndex;
}

Node Graph::getTree() {
  int index;

  while(nodes.size() > 1) {
    vector<Node> adjacents;

    index = getLesserFrequency();
    Node nodeLesser1 = nodes[index];
    nodes.erase(nodes.begin()+index);

    index = getLesserFrequency();
    Node nodeLesser2 = nodes[index];
    nodes.erase(nodes.begin()+index);

    if(!nodeLesser2.hasSymbol && nodeLesser1.hasSymbol) {
      adjacents.push_back(nodeLesser2);
      adjacents.push_back(nodeLesser1);
    } else {
      adjacents.push_back(nodeLesser1);
      adjacents.push_back(nodeLesser2);
    }

    Node newNode(adjacents[0].frequency + adjacents[1].frequency, adjacents);
    //newNode.printOne();
    nodes.push_back(newNode);
  }

  return nodes[0];
}

class Descompressor {
  public:
    string filename;
    vector<char> data;
    vector<Column> codes;
    vector<Node> nodes;
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
      cout << codes[index].symbol << endl;
      return codes[index].symbol;

    }
  }
  return '\0';
}

void Descompressor::descompress() {
  string toDescompress;
  descompressedData.clear();
  char symbol;
  //cout << data.size() << "<<<" << endl;
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

  string dataBin(descompressor.data.begin(), descompressor.data.end());
  int nextEnter = dataBin.find("DELIMITER");
  string frequencies = dataBin.substr(0, nextEnter);
  dataBin = dataBin.substr(nextEnter+9);
  //cout << dataBin << endl;
  //cout << frequencies << endl;
  Graph graph(frequencies);
  //graph.printNodes();

  Node root = graph.getTree();
  root.compressedData = dataBin;
  //cout << dataBin << endl;
  //root.print();
  root.generateDescompressedData();
  cout << root.descompressedData << endl;


  //descompressor.getCodesFromFile();
  //descompressor.printCodes();
  //descompressor.descompress();
  //cout << descompressor.descompressedData << endl;
  return 0;
}