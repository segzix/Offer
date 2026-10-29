// K 个一组反转链表（ACM 模式，class 写法）
// 输入：第一行链表元素（空格分隔），第二行 k
// 输出：反转后的链表，空格分隔
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode dummy(0);
        dummy.next = head;
        ListNode* pre = &dummy;                   // 当前组的前一个节点
        while (true) {
            ListNode* end = pre;
            for (int i = 0; i < k && end; i++) end = end->next;
            if (!end) break;                      // 不足 k 个，保持原样
            ListNode *start = pre->next, *nxt = end->next;
            ListNode *prev = nxt, *cur = start;   // 反转 [start, end]，尾部直接接 nxt
            while (cur != nxt) {
                ListNode* t = cur->next;
                cur->next = prev;
                prev = cur;
                cur = t;
            }
            pre->next = end;                      // end 成为新组头
            pre = start;                          // start 成为新组尾
        }
        return dummy.next;
    }
};

int main() {
    string line;
    getline(cin, line);
    stringstream ss(line);
    ListNode dummy(0), *tail = &dummy;
    for (int x; ss >> x; tail = tail->next) tail->next = new ListNode(x);
    int k;
    cin >> k;

    ListNode* h = Solution().reverseKGroup(dummy.next, k);
    for (bool first = true; h; h = h->next, first = false) cout << (first ? "" : " ") << h->val;
    cout << "\n";
}
