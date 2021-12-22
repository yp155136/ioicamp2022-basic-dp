#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = (1ll << 60);

ll DivCeil(ll x, ll y) { // x / y 取上高斯
  return x / y + (((x < 0) != (y > 0)) && (x % y));
}

struct Line {
  static bool flag;  // true 代表搜斜率， false 代表搜線段
  ll a, b, l, r;  // y = mx + b, 有效查詢區間為 [l, r)
  Line(ll _a, ll _b, ll _l, ll _r): a(_a), b(_b), l(_l), r(_r){}
  ll operator()(ll x) const {
    return a * x + b;
  }
  bool operator<(const Line &l1) const {
    // 搜斜率的話，直接拿 pair(斜率, 截距) 做比較
    // 否則，拿線段的左界當比較
    return flag ? make_pair(a, b) < make_pair(l1.a, l1.b) : l < l1.l;
  }
  ll Intersect(const Line &l1) {
    // 計算這條線與 l1 的 x 軸交點，取上高斯
    return DivCeil(b - l1.b, l1.a - a);
  }
};
bool Line::flag = true;

struct ConvexHullMax {
  set<Line> lines;
  void init() {
    Line::flag = true;
    lines.clear();
  }
  void InsertLine(ll a, ll b) { // 插入 ax + b 這條線
    Line now = Line(a, b, -INF, INF);  // 一開始預設的範圍為 [-INF, INF)
    if (lines.empty()) {
      lines.insert(now);
      return;
    }
    Line::flag = true;
    auto it = lines.lower_bound(now);  // 第一個斜率比較大的線
    auto prv = (it == lines.begin() ? it : prev(it)); // 前面的線
    if (it != lines.end() &&
        ((it != lines.begin() && (*it)(it->l) >= now(it->l) && 
         (*prv)(prv->r - 1) >= now(prv->r - 1)) || 
         (it == lines.begin() && it->a == now.a))) return;
    // 上面那段是判斷 now 有沒有可能需要加入凸包中，不可能的話就直接 return;
    if (it != lines.begin()) { // 蓋掉前面的線
      for (; prv != lines.begin() && (*prv)(prv->l) <= now(prv->l);) {
        prv = --lines.erase(prv);
      }
      if (prv == lines.begin() && now.a == prv->a) {
        lines.erase(prv);
      }
    }
    if (it != lines.end()) { // 蓋掉後面的線
      while (it != prev(lines.end()) && (*it)(it->r) <= now(it->r)) {
        it = lines.erase(it);
      }
    }
    if (it != lines.begin()) { // 更新前面的線段的有效區間
      Line new_prv = *prev(it);
      new_prv.r = now.l = new_prv.Intersect(now);
      lines.erase(prev(it)); lines.insert(new_prv);
    }
    if (it != lines.end()) { // 更新後面的線段的有效區間
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
    cin >> s[i]; s[i] = -s[i];
  }
  for (int i = 1; i <= n; ++i) {
    cin >> f[i];
  }
  convexhull.InsertLine(f[0], dp[0]);
  for (int i = 1; i <= n; ++i) {
    dp[i] = convexhull.query(s[i]);
    convexhull.InsertLine(f[i], dp[i]);
  }
  cout << -dp[n] << '\n';
}

