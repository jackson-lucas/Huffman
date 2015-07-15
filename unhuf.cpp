#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

// Disciplina: Projeto e Análise de Algoritmo
// Feito por: Jackson Lima - 21351221

using namespace std;

class Node {
  public:
    int frequency;
    bool hasSymbol;
    char symbol;
    vector<Node> adjacents;
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

  while(data.size() > 1 && (separator = data.find("BTW")) != -1 && (separator = data.find("BTW")) != 0) {
    //separator = data.find(';');
    string manipulate = data.substr(0, separator);
    data = data.substr(separator+3);
    //cout << manipulate << endl;

    //doublePoint = data.find(':');
    symbol = manipulate[0];
    frequency = stoi(manipulate.substr(2));
    nodes.push_back(Node(frequency, symbol));

  }

  //cout << "DATA: " << data << endl;
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

int main(int argc, char** argv) {
  std::ifstream infile(argv[1], std::ios::binary);
  string filename = string(argv[1]);
  filename = filename.substr
    (0, filename.find(".comp"));

  std::vector<char> data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  string dataBin(data.begin(), data.end());
  int nextEnter = dataBin.find("DELIMITER");
  string frequencies = dataBin.substr(0, nextEnter);
  dataBin = dataBin.substr(nextEnter+9);
  //cout << dataBin.substr(0,50) << endl;
  //cout << frequencies << endl;
  Graph graph(frequencies);
  //graph.printNodes();

  Node root = graph.getTree();
  root.compressedData = dataBin;
  //cout << dataBin << endl;
  //root.print();
  root.generateDescompressedData();

  std::ofstream outfile (filename);

  outfile << root.descompressedData;

  outfile.close();
  return 0;
}