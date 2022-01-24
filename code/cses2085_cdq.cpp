#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 200006;

ll s[N], f[N], dp[N];

struct Line {
  ll a, b;
  Line(ll _a, ll _b): a(_a), b(_b){}
  ll operator()(const ll x) {
    return a * x + b;
  }
};

bool check(Line l1, Line l2, Line l3) {
  return (l3.a - l2.a) * (l1.b - l2.b) >= (l3.b - l2.b) * (l1.a - l2.a);
}

void dc(int l, int r) {
  if (l == r) return;
  int mid = (l + r) >> 1;
  dc(l, mid);
  // use [l, mid] to update [mid + 1, r]
  vector<Line> lines;
  for (int j = l; j <= mid; ++j) {
    lines.push_back(Line(f[j], dp[j]));
  }
  sort(lines.begin(), lines.end(), [](const Line &l1, const Line &l2) {
    return make_pair(l1.a, -l1.b) > make_pair(l2.a, -l2.b);
  });
  vector<int> qs;
  for (int i = mid + 1; i <= r; ++i) {
    qs.push_back(i);
  }
  sort(qs.begin(), qs.end(), [](const int &i, const int &j) {
    return s[i] < s[j];
  });
  // 把線排序好後，先把凸包建立出來
  deque<Line> dq;
  for (Line l_new : lines) {
    while ((int)dq.size() >= 2 && check(dq[(int)dq.size() - 2], dq[(int)dq.size() - 1], l_new)) {
      dq.pop_back();
    }
    dq.push_back(l_new);
  }
  // 再一個一個去詢問
  for (int i : qs) {
    while (dq.size() >= 2 && dq[0](s[i]) > dq[1](s[i])) {
      dq.pop_front();
    }
    dp[i] = min(dp[i], dq[0](s[i]));
  }
  dc(mid + 1, r);
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
  for (int i = 1; i <= n; ++i) {
    dp[i] = (1ll << 60);
  }
  dc(0, n);
  cout << dp[n] << '\n';
}



