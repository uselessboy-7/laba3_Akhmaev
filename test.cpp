#include <iostream>
#include "AVLTree.hpp"
#include "PriorityQueue.hpp"


template<typename T1, typename T2>
void assertEqual(const T1 &a, const T2 &b, const char *msg = "Not equal in assert equal!") {
    if (a != b) {
        throw std::logic_error(msg);
    }
}


void testAVLTreeMap() {
    int n = 10;
    AVLTree<int, int> tree;
    int *check = new int[n];
    for (int i = 0; i < n; ++i) {
        tree[i] = i;
    }
    int i = 0;
    tree.traversal(AVLTree<int, std::string>::LRRt, [&check, &i](const int &a, int &b) {
        check[i++] = b;
    });
    tree = map(tree, [](auto b) { return b * b; });
    for (int j = 0; j < n; ++j) {
        check[j] *= check[j];
    }
    int k = 0;
    tree.traversal(AVLTree<int, int>::LRRt, [&check, &k](auto a, auto b) {
        assertEqual(check[k++], b);
    });
}


void testAVLTreeWhere() {
    int n = 10;
    AVLTree<int, int> tree;
    int *check = new int[n];
    int *result = new int[n / 2];
    for (int i = 0; i < n; ++i) {
        tree[i] = i;
    }
    int i = 0;
    tree.traversal(AVLTree<int, std::string>::LRRt, [&check, &i, n](const int &a, int &b) {
        check[i++] = b;
    });
    tree = where(tree, [n](auto v) {
        return v > n / 2;
    });
    int l = 0;
    for (int q = 0; q < n; q++) {
        if (check[q] > n / 2) {
            result[l] = check[q];
            l++;
        }
    }
    int k = 0;
    tree.traversal(AVLTree<int, int>::LRRt, [&result, &k](auto a, auto b) {
        assertEqual(result[k++], b);
    });
}


void testAVLTreeReduce() {
    int n = 10;
    AVLTree<int, int> tree;
    int sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += i;
        tree[i] += i;
    }

    int psum = reduce(tree, 0, std::plus<>());
    assertEqual(psum, sum);
}

void testAVLTreesubTree() {
    int n = 10;
    AVLTree<int, int> tree;
    for (int i = 0; i < n; ++i) {
        tree[i] = i;
    }
    AVLTree<int, int> *subTree = tree.subTree(1);
    const char *msg = "Not equal in assert equal!";
    if (not tree.isSubtree(tree.getRoot(), subTree->getRoot())) {
        throw std::logic_error(msg);
    }

}

void testPriorityQueue() {
    PriorityQueue<int> queue;
    queue.enqueue(0, 0);
    queue.enqueue(1, 1);
    queue.enqueue(2, 2);
    int i = 0;
    while (!queue.empty())
        assertEqual(2 - (i++), queue.dequeue());
    assertEqual(i, 3);
}

void runTest() {
    testAVLTreeMap();
    std::cout << "Map test is OK\n";
    testAVLTreeWhere();
    std::cout << "Where test is OK\n";
    testAVLTreeReduce();
    std::cout << "Reduce test is OK\n";
    testAVLTreesubTree();
    std::cout << "Sub tree test is OK\n";
    testPriorityQueue();
    std::cout << "Queue test is OK\n";

}

int main() {
    runTest();
}
