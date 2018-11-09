
 * На вокзале есть некоторое количество тупиков, куда прибывают электрички.
 * Этот вокзал является их конечной станцией. Дано расписание движения
 * электричек, в котором для каждой электрички указано время ее прибытия, а
 * также время отправления в следующий рейс. Электрички в расписании
 * упорядочены по времени прибытия. Когда электричка прибывает, ее ставят в
 * свободный тупик с минимальным номером. При этом если электричка из
 * какого-то тупика отправилась в момент времени X, то электричку, которая
 * прибывает в момент времени X, в этот тупик ставить нельзя, а электричку,
 * прибывающую в момент X+1 — можно.
 * В данный момент на вокзале достаточное количество тупиков для работы по
 * расписанию.
 * Напишите программу, которая по данному расписанию определяет, какое
 * минимальное количество тупиков требуется для работы вокзала.
*/

#include <iostream>
#include <algorithm>
#include <cassert>
#include <cstring>

template<class T>
class Heap {
public:
    Heap():
        size(0)
    {
        const int init_size = 1;
        capacity = init_size,
        data = new T[init_size];
    }
    Heap(const Heap& other) = delete;
    Heap(Heap&& other) { *this = std::move(other); }
    ~Heap() { clean(); }

    Heap& operator=(const Heap& other) = delete;
    Heap& operator=(Heap&& other)
    {
        clean();

        std::swap(data, other.data);
        std::swap(capacity, other.capacity);
        std::swap(size, other.size);

        return *this;
    }

    Heap MakeCopy() const
    {
        Heap copy;

        copy.size = size;
        copy.capacity = capacity;
        copy.data = new T[capacity];
        memcpy(copy.data, data, capacity * sizeof(T));

        return copy;
    }

    void Insert(T value)
    {
        if (size >= capacity)
            grow();

        data[size] = value;
        size++;
        ShiftUp(size - 1);
    }

    T ExtractMin()
    {
        assert(!IsEmpty());

        T result = data[0];
        data[0] = data[size - 1];
        size--;

        if (!IsEmpty())
            ShiftDown(0);

        return result;
    }

    T Top() const
    {
        assert(!IsEmpty());
        return data[0];
    }

    size_t Size() const { return size; }
    bool IsEmpty() const { return size <= 0; }

private:
    T *data;
    size_t capacity;
    size_t size;

    void ShiftDown(size_t i)
    {
        assert(i < size);

        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;

        int largest = i;
        if (left < size && data[left] < data[i])
            largest = left;
        if (right < size && data[right] < data[largest])
            largest = right;

        if (largest != i)
        {
            std::swap(data[i], data[largest]);
            ShiftDown(largest);
        }
    }

    void ShiftUp(size_t i)
    {
        assert(i <= size);

        while (i > 0)
        {
            size_t parent = (i - 1) / 2;
            if (data[i] >= data[parent])
                return ;
            std::swap(data[i], data[parent]);
            i = parent;
        }
    }

    void clean() { delete[] data; }
    void grow()
    {
        const int multiplier = 2;

        int *tmp_buffer = new T[capacity * multiplier];
        memcpy(tmp_buffer, data, capacity * sizeof(T));
        delete[] data;
        capacity *= multiplier;
        data = tmp_buffer;
    }
};

//--------------------------------------------------------------------------------

struct Train {
    int arrive = 0;
    int departure = 0;
};

int main()
{
    int n = 0;
    std::cin >> n;

    Heap<int> heap;
    int size = 0;
    for (int i = 0; i < n; i++) {
        Train train;
        std::cin >> train.arrive >> train.departure;
        if (!heap.IsEmpty() && heap.Top() < train.arrive)
            heap.ExtractMin();
        heap.Insert(train.departure);
        if (size < heap.Size())
            size = heap.Size();
    }

    std::cout << size << std::endl;

    return 0;
}
