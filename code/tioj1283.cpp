#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pii;

#define F first
#define S second

const int N = 100006;

pii a[N], b[N];

pii operator+(const pii &p1, const pii &p2) {
  return make_pair(p1.F + p2.F, p1.S + p2.S);
}

ll cal(int i, int j) {
  return (a[i].F - b[j].F) * (a[i].S - b[j].S);
}

ll ans = 0;

void dc(int l, int r, int best_l, int best_r) {
  if (l > r) return;
  int mid = (l + r) >> 1;
  int best = -1;
  for (int i = best_l; i <= best_r; ++i) {
    if (best == -1 || cal(mid, i) > cal(mid, best)) best = i;
  }
  ans = max(ans, cal(mid, best));
  dc(l, mid - 1, best_l, best);
  dc(mid + 1, r, best, best_r);
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  int n; cin >> n;
  a[0] = make_pair(0, 0);
  for (int i = 1; i <= n; ++i) {
    int x; cin >> x;
    if (i & 1) a[i] = a[i - 1] + make_pair(x, 0);
    else a[i] = a[i - 1] + make_pair(0, -x);
  }
  int m; cin >> m;
  b[0] = make_pair(0, 0);
  for (int i = 1; i <= m; ++i) {
    int x; cin >> x;
    if ((i & 1) == 0) b[i] = b[i - 1] + make_pair(x, 0);
    else b[i] = b[i - 1] + make_pair(0, -x);
  }
  dc(0, n, 0, m);
  cout << ans << '\n';
}

