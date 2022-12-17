//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE_MAP_H_
#define SEARCH_ENGINE_MAP_H_

#include <iostream>
#include <stack>
#include <utility>

template<typename K, typename V>
class Map {
  struct Node {
    std::pair<const K, V> node_pair_;
    Node *left_;
    Node *right_;
    int height_{};

    Node() = default;
    explicit Node(const std::pair<const K, V> &node_pair,
                  Node *left = nullptr,
                  Node *right = nullptr,
                  int height = 1)
        : node_pair_{node_pair}, left_{left}, right_{right}, height_{height} {}
  };
  using value_type = std::pair<const K, V>;
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

    MapIterator() = default;
    explicit MapIterator(Node *curr) {
      Fill(curr);
      if (!min_.empty()) {
        curr_ = min_.top();
        min_.pop();
      } else {
        curr_ = nullptr;
      }
    }

    MapIterator(Node* root, Node *curr) {
      Fill(root);
      while (!min_.empty() && curr != curr_) {
        curr_ = min_.top();
        min_.pop();
        if (curr_->right_ != nullptr) {
          Fill(curr_->right_);
        }
      }
    }

    MapIterator(const MapIterator &) = default;
    MapIterator &operator=(const MapIterator &) = default;
    ~MapIterator() = default;

    reference operator*() {
      return curr_->node_pair_;
    }

    pointer operator->() {
      return &curr_->node_pair_;
    }

    SelfType operator++() {
      if (min_.empty()) {
        curr_ = nullptr;
      } else {
        curr_ = min_.top();
        min_.pop();
        if (curr_->right_ != nullptr) {
          Fill(curr_->right_);
        }
      }
      return *this;
    }

    friend bool operator==(const MapIterator &lhs, const MapIterator &rhs) {
      return lhs.curr_ == rhs.curr_;
    }

    friend bool operator!=(const MapIterator &lhs, const MapIterator &rhs) {
      return lhs.curr_ != rhs.curr_;
    }

   private:
    void Fill(Node *curr) {
      while (curr != nullptr) {
        min_.push(curr);
        curr = curr->left_;
      }
    }

    Node *curr_;
    std::stack<Node *> min_;
  };

  template<typename ValueType=value_type>
  class ConstMapIterator {
    using SelfType = ConstMapIterator;
   public:

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;
    using pointer = ValueType *;
    using reference = ValueType &;

    ConstMapIterator() = default;
    explicit ConstMapIterator(const Node *curr) {
      Fill(curr);
      if (!min_.empty()) {
        curr_ = min_.top();
        min_.pop();
      } else {
        curr_ = nullptr;
      }
    }

    ConstMapIterator(const Node* root, const Node *curr) {
      Fill(root);
      while (!min_.empty() && curr != curr_) {
        curr_ = min_.top();
        min_.pop();
        if (curr_->right_ != nullptr) {
          Fill(curr_->right_);
        }
      }
    }

    ConstMapIterator(const ConstMapIterator &it) = default;
    ConstMapIterator &operator=(const ConstMapIterator &it) = default;
    reference operator*() {
      return curr_->node_pair_;
    }

    pointer operator->() {
      return &curr_->node_pair_;
    }

    SelfType operator++() {
      if (min_.empty()) {
        curr_ = nullptr;
      } else {
        curr_ = min_.top();
        min_.pop();
        if (curr_->right_ != nullptr) {
          Fill(curr_->right_);
        }
      }
      return *this;
    }

    friend bool operator==(const SelfType &lhs, const SelfType &rhs) {
      return lhs.curr_ == rhs.curr_;
    }

    friend bool operator!=(const SelfType &lhs, const SelfType &rhs) {
      return lhs.curr_ != rhs.curr_;
    }

   private:
    void Fill(const Node* root) {
      const Node* curr = root;
      while (curr != nullptr) {
        min_.push(curr);
        curr = curr->left_;
      }
    }

    const Node *curr_;
    std::stack<const Node *> min_;
  };

 public:
  using Iterator = MapIterator<value_type>;
  using ConstIterator = ConstMapIterator<const value_type>;

  Map();
  Map(const Map &);
  ~Map();
  Map &operator=(const Map &);

  std::pair<Iterator, bool> Insert(const std::pair<const K, V> &);
  V &operator[](const K &key);
  Iterator Find(const K &);
  ConstIterator Find(const K &) const;

  [[nodiscard]] inline bool Empty() const;
  inline void Clear();
  [[nodiscard]] inline size_t Size() const;

  inline void Inorder() const;
  inline void Preorder() const;
  inline void Postorder() const;

  Iterator Begin();
  ConstIterator CBegin() const;
  Iterator End();
  ConstIterator CEnd() const;

 private:
  Node *root_;
  size_t length_;

  void Init();

  void Insert(Node *&, Node *&);
  inline void Inorder(Node *node) const;
  inline void Preorder(Node *node) const;
  inline void Postorder(Node *node) const;

  inline void Copy(Node *, Node *&);
  inline void Clear(Node *&);
  inline Node *Find(Node *&, const K &);
  inline const Node *Find(const Node *&, const K &) const;

  // A utility function to get maximum
  // of two integers
  [[nodiscard]] inline int Max(int a, int b) const;

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
Map<K, V>::Map() : root_{nullptr}, length_{0} {}

template<typename K, typename V>
Map<K, V>::Map(const Map &map) {
  Init();
  Copy(map.root_, root_);
  length_ = map.length_;
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
void Map<K, V>::Init() {
  root_ = nullptr;
  length_ = 0;
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
typename Map<K, V>::ConstIterator Map<K, V>::Find(const K &key) const {
  const Node* const_root = root_;
  const Node* found_node = Find(const_root, key);
  if (found_node == nullptr) {
    return ConstIterator(const_root, nullptr);
  }

  return ConstIterator(const_root, found_node);
}


template<typename K, typename V>
typename Map<K, V>::Node *Map<K, V>::Find(Node *&node, const K &key){
  if (node == nullptr || node->node_pair_.first == key) {
    return node;
  }
  if (key < node->node_pair_.first) {
    return Find(node->left_, key);
  }

  return Find(node->right_, key);
}

template<typename K, typename V>
const typename Map<K, V>::Node *Map<K, V>::Find(const Node *&node, const K &key) const{
  if (node == nullptr || node->node_pair_.first == key) {
    return node;
  }
  if (key < node->node_pair_.first) {
    const Node* const_left = node->left_;
    return Find(const_left, key);
  }
  const Node* const_right = node->right_;
  return Find(const_right, key);
}

template<typename K, typename V>
bool Map<K, V>::Empty() const {
  return root_ == nullptr;
}

template<typename K, typename V>
void Map<K, V>::Clear() {
  Clear(root_);
  Init();
}

template<typename K, typename V>
void Map<K, V>::Clear(Node *&node) {
  if (node == nullptr) {
    return;
  }

  Clear(node->left_);
  Clear(node->right_);

  delete node;
}

template<typename K, typename V>
V &Map<K, V>::operator[](const K &key) {

  Node *node = Find(root_, key);
  if (node != nullptr) return node->node_pair_.second;

  node = new Node(std::make_pair(key, V()));

  Insert(root_, node);
  length_++;

  return node->node_pair_.second;
}

template<typename K, typename V>
typename std::pair<typename Map<K, V>::Iterator, bool> Map<K, V>::Insert(
    const std::pair<const K, V> &node_pair) {

  Node *node = Find(root_, node_pair.first);
  if (node != nullptr) return std::make_pair(Iterator(node), false);

  node = new Node(node_pair);

  Insert(root_, node);
  length_++;

  return std::make_pair(Iterator(node), true);
}

template<typename K, typename V>
void Map<K, V>::Insert(Node *&node, Node *&new_node) {
  /* 1. Perform the normal BST insertion */
  if (node == nullptr) {
    node = new_node;
  } else if (new_node->node_pair_.first < node->node_pair_.first) {
    Insert(node->left_, new_node);
  } else if (new_node->node_pair_.first > node->node_pair_.first) {
    Insert(node->right_, new_node);
  }
  Balance(node, new_node->node_pair_.first);
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
  if (balance > 1 && key < node->left_->node_pair_.first) {
    node = RightRotate(node);
    return;
  }

  // Right Right Case
  if (balance < -1 && key > node->right_->node_pair_.first) {
    node = LeftRotate(node);
    return;
  }

  // Left Right Case
  if (balance > 1 && key > node->left_->node_pair_.first) {
    node->left_ = LeftRotate(node->left_);
    node = RightRotate(node);
    return;
  }

  // Right Left Case
  if (balance < -1 && key < node->right_->node_pair_.first) {
    node->right_ = RightRotate(node->right_);
    node = LeftRotate(node);
    return;
  }
}

template<typename K, typename V>
void Map<K, V>::Copy(Node *orig_node, Node *&node) {
  if (orig_node == nullptr) {
    return;
  }

  node = new Node(orig_node->node_pair_, orig_node->left_, orig_node->right_);

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
  y->height_ = Max(Height(y->left_), Height(y->right_)) + 1;
  x->height_ = Max(Height(x->left_), Height(x->right_)) + 1;

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
  x->height_ = Max(Height(x->left_), Height(x->right_)) + 1;
  y->height_ = Max(Height(y->left_), Height(y->right_)) + 1;

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
  std::cout << node->node_pair_.first << " ";
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
  std::cout << node->node_pair_.first << " ";
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
  std::cout << node->node_pair_.first << " ";
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::Begin() {
  return Map::Iterator(root_);
}

template<typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::End() {
  return Map::Iterator(nullptr);
}

template<typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::CBegin() const {
  const Node* const_root = root_;
  return Map::ConstIterator(const_root);
}

template<typename K, typename V>
typename Map<K, V>::ConstIterator Map<K, V>::CEnd() const {
  return Map::ConstIterator(nullptr);
}

template<typename K, typename V>
size_t Map<K, V>::Size() const {
  return length_;
}

#endif //SEARCH_ENGINE_MAP_H_