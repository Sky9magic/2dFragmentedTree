#pragma GCC optimize("O3", "unroll-loops")
#include <iostream>
#include <set>
#include <vector>
using namespace std;
typedef long long ll;
#define lgm cin.tie(0)->sync_with_stdio(0);
const ll INF = 9e18;
const ll MOD = 1e9+7;
ll m;
struct Node {
    ll val, lazy;
    Node *l, *r;
    Node(): val(0), lazy(0), l(nullptr), r(nullptr) {}
};
Node* build(ll L, ll R) {
    Node* node = new Node();
    if(L == R) {
        node->val = 0;
        return node;
    }
    ll mid = (L + R) / 2;
    node->l = build(L, mid);
    node->r = build(mid+1, R);
    node->val = min(node->l->val, node->r->val);
    return node;
}
Node* upd(Node* node, ll L, ll R, ll ql, ll qr, ll v) {
    if (qr < L || ql > R) return node;
    Node* newNode = new Node(*node);
    if (ql <= L && R <= qr) {
        newNode->val += v;
        newNode->lazy += v;
        return newNode;
    }
    ll mid = (L + R) / 2;
    if(newNode->lazy != 0) {
        newNode->l = upd(newNode->l, L, mid, L, mid, newNode->lazy);
        newNode->r = upd(newNode->r, mid+1, R, mid+1, R, newNode->lazy);
        newNode->lazy = 0;
    }
    newNode->l = upd(newNode->l, L, mid, ql, qr, v);
    newNode->r = upd(newNode->r, mid+1, R, ql, qr, v);
    newNode->val = min(newNode->l->val, newNode->r->val);
    return newNode;
}
ll qry(Node* node, ll L, ll R, ll ql, ll qr) {
    if (qr < L || ql > R) return INF;
    if (ql <= L && R <= qr) return node->val;
    ll mid = (L + R) / 2;
    Node* left;
    Node* right;
    if(node->lazy != 0) {
        left = upd(node->l, L, mid, L, mid, node->lazy);
        right = upd(node->r, mid+1, R, mid+1, R, node->lazy);
    } else {
        left = node->l;
        right = node->r;
    }
    return min(qry(left, L, mid, ql, qr),qry(right, mid+1, R, ql, qr));
}
struct Fragment {
    ll L, R;
    Node* segtree;
    Fragment(ll l, ll r, Node* st) : L(l), R(r), segtree(st) {}
};
struct FragmentCmp {
    bool operator()(const Fragment* a, const Fragment* b) const {
        return a->L < b->L;
    }
};
struct FragmentTree {
    set<Fragment*, FragmentCmp> frags;
    FragmentTree(ll n, ll m_) {
        m = m_;
        Node* st = build(1, m);
        Fragment* initial = new Fragment(1, n, st);
        frags.insert(initial);
    }
    void update(ll xl, ll xr, ll yl, ll yr, int v) {
        vector<Fragment*> toRemove;
        vector<Fragment*> toAdd;
        for(auto frag : frags) {
            if(frag->R < xl || frag->L > xr) continue;
            toRemove.push_back(frag);
            if(frag->L < xl) {
                Fragment* upper = new Fragment(frag->L, xl - 1, frag->segtree);
                toAdd.push_back(upper);
            }
            ll interL = max(frag->L, xl);
            ll interR = min(frag->R, xr);
            Node* updatedST = upd(frag->segtree, 1, m, yl, yr, v);
            Fragment* mid = new Fragment(interL, interR, updatedST);
            toAdd.push_back(mid);
            if(frag->R > xr) {
                Fragment* lower = new Fragment(xr + 1, frag->R, frag->segtree);
                toAdd.push_back(lower);
            }
        }
        for(auto frag : toRemove) {
            frags.erase(frag);
            delete frag;
        }
        for(auto frag : toAdd) {
            frags.insert(frag);
        }
    }
    ll query(ll xl, ll xr, ll yl, ll yr) {
        ll ans = 0;
        for(auto frag : frags) {
            if(frag->R < xl || frag->L > xr) continue;
            ll interL = max(frag->L, xl);
            ll interR = min(frag->R, xr);
            ll rows = (interR - interL + 1);
            ll minVal = qry(frag->segtree, 1, m, yl, yr)%MOD;
            ans = (ans + minVal * (rows % MOD)) % MOD;
        }
        return ans;
    }
};
int main(){
    lgm;
    ll n, q;
    cin >> n >> m >> q;
    FragmentTree ft(n, m);
    while(q--){
        int t;
        cin >> t;
        if(t == 1) {
            ll x0, x1;
            int y0, y1, v;
            cin >> x0 >> x1 >> y0 >> y1 >> v;
            ft.update(x0, x1, y0, y1, v);
        } else {
            ll x0, x1;
            int y0, y1;
            cin >> x0 >> x1 >> y0 >> y1;
            cout << ft.query(x0, x1, y0, y1) << "\n";
        }
    }
    return 0;
}
