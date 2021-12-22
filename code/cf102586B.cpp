#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 200006;

int n;
ll s;
ll a[N];
ll pre[N];
int bound[N]; // len that can save all
ll pre_1[N];
ll suf_1[N];


ll Q(int l, int r) {
  if (l == 0) return pre[r];
  return pre[r] - pre[l - 1];
}

ll Q_pre(int l, int r) {
  return pre_1[r] - pre_1[l - 1] - Q(l, r) * l;
}

ll Q_suf(int l, int r) {
  return suf_1[l] - suf_1[r + 1] - Q(l, r) * (n - r + 1);
}

ll gr(int r, int x) {
  if (x + bound[x] <= r) {
    if (bound[x] == 0) return 0;
    else {
      ll left = s - Q(x - bound[x] + 1, x + bound[x] - 1);
      return Q_pre(x, x + bound[x] - 1) + Q_suf(x - bound[x] + 1, x) + left * bound[x];
    }
  }
  else {
    int len = r - x;
    ll left = s - Q(x - len, x + len);
    return Q_pre(x, x + len) + Q_suf(x - len, x) + left * (len + 1);
  }
}

ll gl(int l, int x) {
  if (l <= x - bound[x]) {
    if (bound[x] == 0) return 0;
    else {
      ll left = s - Q(x - bound[x] + 1, x + bound[x] - 1);
      return Q_pre(x, x + bound[x] - 1) + Q_suf(x - bound[x] + 1, x) + left * bound[x];
    }
  }
  else {
    int len = x - l;
    ll left = s - Q(x - len, x + len);
    return Q_pre(x, x + len) + Q_suf(x - len, x) + left * (len + 1);
  }
}

typedef pair<int, int> pii;
#define F first
#define S second

struct Node {
  Node *lc, *rc;
  vector<pii> l, r;
  Node(): lc(NULL), rc(NULL){}
};

Node* Build(int L, int R) {
  Node* node = new Node();
  if (L == R) {
    return node;
  }
  int mid = (L + R) >> 1;
  node->lc = Build(L, mid);
  node->rc = Build(mid + 1, R);
  return node;
}

void modify(Node* node, int L, int R, int l, int r, int type, pii x) {
  if (l > R || L > r) return;
  else if (l <= L && R <= r) {
    if (type == 1) {
      node->l.push_back(x);
    }
    else {
      node->r.push_back(x);
    }
    return;
  }
  int mid = (L + R) >> 1;
  modify(node->lc, L, mid, l, r, type, x);
  modify(node->rc, mid + 1, R, l, r, type, x);
}

int ql[N], qr[N];
int now_type;
int ans[N];

ll f(int i, int j) {
  if (now_type == 1) return -gl(i, j);
  else return -gr(i, j);
}

void smawk(vector<int> &r, vector<int> &c);

void interpolate(vector<int> &r, vector<int> &c) {
  vector<int> new_r;
  for (int i = 1; i < (int)r.size(); i += 2) {
    new_r.push_back(r[i]);
  }
  smawk(new_r, c);
  int ptr = 0;
  for (int i = 0; i < (int)r.size(); i += 2) {
    int ansl = c[0], ansr = c[(int)c.size() - 1];
    if (i - 1 >= 0) {
      ansl = ans[ r[i - 1] ];
    }
    if (i + 1 <= (int)r.size() - 1) {
      ansr = ans[ r[i + 1] ];
    }
    ans[ r[i] ] = ansl;
    for (; ptr < (int)c.size() && c[ptr] != ansr; ++ptr) {
      if (f(r[i], ans[ r[i] ]) > f(r[i], c[ptr])) {
        ans[ r[i] ] = c[ptr];
      }
    }
    if (f(r[i], ans[ r[i] ]) > f(r[i], c[ptr])) {
      ans[ r[i] ] = c[ptr];
    }
  }
}

void reduce(vector<int> &r, vector<int> &c) {
  vector<int> new_c;
  for (int j = 0; j < (int)c.size(); ++j) {
    bool pushed = true;
    while (!new_c.empty()) {
      int i = (int)new_c.size() - 1;
      if (f(r[i], c[new_c.back()]) >= f(r[i], c[j])) {
        new_c.pop_back();
        continue;
      }
      else if (i == (int)r.size() - 1) {
        pushed = false;
        break;
      }
      else {
        break;
      }
    }
    if (pushed) {
      new_c.push_back(j);
    }
  }
  for (int &j : new_c) {
    j = c[j];
  }
  smawk(r, new_c);
}

void smawk(vector<int> &r, vector<int> &c) {
  if (max(r.size(), c.size()) <= 2) {
    for (int i : r) {
      ans[i] = c[0];
      for (int j : c) {
        if (f(i, j) < f(i, ans[i])) {
          ans[i] = j;
        }
      }
    }
    return;
  }
  else if (c.size() <= r.size()) {
    interpolate(r, c);
  }
  else {
    reduce(r, c);
  }
}

ll real_ans[N];

void dfs(Node* root, int L, int R) {
  if (!root->l.empty()) {
    now_type = 1;
    sort(root->l.begin(), root->l.end());
    vector<int> r, c;
    for (int i = L; i <= R; ++i) {
      c.push_back(i);
    }
    for (pii p : root->l) {
      r.push_back(p.first);
    }
    smawk(r, c);
    for (pii p : root->l) {
      int i = p.first;
      int id = p.second;
      real_ans[id] = max(real_ans[id], -f(i, ans[i]));
    }
  }
  if (!root->r.empty()) {
    now_type = 2;
    sort(root->r.begin(), root->r.end());
    vector<int> r, c;
    for (int i = L; i <= R; ++i) {
      c.push_back(i);
    }
    for (pii p : root->r) {
      r.push_back(p.first);
    }
    smawk(r, c);
    for (pii p : root->r) {
      int i = p.first;
      int id = p.second;
      real_ans[id] = max(real_ans[id], -f(i, ans[i]));
    }
  }
  if (L == R) {
    return;
  }
  else {
    int mid = (L + R) >> 1;
    dfs(root->lc, L, mid);
    dfs(root->rc, mid + 1, R);
  }
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n >> s;
  a[0] = a[n + 1] = s + 1;
  pre[0] = a[0];
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
    pre[i] = pre[i - 1] + a[i];
  }
  pre[n + 1] = pre[n] + a[n + 1];
  for (int i = 1; i <= n; ++i) {
    int l = -1, r = min(i, n + 1 - i);
    while (r - l > 1) {
      int mid = (l + r) >> 1;
      int bl = i - mid, br = i + mid;
      if (Q(bl, br) >= s) {
        r = mid;
      }
      else {
        l = mid;
      }
    }
    bound[i] = r;
  }
  for (int i = 1; i <= n; ++i) {
    pre_1[i] = pre_1[i - 1] + i * a[i];
  }
  for (int i = n; i >= 1; --i) {
    suf_1[i] = suf_1[i + 1] + (n - i + 1) * a[i];
  }
  /*
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      int v = 0;
      if (i <= j) {
        v = gl(i, j);
      }
      cout << setw(4) << -v << ' ';
    }
    cout << endl;
  }
  cout << "\n-----------------------------\n";
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= n; ++j) {
      int v = 0;
      if (i >= j) {
        v = gr(i, j);
      }
      cout << setw(4) << -v << ' ';
    }
    cout << endl;
  }
  */
  Node* root = Build(1, n);
  int q; cin >> q;
  for (int i = 1; i <= q; ++i) {
    cin >> ql[i] >> qr[i];
    int mid = (ql[i] + qr[i]) >> 1;
    modify(root, 1, n, ql[i], mid, 1, make_pair(ql[i], i));
    if (ql[i] != qr[i]) modify(root, 1, n, mid + 1, qr[i], 2, make_pair(qr[i], i));
  }
  dfs(root, 1, n);
  for (int i = 1; i <= q; ++i) {
    cout << real_ans[i] << '\n';
  }
}

