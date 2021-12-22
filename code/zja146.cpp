#include <bits/stdc++.h>
using namespace std;

const int N = 1000006;
int a[N];
int ans_mx[N], ans_mn[N];

void find_max(int n, int k) {
  deque<int> dq;
  for (int i = 1; i <= n; ++i) {
    while (!dq.empty() && a[dq.back()] < a[i]) dq.pop_back();
    dq.push_back(i);
    if (dq.front() == i - k) dq.pop_front();
    ans_mx[i] = a[dq.front()];
  }
}

void find_min(int n, int k) {
  deque<int> dq;
  for (int i = 1; i <= n; ++i) {
    while (!dq.empty() && a[dq.back()] > a[i]) dq.pop_back(); // 把所有大於 a[i] 的元素通通刪掉
    dq.push_back(i);
    if (dq.front() == i - k) dq.pop_front(); // 如果最前面的元素已經不合法（過期），就 pop 掉
    ans_mn[i] = a[dq.front()];
  }
}

int main () {
  ios::sync_with_stdio(0); cin.tie(0);
  int n, k;
  while (cin >> n >> k) {
    for (int i = 1; i <= n; ++i) {
      cin >> a[i];
    }
    if (k > n) {
      cout << *min_element(a + 1, a + n + 1) << '\n';
      cout << *max_element(a + 1, a + n + 1) << '\n';
      continue;
    }
    find_max(n, k);
    find_min(n, k);
    for (int i = k; i <= n; ++i) {
      cout << ans_mn[i] << " \n"[i == n];
    }
    for (int i = k; i <= n; ++i) {
      cout << ans_mx[i] << " \n"[i == n]; 
    }
  }
}

