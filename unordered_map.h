//
// Created by jacob on 12/9/21.
//

#ifndef SEARCH_ENGINE_UNORDERED_MAP_H_
#define SEARCH_ENGINE_UNORDERED_MAP_H_

#include <vector>

template<typename K, typename V>
class UnorderedMap {

  struct Node {
    std::pair<const K, V> node_pair_;
    Node *next_;
    Node *prev_;

    Node() = default;
    Node(const Node &node) = default;
    Node &operator=(const Node &node) = default;
    explicit Node(const std::pair<const K, V> &node_pair,
                  Node *next = nullptr,
                  Node *prev = nullptr)
        : node_pair_{node_pair}, next_{next}, prev_{prev} {}
    ~Node() = default;
  };

  template<typename ValueType=std::pair<const K, V>>
  class MapIterator {
    using SelfType = MapIterator;
   public:
    friend class UnorderedMap<K, V>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;
    using pointer = ValueType *;
    using reference = ValueType &;

    MapIterator() : map_{nullptr}, node_{nullptr}, bucket_{0} {}
    MapIterator(const UnorderedMap<K, V> *map, Node *node, size_t bucket)
        : map_{map}, node_{node}, bucket_{bucket} {}

    MapIterator(const UnorderedMap<K, V> *map, bool end)
        : map_{map} {
      if (end) {
        bucket_ = map->num_buckets_;
        node_ = nullptr;
      } else {
        bucket_ = 0;
        node_ = map_->table_[bucket_];
        while (node_ == nullptr && ++bucket_ < map_->num_buckets_) {
          node_ = map_->table_[bucket_];
        }
      }
    }

    MapIterator(const MapIterator &it) = default;
    MapIterator &operator=(const MapIterator &) = default;
    ~MapIterator() = default;

    reference operator*() {
      return node_->node_pair_;
    }

    pointer operator->() {
      return &node_->node_pair_;
    }

    SelfType operator++() {
      node_ = node_->next_;
      while (node_ == nullptr && ++bucket_ < map_->num_buckets_) {
        node_ = map_->table_[bucket_];
      }
      return *this;
    }

    SelfType operator++(int) {
      MapIterator copy(*this);
      operator++();
      return copy;
    }

    friend bool operator==(const MapIterator &lhs,
                           const MapIterator &rhs) {
      return lhs.node_ == rhs.node_;
    }

    friend bool operator!=(const MapIterator &lhs,
                           const MapIterator &rhs) {
      return lhs.node_ != rhs.node_;
    }

   private:
    Node *node_;
    const UnorderedMap *map_;
    size_t bucket_;
  };

  template<typename ValueType=std::pair<const K, V>>
  class ConstMapIterator {
    using SelfType = ConstMapIterator;
   public:
    friend class UnorderedMap<K, V>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ValueType;
    using pointer = ValueType *;
    using reference = ValueType &;

    ConstMapIterator() : map_{nullptr}, node_{nullptr}, bucket_{0} {}
    ConstMapIterator(const UnorderedMap<K, V> *map, const Node *node, size_t bucket)
        : map_{map}, node_{node}, bucket_{bucket} {}
    ConstMapIterator(const UnorderedMap<K, V> *map, Node *node, size_t bucket)
        : map_{map}, node_{node}, bucket_{bucket} {}
    ConstMapIterator(const UnorderedMap<K, V> *map, bool end)
        : map_{map} {
      if (end) {
        bucket_ = map->num_buckets_;
        node_ = nullptr;
      } else {
        bucket_ = 0;
        node_ = map_->table_[bucket_];
        while (node_ == nullptr && ++bucket_ < map_->num_buckets_) {
          node_ = map_->table_[bucket_];
        }
      }
    }

    ConstMapIterator(const ConstMapIterator &it) = default;
    ConstMapIterator &operator=(const ConstMapIterator &) = default;
    ~ConstMapIterator() = default;

    reference operator*() {
      return node_->node_pair_;
    }

    pointer operator->() {
      return &node_->node_pair_;
    }

    SelfType operator++() {
      node_ = node_->next_;
      while (node_ == nullptr && ++bucket_ < map_->num_buckets_) {
        node_ = map_->table_[bucket_];
      }
      return *this;
    }

    SelfType operator++(int) {
      ConstMapIterator copy(*this);
      operator++();
      return copy;
    }

    friend bool operator==(const ConstMapIterator &lhs,
                           const ConstMapIterator &rhs) {
      return lhs.node_ == rhs.node_;
    }

    friend bool operator!=(const ConstMapIterator &lhs,
                           const ConstMapIterator &rhs) {
      return lhs.node_ != rhs.node_;
    }

   private:
    const UnorderedMap *map_;
    Node *node_;
    size_t bucket_;
  };

 public:
  using ValueType = std::pair<const K, V>;
  using ValuePointer = ValueType *;
  using ValueReference = ValueType &;
  using Iterator = MapIterator<ValueType>;
  using ConstIterator = ConstMapIterator<const ValueType>;

  UnorderedMap() {
    num_buckets_ = kInitialBucketSize_;
    size_ = 0;
    CreateBuckets(kInitialBucketSize_);
  }
  explicit UnorderedMap(size_t num_buckets)
      : num_buckets_{num_buckets},
        size_{0} {
    CreateBuckets(num_buckets);
  }

  UnorderedMap(const UnorderedMap &);
  ~UnorderedMap();
  UnorderedMap &operator=(const UnorderedMap &);

  Iterator Find(const K &);
  ConstIterator Find(const K &) const;
  std::pair<Iterator, bool> Insert(const std::pair<const K, V> &);
  [[nodiscard]] bool Empty() const;
  [[nodiscard]] size_t Size() const;
  void Clear();

  V &operator[](const K &);

  Iterator Begin();
  Iterator End();
  ConstIterator CBegin() const;
  ConstIterator CEnd() const;

 private:
  void Rehash();
  void CreateBuckets(size_t num_buckets);
  void DeleteBuckets(std::vector<Node *> &old_table);
  void Copy(const UnorderedMap &);
  size_t GetIndex(const K &) const;

  Node *FindNode(size_t bucket, const K &key) const;
  const Node *FindNode(size_t bucket, const K &key, const Node *&parent) const;

  static const int kInitialBucketSize_ = 101;
  static const int kMaxLoadPercentage = 70;
  std::vector<Node *> table_;
  size_t num_buckets_;
  size_t size_;
};

template<typename K, typename V>
UnorderedMap<K, V>::UnorderedMap(const UnorderedMap<K, V> &unordered_map)
    : num_buckets_{0}, size_{0} {
  Copy(unordered_map);
}

template<typename K, typename V>
UnorderedMap<K, V>::~UnorderedMap() {
  Clear();
}

template<typename K, typename V>
UnorderedMap<K, V> &UnorderedMap<K, V>::operator=(
    const UnorderedMap<K, V> &unordered_map) {

  if (this != &unordered_map) {
    Clear();
    Copy(unordered_map);
  }

  return *this;
}

template<typename K, typename V>
void UnorderedMap<K, V>::Copy(const UnorderedMap<K, V> &unordered_map) {

  CreateBuckets(unordered_map.num_buckets_);
  for (int i = 0; i < unordered_map.num_buckets_; i++) {
    for (Node *curr = unordered_map.table_.at(i); curr != nullptr;
         curr = curr->next_) {
      Insert(curr->node_pair_);
    }
  }
}

template<typename K, typename V>
bool UnorderedMap<K, V>::Empty() const {
  return size_ == 0;
}

template<typename K, typename V>
void UnorderedMap<K, V>::Clear() {
  DeleteBuckets(table_);
  size_ = 0;
}

template<typename K, typename V>
typename std::pair<typename UnorderedMap<K, V>::Iterator, bool> UnorderedMap<K,
                                                                             V>::Insert(
    const std::pair<const K, V> &pair) {
  size_t loc = GetIndex(pair.first);
  Node *node = table_[loc];
  if (node != nullptr) {
    return std::make_pair(Iterator(this, node, loc), false);
  }

  if (size_ > kMaxLoadPercentage * num_buckets_ / 100.0) {
    Rehash();
    loc = GetIndex(pair.first);
  }
  node = new Node(pair);
  node->next_ = table_[loc];
  table_[loc] = node;
  size_++;

  return std::make_pair(Iterator(this, node, loc), true);
}

template<typename K, typename V>
V &UnorderedMap<K, V>::operator[](const K &key) {
  size_t loc = GetIndex(key);
  Node *node = table_[loc];
  if (node == nullptr) {
    if (size_ > kMaxLoadPercentage * num_buckets_ / 100.0) {
      Rehash();
      loc = GetIndex(key);
    }
    node = new Node(std::make_pair(key, V()));
    node->next_ = table_[loc];
    table_[loc] = node;
    size_++;
  }

  return node->node_pair_.second;
}

template<typename K, typename V>
typename UnorderedMap<K, V>::Iterator UnorderedMap<K, V>::Find(const K &key) {
  size_t loc = GetIndex(key);
  Node *cp = FindNode(loc, key);
  return Iterator(this, cp, loc);
}

template<typename K, typename V>
typename UnorderedMap<K, V>::ConstIterator UnorderedMap<K, V>::Find(const K &key) const {
  size_t loc = GetIndex(key);
  const Node *cp = FindNode(loc, key);
  return ConstIterator(this, cp, loc);
}

template<typename K, typename V>
size_t UnorderedMap<K, V>::GetIndex(const K &key) const {
  auto hash_value = std::hash<K>{}(key);
  return hash_value % num_buckets_;
}

template<typename K, typename V>
typename UnorderedMap<K, V>::Iterator UnorderedMap<K, V>::Begin() {
  return Iterator(this, false);
}

template<typename K, typename V>
typename UnorderedMap<K, V>::Iterator UnorderedMap<K, V>::End() {
  return Iterator(this, true);
}

template<typename K, typename V>
typename UnorderedMap<K, V>::ConstIterator UnorderedMap<K, V>::CBegin() const {
  return ConstIterator(this, false);
}

template<typename K, typename V>
typename UnorderedMap<K, V>::ConstIterator UnorderedMap<K, V>::CEnd() const {
  return ConstIterator(this, true);
}

template<typename K, typename V>
void UnorderedMap<K, V>::CreateBuckets(size_t num_buckets) {
  if (num_buckets == 0) num_buckets_ = 1;
  num_buckets_ = num_buckets;
  table_ = std::vector<Node *>(num_buckets, nullptr);
  size_ = 0;
}

template<typename K, typename V>
void UnorderedMap<K, V>::DeleteBuckets(std::vector<Node *> &old_table) {
  for (int i = 0; i < old_table.size(); i++) {
    Node *curr = old_table[i];
    while (curr != nullptr) {
      Node *prev = curr->next_;
      delete curr;
      curr = prev;
    }
    old_table[i] = nullptr;
  }
}

template<typename K, typename V>
void UnorderedMap<K, V>::Rehash() {
  std::vector<Node *> old_table = table_;
  CreateBuckets(old_table.size() * 2 + 1);
  for (size_t i = 0; i < old_table.size(); i++) {
    for (Node *curr = old_table[i]; curr != nullptr; curr = curr->next_) {
      Insert(curr->node_pair_);
    }
  }
  DeleteBuckets(old_table);
}

template<typename K, typename V>
typename UnorderedMap<K, V>::Node *UnorderedMap<K, V>::FindNode(
    size_t bucket,
    const K &key) const {
  Node *dummy;
  return FindNode(bucket, key, dummy);
}

template<typename K, typename V>
const typename UnorderedMap<K, V>::Node *UnorderedMap<K, V>::FindNode(
    size_t bucket,
    const K &key,
    const Node *&parent) const {
  parent = nullptr;
  const Node *curr = table_.at(bucket);
  while (curr != nullptr && key != curr->node_pair_.first) {
    parent = curr;
    curr = curr->next_;
  }
  return curr;
}

template<typename K, typename V>
size_t UnorderedMap<K, V>::Size() const {
  return size_;
}

#endif //SEARCH_ENGINE_UNORDERED_MAP_H_
