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
    vector<char> code;
    char symbol;
    Column(char symbol, vector<char> code);
    void print();
};

void Column::print() {
  cout << symbol << ": ";
  for(int index=0; index<code.size(); index++) {
    cout << code[index];
  }
  cout << endl;
}

Column::Column(char symbol, vector<char> code) {
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
    Node(int frequency, char symbol);
    Node(int frequency, vector<Node> adjacents);
    void print();
    void printOne();
    void plusOneFrequency();
    vector<Column> getCodes(vector<char> code);
    void printCodes();
    vector<char> generateCode(vector<char> data);
    vector<char> getCodeBySymbol(char symbol);
};

vector<Column> Node::getCodes(vector<char> code) {

  if(this->hasSymbol) {
    cout << "Adding " << this->symbol << endl;
    codes.push_back(Column (this->symbol, code));
  }

  if(adjacents.size() > 0) {
    vector<Column> codes1;
    code.push_back('1');
    codes1 = adjacents[0].getCodes(code);
    codes.insert( codes.end(), codes1.begin(), codes1.end() );
  }

  if(adjacents.size() > 1) {
    vector<Column> codes2;
    code.push_back('0');
    codes2 = adjacents[1].getCodes(code);
    codes.insert( codes.end(), codes2.begin(), codes2.end() );
  }

  return codes;
}

vector<char> Node::getCodeBySymbol(char symbol) {
  return vector<char>();
}

Node::Node(int frequency, char symbol) {
  this->frequency = frequency;
  this->hasSymbol = true;
  this->symbol = symbol;
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

void Node::printOne() {
  cout << symbol << ": " << frequency << endl;
}

void Node::printCodes() {
  cout << "Printing Codes" << endl;
  for(int index=0; index<codes.size(); index++) {
    codes[index].print();
  }
}

void Node::plusOneFrequency() {
  frequency++;
}

vector<char> generateCode(vector<char> data) {
  return vector<char>();
}

class Graph {
  public:
    vector<Node> nodes;
    Graph(vector<Node> nodes);
    void printNodes();
    int getLesserFrequency();
    Node getTree();
};

Graph::Graph(vector<Node> nodes) {
  this->nodes = nodes;
}

void Graph::printNodes() {
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

int getIndexBySymbol(char symbol, vector<Node> graph) {
  for(int index=0; index<graph.size(); index++) {
    if (graph[index].symbol == symbol) {
      return index;
    }
  }

  return -1;
}

Graph getGraph(vector<char> data) {
  std::vector<Node> nodes;
  int indexFound;

  for(int index=0; index<data.size(); index++) {
    // Found
    indexFound = getIndexBySymbol(data[index], nodes);
    if(indexFound != -1) {
      //nodes[indexFound].printOne();
      nodes[indexFound].plusOneFrequency();
    } else {
      Node node(1, data[index]);
      //node.printOne();
      nodes.push_back(node);
    }
  }

  Graph graph(nodes);
  return graph;
}

void printVector(vector<char> vertex) {
  for(int index=0; index<vertex.size(); index++) {
    cout << vertex[index] << " ";
  }
  cout << endl;
}

// adjacent[0] means 1 in code
// adjacent[1] means 0 in code
int main(int argc, char** argv) {
  std::ifstream infile(argv[1], std::ios::binary);

  std::vector<char> data = std::vector<char>
    (std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());

  //printVector(data);

  Graph graph = getGraph(data);
  graph.printNodes();
  Node root = graph.getTree();
  root.print();
  cout << endl;
  vector<char> code;
  root.getCodes(code);
  root.printCodes();
  infile.close();

  return 0;
}
