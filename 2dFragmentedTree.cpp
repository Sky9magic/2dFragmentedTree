/********************
 * what  the  sigma *
 ********************/
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;
#define ve vector
#define ll long long
#define ld long double
bool enabledb=0;
#define pb push_back
const ll mod = 1e9+7,maxn=200005;
const ll INF=(ll)9e18;
ll n,m;
struct Node{
    ll val,lazy;
    Node*l,*r;
    Node():val(0),lazy(0),l(nullptr),r(nullptr){}
};
ll cmb1(ll n1,ll n2) {
    return min(n1,n2); // you can change this to any associative operation
}
Node* build(ll L,ll R) {
    Node*node=new Node();
    if(L==R) {
        node->val=0;
        return node;
    }
    ll mid = (L+R)/2;
    node->l=build(L,mid);
    node->r=build(mid+1,R);
    node->val=cmb1(node->l->val,node->r->val);
    return node;
}
Node* upd(Node*node,ll L,ll R,ll ql,ll qr,ll v) {
    if (qr<L||ql>R) return node;
    Node* newNode = new Node(*node);
    if (ql<=L&&qr>=R) {
        newNode->val += v;
        newNode->lazy += v;
        return newNode;
    }
    ll mid=(L+R)/2;
    if (newNode->lazy!=0) {
        newNode->l=upd(newNode->l,L,mid,L,mid,newNode->lazy);
        newNode->r=upd(newNode->r,mid+1,R,mid+1,R,newNode->lazy);
        newNode->lazy=0;
    }
    newNode->l=upd(newNode->l,L,mid,ql,qr,v);
    newNode->r=upd(newNode->r,mid+1,R,ql,qr,v);
    newNode->val=cmb1(newNode->l->val,newNode->r->val);
    return newNode;
}
ll qry(Node*node,ll L,ll R,ll ql,ll qr) {
    if (qr<L||ql>R) return INF; // change this to be default value
    if (ql<=L&&qr>=R) return node->val;
    ll mid=(L+R)/2;
    Node*left,*right;
    if (node->lazy!=0) {
        left=upd(node->l,L,mid,L,mid,node->lazy);
        right=upd(node->r,mid+1,R,mid+1,R,node->lazy);
    } else {
        left=node->l;
        right=node->r;
    }
    return cmb1(qry(left,L,mid,ql,qr),qry(right,mid+1,R,ql,qr));
}
struct Fragment{
    ll L,R;
    Node*segtree;
    Fragment(ll l,ll r,Node*st):L(l),R(r),segtree(st){}
};
struct FragmentCmp{
    bool operator()(const Fragment*a, const Fragment*b) const {
        return a->L<b->L;
    }
};
struct FragmentTree{
    set<Fragment*,FragmentCmp> frags;
    FragmentTree(ll n,ll m_) {
        m=m_;
        Node*st=build(1,m);
        Fragment*init=new Fragment(1,n,st);
        frags.insert(init);
    }
    void update(ll xl,ll xr,ll yl,ll yr,int v) {
        ve<Fragment*> toRemove,toAdd;
        for (auto frag:frags) {
            if (frag->R<xl||frag->L>xr) continue;
            toRemove.push_back(frag);
            if (frag->L<xl) {
                Fragment*upper=new Fragment(frag->L,xl-1,frag->segtree);
                toAdd.push_back(upper);
            }
            ll interL=max(frag->L,xl),interR=min(frag->R,xr);
            Node*updatedST=upd(frag->segtree,1,m,yl,yr,v);
            Fragment*mid=new Fragment(interL,interR,updatedST);
            toAdd.push_back(mid);
            if (frag->R>xr) {
                Fragment*lower=new Fragment(xr+1,frag->R,frag->segtree);
                toAdd.push_back(lower);
            }
        }
        for (auto frag:toRemove) {
            frags.erase(frag);
            delete frag;
        }
        for (auto frag:toAdd) {
            frags.insert(frag);
        }
    }
    ll query(ll xl,ll xr,ll yl,ll yr) {
        ll ans=0;
        for(auto frag:frags) {
            if(frag->R<xl||frag->L>xr) continue;
            ll  interL=max(frag->L,xl),
                interR=min(frag->R,xr),
                rows=(interR-interL+1),
                minVal=qry(frag->segtree,1,m,yl,yr)%mod; // you can remove the modulo if you want to
            ans=(ans+minVal*(rows%mod))%mod; // change this to what you need, or just remove modulo
        }
        return ans;
    }
};
