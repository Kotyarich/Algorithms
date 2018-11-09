/*
 * 7_3. Binary MSD для long long.
 * Дан массив неотрицательных целых 64-разрядных чисел. Количество чисел
 * не больше 10^6. Отсортировать массив методом MSD по битам (бинарный QuickSort).
*/

#include <iostream>
#include <cassert>
#include <algorithm>
#include <string>
#include <cstring>
#include <climits>


struct Bounds {
  int Min;
  int Max;
};

template <class T>
class Key {
 public:
  explicit Key(int bit): bit_number(bit) {}

  int operator()(const T & elem) { return 1 & (elem >> bit_number); }

  int bit_number;
};

template <class T, class TKey>
Bounds findMinMaxKey(const T* arr, int size, TKey getKey)
{
    int min = INT_MAX;
    int max = INT_MIN;
    for (int i = 0; i < size; i++) {
        min = std::min(min, getKey(arr[i]));
        max = std::max(max, getKey(arr[i]));
    }

    return { min, max };
}

template <class T, class TKey>
void CountSort(T* arr, int size, TKey key, int** groups = nullptr)
{
    auto bounds = findMinMaxKey(arr, size, key);

    const int binsCount = bounds.Max - bounds.Min + 1;
    int* bins = new int[binsCount];
    if (groups)
      *groups = new int[binsCount];

    memset(bins, 0, binsCount * sizeof(int));
    for (int i = 0; i < size; i++) {
        bins[key(arr[i]) - bounds.Min]++;
    }

    for (int i = 1; i < binsCount; i++) {
        bins[i] += bins[i - 1];
    }

    T* tmp = new T[size];
    for (int i = 0; i < size; i++) {
        tmp[i] = arr[i];
    }

    for (int i = size - 1; i >= 0; i--) {
        int& bin = bins[key(tmp[i]) - bounds.Min];
        arr[--bin] = tmp[i];
    }

    if (groups) {
        std::copy_n(bins, binsCount, *groups);
    }

    delete[] tmp;
    delete[] bins;
}

template <class T, class TKey>
void BinMSD(T* arr, int n, TKey key)
{
    if (n <= 1 || key.bit_number < 0) {
        return;
    }

    int* groups = new int[2];
    CountSort(arr, n, key, &groups);

    key.bit_number--;
    BinMSD(arr, groups[1], key);
    BinMSD(arr + groups[1], n - groups[1], key);

    delete[] groups;
}

int main()
{
    int len = -1;
    std::cin >> len;
    auto *arr = new long long[len];

    for (int i = 0; i < len; i++) {
        std::cin >> arr[i];
    }

    BinMSD(arr, len, Key<long long>(63));

    for (int i = 0; i < len; i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;

    delete[] arr;

    return 0;
}
