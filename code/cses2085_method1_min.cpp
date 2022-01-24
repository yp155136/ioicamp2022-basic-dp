#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = (1ll << 60);

ll DivCeil(ll n, ll d) {
  return n / d + (((n < 0) != (d > 0)) && (n % d));
}

struct Line {
  static bool flag;
  ll m, b, l, r;  // y = mx + b, [l, r)
  Line(ll _m, ll _b, ll _l, ll _r): m(_m), b(_b), l(_l), r(_r){}
  ll operator()(ll x) const {
    return m * x + b;
  }
  bool operator<(const Line &l1) const {
    return flag ? make_pair(-m, -b) < make_pair(-l1.m, -l1.b) : l < l1.l;
  }
  ll Intersect(const Line &l1) {
    return DivCeil(b - l1.b, l1.m - m);
  }
};
bool Line::flag = true;

struct ConvexHullMin {
  set<Line> lines;
  void init() {
    Line::flag = true;
    lines.clear();
  }
  void InsertLine(ll m, ll b) {
    Line now = Line(m, b, -INF, INF);
    if (lines.empty()) {
      lines.insert(now);
      return;
    }
    Line::flag = true;
    auto it = lines.lower_bound(now);
    auto prv = (it == lines.begin() ? it : prev(it));
    if (it != lines.end() &&
        ((it != lines.begin() && (*it)(it->l) <= now(it->l) && 
         (*prv)(prv->r - 1) <= now(prv->r - 1)) || 
         (it == lines.begin() && it->m == now.m))) return;
    if (it != lines.begin()) {
      for (; prv != lines.begin() && (*prv)(prv->l) >= now(prv->l);) {
        prv = --lines.erase(prv);
      }
      if (prv == lines.begin() && now.m == prv->m) {
        lines.erase(prv);
      }
    }
    if (it != lines.end()) {
      while (it != prev(lines.end()) && (*it)(it->r) >= now(it->r)) {
        it = lines.erase(it);
      }
    }
    if (it != lines.begin()) {
      Line new_prv = *prev(it);
      new_prv.r = now.l = new_prv.Intersect(now);
      lines.erase(prev(it)); lines.insert(new_prv);
    }
    if (it != lines.end()) {
      Line new_it = *it;
      new_it.l = now.r = new_it.Intersect(now);
      lines.erase(it); lines.insert(new_it);
    }
    lines.insert(now);
  }
  ll query(ll x) {
    if (lines.empty()) return -INF;
    Line::flag = false;
    auto it = --lines.upper_bound({0, 0, x, 0});
    return (*it)(x);
  }
} convexhull;

const int N = 200006;
ll s[N], f[N], dp[N];

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  int n; cin >> n >> f[0];
  for (int i = 1; i <= n; ++i) {
    cin >> s[i];
  }
  for (int i = 1; i <= n; ++i) {
    cin >> f[i];
  }
  convexhull.InsertLine(f[0], dp[0]);
  for (int i = 1; i <= n; ++i) {
    dp[i] = convexhull.query(s[i]);
    convexhull.InsertLine(f[i], dp[i]);
  }
  cout << dp[n] << '\n';
}

