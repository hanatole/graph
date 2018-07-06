#include "Graph.h"

using namespace std;

Graph::Graph(bool type):nodes(),edges(),oriented(type){}

void Graph::addNode(string node)
{
    if(nodes.find(node)==nodes.end())
        nodes.insert(node);
}

void Graph::removeNode(string node)
{
    for(const auto &n:nodes)
        removeEdge(n,node);
    edges.erase(edges.find(node));
    nodes.erase(node);
}

void Graph::addNode()
{
    vector<string>defNodes;
    defNodes.push_back("S0");
    defNodes.push_back("S1");
    defNodes.push_back("S2");
    static int i(0);
    addNode(defNodes[i++]);
}

void Graph::addEdge(string node1,string node2,int c)
{
    addNode(node1);addNode(node2);

    edges[node1].push_back(Neighbor(node2,c));
    if(!oriented)
        edges[node2].push_back(Neighbor(node1,c));
}

void Graph::removeEdge(string node1,string node2)
{
     vector<Neighbor>&neighbors=edges[node1];
     vector<Neighbor>::iterator it=neighbors.begin();

     while(it!=neighbors.end())
     {
         if(it->name==node2)
         {
             neighbors.erase(it);
             break;
         }
         it++;
     }

     if(!oriented)
     {
         vector<Neighbor>&neighbors=edges[node2];
         it=neighbors.begin();

        while(it!=neighbors.end())
         {
             if(it->name==node1)
             {
                 neighbors.erase(it);
                 break;
             }
             it++;
         }
     }
}

void Graph::displayNodes()
{
    for(const auto &n:nodes)
        cout<<n<<"\t";
    cout<<endl;
}

void Graph::displayMatrix()
{
   int len((int)nodes.size()),i(0),j(0); // Number of nodes
   vector<int>line(len,0);
   vector<vector<int> >matrice(len,line); // Null Len order Matrix
   map<int,string>indexToName;

    for(const auto &n:nodes)
        indexToName[i++]=n;

    // Construction of the matrix
    for(i=0;i<len;i++)
    {
        for(j=0;j<len;j++)
            matrice[i][j]=getWeight(indexToName[i],indexToName[j]);
    }
   // For displaying the matrix
   cout<<endl<<endl<<"\t\t";
   for(const auto &n:indexToName)
        cout<<n.second<<"\t";
   cout<<endl<<endl<<endl;
   i=0;
   for(const auto &l:matrice)
   {
       cout<<"\t"<<indexToName[i++]<<"\t";
       for(const auto &cel:l)
         cout<<cel<<"\t";
       cout<<endl<<endl<<endl;
   }
}

bool Graph::fpath(string node1,string node2)
{
    if(nodes.find(node1)==nodes.end()|| nodes.find(node1)==nodes.end())
      {
        NOTFOUND()
        return false;
      }
    vector<string> path=bfs(node1);

    return find(path.begin(),path.end(),node2)!=path.end();
}

void Graph::existPath(string node1,string node2)
{
    bool path=fpath(node1,node2);
    if(path)
        cout<<"\nYes there is a path between "<<node1<<" and "<<node2<<endl;
    else
        cout<<"\nThere is no path between "<<node1<<" and "<<node2<<endl;
}

void Graph::order()
{
    if(nodes.size())
        cout<<"The order of the graph is "<<nodes.size()<<endl;
    else
        cout<<"There is no node on the graph"<<endl;
}

void Graph::displayDegree(string node)
{
    if(nodes.find(node)==nodes.end())
    {
        NOTFOUND()
        return;
    }

    if (!oriented)
            cout<<"Deg("<<node<<")="<<edges[node].size()<<endl;
    else
        {
            cout<<"Deg+("<<node<<")="<<nextTo(node).size()<<endl;
            cout<<"Deg-("<<node<<")="<<previousTo(node).size()<<endl;
            cout<<"Deg("<<node<<")="<<nextTo(node).size()+previousTo(node).size()<<endl;
        }
}

int  Graph::degree(string node)
{
    if(nodes.find(node)==nodes.end())
        return -1;
    if(!oriented)
        return edges[node].size();
    return edges[node].size()+previousTo(node).size();
}

void Graph::displayNextTo(string node)
{
    if(nodes.find(node)==nodes.end())
    {
       NOTFOUND( )
       return;
    }
    vector<string> nex=nextTo(node);

    if(nex.size()==0)
        cout<<"The node "<<node<<" has no successor."<<endl;
    else
     {
        cout<<"The successor of "<<node<<" : ";
        for(const auto &n:nex)
            cout<<n<<"\t";
        cout<<endl;
     }
}

vector<string> Graph::nextTo(string node)
{
    vector<string> successor;

    if(nodes.find(node)==nodes.end())
        return successor;
    vector<Neighbor> neighbor=edges[node];

    for(const auto &n:neighbor)
        successor.push_back(n.name);
    return successor;
}

vector<string> Graph::previousTo(string node)
{
    if(!oriented)
        return nextTo(node);
    vector<string> prev;

    for(const auto &n:nodes)
    {
        vector<string> nex=nextTo(n);
        if(find(nex.begin(),nex.end(),node)!=nex.end())
            prev.push_back(n);
    }
    return prev;
}

void Graph::displayPreviousTo(string node)
{
    if(nodes.find(node)==nodes.end())
    {
       NOTFOUND( )
       return;
    }
    vector<string> prev=previousTo(node);

    if(prev.size()==0)
        cout<<"The node "<<node<<" has no previous."<<endl;
    else
        {
             cout<<"The previous of "<<node<<" : ";
             for(const auto &p:prev)
                 cout<<p<<"\t";
              cout<<endl;
        }
}

int Graph::getWeight(string node1,string node2)
{
    vector<Neighbor> neighbors=edges[node1];

    for(const auto &neighbor:neighbors)
    {
        if(neighbor.name==node2)
            return neighbor.cost;
    }
    return 0;
}

string Graph::miniDistance(map<string,int> &tab)
{
    map<string,int>::iterator it1=tab.begin();
    map<string,int>::iterator it2=tab.begin();

    while(it1!=tab.end())
    {
        if((it1->second)<(it2->second))
            it2=it1;
        it1++;
    }

    return it2->first;
}

void Graph::dijkstra(string source)
{
    if(nodes.find(source)==nodes.end())
    {
       NOTFOUND()
       return;
    }
    map<string,int>dist1,dist2;
    set<string> aTraiter(nodes),traiter;

    for(const auto &n:nodes)
        dist1[n]=INFINI;
    dist1[source]=0;
    while(!aTraiter.empty())
    {
        string x=miniDistance(dist1);

        traiter.insert(x);
        aTraiter.erase(find(aTraiter.begin(),aTraiter.end(),x));

        for(const auto &y:nextTo(x))
        {
            if(traiter.find(y)==traiter.end())
               {
                    int val(dist1[x]+getWeight(x,y));

                    if(dist1[y]>val)
                        dist1[y]=val;
                }
        }
        dist2[x]=dist1[x];
        dist1.erase(dist1.find(x));
     }

    for(const auto &n:dist2)
    {
        cout<<source<<"---->"<<n.first;
        if(n.second<0||n.second==INFINI)
            cout<<" : INFINI"<<endl;
        else
            cout<<" : "<<n.second<<endl;
        cout<<endl;

    }

}

int Graph::Eulerian()
{
  int odd(0);

  if(oriented)
       {
           for(const auto &n:nodes)
                if(inDegree(n)==outDegree(n))
                    odd++;
       }
   else
      {
          for(const auto &n:nodes) // count odd nodes
            if(degree(n)>0 && degree(n)%2)
                odd++;
      }
   return odd;
}

void Graph::isEulerian()
{

   int rep=(connected())?Eulerian():3;

   if(rep==0)
        cout<<"The graph is Eulerian \n"<<endl;
    else if(rep==2)
        cout<<"The graph is Semi Eulerian \n"<<endl;
    else
        cout<<"The graph is not Eulerian \n"<<endl;
}

void Graph::isHamiltonian()
{

}

bool Graph::nIsHamiltonian()
{
    if(nodes.size()<3 && connected())
    {
        cout<<"The graph is Hamiltonian "<<endl;
        return true;
    }
    else if(nodes.size()<3 && !connected())
    {
        cout<<"The graph is not Hamiltonian "<<endl;
        return false;
    }
    return false;
}

bool Graph::oIsHamiltonian()
{
    bool ok(true);

    for(const auto &n:nodes)
    {
        if(degree(n)<=1)
        {
            cout<<"The graph is not Hamiltonian "<<endl;
            return false;
        }
    }

    return ok;
}

bool Graph::connected()
{
    int zero(0); // zero-node degree
    for(const auto &n:nodes)
        if(!degree(n))
            zero++;
    return (int)connectedComponents().size()== zero+1;
}

void Graph::isConnected()
{
    if(connected())
        cout<<"\nThe graph is connected"<<endl;
    else
        cout<<"\nThe graph is not connected"<<endl;
}

int Graph::outDegree(string node)
{
    if(edges.find(node)!=edges.end())
        return edges[node].size();
    return 0;
}

int Graph::inDegree(string node)
{
    if(edges.find(node)!=edges.end())
           return previousTo(node).size();
    return 0;
}

vector<string>Graph::bfs(string node)
{
    queue<string>file;
    map<string,bool>colored;
    vector<string> path;

    for(const auto &n:nodes)
        colored[n]=false;
    colored[node]=true;
    file.push(node);

    while(!file.empty())
    {
        string nex=file.front();
        file.pop();
        path=nextTo(nex);

        for(const auto &n:path)
        {
            if(!colored[n])
            {
                colored[n]=true;
                file.push(n);
            }
        }
    }

    path.clear();
    for(const auto &n:colored)
        if(n.second)  // All nodes reachable from node
          path.push_back(n.first);

   return path;
}

void Graph::displayBfs(string node)
{
    if(nodes.find(node)==nodes.end())
    {
        NOTFOUND()
        return;
    }
    vector<string>path=bfs(node);

    for(const auto &n:path)
        cout<<n<<"\t";
}
/*********** Connected components ********* */
void Graph::dfs(string source,map<string,bool>&colored,vector<string>&compo)
{
    colored[source]=true;
    compo.push_back(source);

    vector<string> nex=nextTo(source);
    vector<string>::iterator i;

    for(i=nex.begin();i!=nex.end();i++)
        if(!colored[*i])
              dfs(*i,colored,compo);
}

Graph Graph::getTranspose()
{
    Graph gr(oriented);

    for(const auto &n:nodes)
    {
        vector<string> prev=previousTo(n);

        for(const auto &pr:prev)
            gr.addEdge(n,pr,getWeight(pr,n));
    }

    return gr;
}

void Graph::fillOrder(string node,map<string,bool>&colored,stack<string>&file)
{
    vector<string> neighors=nextTo(node);

    colored[node]=true;
    for(const auto &n:neighors)
        if(!colored[n])
          fillOrder(n,colored,file);
    file.push(node);
}

map<int,vector<string>>Graph::connectedComponents()
{
    stack<string> file;
    map<string,bool> colored;

    for(const auto &n:nodes)
        colored[n]=false;
    for(const auto &n:nodes)
        if(!colored[n])
            fillOrder(n,colored,file);

    Graph gr=getTranspose();

    for(const auto &n:nodes)
        colored[n]=false;

    int j(1);
    map<int,vector<string>> components;

    while(!file.empty())
    {
        string node=file.top();
        file.pop();
        if(!colored[node])
        {
            vector<string> cp;

            gr.dfs(node,colored,cp);
            components[j++]=cp;
        }
    }
return components;
}

void Graph::displayComponents()
{
    map<int,vector<string>>components=connectedComponents();
    int len=(int) components.size();

    if(len==1)
        cout<<"The only connected component of the graph is: ";
    else
        cout<<"The connected components of the graph:\n"<<endl;
    for(const auto &n:components)
    {
        if (len>1) cout<<"Component "<<n.first;
        cout<<":\t{ ";
        for(const auto &k:n.second)
            cout<<k<<" ";
        cout<<"}"<<endl;
    }
}

