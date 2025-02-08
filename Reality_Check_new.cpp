/********************
 * what  the  sigma *
 ********************/
#include <iostream>
#include <vector>
#include <set>
using namespace std;
#define lgm cin.tie(0)->sync_with_stdio(0);
#define be(x) x.begin(),x.end()
#define ve vector
#define ll long long
#define ld long double
bool enabledb=0;
#define DB(CODE) cout<<'\t'<<CODE<<endl;
#define SP <<' '<<
#define ull unsigned ll
#define f first
#define s second
#define pii pair<int, int>
#define tii tuple<int,int,int>
#define pll pair<ll,ll>
#define sz(x) (int)x.size()
#define pb push_back
const ll mod = 1e9+7,maxn=200005;
const ll INF=(ll)9e18;
ll n,m;
struct minsegtree{
    int l,r,yl,yr;
    ll val,lazy;
    minsegtree *left,*right;
    minsegtree(int l_,int r_,int yl_,int yr_):l(l_),r(r_),yl(yl_),yr(yr_),val(0),lazy(0),left(NULL),right(NULL){}
    ~minsegtree() {
        if (left) delete left;
        if (right) delete right;
    }
    bool operator<(minsegtree&o)const {
        return l<o.l;
    }
    minsegtree* copy() {
        minsegtree*tmp=new minsegtree(l,r,yl,yr);
        tmp->val=val;
        tmp->lazy=lazy;
        if (left) {
            tmp->left=left->copy();
        }
        if (right) {
            tmp->right=right->copy();
        }
        return tmp;
    }
    minsegtree* divide(int idx) {
        minsegtree* p;
        p=copy();
        r=idx;
        p->l=idx+1;
        return p;
    }
    void push() {
        if (yl!=yr) {
            int mid=(yl+yr)>>1;
            if (left==NULL) {
                left=new minsegtree(l,r,yl,mid);
            }
            left->lazy+=lazy;
            left->val+=lazy;
            if (right==NULL) {
                right=new minsegtree(l,r,mid+1,yr);
            }
            right->lazy+=lazy;
            right->val+=lazy;
        }
        lazy=0;
    }
    void upd(int tl,int tr,int v) {
        if (yl>tr||yr<tl) return;
        if (yl>=tl&&yr<=tr) {
            lazy+=v;
            val+=v;
            return;
        }
        push();
        left->upd(tl,tr,v);
        right->upd(tl,tr,v);
        val=min(left->val,right->val);
    }
    ll query(int tl,int tr) {
        if (yl>tr||yr<tl) return INF;
        if (yl>=tl&&yr<=tr) {
            return val;
        }
        push();
        int mid=(l+r)>>1;
        return min(left->query(tl,tr),right->query(tl,tr));
    }
};
struct fragmentsegtree{
    set<minsegtree*> segtree;
    fragmentsegtree(int n_,int m_){segtree.clear(),segtree.emplace(new minsegtree(1, n_, 1, m_));}
    void upd(int xl,int xr,int yl,int yr,int v) {
        ve<minsegtree*> add;
        for (auto&i:segtree) {
            if (i->l>xr||i->r<xl) continue;
            if (i->l>=xl&&i->r<=xr) {
                i->upd(yl,yr,v);
            } else if (i->l<xl&&i->r>xr) {
                minsegtree*p1=i->divide(xl-1),*p2=p1->divide(xr);
                p1->upd(yl,yr,v);
                add.pb(p1);
                add.pb(p2);
            } else if (i->l<xl&&i->r>=xl) {
                minsegtree*p1=i->divide(xl-1);
                p1->upd(yl,yr,v);
                add.pb(p1);
            } else if (i->r>xr&&i->l<=xr) {
                minsegtree* p1=i->divide(xr);
                i->upd(yl,yr,v);
                add.pb(p1);
            }
        }
        for (auto&i:add) {
            segtree.insert(i);
        }
    }
    ll query(int xl,int xr,int yl,int yr) {
        ll ans=0;
        ll val;
        for (auto&i:segtree) {
            if (i->l>xr||i->r<xl) continue;
            if (i->l<=xl&&i->r>=xr) {
                val=i->query(yl,yr)*(xr-xl+1);
            } else if (i->l<=xl&&i->r>=xl) {
                val=i->query(yl,yr)*(i->r-xl+1);
            } else if (i->l<=xr&&i->r>=xr) {
                val=i->query(yl,yr)*(xr-i->l+1);
            } else if (i->l>=xl&&i->r<=xr) {
                val=i->query(yl,yr)*(i->r-i->l+1);
            }
            ans+=val;
        }
        return ans;
    }
};
int main() {
    lgm;
    int q;
    cin >> n >> m >> q;
    fragmentsegtree f(n,m);
    int t,x0,x1,y0,y1;
    int v;
    while (q--) {
        cin >> t;
        if (t==1) {
            cin >> x0 >> x1 >> y0 >> y1 >> v;
            f.upd(x0,x1,y0,y1,v);
        } else {
            cin >> x0 >> x1 >> y0 >> y1;
            cout << f.query(x0,x1,y0,y1) << '\n';
        }
    }
}