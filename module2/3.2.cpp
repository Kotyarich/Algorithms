/*
 * Дано число N < 106 и последовательность пар целых чисел из [-231..231]
 * длиной N. Построить декартово дерево из N узлов, характеризующихся парами
 * чисел {Xi, Yi}.
 * Каждая пара чисел {Xi, Yi} определяет ключ Xi и приоритет Yi в декартовом
 * дереве. Добавление узла в декартово дерево выполняйте второй версией
 * алгоритма, рассказанного на лекции:
 *  При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим
 *  приоритетом. Затем разбейте найденное поддерево по ключу x так, чтобы в
 *  первом поддереве все ключи меньше x, а во втором больше или равны x.
 *  Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 *  Новый узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi методом из задачи 2.
 *
 * 3_2. Вычислить количество узлов в самом широком слое декартового дерева и
 * количество узлов в самом широком слое наивного дерева поиска.
 * Вывести их разницу. Разница может быть отрицательна.
 */

#include <cassert>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <stack>
#include <queue>

template <class T>
class BinaryTree {
 public:
  BinaryTree(): root_(nullptr) {};
  BinaryTree(const BinaryTree & other) = delete;
  BinaryTree(BinaryTree && other) { *this = std::move(other); }
  ~BinaryTree() { clear(); }

  BinaryTree &operator=(const BinaryTree & other) = delete;
  BinaryTree &operator=(BinaryTree && other)
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

  int get_width() const
  {
    if (!root_) {
      return 0;
    }

    int width = 0;
    std::queue<Node*> queue;
    queue.push(root_);
    while (!queue.empty()) {
      int size = queue.size();
      if (size > width) {
        width = size;
      }
      for (int i = 0; i < size; i++) {
        Node * current_node = queue.front();
        queue.pop();
        if (current_node->left) {
          queue.push(current_node->left);
        }
        if (current_node->right) {
          queue.push(current_node->right);
        }
      }
    }

    return width;
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

template <class T>
class DTree {
 public:
  DTree(): root_(nullptr) {};
  DTree(const DTree & other) = delete;
  DTree(DTree && other) { *this = std::move(other); }
  ~DTree() { clear(); }

  DTree &operator=(const DTree & other) = delete;
  DTree &operator=(DTree && other)
  {
    clear();

    std::swap(root_, other.root_);

    return *this;
  }

  void add(T key, T prior)
  {
    DNode * new_node = new DNode;
    *new_node = {key, prior, nullptr, nullptr};

    if (!root_) {
      root_ = new_node;
      return;
    }

    DNode * current_parent = nullptr;
    DNode * current_node = root_;
    bool is_left = false;
    while (current_node && current_node->prior >= prior) {
      current_parent = current_node;
      if (current_node->key <= key) {
        is_left = false;
        current_node = current_node->right;
      } else {
        is_left = true;
        current_node = current_node->left;
      }
    }

    split(key, current_node, new_node->left, new_node->right);

    if (current_parent) {
      if (is_left) {
        current_parent->left = new_node;
      } else {
        current_parent->right = new_node;
      }
    } else {
      root_ = new_node;
    }
  }

  int get_width() const
  {
    if (!root_) {
      return 0;
    }

    int width = 0;
    std::queue<DNode*> queue;
    queue.push(root_);
    while (!queue.empty()) {
      int size = queue.size();
      if (size > width) {
        width = size;
      }
      for (int i = 0; i < size; i++) {
        DNode * current_node = queue.front();
        queue.pop();
        if (current_node->left) {
          queue.push(current_node->left);
        }
        if (current_node->right) {
          queue.push(current_node->right);
        }
      }
    }

    return width;
  }

 private:
  struct DNode {
    T key;
    int prior;
    DNode * left;
    DNode * right;
  };

  DNode * root_;

  void split(T key, DNode * current_node, DNode *& left, DNode *& right)
  {
    if (!current_node) {
      left = nullptr;
      right = nullptr;
    } else if (current_node->key < key) {
      split(key, current_node->right, current_node->right, right);
      left = current_node;
    } else {
      split(key, current_node->left, left, current_node->left);
      right = current_node;
    }
  }

  void clear()
  {
    auto * nodes = new std::stack<DNode*>;

    nodes->push(root_);
    while (!nodes->empty()) {
      DNode * curr_node = nodes->top();
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

  BinaryTree<int> binary_tree;
  DTree<int> d_tree;
  for (int i = 0; i < n; i++) {
    int tmp_key = 0, tmp_priority = 0;
    std::cin >> tmp_key >> tmp_priority;
    binary_tree.add(tmp_key);
    d_tree.add(tmp_key, tmp_priority);
  }

  std::cout <<  d_tree.get_width() - binary_tree.get_width() << std::endl;

  return 0;
}