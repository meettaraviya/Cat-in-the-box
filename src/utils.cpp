#include "common.h"

class UnionFind
{
public:
    vector<int> parent;
    vector<int> size;
    
    UnionFind(int n): size(n, 1) {
        for(int i=0; i<n; i++) parent.push_back(i);
    }

    int find_set(int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set(parent[v]);
    }

    void union_sets(int a, int b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            if (size[a] < size[b])
                swap(a, b);
            parent[b] = a;
            size[a] += size[b];
        }
}
};