// ACM 模式：链表 / 二叉树 的输入解析、建表建树、输出 演示
// 编译：g++ -std=c++17 -O2 acm_list_tree_demo.cpp -o demo && ./demo < input.txt
//
// 输入：每行一个用例，第一个词是类型，读到 EOF 结束。
//   数据兼容两种写法：  1 2 3 null 5   或   [1,2,3,null,5]
//
//   list    1 2 3 4 5          -> 反转链表
//   cycle   3 2 0 -4 | 1       -> 尾节点接到下标 1 处成环，求入环点（-1 表示无环）
//   merge   1 2 4 | 1 3 4      -> 合并两个有序链表
//   tree    [3,9,20,null,null,15,7] -> 层序/前序/中序/后序遍历 + 最大深度
//   lca     [3,5,1,6,2,0,8,null,null,7,4] | 5 4 -> 最近公共祖先
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

// ---------- 输入解析 ----------
// "[1,2,null]" / "1 2 null" -> {"1","2","null"}
vector<string> tokens(string s) {
    for (char& c : s)
        if (c == '[' || c == ']' || c == ',') c = ' ';
    stringstream ss(s);
    vector<string> v;
    string t;
    while (ss >> t) v.push_back(t);
    return v;
}

// 按 '|' 切成多段
vector<string> splitBar(const string& s) {
    vector<string> parts;
    stringstream ss(s);
    string p;
    while (getline(ss, p, '|')) parts.push_back(p);
    return parts;
}

// ---------- 链表 ----------
ListNode* buildList(const vector<string>& v) {
    ListNode dummy(0), *cur = &dummy;  // 哑节点，省去头节点特判
    for (auto& t : v) {
        cur->next = new ListNode(stoi(t));
        cur = cur->next;
    }
    return dummy.next;
}

void printList(ListNode* h) {
    for (bool first = true; h; h = h->next, first = false)
        cout << (first ? "" : " ") << h->val;
    cout << "\n";
}

ListNode* reverseList(ListNode* h) {
    ListNode* prev = nullptr;
    while (h) {
        ListNode* nxt = h->next;
        h->next = prev;
        prev = h;
        h = nxt;
    }
    return prev;
}

// Floyd 判环：相遇后一个指针回到头，两者同速再走，相遇点即入环点
ListNode* detectCycle(ListNode* h) {
    ListNode *slow = h, *fast = h;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            for (slow = h; slow != fast; slow = slow->next, fast = fast->next) {}
            return slow;
        }
    }
    return nullptr;
}

ListNode* mergeTwo(ListNode* a, ListNode* b) {
    ListNode dummy(0), *cur = &dummy;
    while (a && b) {
        ListNode*& small = (a->val <= b->val) ? a : b;
        cur->next = small;
        small = small->next;
        cur = cur->next;
    }
    cur->next = a ? a : b;
    return dummy.next;
}

// ---------- 二叉树 ----------
// LeetCode 层序格式建树：队列逐个给出队节点挂左右孩子
TreeNode* buildTree(const vector<string>& v) {
    if (v.empty() || v[0] == "null" || v[0] == "#") return nullptr;
    auto isNull = [](const string& t) { return t == "null" || t == "#"; };
    TreeNode* root = new TreeNode(stoi(v[0]));
    queue<TreeNode*> q;
    q.push(root);
    size_t i = 1;
    while (!q.empty() && i < v.size()) {
        TreeNode* node = q.front(); q.pop();
        if (i < v.size() && !isNull(v[i])) q.push(node->left = new TreeNode(stoi(v[i])));
        ++i;
        if (i < v.size() && !isNull(v[i])) q.push(node->right = new TreeNode(stoi(v[i])));
        ++i;
    }
    return root;
}

// 输出成 LeetCode 格式 [3,9,20,null,null,15,7]（去掉末尾 null）
string serialize(TreeNode* root) {
    vector<string> out;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        TreeNode* n = q.front(); q.pop();
        if (!n) { out.push_back("null"); continue; }
        out.push_back(to_string(n->val));
        q.push(n->left);
        q.push(n->right);
    }
    while (!out.empty() && out.back() == "null") out.pop_back();
    string s = "[";
    for (size_t i = 0; i < out.size(); ++i) s += (i ? "," : "") + out[i];
    return s + "]";
}

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        res.emplace_back();
        for (int k = q.size(); k > 0; --k) {  // 一次处理一整层
            TreeNode* n = q.front(); q.pop();
            res.back().push_back(n->val);
            if (n->left) q.push(n->left);
            if (n->right) q.push(n->right);
        }
    }
    return res;
}

void preorder(TreeNode* r, vector<int>& o) { if (!r) return; o.push_back(r->val); preorder(r->left, o); preorder(r->right, o); }
void postorder(TreeNode* r, vector<int>& o) { if (!r) return; postorder(r->left, o); postorder(r->right, o); o.push_back(r->val); }

// 中序用迭代写法（面试常要求非递归）
vector<int> inorder(TreeNode* root) {
    vector<int> o;
    stack<TreeNode*> st;
    TreeNode* cur = root;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        o.push_back(cur->val);
        cur = cur->right;
    }
    return o;
}

int maxDepth(TreeNode* r) { return r ? 1 + max(maxDepth(r->left), maxDepth(r->right)) : 0; }

TreeNode* findVal(TreeNode* r, int v) {
    if (!r || r->val == v) return r;
    TreeNode* L = findVal(r->left, v);
    return L ? L : findVal(r->right, v);
}

TreeNode* lca(TreeNode* r, TreeNode* p, TreeNode* q) {
    if (!r || r == p || r == q) return r;
    TreeNode* L = lca(r->left, p, q);
    TreeNode* R = lca(r->right, p, q);
    return L && R ? r : (L ? L : R);
}

void printVec(const string& name, const vector<int>& v) {
    cout << name << ":";
    for (int x : v) cout << " " << x;
    cout << "\n";
}

// ponytail: 演示代码不 delete 节点，进程退出由 OS 回收；需要时补后序释放
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {          // 读到 EOF 为止，处理多组输入
        stringstream ss(line);
        string type;
        if (!(ss >> type)) continue;      // 跳过空行
        string rest;
        getline(ss, rest);
        auto parts = splitBar(rest);

        if (type == "list") {
            printList(reverseList(buildList(tokens(rest))));
        } else if (type == "cycle") {
            ListNode* h = buildList(tokens(parts[0]));
            int pos = parts.size() > 1 ? stoi(parts[1]) : -1;
            if (pos >= 0 && h) {          // 按下标把尾巴接回去
                ListNode *tail = h, *entry = nullptr;
                for (int i = 0; ; ++i, tail = tail->next) {
                    if (i == pos) entry = tail;
                    if (!tail->next) break;
                }
                tail->next = entry;
            }
            ListNode* e = detectCycle(h);
            cout << (e ? to_string(e->val) : "no cycle") << "\n";
        } else if (type == "merge") {
            printList(mergeTwo(buildList(tokens(parts[0])), buildList(tokens(parts[1]))));
        } else if (type == "tree") {
            TreeNode* root = buildTree(tokens(rest));
            cout << "serialize: " << serialize(root) << "\n";
            for (auto& lv : levelOrder(root)) printVec("level", lv);
            vector<int> pre, post;
            preorder(root, pre);
            postorder(root, post);
            printVec("pre", pre);
            printVec("in", inorder(root));
            printVec("post", post);
            cout << "depth: " << maxDepth(root) << "\n";
        } else if (type == "lca") {
            TreeNode* root = buildTree(tokens(parts[0]));
            auto pq = tokens(parts[1]);
            TreeNode* a = lca(root, findVal(root, stoi(pq[0])), findVal(root, stoi(pq[1])));
            cout << (a ? to_string(a->val) : "null") << "\n";
        }
    }
    return 0;
}

/* 样例输入：
list 1 2 3 4 5
cycle 3 2 0 -4 | 1
cycle 1 2 | -1
merge [1,2,4] | [1,3,4]
tree [3,9,20,null,null,15,7]
lca [3,5,1,6,2,0,8,null,null,7,4] | 5 4

样例输出：
5 4 3 2 1
2
no cycle
1 1 2 3 4 4
serialize: [3,9,20,null,null,15,7]
level: 3
level: 9 20
level: 15 7
pre: 3 9 20 15 7
in: 9 3 15 20 7
post: 9 15 7 20 3
depth: 3
5
*/
