#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int N = 500006 * 2;

ll x[N], y[N];
int ans2[N];

int n;

pair<ll, ll> f(int i, int j) {
  ll ret;
  if (j <= i) {
    ret = j - i;
  }
  else if (j >= i + n) {
    ret = (i + n) - j - 1;
  }
  else {
    ret = ((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
  }
  return make_pair(-ret, j % n);
}

// assume that f(i, j) is a function that calculate A[i][j]

int ans[N];

void smawk(vector<int> &r, vector<int> &c);
// r contains the row index of the matrix
// c contains the column index of the matrix

void interpolate(vector<int> &r, vector<int> &c) {
  vector<int> new_r;
  for (int i = 1; i < (int)r.size(); i += 2) {
    new_r.push_back(r[i]);
  }
  smawk(new_r, c);
  int ptr = 0;
  for (int i = 0; i < (int)r.size(); i += 2) {
    int ansl = c[0], ansr = c[(int)c.size() - 1];
    if (i - 1 >= 0) {
      ansl = ans[ r[i - 1] ];
    }
    if (i + 1 <= (int)r.size() - 1) {
      ansr = ans[ r[i + 1] ];
    }
    ans[ r[i] ] = ansl;
    for (; ptr < (int)c.size() && c[ptr] <= ansr; ++ptr) {
      if (f(r[i], ans[ r[i] ]) > f(r[i], c[ptr])) {
        ans[ r[i] ] = c[ptr];
      }
      if (c[ptr] == ansr) {
        break;
      }
    }
  }
}

void reduce(vector<int> &r, vector<int> &c) {
  vector<int> new_c;
  for (int j = 0; j < (int)c.size(); ++j) {
    bool pushed = true;
    while (!new_c.empty()) {
      int i = (int)new_c.size() - 1;
      if (f(r[i], c[new_c.back()]) >= f(r[i], c[j])) {
        new_c.pop_back();
        continue;
      }
      else if (i == (int)r.size() - 1) {
        pushed = false;
        break;
      }
      else {
        break;
      }
    }
    if (pushed) {
      new_c.push_back(j);
    }
  }
  for (int &j : new_c) {
    j = c[j];
  }
  smawk(r, new_c);
}

void smawk(vector<int> &r, vector<int> &c) {
  if (max(r.size(), c.size()) <= 2) {
    for (int i : r) {
      ans[i] = c[0];
      for (int j : c) {
        if (f(i, j) < f(i, ans[i])) {
          ans[i] = j;
        }
      }
    }
    return;
  }
  else if (c.size() <= r.size()) {
    interpolate(r, c);
  }
  else {
    reduce(r, c);
  }
}

int main () {
  int T; scanf("%d", &T);
  while (T--) {
    scanf("%d", &n);
    vector<int> r, c;
    for (int i = 0; i < n; ++i) {
      scanf("%lld%lld", &x[i], &y[i]);
      x[i + n] = x[i];
      y[i + n] = y[i];
    }
    for (int i = 0; i < n; ++i) {
      r.push_back(i);
    }
    for (int i = 0; i < 2 * n - 1; ++i) {
      c.push_back(i);
    }
    smawk(r, c);
    for (int i = 0; i < n; ++i) {
      printf("%d\n", ans[i] % n + 1);
    }
  }
}

