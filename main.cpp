#include <iostream>
#include "AVLTree.hpp"
#include "PriorityQueue.hpp"
#include <string>

using namespace std;

int main() {
    AVLTree<int, string> Tree, Tree2;
    for(int i = 0; i < 15; ++i)
        Tree[i] = std::to_string(i);

    Tree2 = Tree;

    /*Tree2 = map<int,string>(Tree2, [](string &s){
        return "-" + s;
    });*/

    Tree2.traversal(AVLTree<int, std::string>::LRRt, [](const int &a, string &b){
        std::cout << b << ' ';
    });
    cout << '\n';
    Tree.traversal(AVLTree<int, std::string>::RRtL, [](const int &a, string &b){
        std::cout << b << ' ';
    });
    cout << '\n';

    PriorityQueue<string> queue;
    queue.enqueue(10, "low");
    queue.enqueue(15, "middle");
    queue.enqueue(4321, "hight");

    while (!queue.empty()){
        std::cout << queue.dequeue() << ' ';
    }

    return 0;
}
