#include<bits/stdc++.h>
using namespace std;
 
#define MX 10004
#define MAX 1000006
#define ll long long
 
bool Mark[MAX];
 
struct Node{
    int ans;
    int total;
};
 
Node combine(Node A, Node B){
    Node res;
    res.ans = A.ans + B.ans;
    res.total = A.total + B.total;
    return res;
}
 
int A[MX];
Node tree[4*MX];
int lazy[4*MX];
 
void sieve(){
    Mark[0] = 1;
    Mark[1] = 1;
    for (ll i = 2; i < MAX; i++){
        if (!Mark[i]){
            for (ll j = i * i; j < MAX; j += i){
                Mark[j] = 1;
            }
        }
    }
}
 
void build(int node, int L, int R){
    if (L == R){
        (!Mark[A[L]]) ? tree[node] = {1, 1} : tree[node] = {0, 1};
        lazy[node] = 0;
        return;
    }
    int mid = (L + R) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
    build(left, L, mid);
    build(right, mid+1, R);
    lazy[node] = 0;
    tree[node] = combine(tree[left], tree[right]);
}
 
// function to push parent's lazy value to it's both children
void pushToChildren(int node){
    int left = 2 * node;
    int right = 2 * node + 1;
    int cur = lazy[node];
 
    // updating left and right children with parent's lazy value
    lazy[left] = cur;
    lazy[right] = cur;
    lazy[node] = 0; // making parent's lazy 0
 
    if (!Mark[cur]){ // if lazy value is prime
        tree[left].ans = tree[left].total;
        tree[right].ans = tree[right].total;
    }
    else{ // if not prime, ans is zero
        tree[left].ans = 0;
        tree[right].ans = 0;
    }
}
 
void Update(int node, int L, int R, int l, int r, int v){
    if (L > r || R < l) return;
    if (L >= l && R <= r){
        lazy[node] = v;
        if (!Mark[v]) tree[node].ans = tree[node].total;
        else tree[node].ans = 0;
        return;
    }
    int mid = (L + R) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
 
    // pushing lazy value to child nodes
    if (lazy[node] > 0) pushToChildren(node);
 
    Update(left, L, mid, l, r, v);
    Update(right, mid+1, R, l, r, v);
    tree[node] = combine(tree[left], tree[right]);
}
 
int Query(int node, int L, int R, int l, int r){
    if (L > r || R < l) return 0;
    if (L >= l && R <= r){
        return tree[node].ans;
    }
    int mid = (L + R) / 2;
    int left = 2 * node;
    int right = 2 * node + 1;
    if (lazy[node] > 0) pushToChildren(node);
    return Query(left, L, mid, l, r) + Query(right, mid+1, R, l, r);
}
 
int main()
{
    sieve();
    int T; scanf("%d", &T);
    for (int cs = 1; cs <= T; cs++){
        int n, q; scanf("%d%d", &n, &q);
        for (int i = 1; i <= n; i++){
            scanf("%d", &A[i]);
        }
        build(1, 1, n);
        printf("Case %d:\n", cs);
        while(q--){
            int check; scanf("%d", &check);
            if (check == 0){
                int l, r, v; scanf("%d%d%d", &l, &r, &v);
                Update(1, 1, n, l, r, v);
            }
            else{
                int l, r; scanf("%d%d", &l, &r);
                int ans = Query(1, 1, n, l, r);
                printf("%d\n", ans);
            }
        }
    }
    return 0;
}
