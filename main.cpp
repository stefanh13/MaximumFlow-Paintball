//
//  main.cpp
//  Paintball
//
//  Created by Stefán Hafsteinsson on 12/12/14.
//  Copyright (c) 2014 Stefán Hafsteinsson. All rights reserved.
//

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct flow_network {
    
    struct edge {
        int u, v, cap;
        edge *rev;
        bool forward;
        edge(int _u, int _v, int _cap, bool forw)
        : u(_u), v(_v), cap(_cap), forward(forw) { }
    };
    
    int n;
    vector<vector<edge*> > adj;
    flow_network(int _n) : n(_n), adj(_n) { }
    
    void add_edge(int u, int v, int cap) {
        edge *e = new edge(u, v, cap, true);
        edge *rev = new edge(v, u, 0, false);
        e->rev = rev;
        rev->rev = e;
        adj[u].push_back(e);
        adj[v].push_back(rev);
    }
    
    int augment(int s, int t) {
        vector<edge*> back(n, NULL);
        queue<int> Q;
        Q.push(s);
        back[s] = (edge*)1;
        while (!Q.empty()) {
            int u = Q.front(); Q.pop();
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i]->v;
                if (back[v] == NULL && adj[u][i]->cap > 0) {
                    back[v] = adj[u][i];
                    Q.push(v);
                }
            }
        }
        
        if (back[t] == NULL)
            return 0;
        
        stack<edge*> S;
        S.push(back[t]);
        int bneck = back[t]->cap;
        while (S.top()->u != s) {
            S.push(back[S.top()->u]);
            bneck = min(bneck, S.top()->cap);
        }
        
        while (!S.empty()) {
            S.top()->cap -= bneck;
            S.top()->rev->cap += bneck;
            S.pop();
        }
        
        return bneck;
    }
    
    int max_flow(int source, int sink) {
        int flow = 0;
        while (true) {
            int f = augment(source, sink);
            if (f == 0) {
                break;
            }
            
            flow += f;
        }
        
        return flow;
    }
};

int main(int argc, const char * argv[]) {
    
    int N, M, A = 0, B = 0;
    
    scanf("%d%d", &N, &M);
    
    int source = 0, sink = 1, shooters = 2, targets = shooters + N, cnt = targets + N;
    
    flow_network f(cnt);
    for(int i = 0; i < M; i++)
    {
        scanf("%d%d", &A, &B);
        A--, B--;
        //If A can see B, then B can see A.
        f.add_edge(A + shooters, B + targets, 1);
        f.add_edge(B + shooters, A + targets, 1);
        
    }
    
    //Connect source and sink to shooters and targets.
    for(int i = 0; i < N; i++)
    {
        f.add_edge(source, i + shooters, 1);
        f.add_edge(i + targets, sink, 1);
    }
    
    int flow = f.max_flow(source, sink);
    
    //If flow is not N, then not all players can be hit.
    if(flow !=  N)
    {
        cout << "Impossible" << endl;
    }
    else
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < f.adj[shooters + i].size(); j++)
            {
                if(f.adj[shooters + i][j]->forward && f.adj[shooters + i][j]->rev->cap)
                {
                    cout << f.adj[shooters + i][j]->v - targets + 1 << endl;
                }
            }
        }
    }
    return 0;
}
