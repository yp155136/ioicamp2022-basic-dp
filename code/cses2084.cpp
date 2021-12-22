#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

struct Line {
  ll a, b; // 一條 ax + b 的直線
  Line(ll _a, ll _b): a(_a), b(_b){}
  ll operator()(const ll x) {
    return a * x + b;
  }
};

bool check(Line l1, Line l2, Line l3) {
  // l1 是講義中的 L_{-2}，l2 是講義中的 L_{-1}，l3 是想要新增的直線
  // double v12 = (l1.b - l2.b) / (l2.a - l1.a) 
  // double v23 = (l2.b - l3.b) / (l3.a - l2.a)
  // return v12 >= v13
  // 但是上面的方法會有浮點數誤差，因此在這裡只考慮使用整數運算，方法如下：
  return (l3.a - l2.a) * (l1.b - l2.b) >= (l3.b - l2.b) * (l1.a - l2.a);
}

const int N = 200006;
ll dp[N], s[N], f[N];

void solve(int n) {
  deque<Line> dq;
  dq.push_back(Line(f[0], dp[0]));
  for (int i = 1; i <= n; ++i) {
    while ((int)dq.size() >= 2 && dq[0](s[i]) <= dq[1](s[i])) {
      // 把比較差的線丟掉，注意到這邊寫 <= 或 < 其實都 ok
      dq.pop_front();
    }
    dp[i] = dq[0](s[i]);
    Line l = Line(f[i], dp[i]);
    while ((int)dq.size() >= 2 && check(dq[(int)dq.size() - 2], dq[(int)dq.size() - 1], l)) {
      // 把新的線加進去，看看 L_{-2} 跟 L 有沒有辦法把 L_{-1} 殺掉
      dq.pop_back();
    }
    dq.push_back(l);
  }
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  int n; cin >> n >> f[0];
  for (int i = 1; i <= n; ++i) {
    cin >> s[i];
  }
  for (int i = 1; i <= n; ++i) {
    cin >> f[i];
  }
  for (int i = 0; i <= n; ++i) {
    f[i] = -f[i];
  }
  solve(n);
  cout << -dp[n] << '\n';
}


