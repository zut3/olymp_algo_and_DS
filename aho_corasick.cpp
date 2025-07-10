#include<bits/stdc++.h>

using namespace std;

#define ll long long

const int k = 26;

struct Node {
    Node* to[k] = {0};
    Node* go[k] = {0};

    Node* sufflink = nullptr;
    Node* p = nullptr;

    int pch;
    bool is_terminal = false;
    bool used = false;
    vector<int> ids;

    Node() {}
    Node(int c, Node* p) : pch(c), p(p) {}

} *root = new Node(-1, 0);

void add_string(string& s, int i) {
    Node* v = root;
    for(char _c: s) {
        int c = int(_c - 'a');

        if(!v->to[c]) {
            v->to[c] = new Node(c, v);
        }
        v = v->to[c];
    }
    v->is_terminal = true;
    v->ids.push_back(i);
}

vector<Node*> q;

void BFS() {
    q.push_back(root);

    int cur = 0;
    while(cur < q.size()) {
        Node* v = q[cur];

        if(v == root || v->p == root) v->sufflink = root;
        else {
            v->sufflink = v->p->sufflink->go[v->pch];
        }

        for(int c = 0; c < k; c++) {
            if(v->to[c]) {
                v->go[c] = v->to[c];
                q.push_back(v->to[c]);
            } else if(v == root) {
                v->go[c] = root;
            } else {
                v->go[c] = v->sufflink->go[c];
            }
        }
        cur++;
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    string t; cin >> t;

    int m; cin >> m;

    for(int i = 0; i < m; i++) {
        string s; cin >> s;
        add_string(s, i);
    }

    BFS();

    Node* v = root;
    for(int i = 0; i < t.size(); i++) {
        v = v->go[t[i] - 'a'];
        v->used = true;
    }

    vector<bool> ans(m);

    reverse(q.begin(), q.end());
    for(Node* v: q) {
        for(int id: v->ids) ans[id] = v->used;
        if(v->used) v->sufflink->used = v->used;
    }

    for(int i = 0; i < m; i++) {
        if(ans[i]) cout << "YES\n";
        else cout << "NO\n";
    }
}
