/*
 * Во всех задачах данного раздела необходимо реализовать и использовать
 * сортировку слиянием. Общее время работы алгоритма O(n log n).
 * 5_1. Реклама.
 * В супермаркете решили оптимизировать показ рекламы. Известно расписание
 * прихода и ухода покупателей (два целых числа). Каждому покупателю
 * необходимо показать минимум 2 рекламы.  Рекламу можно транслировать
 * только в целочисленные моменты времени. Покупатель может видеть рекламу
 * от момента прихода до момента ухода из магазина.
 * В каждый момент времени может показываться только одна реклама. Считается,
 * что реклама показывается мгновенно. Если реклама показывается в момент
 * ухода или прихода, то считается, что посетитель успел её посмотреть.
 * Требуется определить минимальное число показов рекламы.
*/

#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

template<typename T>
void merge(T *dst, size_t dst_len, T *first, size_t first_len,
           T *second, size_t second_len)
{
    // Слияние отсортированных массивов first и second в массив dst.
    assert(dst_len > 0 && first && first_len > 0 && second && second_len > 0);

    int i = 0, j = 0, k = 0;
    for(; i < first_len && j < second_len; k++)
    {
        if (first[i] < second[j])
        {
            dst[k] = first[i];
            i++;
        }
        else
        {
            dst[k] = second[j];
            j++;
        }
    }
    // Если в первом массиве остались элементы.
    for (; i < first_len; k++, i++)
        dst[k] = first[i];
    // Если во втором массиве остались элементы.
    for (; j < second_len; k++, j++)
        dst[k] = second[j];
}

template<typename T>
void merge_sort(T *arr, size_t len)
{
    assert(arr);
    if (len <= 1)
        return;

    size_t middle = len / 2;
    merge_sort(arr, middle);
    merge_sort(arr + middle, len - middle);

    T *merged = new T[len];
    merge(merged, len, arr, middle, arr + middle, len - middle);
    memcpy(arr, merged, sizeof(T) * len);

    delete[] merged;
}

class Customer
{
public:
    Customer():
        arrive(0),
        departure(0) {}

    int arrive;
    int departure;
};

const bool operator<(const Customer &left, const Customer &right)
{
    return left.departure < right.departure ||
           (left.arrive <= right.arrive && left.departure == right.departure);
}

size_t get_ad_count(Customer *customers, size_t len)
{
    assert(customers);
    if (!len)
      return 0;

    size_t count = 2;
    int end_time = customers[0].departure;
    int penult_end_time = customers[0].departure - 1;

    for (int i = 1; i < len; i++)
    {
        if (customers[i].arrive > end_time)
        {
            count += 2;
            end_time = customers[i].departure;
            penult_end_time = customers[i].departure - 1;
        }
        else if (customers[i].arrive > penult_end_time)
        {
            count++;
            if (customers[i].departure > end_time)
            {
                end_time = customers[i].departure;
                penult_end_time = end_time;
            }
            else
            {
                penult_end_time = customers[i].departure;
            }
        }
    }

    return count;
}

int main()
{
    size_t n = 0;
    std::cin >> n;

    auto *customers = new Customer[n];
    for (int i = 0; i < n; i++)
        std::cin >> customers[i].arrive >> customers[i].departure;

    merge_sort(customers, n);
    std::cout << get_ad_count(customers, n) << std::endl;

    delete[] customers;

    return 0;
}
