#pragma once

#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <vector>

static const std::vector<unsigned long long int> primes{ 7, 13, 31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381, 32749, 65521, 131071, 262139, 524287, 1048573, 2097143, 4194301, 8388593, 16777213, 33554393, 67108859, 134217689, 268435399, 536870909, 1073741789, 2147483647, 4294967291, 8589934583, 17179869143, 34359738337, 68719476731, 137438953447, 274877906899, 549755813881, 1099511627689, 2199023255531, 4398046511093, 8796093022151, 17592186044399, 35184372088777, 70368744177643 };

static const std::vector<unsigned long long int> powers_of_two{ 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1099511627776, 2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664 };


template <typename Key, typename Value, typename Hash = std::hash<Key>>
class fast_hash_map
{
    struct node
    {
        enum Status { FREE, OCCUPIED, DELETED };
        Status status = FREE;
        std::pair<Key, Value> element_;
    };

public:
    fast_hash_map() : size_(0), index_(0), capacity_(primes[index_]), arr_(new node[powers_of_two[index_]]{}) {}

    fast_hash_map(const int capacity)
    {
        size_ = 0;
        auto it = std::lower_bound(primes.begin(), primes.end(), capacity);
        index_ = it == primes.end() ? primes.size() - 1 : std::distance(primes.begin(), it); // ���������
        capacity_ = primes[index_];
        arr_ = new node[powers_of_two[index_]]{};
    }

    fast_hash_map(const std::initializer_list<std::pair<Key, Value>>& list)
    {
        size_ = 0;
        auto it = std::lower_bound(primes.begin(), primes.end(), list.size() * 2);
        index_ = it == primes.end() ? primes.size() - 1 : std::distance(primes.begin(), it); // ���������
        capacity_ = primes[index_];
        arr_ = new node[powers_of_two[index_]]{};

        for (const auto& element : list)
        {
            insert(element);
        }
    }

    fast_hash_map(const fast_hash_map& other) : size_(0), index_(other.index_), capacity_(other.capacity_), arr_(new node[powers_of_two[index_]])
    {
        for (int i = 0; i < capacity_; ++i)
        {
            if (other.arr_[i].status == node::OCCUPIED)
                insert(other.arr_[i].element_);
        }
    }

    fast_hash_map& operator=(const fast_hash_map& other)
    {
        if (this != &other)
        {
            clear();
            for (int i = 0; i < other.capacity_; ++i)
            {
                if (other.arr_[i].status == node::OCCUPIED)
                    insert(other.arr_[i].element_);
            }
        }
        return *this;
    }

    fast_hash_map(fast_hash_map&& other) noexcept : size_(other.size_), index_(other.index_), capacity_(other.capacity_), arr_(other.arr_)
    {
        other.size_ = 0;
        other.index_ = 0;
        other.capacity_ = primes[other.index_];
        other.arr_ = new node[powers_of_two[other.index_]];
    }

    fast_hash_map& operator=(fast_hash_map&& other) noexcept
    {
        if (this != &other)
        {
            delete[] arr_;
            size_ = other.size_;
            index_ = other.index_;
            capacity_ = other.capacity_;
            arr_ = other.arr_;
            other.size_ = 0;
            other.index_ = 0;
            other.capacity_ = primes[other.index_];
            other.arr_ = new node[powers_of_two[other.index_]];
        }
        return *this;
    }

    void insert(const std::pair<Key, Value>& new_element)
    {
        check_capacity();
        int index = get_index(new_element.first);

        if (arr_[index].status == node::OCCUPIED)
        {
            arr_[index].element_.second = new_element.second;
        }
        else
        {
            arr_[index] = { node::OCCUPIED, new_element };
            size_++;
        }
    }

    void erase(const Key& key)
    {
        int index = get_index(key);
        if (arr_[index].status == node::OCCUPIED)
        {
            arr_[index].status = node::DELETED;
            size_--;
        }
    }

    bool contains(const Key& key) const
    {
        int index = get_index(key);
        return arr_[index].status == node::OCCUPIED;
    }

    Value& operator[](const Key& key)
    {
        int index = get_index(key);
        if (arr_[index].status != node::OCCUPIED)
        {
            if (check_capacity())
                index = get_index(key);

            arr_[index] = { node::OCCUPIED, {key, Value{}} };
            size_++;
        }
        return arr_[index].element_.second;
    }

    const Value& at(const Key& key) const
    {
        int index = get_index(key);
        if (arr_[index].status != node::OCCUPIED)
        {
            throw std::out_of_range("Key not found");
        }
        return arr_[index].element_.second;
    }


    void clear()
    {
        delete[] arr_;
        size_ = 0;
        index_ = 0;
        capacity_ = primes[index_];
        arr_ = new node[powers_of_two[index_]]{};
    }

    ~fast_hash_map()
    {
        delete[] arr_;
    }

    int size() const
    {
        return size_;
    }

    int capacity() const
    {
        return capacity_;
    }

    bool operator==(const fast_hash_map& other) const
    {
        if (size_ != other.size_)
            return false;

        for (int i = 0; i < capacity_; ++i)
        {
            if (arr_[i].status == node::OCCUPIED)
            {
                const Key& key = arr_[i].element_.first;
                if (!other.contains(key) || other.at(key) != arr_[i].element_.second)
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const fast_hash_map& other) const
    {
        return !(*this == other);
    }

    void print() const
    {
        for (int i = 0; i < capacity_; ++i)
        {
            if (arr_[i].status == node::OCCUPIED)
                std::cout << arr_[i].element_.first << " " << arr_[i].element_.second << std::endl;
        }
    }

private:
    int size_;
    int index_;
    int capacity_;
    node* arr_;
    Hash hasher_;

    // �������� ������������
    int get_index(const Key& key) const
    {
        int index = hasher_(key) % capacity_;

        while (arr_[index].status != node::FREE)
        {
            if (arr_[index].status == node::OCCUPIED && arr_[index].element_.first == key)
                return index;

            index = index == capacity_ - 1 ? 0 : index + 1;
        }
        return index;
    }

    // ������������ ������������
    //int get_index(const Key& key) const
    //{
    //    int index = hasher_(key) % capacity_;
    //    int i = 0; // ������� ��� ������������� ������������

    //    while (arr_[index].status != node::FREE)
    //    {
    //        if (arr_[index].status == node::OCCUPIED && arr_[index].element_.first == key)
    //            return index;
    // 
    //        i++;
    //        index = (index + i * i) % capacity_;
    //    }
    //    return index;
    //}

    // ������� �����������
   /* int get_index(const Key& key) const
    {
        int index1 = hasher_(key) % capacity_;
        int index2 = 1 + (hasher_(key) % (capacity_ - 1));

        int index = index1;
        int i = 0;

        while (arr_[index].status != node::FREE)
        {
            if (arr_[index].status == node::OCCUPIED && arr_[index].element_.first == key)
                return index;

            i++;
            index = (index1 + i * index2) % capacity_;
        }
        return index;
    }*/

    bool check_capacity()
    {
        if (size_ > capacity_ / 2)
        {
            int old_capacity = capacity_;
            capacity_ = primes[++index_];
            node* old_arr = arr_;
            arr_ = new node[powers_of_two[index_]]{};
            size_ = 0;

            for (int i = 0; i < old_capacity; ++i)
            {
                if (old_arr[i].status == node::OCCUPIED)
                    insert(old_arr[i].element_);
            }
            delete[] old_arr;
            return true;
        }
        return false;
    }
};
