//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE__MAP_H_
#define SEARCH_ENGINE__MAP_H_

#include <iostream>
#include <utility>

template<typename K, typename V>
class Map {
  struct Node {
    K key_;
    V value_;
    Node *left_;
    Node *right_;
    int height_{};

    Node() = default;
    explicit Node(const K &key,
                  Node *left = nullptr,
                  Node *right = nullptr,
                  int height = 1)
        : key_{key}, left_{left}, right_{right}, height_{height} {}
    Node(const K &key,
         const V &value,
         Node *left = nullptr,
         Node *right = nullptr,
         int height = 1)
        : key_{key},
          value_{value},
          left_{left},
          right_{right},
          height_{height} {}
  };
  using value_type = std::pair<K, V>;
  using size_type = std::size_t;
  using reference = value_type &;
  using const_reference = const value_type *;

  template<typename ValueType=value_type>
  class MapIterator {
    using SelfType = MapIterator;
   public:

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;
    using pointer = ValueType *;
    using reference = ValueType &;

    explicit MapIterator(Node *root = nullptr) : root_{root} {
      root_pair_ptr_ = nullptr;
    }
    ~MapIterator() {
      delete root_pair_ptr_;
    }
    MapIterator(const MapIterator &it) {
      root_pair_ptr_ = new ValueType;
      root_pair_ptr_ = it.root_pair_ptr_;
      root_ = it.root_;
    }

    MapIterator &operator=(const MapIterator &it) {
      if (this != &it) {
        delete root_pair_ptr_;
        root_pair_ptr_ = new ValueType;
        root_pair_ptr_ = it.root_pair_ptr_;
        root_ = it.root_;
      }

      return *this;
    }

    reference operator*() {
      delete root_pair_ptr_;
      root_pair_ptr_ = new ValueType;
      *root_pair_ptr_ = std::make_pair(root_->key_, root_->value_);
      return *root_pair_ptr_;
    }

    pointer operator->() {
      delete root_pair_ptr_;
      root_pair_ptr_ = new ValueType;
      *root_pair_ptr_ = std::make_pair(root_->key_, root_->value_);
      return root_pair_ptr_;
    }

    SelfType operator++() {

    }

    friend bool operator==(const MapIterator &lhs, const MapIterator &rhs) {
      return lhs.root_ == rhs.root_;
    }

    friend bool operator!=(const MapIterator &lhs, const MapIterator &rhs) {
      return lhs.root_ != rhs.root_;
    }

   private:
    Node *root_;
    pointer root_pair_ptr_;
  };
 public:
  using Iterator = MapIterator<value_type>;
  using ConstIterator = MapIterator<const value_type>;

  Map() { root_ = nullptr; }
  Map(const Map &);
  ~Map();
  Map &operator=(const Map &);

  Iterator Insert(const K &);
  Iterator Find(const K &);
  inline bool Empty() const;
  inline void Clear();

  inline void Inorder() const;
  inline void Preorder() const;
  inline void Postorder() const;

 private:
  Node *root_;

  Node *Insert(Node *&, const K &);
  inline void Inorder(Node *node) const;
  inline void Preorder(Node *node) const;
  inline void Postorder(Node *node) const;

  inline void Copy(Node *, Node *&);
  inline void Clear(Node *&);
  inline Node *Find(Node *&, const K &);

  // A utility function to get maximum
  // of two integers
  inline int Max(int a, int b) const;

  // A utility function to get the
  // height of the tree
  inline int Height(Node *N) const;

  // A utility function to right
  // rotate subtree rooted with y
  // See the diagram given above.
  inline Node *RightRotate(Node *y);

  // A utility function to left
  // rotate subtree rooted with x
  // See the diagram given above.
  inline Node *LeftRotate(Node *x);

  // Get Balance factor of node N
  inline int GetBalance(Node *N);

  inline void Balance(Node *&node, const K &key);

};

template<typename K, typename V>
Map<K, V>::Map(const Map &map) {
  Copy(map.root_, root_);
}

template<typename K, typename V>
Map<K, V> &Map<K, V>::operator=(const Map &map) {
  if (this != &map) {
    Clear();
    Copy(map.root_, root_);
  }
  return *this;
}

template<typename K, typename V>
Map<K, V>::~Map() {
  Clear(root_);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Find(const K &key) {
  Node *node = Find(root_, key);
  if (node == nullptr) {
    return Iterator(nullptr);
  }

  return Iterator(node);
}

template<typename K, typename V>
typename Map<K, V>::Node *Map<K, V>::Find(Node *&node, const K &key) {
  if (node == nullptr || node->key_ == key) {
    return node;
  }
  if (key < node->key_) {
    return Find(node->left_, key);
  }

  return Find(node->right_, key);
}

template<typename K, typename V>
bool Map<K, V>::Empty() const {
  return root_ == nullptr;
}

template<typename K, typename V>
void Map<K, V>::Clear() {
  Clear(root_);
}

template<typename K, typename V>
void Map<K, V>::Clear(Node *&node) {
  if (node == nullptr) {
    return;
  }

  Clear(node->left_);
  Clear(node->right_);

  delete node;
  node = nullptr;
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Insert(const K &key) {
  Node *node = Insert(root_, key);
  if (node == nullptr) {
    return Iterator(nullptr);
  }

  return Iterator(node);
}

template<typename K, typename V>
typename Map<K, V>::Node *Map<K, V>::Insert(Node *&node, const K &key) {
  /* 1. Perform the normal BST insertion */
  if (node == nullptr) {
    node = new Node(key);
    Balance(node, key);
    return node;
  } else if (key < node->key_) {
    Node *newNode = Insert(node->left_, key);
    Balance(node, key);
    return newNode;
  } else if (key > node->key_) {
    Node *newNode = Insert(node->right_, key);
    Balance(node, key);
    return newNode;
  } else {
    return node;
  }
}

template<typename K, typename V>
void Map<K, V>::Balance(Node *&node, const K &key) {
/* 2. Update height of this ancestor node */
  node->height_ = 1 + Max(Height(node->left_),
                          Height(node->right_));

  /* 3. Get the balance factor of this ancestor
      node to check whether this node became
      unbalanced */
  int balance = GetBalance(node);

  // If this node becomes unbalanced, then
  // there are 4 cases

  // Left Left Case
  if (balance > 1 && key < node->left_->key_)
    node = RightRotate(node);

  // Right Right Case
  if (balance < -1 && key > node->right_->key_)
    node = LeftRotate(node);

  // Left Right Case
  if (balance > 1 && key > node->left_->key_) {
    node->left_ = LeftRotate(node->left_);
    node = RightRotate(node);
  }

  // Right Left Case
  if (balance < -1 && key < node->right_->key_) {
    node->right_ = RightRotate(node->right_);
    node = LeftRotate(node);
  }
}

template<typename K, typename V>
void Map<K, V>::Copy(Node *orig_node, Node *&node) {
  if (orig_node == nullptr) {
    return;
  }

  node = new Node(orig_node->key_,
                  orig_node->left_,
                  orig_node->right_);

  Copy(orig_node->left_, node->left_);
  Copy(orig_node->right_, node->right_);
}

template<typename K, typename V>
int Map<K, V>::Height(Node *N) const {
  if (N == nullptr)
    return 0;
  return N->height_;
}

template<typename K, typename V>
int Map<K, V>::Max(int a, int b) const {
  return (a > b) ? a : b;
}

template<typename K, typename V>
typename Map<K, V>::Node *Map<K, V>::RightRotate(Node *y) {
  Node *x = y->left_;
  Node *T2 = x->right_;

  // Perform rotation
  x->right_ = y;
  y->left_ = T2;

  // Update heights
  y->height_ = Max(Height(y->left_),
                   Height(y->right_)) + 1;
  x->height_ = Max(Height(x->left_),
                   Height(x->right_)) + 1;

  // Return new root
  return x;
}

template<typename K, typename V>
typename Map<K, V>::Node *Map<K, V>::LeftRotate(Node *x) {
  Node *y = x->right_;
  Node *T2 = y->left_;

  // Perform rotation
  y->left_ = x;
  x->right_ = T2;

  // Update heights
  x->height_ = Max(Height(x->left_),
                   Height(x->right_)) + 1;
  y->height_ = Max(Height(y->left_),
                   Height(y->right_)) + 1;

  // Return new root
  return y;
}

template<typename K, typename V>
int Map<K, V>::GetBalance(Node *N) {
  if (N == nullptr)
    return 0;
  return Height(N->left_) - Height(N->right_);
}

template<typename K, typename V>
void Map<K, V>::Inorder() const {
  Inorder(root_);
  std::cout << std::endl;
}

template<typename K, typename V>
void Map<K, V>::Inorder(Node *node) const {
  if (node == nullptr) {
    return;
  }

  Inorder(node->left_);
  std::cout << node->key_ << " ";
  Inorder(node->right_);
}

template<typename K, typename V>
void Map<K, V>::Preorder() const {
  Preorder(root_);
  std::cout << std::endl;
}

template<typename K, typename V>
void Map<K, V>::Preorder(Node *node) const {
  if (node == nullptr) {
    return;
  }
  std::cout << node->key_ << " ";
  Inorder(node->left_);
  Inorder(node->right_);
}

template<typename K, typename V>
void Map<K, V>::Postorder() const {
  Postorder(root_);
  std::cout << std::endl;
}

template<typename K, typename V>
void Map<K, V>::Postorder(Node *node) const {
  if (node == nullptr) {
    return;
  }

  Inorder(node->left_);
  Inorder(node->right_);
  std::cout << node->key_ << " ";
}

#endif //SEARCH_ENGINE__MAP_H_
