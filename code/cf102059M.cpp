#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 250006;
const ll INF = (1ll << 60);
vector<pii> G[N];
pii dp[N][2];
int n, k;

typedef pair<ll, ll> pii;
#define F first
#define S second

pii operator+(const pii &p1, const pii &p2) {
  return make_pair(p1.F + p2.F, p1.S + p2.S);
}

pii operator-(const pii &p1, const pii &p2) {
  return make_pair(p1.F - p2.F, p1.S - p2.S);
}

void dfs(int v, int par, ll K) {
  pii sum = make_pair(0, 0);
  for (pii p : G[v]) {
    int u = p.F;
    if (u == par) continue;
    dfs(u, v, K);
    sum = sum + max(dp[u][0], dp[u][1]);
  }
  dp[v][0] = sum;  // (DP值, 選了多少條邊)
  dp[v][1] = make_pair(-INF, 0);
  for (pii p : G[v]) {
    int u = p.F, w = p.S;
    if (u == par) continue;
    dp[v][1] = max(dp[v][1], dp[u][0] + make_pair(w + K, 1) + (sum - max(dp[u][0], dp[u][1])));
  }
}

const ll C = 1000000ll * N;

void solve() {
  ll l = -C - 5, r = C + 5;
  while (r - l > 1) {
    ll mid = (l + r) >> 1;
    dfs(1, 1, mid);
    pii dp_max = max(dp[1][0], dp[1][1]);
    if (dp_max.S == k) {
      cout << dp_max.F - mid * k << '\n';
      return;
    }
    else if (dp_max.S > k) r = mid;
    else l = mid;
  }
  if (r > C) cout << "Impossible" << '\n';
  else {
    dfs(1, 1, l);
    pii p1 = max(dp[1][0], dp[1][1]);
    dfs(1, 1, r);
    pii p2 = max(dp[1][0], dp[1][1]);
    // 內插！
    ll dp_1 = p1.F - l * p1.S;
    ll dp_2 = p2.F - r * p2.S;
    cout << (dp_1 + (dp_2 - dp_1) * (k - p1.S) / (p2.S - p1.S)) << '\n';
  }
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i < n; ++i) {
    int u, v, w; cin >> u >> v >> w;
    G[u].push_back(make_pair(v, w));
    G[v].push_back(make_pair(u, w));
  }
  solve();
}

