// Solution for "Merging Rectangles" problem: https://informatics.msk.ru/mod/statements/view.php?chapterid=3858
// O(nlogn)

#include<bits/stdc++.h>

using namespace std;

#define ll long long

struct Rec {
    int x1, y1, x2, y2;
};

struct Event {
    int x, y1, y2, type;
};

const int INF = int(1e9) + 1;

struct Segtree {
    vector<pair<int, ll>> tree;
    vector<int> add;
    int size = 1;

    pair<int, ll> join(pair<int, ll> a, pair<int, ll> b) {
        if(a.first == b.first) return {a.first, a.second+b.second};
        return min(a, b);
    }

    void build(int v, int l, int r, vector<int>& delta) {
        if(r - l == 1) {
            if(l < delta.size()) tree[v] = {0, delta[l]};
            return;
        }
        int m = (l+r) / 2;
        build(2*v+1, l, m, delta);
        build(2*v+2, m, r, delta);

        tree[v] = join(tree[2*v+1], tree[2*v+2]);
    }

    void init(vector<int>& delta) {
        while(size < delta.size()) size *= 2;

        tree.assign(2*size, {INF, 0});
        add.assign(2*size, 0);

        build(0, 0, size, delta);
    }

    void push(int v) {
        if(add[v] != 0) {
            tree[v].first += add[v];
            if(2*v+1 < tree.size()) add[2*v+1] += add[v];
            if(2*v+2 < tree.size()) add[2*v+2] += add[v];
        }
        add[v] = 0;
    }

    void add_range(int v, int l, int r, int lx, int rx, int val) {
        push(v);
        if(l >= lx && r <= rx) {
            add[v] += val;
            push(v);
            return;
        }
        if(l >= rx || r <= lx) return;
        int m = (l+r) / 2;
        add_range(2*v+1, l, m, lx, rx, val);
        add_range(2*v+2, m, r, lx, rx, val);

        tree[v] = join(tree[2*v+1], tree[2*v+2]);
    }

    void add_range(int l, int r, int val) {
        add_range(0, 0, size, l, r, val);
    }

    pair<int, ll> query(int v, int l, int r, int lx, int rx) {
        push(v);
        if(l >= lx && r <= rx) return tree[v];
        if(l >= rx || r <= lx) return {INF, 0};

        int m = (l+r) / 2;
        return join(query(2*v+1, l, m, lx, rx), query(2*v+2, m, r, lx, rx));
    }

    pair<int, ll> query(int l, int r) {
        return query(0, 0, size, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    int n; cin >> n;

    if(n == 0) {
        cout << 0 << endl;
        return 0;
    }

    vector<Rec> r(n);
    vector<int> ys;

    for(int i = 0; i < n; i++) {
        cin >> r[i].x1 >> r[i].y1 >> r[i].x2 >> r[i].y2;
        ys.push_back(r[i].y1);
        ys.push_back(r[i].y2);
    }

    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int mn = *ys.begin(); int mx = ys.back(); int Y = ys.size();

    vector<int> delta(Y);
    for(int i = 0; i < Y-1; i++) delta[i] = ys[i+1] - ys[i];

    vector<Event> evs;
    for(int i = 0; i < n; i++) {
        auto [x1, y1, x2, y2] = r[i];
        y1 = lower_bound(ys.begin(), ys.end(), y1) - ys.begin();
        y2 = lower_bound(ys.begin(), ys.end(), y2) - ys.begin();

        evs.push_back({x1, y1, y2, 1});
        evs.push_back({x2, y1, y2, -1});
    }

    sort(evs.begin(), evs.end(), [](Event& a, Event& b){
        return a.x < b.x || (a.x == b.x && a.type < b.type);
    });

    Segtree t;
    t.init(delta);

    int prev = evs[0].x;
    ll ans = 0;

    for(auto [x, y1, y2, type]: evs) {

        auto p = t.query(0, Y);
        if(p.first > 0) ans += (mx - mn)*(x - prev);
        else ans += (mx - mn - p.second)*(x - prev);

        if(type == 1) {
            t.add_range(y1, y2, 1);
        } else {
            t.add_range(y1, y2, -1);
        }
        prev = x;
    }

    cout << ans << endl;
}
