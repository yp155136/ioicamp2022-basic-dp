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
  int pos, l, r;  // 轉移點為 pos ，在區間 [l, r] 有最優解
  Seg(int _pos, int _l, int _r): pos(_pos), l(_l), r(_r){}
};

// 假設 cal(i, j) 是計算從轉移點 i 轉到 j 的代價

void solve() {
  deque<Seg> dq;
  dq.push_back(Seg(0, 1, n));
  for (int j = 1; j <= n; ++j) {
    while (!dq.empty() && dq[0].r < j) dq.pop_front(); // 把過期的最佳解丟掉
    dp[j] = cal(dq[0].pos, j);
    while (!dq.empty() && cal(dq.back().pos, dq.back().l) > cal(j, dq.back().l)) {
      // 把被完全覆蓋的線段刪掉
      dq.pop_back();
    }
    if (dq.empty()) {
      dq.push_back(Seg(j, j + 1, n));
    }
    else {
      Seg seg = dq.back(); dq.pop_back();
      // 二分搜斷點
      int l = seg.l, r = seg.r + 1;
      while (r - l > 1) {
        int mid = (l + r) >> 1;
        if (cal(seg.pos, mid) > cal(j, mid)) r = mid;
        else l = mid;
      }
      dq.push_back(Seg(seg.pos, seg.l, l));
      if (l != n) dq.push_back(Seg(j, l + 1, n));
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

