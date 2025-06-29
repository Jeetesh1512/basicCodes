class DSU{
    vector<int> parent, rank;
public:
    DSU(int n){
        rank.resize(n + 1, 0);
        parent.resize(n + 1);
        for (int i = 0; i <= n; i++){
            parent[i] = i;
        }
    }

    int Par(int node){
        if (node == parent[node])
            return node;
        return parent[node] = Par(parent[node]);
    }

    void unionByRank(int u, int v){
        int ulp_u = Par(u);
        int ulp_v = Par(v);
        if (ulp_u == ulp_v)
            return;
        if (rank[ulp_u] < rank[ulp_v])
        {
            parent[ulp_u] = ulp_v;
        }
        else if (rank[ulp_v] < rank[ulp_u])
        {
            parent[ulp_v] = ulp_u;
        }
        else
        {
            parent[ulp_v] = ulp_u;
            rank[ulp_u]++;
        }
    }
};


class DSU{
    vector<int> parent,size;
public:
    DSU(int n){
        size.resize(n+1,1);
        parent.resize(n+1);
        for(int i=0;i<=n;i++)
            parent[i]=i;
    }

    int findPar(int node)
    {
        if(parent[node]==node)
            return node;
        return parent[node]=findPar(parent[node]);
    }

    void unionBySize(int u,int v){
        int ulp_u=findPar(u);
        int ulp_v=findPar(v);

        if(ulp_u==ulp_v)
            return;

        if(size[ulp_u]<size[ulp_v]){
            parent[ulp_u]=ulp_v;
            size[ulp_v]+=size[ulp_u];
        }
        else{
            parent[ulp_v]=ulp_u;
            size[ulp_u]+=size[ulp_v];
        }
    }
};



