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
    Node(int frequency, vector<Node> adjacents);
    void print();
    void printOne();
    void plusOneFrequency();
    vector<Column> getCodes(string code);
   void printCodes();
   string getCodeBySymbol(char symbol);
   void generateCode(vector<char> data);
   void printCompactedData();
};

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
    cout << codes[index].symbol << ": " << codes[index].code << endl;
  }
}

vector<Column> Node::getCodes(string code = "") {

  if(this->hasSymbol) {
    cout << "Adding " << this->symbol << endl;
    codes.push_back(Column (this->symbol, code));
  }

  if(adjacents.size() > 0) {
    vector<Column> codes1;
    codes1 = adjacents[0].getCodes(code + '1');
    codes.insert( codes.end(), codes1.begin(), codes1.end() );
  }

  if(adjacents.size() > 1) {
    vector<Column> codes2;
    codes2 = adjacents[1].getCodes(code + '0');
    codes.insert( codes.end(), codes2.begin(), codes2.end() );
  }

  return codes;
}

string Node::getCodeBySymbol(char symbol) {
  for(int index=0; index<codes.size(); index++) {
    if(codes[index].symbol == symbol) {
      return codes[index].code;;
    }
  }

   return "";
 }
void Node::generateCode(vector<char> data) {
  compactedData.clear();
  for(int index=0; index<data.size(); index++) {
    string temp;
    temp = getCodeBySymbol(data[index]);
    compactedData += temp;
  }
 }

void Node::plusOneFrequency() {
  frequency++;
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
  root.getCodes();
  root.printCodes();
  root.generateCode(data);
  cout << endl;
  cout << root.compactedData << endl;
  cout << root.compactedData.size()<< endl;
  infile.close();

  return 0;
}
