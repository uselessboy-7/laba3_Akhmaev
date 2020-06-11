#ifndef LAB3_CLION_PRIORITYQUEUE_HPP
#define LAB3_CLION_PRIORITYQUEUE_HPP


#include "AVLTree.hpp"


template <typename V, typename T=size_t>
class PriorityQueue {
public:
    PriorityQueue() = default;
    PriorityQueue(const PriorityQueue<V,T> &queue);

    void enqueue(T, V);

    V& top() const {return _tree.findMax().second;}
    V dequeue();

    size_t size() const  {return _tree.size();}
    bool empty() const  {return _tree.size() == 0;}

    void clear();

    ~PriorityQueue();
private:
    AVLTree<T,V> _tree;
};

template <typename V, typename T>
PriorityQueue<V,T>::PriorityQueue(const PriorityQueue<V,T> &queue): _tree(queue._tree) {};



template <typename V, typename T>
void PriorityQueue<V,T>::enqueue(T priority, V value)
{
    _tree[priority] = value;
}

template <typename V, typename T>
V PriorityQueue<V,T>::dequeue()
{
    std::pair<const T&, V&> mx = _tree.find_max();
    V ret(mx.second);
    _tree.erase(mx.first);

    return ret;
}

template<typename V, typename T>
void PriorityQueue<V, T>::clear() {
    _tree.clear();
}

template<typename V, typename T>
PriorityQueue<V, T>::~PriorityQueue() = default;


#endif //LAB3_CLION_PRIORITYQUEUE_HPP
