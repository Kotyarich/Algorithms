/*
 * Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если
 * root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое
 * поддерево root.
 * Рекурсия запрещена.
 * 2_2. Выведите элементы в порядке pre-order (сверху вниз).
 */

#include <cassert>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <stack>

template <class T>
class Tree {
 public:
  Tree(): root_(nullptr) {};
  Tree(const Tree & other) = delete;
  Tree(Tree && other) { *this = std::move(other); }
  ~Tree() { clear(); }

  Tree &operator=(const Tree & other) = delete;
  Tree &operator=(Tree && other)
  {
    clear();

    std::swap(root_, other.root_);

    return *this;
  }

  void preOrderPrint()
  {
    auto * nodes = new std::stack<Node*>;

    nodes->push(root_);
    while (!nodes->empty()) {
      Node * curr_node = nodes->top();
      nodes->pop();

      if (curr_node->right) {
        nodes->push(curr_node->right);
      }
      if (curr_node->left) {
        nodes->push(curr_node->left);
      }

      std::cout << curr_node->value << " ";
    }

    delete nodes;
  }

  void add(T & value)
  {
    auto new_node = new Node;
    *new_node = {value, nullptr, nullptr};
    auto next = root_;
    if (!root_) {
      root_ = new_node;
      return;
    }

    while (true) {
      if (next->value > value) {
        if (next->left) {
          next = next->left;
        } else {
          next->left = new_node;
          return;
        }
      } else {
        if (next->right) {
          next = next->right;
        } else {
          next->right = new_node;
          return;
        }
      }
    }
  }

 private:
  struct Node {
    T value;
    Node * left;
    Node * right;
  };

  Node * root_;

  void clear()
  {
    auto * nodes = new std::stack<Node*>;

    nodes->push(root_);
    while (!nodes->empty()) {
      Node * curr_node = nodes->top();
      nodes->pop();

      if (curr_node->right) {
        nodes->push(curr_node->right);
      }
      if (curr_node->left) {
        nodes->push(curr_node->left);
      }

      delete curr_node;
    }

    delete nodes;
  }
};

int main()
{
  int n = 0;
  std::cin >> n;

  Tree<int> tree;
  for (int i = 0; i < n; i++) {
    int tmp;
    std::cin >> tmp;
    tree.add(tmp);
  }

  tree.preOrderPrint();

  return 0;
}