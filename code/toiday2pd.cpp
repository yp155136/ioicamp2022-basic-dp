#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 200006;
const ll INF = (1ll << 60);
ll a[N], pre[N];
ll dp[N];
int n, k;

ll cal(int l, int r) { // dp[r] = dp[l] + ...
  if (r - l < k) return INF;
  int mid = ((l + r) >> 1) + 1;
  return dp[l] + (a[mid] * (mid - l) - (pre[mid] - pre[l])) + (pre[r] - pre[mid] - a[mid] * (r - mid));
}

struct Seg {
  int pos, l, r;
  Seg(int _pos, int _l, int _r): pos(_pos), l(_l), r(_r){}
};

void solve() {
  deque<Seg> dq;
  dq.push_back(Seg(0, 1, n));
  for (int i = 1; i <= n; ++i) {
    while (!dq.empty() && dq[0].r < i) dq.pop_front();
    dp[i] = cal(dq[0].pos, i);
    while (!dq.empty() && cal(dq.back().pos, dq.back().l) > cal(i, dq.back().l)) {
      dq.pop_back();
    }
    if (dq.empty()) {
      dq.push_back(Seg(i, i + 1, n));
    }
    else {
      Seg seg = dq.back(); dq.pop_back();
      int l = seg.l, r = seg.r + 1;
      while (r - l > 1) {
        int mid = (l + r) >> 1;
        if (cal(seg.pos, mid) > cal(i, mid)) r = mid;
        else l = mid;
      }
      dq.push_back(Seg(seg.pos, seg.l, l));
      if (l != n) dq.push_back(Seg(i, l + 1, n));
    }
  }
}


int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  sort(a + 1, a + n + 1);
  for (int i = 1; i <= n; ++i) {
    pre[i] = pre[i - 1] + a[i];
  }
  solve();
  cout << dp[n] << '\n';
}

