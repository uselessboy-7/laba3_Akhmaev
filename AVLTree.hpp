#ifndef LAB3_CLION_AVLTREE_HPP
#define LAB3_CLION_AVLTREE_HPP

#include <utility>
#include <stdexcept>

template<typename T>
T max(T &&a, T &&b) {
    return a > b ? a : b;
}

template<typename T, typename V>
class AVLTree {
public:
    using traversalType = void (*)(void *&, void *&, void *&);
private:
    struct Node {
    public:
        Node(T k, V v) : key(k), val(v), height(1), left(nullptr), right(nullptr) {}

        explicit Node(Node *node);

        explicit Node(const Node *node);

        T key;
        V val;
        unsigned int height;
        Node *left, *right;

        ~Node();
    };

    int _height(Node *) const;

    void _updateHeight(Node *);

    int _balanceFactor(Node *) const;

    Node *_rotateRight(Node *);

    Node *_rotateLeft(Node *);

    Node *_balance(Node *);

    Node *_insert(Node *, T, V);

    Node *_get(Node *, T);

    Node *_findMin(Node *) const;

    Node *_findMax(Node *) const;

    Node *_find(Node *, T) const;

    Node *_removeMin(Node *);

    Node *_remove(Node *, T);

    void _assertEmpty() const;

    template<typename func>
    void _traversal(Node *, Node *, traversalType, func);

    template<typename func>
    void _constTraversal(Node *, Node *, traversalType, func) const;

    size_t _sumSize(Node *) const;

    Node *_root;

    int _size;

public:

    AVLTree() : _root(nullptr), _size(0) {}

    AVLTree(const AVLTree<T, V> &Tree);

    AVLTree &operator=(const AVLTree<T, V> &Tree);

    void insert(T, V);

    void erase(T key) {
        _assertEmpty();
        _root = _remove(_root, key);
        --_size;
    }

    V &get(T);

    Node *getRoot();

    bool canFind(T key) { return _find(_root, key) != nullptr; }

    std::pair<const T &, V &> find_min() const {
        _assertEmpty();
        Node *min_node = _findMin(_root);
        return {min_node->key, min_node->val};
    }

    std::pair<const T &, V &> find_max() const {
        _assertEmpty();
        Node *max_node = _findMax(_root);
        return {max_node->key, max_node->val};
    }

    template<typename func>
    void traversal(traversalType type, func foo) { _traversal(_root, nullptr, type, foo); }

    template<typename func>
    void constTraversal(traversalType type, func foo) const { _constTraversal(_root, nullptr, type, foo); }

    size_t size() const noexcept { return _size; }

    int height() const noexcept { return _root->height; }

    AVLTree<T, V> *subTree(T key) const;

    bool areIdentical(Node *, Node *);

    bool isSubtree(Node *, Node *);

    V &operator[](T key);

    void clear();


    ~AVLTree();

    // Traversals
    static void RtLR(void *&n1, void *&n2, void *&n3) { std::swap(n1, n2); }

    static void RtRL(void *&n1, void *&n2, void *&n3) {
        std::swap(n2, n3);
        std::swap(n1, n3);
    }

    static void LRRt(void *&n1, void *&n2, void *&n3) { std::swap(n2, n3); }

    static void LRtR(void *&n1, void *&n2, void *&n3) {}

    static void RLRt(void *&n1, void *&n2, void *&n3) {
        std::swap(n1, n3);
        std::swap(n2, n3);
    }

    static void RRtL(void *&n1, void *&n2, void *&n3) { std::swap(n1, n3); }


};

template<typename T, typename V>
AVLTree<T, V>::Node::Node(const AVLTree::Node *node) :
        key(node->key),
        val(node->val),
        height(node->height) {
    if (node->left != nullptr)
        left = new Node(node->left);
    else
        left = nullptr;

    if (node->right != nullptr)
        right = new Node(node->right);
    else
        right = nullptr;
}

template<typename T, typename V>
AVLTree<T, V>::Node::Node(AVLTree::Node *node) :
        key(node->key),
        val(node->val),
        height(node->height) {
    if (node->left != nullptr)
        left = new Node(node->left);
    else
        left = nullptr;

    if (node->right != nullptr)
        right = new Node(node->right);
    else
        right = nullptr;
}


template<typename T, typename V>
AVLTree<T, V>::Node::~Node() {
    delete left;
    delete right;
}


template<typename T, typename V>
inline int AVLTree<T, V>::_height(Node *node) const {
    return node ? node->height : 0;
}

template<typename T, typename V>
inline void AVLTree<T, V>::_updateHeight(Node *node) {
    node->height = max(_height(node->left), _height(node->right)) + 1;
}

template<typename T, typename V>
inline int AVLTree<T, V>::_balanceFactor(Node *node) const {
    return _height(node->right) - _height(node->left);
}


template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_rotateRight(Node *node) {
    Node *buff = node->left;
    node->left = buff->right;
    buff->right = node;
    _updateHeight(node);
    _updateHeight(buff);

    return buff;

}


template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_rotateLeft(Node *node) {
    Node *buff = node->right;
    node->right = buff->left;
    buff->left = node;

    _updateHeight(node);
    _updateHeight(buff);

    return buff;

}


template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_balance(Node *node) {
    _updateHeight(node);

    if (_balanceFactor(node) == 2) {
        if (_balanceFactor(node->right) < 0)
            node->right = _rotateRight(node->right);
        node = _rotateLeft(node);
    }
    if (_balanceFactor(node) == -2) {
        if (_balanceFactor(node->left) > 0)
            node->left = _rotateLeft(node->left);
        node = _rotateRight(node);
    }

    return node;
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_insert(Node *node, T key, V val) {
    if (node == nullptr) {
        return new Node(key, val);
    }
    if (node->key == key) {
        node->val = key;
    }
    if (key < node->key)
        node->left = _insert(node->left, key, val);
    else
        node->right = _insert(node->right, key, val);

    _updateHeight(node);
    _size++;
    return _balance(node);
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_get(Node *node, T key) {
    if (node == nullptr)
        throw std::out_of_range("AVL_Tree out of range!");

    if (key == node->key)
        return node;

    if (key < node->key)
        return _get(node->left, key);
    else
        return _get(node->right, key);
}


template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_findMin(Node *node) const {
    return node->left ? _findMin(node->left) : node;
}

template<typename T, typename V>
inline void AVLTree<T, V>::insert(T key, V val) {
    if (_root == nullptr) {
        _root = new Node(key, val);
    } else {
        _root = _insert(_root, key, val);
    }
}

template<typename T, typename V>
inline V &AVLTree<T, V>::get(T key) {
    return _get(_root, key)->val;
}


template<typename T, typename V>
AVLTree<T, V>::~AVLTree() {
    delete _root;
    _size = 0;
}

template<typename T, typename V>
AVLTree<T, V>::AVLTree(const AVLTree<T, V> &Tree) : _root(Tree._root ? new Node(Tree._root) : nullptr),
                                                    _size(Tree._size) {}

template<typename T, typename V>
AVLTree<T, V> &AVLTree<T, V>::operator=(const AVLTree<T, V> &Tree) {
    delete _root;
    _root = new Node(Tree._root);
    _size = Tree._size;
    return *this;
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_find(AVLTree::Node *node, T key) const {
    if (node == nullptr)
        return nullptr;
    if (key == node->key)
        return node;
    if (key < node->key)
        return _find(node->left, key);
    if (key > node->key)
        return _find(node->right, key);
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_remove(AVLTree::Node *node, T key) {
    if (node == nullptr)
        throw std::out_of_range("AVLTree out of range!");
    if (key < node->key)
        node->left = _remove(node->left, key);
    else if (key > node->key)
        node->right = _remove(node->right, key);
    else {
        Node *left = node->left;
        Node *right = node->right;

        node->left = nullptr;
        node->right = nullptr;
        delete node;

        if (right == nullptr)
            return left;

        Node *min = _findMin(right);
        min->right = _removeMin(right);
        min->left = left;
        return _balance(min);
    }
    return _balance(node);
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_removeMin(AVLTree::Node *node) {
    if (node->left == nullptr)
        return node->right;
    node->left = _removeMin(node->left);
    return _balance(node);
}

template<typename T, typename V>
V &AVLTree<T, V>::operator[](T key) {
    if (!canFind(key)) {
        insert(key, V());
    }

    return get(key);
}

template<typename T, typename V>
template<typename func>
void AVLTree<T, V>::_traversal(AVLTree::Node *node, AVLTree::Node *parent, AVLTree::traversalType type,
                               func foo) {
    if (node == nullptr)
        return;

    if (node == parent) {
        foo(node->key, node->val);
    } else {
        void *n1 = node->left, *n2 = node, *n3 = node->right;
        type(n1, n2, n3);

        _traversal((Node *) n1, node, type, foo);
        _traversal((Node *) n2, node, type, foo);
        _traversal((Node *) n3, node, type, foo);
    }
}

template<typename T, typename V>
void AVLTree<T, V>::_assertEmpty() const {
    if (_root == nullptr)
        throw std::logic_error("AVLTree assert empty");
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::_findMax(AVLTree::Node *node) const {
    return node->right ? _findMax(node->right) : node;
}

template<typename T, typename V>
template<typename func>
void
AVLTree<T, V>::_constTraversal(AVLTree::Node *node, AVLTree::Node *parent, AVLTree::traversalType type,
                               func foo) const {
    if (node == nullptr)
        return;

    if (node == parent) {
        foo(node->key, node->val);
    } else {
        void *n1 = node->left, *n2 = node, *n3 = node->right;
        type(n1, n2, n3);

        _constTraversal((Node *) n1, node, type, foo);
        _constTraversal((Node *) n2, node, type, foo);
        _constTraversal((Node *) n3, node, type, foo);
    }
}

template<typename T, typename V>
AVLTree<T, V> *AVLTree<T, V>::subTree(T key) const {
    const Node *node = _find(_root, key);
    auto *res = new AVLTree();
    res->_root = new Node(node);
    res->_size = res->_sumSize(res->_root);

    return res;
}

template<typename T, typename V>
size_t AVLTree<T, V>::_sumSize(AVLTree::Node *node) const {
    if (node == nullptr)
        return 0;
    else
        return _sumSize(node->left) + _sumSize(node->right) + 1;
}

template<typename T, typename V>
void AVLTree<T, V>::clear() {
    delete _root;
    _root = nullptr;
}

template<typename T, typename V>
typename AVLTree<T, V>::Node *AVLTree<T, V>::getRoot() {
    return _root;
}

template<typename T, typename V>
bool AVLTree<T, V>::areIdentical(AVLTree::Node *root1, AVLTree::Node *root2) {
    if (root1 == NULL && root2 == NULL)
        return true;

    if (root1 == NULL || root2 == NULL)
        return false;

    return (root1->val == root2->val &&
            areIdentical(root1->left, root2->left) &&
            areIdentical(root1->right, root2->right));
}

template<typename T, typename V>
bool AVLTree<T, V>::isSubtree(AVLTree::Node *root1, AVLTree::Node *root2) {
    if (root2 == NULL)
        return true;

    if (root1 == NULL)
        return false;

    if (areIdentical(root1, root2))
        return true;

    return isSubtree(root1->left, root2) ||
           isSubtree(root1->right, root2);
}


template<typename T, typename V, typename func>
AVLTree<T, V> map(const AVLTree<T, V> &tree, func foo) {
    AVLTree<T, V> res = tree;
    res.traversal(res.LRtR, [foo](const T &key, V &val) {
        val = foo(val);
    });

    return res;
}

template<typename T, typename V, typename func>
AVLTree<T, V> where(const AVLTree<T, V> &tree, func foo) {
    AVLTree<T, V> res;
    tree.constTraversal(tree.LRtR, [&foo, &res](const T &key, const V &val) {
        if (foo(val)) {
            res.insert(key, val);
        }
    });
    return res;
}

template<typename T, typename V, typename func>
V reduce(const AVLTree<T, V> &tree, V init, func foo,
         typename AVLTree<T, V>::traversalType t_type = AVLTree<T, V>::LRtR) {
    V res = init;

    tree.constTraversal(t_type, [&foo, &res](const T &key, const V &val) {
        res = foo(val, res);
    });

    return res;
}


#endif //LAB3_CLION_AVLTREE_HPP
