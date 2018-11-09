/*
 * Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9]
 * размера n. Требуется найти k-ю порядковую статистику. т.е. напечатать
 * число, которое бы стояло на позиции с индексом k (0..n-1) в отсортированном
 * массиве. Напишите нерекурсивный алгоритм.
 * Требования к дополнительной памяти: O(n). Требуемое среднее время
 * работы: O(n).
 * Функцию Partition следует реализовывать методом прохода двумя итераторами
 * в одном направлении. Описание для случая прохода от начала массива к концу:
 *  - Выбирается опорный элемент. Опорный элемент меняется с последним
 * элементом массива.
 *   - Во время работы Partition в начале массива содержатся элементы, не
 * бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного.
 * В конце массива лежат нерассмотренные элементы. Последним элементом лежит
 * опорный.
 *   - Итератор (индекс) i указывает на начало группы элементов, строго бОльших
 * опорного.
 *   - Итератор j больше i, итератор j указывает на первый нерассмотренный
 * элемент.
 *   - Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он
 * больше опорного, то сдвигаем j.
 *   Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и
 * сдвигаем j.
 *   - В конце работы алгоритма меняем опорный и элемент, на который указывает
 * итератор i.
 *
 * 6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию
 * Partition реализуйте методом прохода двумя итераторами от конца массива к
 * началу.
*/

#include <iostream>
#include <cassert>

template <class T>
int pivot(T *arr, int begin, int end)
{
    assert(arr && end > begin);

    if (end - begin < 3)
        return begin;
    const int median = (begin + end - 1) / 2;

    if ((arr[begin] >= arr[median] && arr[begin] <= arr[end - 1]) ||
        (arr[begin] <= arr[median] && arr[begin] >= arr[end - 1]))
        return begin;

    if ((arr[median] >= arr[begin] && arr[median] <= arr[end - 1]) ||
        (arr[median] <= arr[begin] && arr[median] >= arr[end - 1]))
        return median;

    return end - 1;
}

template <class T>
int partition(T* arr, int begin, int end)
{
    std::swap(arr[begin], arr[pivot(arr, begin, end)]);

    int i = end - 1;
    int j = end - 1;

    while (j > begin && i > begin)
    {
        while (j > begin && arr[j] < arr[begin])
            j--;

        while (j > begin && arr[j] >= arr[begin])
            std::swap(arr[i--], arr[j--]);
    }

    std::swap(arr[begin], arr[i]);

    return i;
}

template <class T>
T k_stat(T *arr, int begin, int end, int k)
{
    assert(begin < end);

    const int pivot = partition(arr, begin, end);
    if (pivot > k)
        return k_stat(arr, begin, pivot, k);

    if (pivot < k)
        return k_stat(arr, pivot + 1, end, k);

    return arr[pivot];
}

int main(void)
{
    size_t n = 0, k = 0;
    std::cin >> n >> k;

    int *arr = new int[n];
    for (size_t i = 0; i < n; i++)
        std::cin >> arr[i];

    std::cout << k_stat(arr, 0, n, k) << std::endl;

    delete[] arr;

    return 0;
}