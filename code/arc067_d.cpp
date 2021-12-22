#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 5006;
const int M = 206;
const int P = 14;

int n, m;
ll a[N], pre_a[N];
int b[N][M];
int lg_form[N];

struct Sparse_table {
  int sp[P][N];
  void init(int _id) {
    for (int i = 1; i <= n; ++i) {
      sp[0][i] = b[i][_id];
    }
    for (int i = 1; i < P; ++i) {
      for (int j = 1; j <= n; ++j) {
        int to = min(n, j + (1 << (i - 1)));
        sp[i][j] = max(sp[i - 1][j], sp[i - 1][to]);
      }
    }
  }
  int query(int l, int r) {
    int lg = lg_form[r - l + 1];
    return max(sp[lg][l], sp[lg][r - (1 << lg) + 1]);
  }
} sp[M];

ll f(int i, int j) {
  if (i > j) return -(1ll << 60);
  ll ret = -(pre_a[j] - pre_a[i]);
  for (int x = 1; x <= m; ++x) {
    ret += sp[x].query(i, j);
  }
  return ret;
}

int h[N];

void dc(int L, int R, int best_L, int best_R) {
  if (L > R) return;
  int mid = (L + R) >> 1;
  for (int i = best_L; i <= best_R; ++i) {
    if (h[mid] == 0 || f(mid, i) > f(mid, h[mid])) h[mid] = i;
  }
  dc(L, mid - 1, best_L, h[mid]);
  dc(mid + 1, R, h[mid], best_R);
}

int main () {
  lg_form[1] = 0;
  for (int i = 2; i < N; ++i) {
    lg_form[i] = lg_form[i >> 1] + 1;
  }
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n >> m;
  for (int i = 2; i <= n; ++i) {
    cin >> a[i];
    pre_a[i] = pre_a[i - 1] + a[i];
  }
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      cin >> b[i][j];
    }
  }
  for (int i = 1; i <= m; ++i) {
    sp[i].init(i);
  }
  dc(1, n, 1, n);
  ll ans = 0;
  for (int i = 1; i <= n; ++i) {
    ans = max(ans, f(i, h[i]));
  }
  cout << ans << endl;
}


