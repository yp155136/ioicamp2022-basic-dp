#include <bits/stdc++.h>
using namespace std;

const int N = 500006;

int n;

struct Node {
  Node *lc, *rc;
  int sum;
  Node(): lc(NULL), rc(NULL), sum(0){}
  void pull() {
    sum = lc->sum + rc->sum;
  }
};

Node* Build(int L, int R) {
  Node* node = new Node();
  if (L == R) return node;
  int mid = (L + R) >> 1;
  node->lc = Build(L, mid);
  node->rc = Build(mid + 1, R);
  return node;
}

Node* getNode(Node* old) {
  Node* node = new Node();
  node->lc = old->lc;
  node->rc = old->rc;
  node->sum = old->sum;
  return node;
}

void modify(Node* old, Node* node, int L, int R, int pos, int val) {
  if (L == R) {
    node->sum += val;
    return;
  }
  int mid = (L + R) >> 1;
  if (pos <= mid) {
    node->lc = getNode(old->lc);
    modify(old->lc, node->lc, L, mid, pos, val);
  }
  else {
    node->rc = getNode(old->rc);
    modify(old->rc, node->rc, mid + 1, R, pos, val);
  }
  node->pull();
  return;
}

int query(Node* node, int L, int R, int l, int r) {
  if (l > R || L > r) return 0;
  else if (l <= L && R <= r) return node->sum;
  int mid = (L + R) >> 1;
  return query(node->lc, L, mid, l, r) + query(node->rc, mid + 1, R, l, r);
}

Node* root[N];
vector<int> G[N];

int dp[N];
int a[N];

int Z(int l, int r) {
  if (l > r) return 0;
  return query(root[r], 1, n, r, n) - query(root[l - 1], 1, n, r, n);
}

int cal(int l, int r) {
  if (l > r) return 0x3f3f3f3f;
  return dp[l] + Z(a[l] + 1, a[r]);
}

struct Seg {
  int pos, l, r;  // 轉移點為 pos ，在區間 [l, r] 有最優解
  Seg(int _pos, int _l, int _r): pos(_pos), l(_l), r(_r){}
};

// 假設 cal(i, j) 是計算從轉移點 i 轉到 j 的代價

void go(int m) {
  stack<Seg> sta;
  sta.push(Seg(0, 1, m));
  for (int j = 1; j <= m; ++j) {
    while (!sta.empty() && sta.top().r < j) sta.pop(); // 把過期的最佳解丟掉
    dp[j] = cal(sta.top().pos, j) - a[j]; // 平常的題目應該是不用 -a[j] 這項的，這邊 -a[j] 是因應題目要求
    
    while (!sta.empty() && cal(sta.top().pos, sta.top().r) > cal(j, sta.top().r)) {
      // 把被完全覆蓋的線段刪掉
      sta.pop();
    }
    if (sta.empty()) {
      sta.push(Seg(j, j + 1, m));
    }
    else {
      Seg seg = sta.top(); sta.pop();
      // 二分搜斷點
      int l = seg.l - 1, r = seg.r;
      while (r - l > 1) {
        int mid = (l + r) >> 1;
        if (cal(seg.pos, mid) >= cal(j, mid)) l = mid;
        else r = mid;
      }
      sta.push(Seg(seg.pos, r, seg.r));
      if (j + 1 <= l) sta.push(Seg(j, j + 1, l));
    }
  }
}

void gogo() {
  int m; cin >> m;
  for (int i = 1; i <= m; ++i) {
    cin >> a[i];
    dp[i] = 0x3f3f3f3f;
  }
  dp[0] = 0;
  sort(a + 1, a + m + 1);
  go(m);
  if (*min_element(dp + 1, dp + m + 1) < 0) cout << 0 << '\n';
  else cout << 1 << '\n';
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    int x, y; cin >> x >> y;
    G[x].push_back(y);
  }
  root[0] = Build(1, n);
  for (int i = 1; i <= n; ++i) {
    root[i] = getNode(root[i - 1]);
    for (int y : G[i]) {
      modify(root[i], root[i], 1, n, y, 1);
    }
  }
  int q; cin >> q;
  while (q--) {
    gogo();
  }
}


