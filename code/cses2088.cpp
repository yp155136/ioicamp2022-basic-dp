#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 5006;

int K[N][N];
ll dp[N][N];

ll x[N], pre[N];

ll w(int i, int j) {
  return pre[j] - pre[i - 1];
}

void solve(int n) {
  memset(K, -1, sizeof(K));
  for (int len = 1; len <= n; ++len) {
    for (int i = 1; i + len - 1 <= n; ++i) {
      int j = i + len - 1;
      if (len == 1) {
        dp[i][j] = 0;
        K[i][j] = i;
      }
      else {
        int kl = K[i][j - 1];
        int kr = K[i + 1][j];
        for (int k = kl; k <= kr; ++k) {
          if (K[i][j] == -1) {
            K[i][j] = k;
            dp[i][j] = w(i, j) + dp[i][k] + dp[k + 1][j];
          }
          else if (w(i, j) + dp[i][k] + dp[k + 1][j] < dp[i][j]) {
            dp[i][j] = w(i, j) + dp[i][k] + dp[k + 1][j];
            K[i][j] = k;
          }
        }
      }
    }
  }
}

int main () {
  int n; cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> x[i];
    pre[i] = pre[i - 1] + x[i];
  }
  solve(n);
  cout << dp[1][n] << '\n';
}

