/*
  * Реализуйте структуру данных типа “множество строк” на основе динамической
  * хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
  * строчных латинских букв.
  * Хеш-функция строки должна быть реализована с помощью вычисления значения
  * многочлена методом Горнера.
  * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
  * при добавлении элементов в случае, когда коэффициент заполнения таблицы
  * достигает 3/4.
  * Структура данных должна поддерживать операции добавления строки в множество,
  * удаления строки из множества и проверки принадлежности данной строки
  * множеству.
  * 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая
  * проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <cassert>
#include <iostream>
#include <string>

using std::string;

int hash(const string & elem, int m)
{
  int hash = 0;
  for (char c : elem) {
    hash = (hash * 19 + c) % m;
  }

  return hash;
}

template <class T>
class Hasher {
 public:
  int operator()(const T & elem, int m) const { return hash(elem, m); }
};

template <class T, class THash=Hasher<T>>
class HashTable {
 public:
  HashTable();
  HashTable(const HashTable & other) = delete;
  HashTable(HashTable && other) { *this = std::move(other); }
  ~HashTable() { clear(); }

  HashTable &operator=(const HashTable & other) = delete;
  HashTable &operator=(HashTable && other);
  bool Add(const T & elem);
  bool Has(const T & elem) const;
  bool Remove(const T & elem);
 private:
  static const int INIT_SIZE = 8;
  static const int MULTIPLIER = 2;

  struct TableElem
  {
    TableElem(): data_(T()), deleted_(false) {}
    T data_;
    bool deleted_;
  };

  TableElem ** buffer_;
  int buffer_size_;
  int size_;
  THash hash_;

  void rehash();
  void clear();
};

template <class T, class THash>
HashTable<T, THash>::HashTable(): buffer_size_(INIT_SIZE), size_(0) {
  buffer_ = new TableElem*[INIT_SIZE];
  for (int i = 0; i < INIT_SIZE; i++) {
    buffer_[i] = nullptr;
  }
}

template <class T, class THash>
HashTable<T, THash> & HashTable<T, THash>::operator=(HashTable && other) {
  clear();

  std::swap(buffer_, other.buffer_);
  std::swap(buffer_size_, other.buffer_size_);
  std::swap(size_, other.size_);

  return *this;
}

template <class T, class THash>
void HashTable<T, THash>::clear() {
  for (int i = 0; i < buffer_size_; i++) {
    if (buffer_[i]) {
      delete buffer_[i];
    }
  }
  delete[] buffer_;
}

template <class T, class THash>
bool HashTable<T, THash>::Add(const T & elem)
{
  if ((float) size_ / buffer_size_ >= 0.75) {
    rehash();
  }

  int key = hash_(elem, buffer_size_);
  for (int i = 0; i < buffer_size_; i++) {
    key = (key + i) % buffer_size_;

    if (buffer_[key] && !buffer_[key]->deleted_ && buffer_[key]->data_ == elem) {
      return false;
    }

    if (!buffer_[key] || buffer_[key]->deleted_) {
      if (!buffer_[key]) {
        buffer_[key] = new TableElem;
      }
      buffer_[key]->deleted_ = false;
      buffer_[key]->data_ = elem;

      size_++;

      return true;
    }
  }

  return true;
}

template <class T, class THash>
bool HashTable<T, THash>::Remove(const T &elem) {
  int key = hash_(elem, buffer_size_);
  for (int i = 0; i < buffer_size_; i++) {
    key = (key + i) % buffer_size_;

    if (!buffer_[key]) {
      return false;
    }

    if (!buffer_[key]->deleted_ && buffer_[key]->data_ == elem) {
      buffer_[key]->deleted_ = true;
      size_--;
      return true;
    }
  }

  return false;
}

template <class T, class THash>
bool HashTable<T, THash>::Has(const T &elem) const {
  int key = hash_(elem, buffer_size_);
  for (int i = 0; i < buffer_size_; i++) {
    key = (key + i) % buffer_size_;

    if (!buffer_[key]) {
      return false;
    }
    if (!buffer_[key]->deleted_ && buffer_[key]->data_ == elem) {
      return true;
    }
  }
  return false;
}

template <class T, class THash>
void HashTable<T, THash>::rehash() {
  auto ** buffer = new TableElem*[buffer_size_ * MULTIPLIER];
  for (int i = 0; i < buffer_size_; i++) {
    buffer_[i] = nullptr;
  }
  int old_size = buffer_size_;
  buffer_size_ *= 2;

  for (int i = 0; i < old_size; i++) {
    if (!buffer_[i] || buffer_[i]->deleted_) {
      continue;
    }

    int key = hash_(buffer[i]->data_, buffer_size_);
    for (int j = 0; j < buffer_size_; j++) {
      key = (key + i) % buffer_size_;
      if (!buffer[key]) {
        buffer[key] = buffer_[i];
        break;
      }
    }
  }

  delete[] buffer_;
  buffer_ = buffer;
}

int main() {
  HashTable<string, Hasher<string>> table;
  char command = 0;
  std::string key;
  while (std::cin >> command >> key) {
    switch (command) {
      case '?':
        std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
        break;
      case '+':
        std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
        break;
      case '-':
        std::cout << (table.Remove(key) ? "OK" : "FAIL") << std::endl;
        break;
      default:
        std::cerr << "Wrong input" << std::endl;
        return 0;
    }
  }
  return 0;
}