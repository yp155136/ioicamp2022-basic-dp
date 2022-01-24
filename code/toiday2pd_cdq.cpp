#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 200006;
const ll INF = (1ll << 60);
ll a[N], pre[N], dp[N];
int n, k;

ll cal(int l, int r) { // dp[r] = dp[l] + ...
  if (r - l < k) return INF;
  int mid = ((l + r) >> 1) + 1;
  return dp[l] + (a[mid] * (mid - l) - (pre[mid] - pre[l])) + (pre[r] - pre[mid] - a[mid] * (r - mid));
}

void go(int l, int r, int best_l, int best_r) {
  if (l > r) return;
  int mid = (l + r) >> 1;
  int best = -1;
  for (int i = best_l; i <= best_r; ++i) {
    if (best == -1 || cal(best, mid) > cal(i, mid)) best = i;
  }
  dp[mid] = min(dp[mid], cal(best, mid));
  go(l, mid - 1, best_l, best);
  go(mid + 1, r, best, best_r);
}

void dc(int l, int r) {
  if (l == r) return;
  int mid = (l + r) >> 1;
  dc(l, mid);
  go(mid + 1, r, l, mid);
  dc(mid + 1, r);
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
    dp[i] = INF;
  }
  dc(0, n);
  cout << dp[n] << '\n';
}


