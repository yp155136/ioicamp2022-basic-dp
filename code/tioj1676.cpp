#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 500006;

int n, k;
ll a[N], pre[N], dp[N];

struct Line {
  ll a, b, i; // 編號為 i 的線，斜率是 a ，截距是 b
  Line(ll _a, ll _b, ll _i): a(_a), b(_b), i(_i){}
  ll operator()(const ll x) {
    return a * x + b;
  }
};

ll g(int j) { // 截距
  return dp[j] - j * 1ll * j;
}

ll h(int j) { // 斜率
  return 2 * j;
}

ll DivCeil(ll x, ll y) { // 計算 x / y 取上高斯，不管 x, y 的正負情況都適用
  return x / y + (((x < 0) != (y > 0)) && (x % y));
}

bool check(Line l1, Line l2, Line l3) {
  // 注意到這題點只有定義在整數上，因此可以用下面的技巧來避免浮點數
  ll v1 = DivCeil((l1.b - l2.b), (l2.a - l1.a)); // L_{-2} 與 L_{-1} 的交點。在這邊代表 L_{-1} 第一個超越 L_{-2} 的整數點
  ll v2 = DivCeil((l2.b - l3.b), (l3.a - l2.a)); // L_{-1} 與 L 的交點。在這邊代表 L 第一個超越 L_{-1} 的整數點
  ll v3 = l1.i + k; // L_{-2} 最後一個合法的位置， v3 + 1 就過期了
  return v2 <= v3 && v2 <= v1;
}

void solve() {
  deque<Line> dq;
  dq.push_back(Line(h(0), g(0), 0));
  for (int i = 1; i <= n; ++i) {
    if (dq.front().i == i - k - 1) dq.pop_front();
    while (dq.size() >= 2 && dq[0](i) <= dq[1](i)) dq.pop_front();
    dp[i] = pre[i] - i * 1ll * i + dq.front()(i);
    Line l = Line(h(i), g(i), i);
    while (dq.size() >= 2 && check(dq[(int)dq.size() - 2], dq[(int)dq.size() - 1], l)) {
      dq.pop_back();
    }
    dq.push_back(l);
  }
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  cin >> n >> k;
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }
  reverse(a + 1, a + n + 1);
  for (int i = 1; i <= n; ++i) {
    pre[i] = pre[i - 1] + a[i];
  }
  solve();
  cout << dp[n] - pre[n] << '\n';
}

