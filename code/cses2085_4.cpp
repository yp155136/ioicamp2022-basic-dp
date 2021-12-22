#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Line {
  mutable ll a, b, l; // 直線為 ax + b ，有效區間的左界為 l
  Line(ll _a, ll _b, ll _l): a(_a), b(_b), l(_l){}
  bool operator<(const Line &rhs) const {
    return a < rhs.a;
  }
  bool operator<(ll rhs_l) const {
    return l < rhs_l;
  }
};

struct ConvexHullMax : std::multiset<Line, std::less<>> {
  static const ll INF = (1ll << 60);
  static ll DivCeil(ll a, ll b) { // a / b 取上高斯
    return a / b - ((a ^ b) < 0 && a % b);
  }
  bool Intersect(iterator x, iterator y) { // 用斜率相鄰的兩條線 x, y 來更新 x 的有效區間
    if (y == end()) {
      x->l = INF;
      return false;
    }
    if (x->a == y->a) {
      x->l = x->b > y->b ? INF : -INF;
    }
    else {
      x->l = DivCeil(y->b - x->b, x->a - y->a);
    }
    return x->l >= y->l; // 代表斜率比較低的線的有效區間比較好，可以把斜率比較高的線給殺掉
  }
  void Insert(ll a, ll b) {
    auto z = insert(Line(a, b, 0)), y = z++, x = y;
    while (Intersect(y, z)) z = erase(z);
    if (x != begin() && Intersect(--x, y)) Intersect(x, y = erase(y));
    while ((y = x) != begin() && (--x)->l >= y->l) Intersect(x, erase(y));
  }
  ll query(ll x) const {
    auto l = *lower_bound(x);
    return l.a * x + l.b;
  }
} convexhull;

const int N = 200006;
ll s[N], f[N], dp[N];

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  int n; cin >> n >> f[0];
  for (int i = 1; i <= n; ++i) {
    cin >> s[i]; s[i] = -s[i];
  }
  for (int i = 1; i <= n; ++i) {
    cin >> f[i];
  }
  convexhull.Insert(f[0], dp[0]);
  for (int i = 1; i <= n; ++i) {
    dp[i] = convexhull.query(s[i]);
    convexhull.Insert(f[i], dp[i]);
  }
  cout << -dp[n] << '\n';
}

