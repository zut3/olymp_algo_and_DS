// Solution for CSES problem: https://cses.fi/problemset/task/1737/

#include<bits/stdc++.h>

using namespace std;

#define ll long long

class PersistentSegTree {
private:
    struct Node {
        ll sum;
        int l, r; // indexes in tree vector, where you can find childrens
    };

    const int n;
    vector<Node> tree;
    int timer = 1;

    Node join(int l, int r) {
        return Node{tree[l].sum + tree[r].sum, l, r};
    }

    int build(int lx, int rx, vector<int>& a) {
        if(lx == rx) {
            tree[timer] = {a[lx], 0, 0};
            return timer++;
        }

        int m = (lx+rx) / 2;

        tree[timer] = join(build(lx, m, a), build(m+1, rx, a));

        return timer++;
    }

    int set(int v, int pos, int val, int l, int r) {
        if(l == r) {
            tree[timer] = {val, 0, 0};
            return timer++;
        }

        int m = (l+r) / 2;
        if(pos <= m) {
            tree[timer] = join(set(tree[v].l, pos, val, l, m), tree[v].r);
        } else {
            tree[timer] = join(tree[v].l, set(tree[v].r, pos, val, m+1, r));
        }

        return timer++;
    }

    ll sum(int v, int l, int r, int lx, int rx) {
        if(l >= lx && r <= rx) return tree[v].sum;
        if(l > rx || r < lx) return 0;

        int m = (l+r) / 2;

        return sum(tree[v].l, l, m, lx, rx) + sum(tree[v].r, m+1, r, lx, rx);
    }

public:
    PersistentSegTree(int n, int mx_nodes) : n(n), tree(mx_nodes) {}

    int build(vector<int>& a) {
        return build(0, n-1, a);
    }

    int set(int root, int pos, int val) {
        return set(root, pos, val, 0, n-1);
    }

    ll sum(int root, int l, int r) {
        return sum(root, 0, n-1, l, r);
    }

    int add_copy(int root) {
        tree[timer] = tree[root];
        return timer++;
    }
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    int n, q; cin >> n >> q;

    vector<int> a(n);
    for(int i = 0; i < n; i++) cin >> a[i];

    const int mx_nodes = 2*n + q*(2 + __lg(n));

    PersistentSegTree t(n, mx_nodes);

    vector<int> roots = {t.build(a)};

    while(q--) {
        int f, k; cin >> f >> k; k--;

        if(f == 1) {
            int pos, val; cin >> pos >> val; pos--;

            roots[k] = t.set(roots[k], pos, val);
        }
        if(f == 2) {
            int l, r; cin >> l >> r; l--; r--;

            cout << t.sum(roots[k], l, r) << '\n';
        }

        if(f == 3) {
            roots.push_back(t.add_copy(roots[k]));
        }
    }
}
