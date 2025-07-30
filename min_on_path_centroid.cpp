// Centroid decomposition with O(log log n) per query

#include<bits/stdc++.h>

using namespace std;

#define ll long long

const int maxn = 1e5;

int w[maxn], sz[maxn];
vector<int> g[maxn], ct[maxn], mn[maxn];

bool used[maxn];

void sizes(int v, int p = -1) {
    sz[v] = 1;
    for(auto u: g[v]) {
        if(used[u] || u == p) continue;
        sizes(u, v);
        sz[v] += sz[u];
    }
}

int centroid(int v, int n, int p = -1) {
    for(auto u: g[v]) {
        if(used[u] || u == p) continue;
        if(2*sz[u] > n) return centroid(u, n, v);
    }
    return v;
}

void dfs(int v, int c, int cur, int p = -1) {
    cur = min(cur, w[v]);

    ct[v].push_back(c);
    mn[v].push_back(cur);

    for(auto u: g[v]) {
        if(used[u] || u == p) continue;
        dfs(u, c, cur, v);
    }
}

void solve(int v) {
    sizes(v);

    ct[v].push_back(v);
    mn[v].push_back(w[v]);

    for(auto u: g[v]) {
        if(used[u]) continue;

        dfs(u, v, w[v], v);
    }

    used[v] = true;
    for(auto u: g[v]) {
        if(!used[u]) solve(centroid(u, sz[u], v));
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;

    for(int i = 1; i < n; i++) {
        int p; cin >> p; p--;
        g[i].push_back(p);
        g[p].push_back(i);
    }

    for(int i = 0; i < n; i++) cin >> w[i];

    sizes(0);
    solve(centroid(0, n));

    while(q--) {
        int a, b; cin >> a >> b; a--; b--;

        if(a == b) {
            cout << w[a] << '\n';
            continue;
        }

        int l = 0;
        int r = min(ct[a].size(), ct[b].size());

        while(r - l > 1) {
            int m = (l+r) / 2;
            if(ct[a][m] == ct[b][m]) {
                l = m;
            } else {
                r = m;
            }
        }

        cout << min(mn[a][l], mn[b][l]) << '\n';
    }
}
