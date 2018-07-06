#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#define INFINI 75849365
#define NOTFOUND() std::cout<<"\nThere is no such node on the graph \n";

#include<string>
#include<set>
#include<map>
#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>
#include<list>
#include<stack>

typedef struct Neighbor Neighbor;
struct Neighbor
{
    std::string name;
    int cost;

    Neighbor(std::string node, int c): name(node), cost(c) {};
};

class Graph
{
private:
    std::set<std::string> nodes;
    std::map<std::string,std::vector<Neighbor>>edges;
    bool oriented;

    /***** Private methods  */
    bool  nIsHamiltonian();
    bool  oIsHamiltonian();
    std::vector<std::string> nextTo(std::string);
    std::vector<std::string> previousTo(std::string);
    int outDegree(std::string);
    int inDegree(std::string);
    void visit(std::map<std::string,bool>&v,std::string);
    std::vector<std::string> bfs(std::string);
    void dfs(std::string,std::map<std::string,bool>&,std::vector<std::string>&);
    void fillOrder(std::string, std::map<std::string,bool>&,std::stack<std::string>&);
    Graph getTranspose();
    int getWeight(std::string node1,std::string node2);
    std::map<int,std::vector<std::string>> connectedComponents();
    bool connected();
    bool fpath(std::string,std::string);
    std::string miniDistance(std::map<std::string,int> &tab);
    int  degree(std::string);
    int Eulerian();

public:
    Graph(bool type=false);
    void addNode(std::string);
    void addNode();
    void addEdge(std::string,std::string,int c=1);
    void order();
    void removeNode(std::string);
    void removeEdge(std::string,std::string);
    void dijkstra(std::string);
    void isEulerian();
    void isHamiltonian();
    void isConnected();
    void existPath(std::string,std::string);
    void displayNextTo(std::string);
    void displayPreviousTo(std::string);
    void displayDegree(std::string);
    void displayNodes();
    void displayMatrix();
    void displayBfs(std::string);
    void displayComponents();
    friend void editGraph(Graph &g);
    void distance();
};

#endif // GRAPH_H_INCLUDED
