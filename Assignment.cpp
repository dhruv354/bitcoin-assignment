#include<bits/stdc++.h>
#include<unordered_map>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
using namespace std;




//topological sort

// Class to represent a graph and
// and to save topological view 
class Graph {
    // No. of vertices'
    int V;

    // Pointer to an array containing adjacency listsList
    list<int>* adj;

    // A function used by topologicalSort
    void topologicalSortUtil(int v, bool visited[],
                             stack<int>& Stack);

public:
    // Constructor
    Graph(int V);

    // function to add an edge to graph
    void addEdge(int v, int w);

    // prints a Topological Sort of
    // the complete graph
    stack<int> topologicalSort();
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<int>[V];
}

void Graph::addEdge(int v, int w)
{
    // Add w to v’s list.
    adj[v].push_back(w);
}

// A recursive function used by topologicalSort
void Graph::topologicalSortUtil(int v, bool visited[],
                                stack<int>& Stack)
{
    // Mark the current node as visited.
    visited[v] = true;

    // Recur for all the vertices
    // adjacent to this vertex
    list<int>::iterator i;
    for (i = adj[v].begin(); i != adj[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack);

    // Push current vertex to stack
    // which stores result
    Stack.push(v);
}

// The function to do Topological Sort.
// It uses recursive topologicalSortUtil()
stack<int> Graph::topologicalSort()
{
    stack<int> Stack;

    // Mark all the vertices as not visited
    bool* visited = new bool[V];
    for (int i = 0; i < V; i++)
        visited[i] = false;

    // Call the recursive helper function
    // to store Topological
    // Sort starting from all
    // vertices one by one
    for (int i = 0; i < V; i++)
        if (visited[i] == false)
            topologicalSortUtil(i, visited, Stack);
    //return topological stack
    return Stack;
}


//apply topological sorting on my
stack<int> topological_sort(vector<string> &tx_id, vector<vector<string>> &parent, unordered_map<string, int> &mp){
    Graph g(tx_id.size());
    for(int i = 0;i < tx_id.size();i++){

        if(parent[i][0] == "0"){
           //g.addEdge(i, i);
            continue;
        }

        for(int j = 0;j < parent[i].size();i++){
            g.addEdge(mp[parent[i][j]], i);
        }
    }
    return g.topologicalSort();
}


//function to read CSV file
vector<vector<string>> parseCSV()
{
    ifstream  data("../Desktop/bitcoin_challenge/dataset.csv");
    string line;
    vector<vector<string> > parsedCsv;
    while(getline(data,line))
    {
        stringstream lineStream(line);
        string cell;
        vector<string> parsedRow;
        while(getline(lineStream,cell,','))
        {
            parsedRow.push_back(cell);
        }

        parsedCsv.push_back(parsedRow);
    }
    return parsedCsv;
};


//function to map tx_id to their indices
void solve(vector<string>tx_id, unordered_map<string, int>&mp){
    for(int i = 0;i < tx_id.size();i++){
        mp[tx_id[i]] = i;
    }
}


//tokenize the string based on a character to tokenize parent as it contains many tx_ids separated by ';'
// it will help in processing through the parent ids
vector<string> tokenize(string s, string del = ";")
{
    vector<string> output;
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        output.push_back(s.substr(start, end - start));
        //cout << s.substr(start, end - start) << endl;
        start = end + del.size();
        end = s.find(del, start);
    }
    output.push_back(s.substr(start, end - start));
    return output;
    //cout << s.substr(start, end - start);
}


//main function that will ultimately return the maximum value achieved
int knapsack3(vector<int>topo, vector<int>fee, vector<int>weight, int max_weight, vector<string>tx_id, unordered_map<string, int>mp, vector<vector<string>>parent, vector<string>&p){
        int n = 5213;
	//using 2 rows as only two rows are needed at a time to save space
        int**dp = new int*[2];
        for(int i = 0;i < 2;i++){
            dp[i] = new int[max_weight+1];
        }
        for(int i = 0;i <= max_weight;i++){
            dp[0][i] = 0;
        }
        int flag = 0;
        for(int i = 1;i <= n;i++){
	    //get current index from topological graph
            int cur_index = topo[i-1];
           cout << "i = " <<  i <<endl;
            for(int j = 1;j <= max_weight;j++){
            //cout << "j = " << j << endl;
               if(j < weight[cur_index]){
                 //cout << "inside if" << endl;
               //cout << dp[flag^1][j] << endl;
                    dp[flag^1][j] = dp[flag][j];
               // cout << "checked j" << endl;
                    continue;
                    }
                //cout << "outside if" << endl;
                int max_value = 0;
                int index = 0;
                //cout << "checking parent" << endl;
                if(parent[cur_index][0] == "0"){
                   // cout << "parent does not exist" << endl;
                    int temp = dp[flag][j-weight[cur_index]] + fee[cur_index];
                  //  cout << "temp" << temp << endl;
                    if(temp > dp[flag][j]){
                        dp[flag^1][j] = temp;
                        p[i] = tx_id[cur_index];
                    }
                    else{
                        dp[flag^1][j] = dp[flag][j];
                        p[i] = "";
                    }
                }
                else{
                  //  cout << "parent exist" << endl;
                    int parent_index = cur_index;
                    for(int k = 0;k < parent[cur_index].size();k++){
                        parent_index = mp[parent[cur_index][k]];
                        if(p[parent_index] != ""){
                            continue;
                        }
                        if(j < weight[parent_index]){
                            continue;
                        }
                        int temp = dp[flag][j-weight[parent_index]] + fee[parent_index];
                        if(max_value > temp){
                            max_value = temp;
                            //cout << "max_value" << max_value;
                            p[cur_index] = tx_id[parent_index];
                        }
                    }
                    if(dp[flag][j] > max_value){
                        dp[flag^1][j] =  dp[flag][j];
                        p[cur_index] = "";
                    }
                    else{
                        dp[flag^1][j] = max_value;
                        p[cur_index] = tx_id[parent_index];
                    }
                }
            }
            flag = flag^1;
        }
        return dp[flag^1][max_weight];
    }


// write vector v to block.txt
void write(vector<string> &v){
	ofstream file;
	file.open("./block.txt");
	for(int i=0;i<v.size();++i){
		file<<v[i]<<endl;
	}
	file.close();
}


//main function
int main(){
   vector<vector<string>> output =  parseCSV();
   // cout << output.size() << endl;

   
    vector<string>tx_id;
    vector<int>fee;
    vector<int>weight;
    vector<string>parent;
    for(int i = 1;i < output.size();i++){
        tx_id.push_back(output[i][0]);
    }
    for(int i = 1;i < output.size();i++){
        fee.push_back(stoi(output[i][1]));
    }
    for(int i = 1;i < output.size();i++){
        weight.push_back(stoi(output[i][2]));
    }
    for(int i = 1;i < output.size();i++){
        if(output[i].size() == 3){
            parent.push_back("0");
            continue;
        }
        parent.push_back(output[i][3]);
    }
    unordered_map<string, int> mp;
    solve(tx_id, mp);
    vector<vector<string>>parent_new;
    for(int i = 0;i < parent.size();i++){
        vector<string>temp;
        if(parent[i] == "0"){
            temp.push_back("0");
            parent_new.push_back(temp);
            continue;
        }
        vector<string>temp2 = tokenize(parent[i]);
        parent_new.push_back(temp2);
    }
    //cout << parent_new[0][0] << endl;
    //tokenize parent string
   stack<int>topological = topological_sort(tx_id, parent_new, mp);
    //cout << "topological size " << topological.size() << endl;
    int max_weight = 0;
    for(int i = 0;i < weight.size();i++){
        max_weight = max(max_weight, weight[i]);
    }

    vector<int>topo;
    while(!topological.empty()){
    topo.push_back(topological.top());
    topological.pop();
    }
   /* int sol = knapsackW2(topo, fee, weight, 10000, tx_id, mp, parent_new, visited);
    vector<string> sol = knapsack(topo, fee, weight, 5000, tx_id, mp, parent_new, visited);
    cout << sol.size() << endl;*/
    vector<string>p(fee.size());
    int ans = knapsack3(topo, fee, weight, 400000, tx_id, mp, parent_new, p);
    cout << "ans = " << ans << endl;
    int count_ = 0;
    vector<string>my_output;
    for(int i = 0;i < p.size();i++){
        if(p[i] != ""){
            count_++;
            my_output.push_back(p[i]);
        }
    } 
   //writing file into the block.txt
   write(my_output);
}


