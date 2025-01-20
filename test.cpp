#include <iostream>
#include <cassert>
#include <initializer_list>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class fast_hash_map
{
    struct node
    {
        bool free_ = true;
        std::pair<Key, Value> element_;
    };

public:
    fast_hash_map() : size_(0), capacity_(7), arr_(new node[7]{}) {}

    explicit fast_hash_map(const int capacity) : size_(0), capacity_(capacity), arr_(new node[capacity]{}) {}

    fast_hash_map(const std::initializer_list<std::pair<Key, Value>>& list) : size_(0), capacity_(list.size() * 2), arr_(new node[capacity_])
    {
        for (const auto& element : list)
        {
            insert(element);
        }
    }

    fast_hash_map(const fast_hash_map& other) : size_(0), capacity_(other.capacity_), arr_(new node[capacity_])
    {
        for (int i = 0; i < capacity_; ++i)
        {
            if (!other.arr_[i].free_)
                insert(other.arr_[i].element_);
        }
    }


    fast_hash_map& operator=(const fast_hash_map& other)
    {
        if (&other != this)
        {
            clear();
            for (int i = 0; i < other.capacity_; ++i)
            {
                if (!other.arr_[i].free_)
                    insert(other.arr_[i].element_);
            }
        }
        return *this;
    }

    fast_hash_map(fast_hash_map&& other) : size_(other.size_), capacity_(other.capacity_), arr_(other.arr_)
    {
        other.size_ = 0;
        other.capacity_ = 7;
        other.arr_ = new node[other.capacity_];
    }

    fast_hash_map& operator=(fast_hash_map&& other)
    {
        if (&other != this) {
            size_ = other.size_;
            capacity_ = other.capacity_;
            delete[] arr_;
            arr_ = other.arr_;
            other.size_ = 0;
            other.capacity_ = 7;
            other.arr_ = new node[other.capacity_];
        }
        return *this;
    }

    void insert(const std::pair<Key, Value>& new_element)
    {
        check_capacity();
        arr_[get_index(new_element.first)] = { false, new_element };
        size_++;
    }

    void erase(const Key& key)
    {
        node& ref = arr_[get_index(key)];
        if (!ref.free_)
        {
            ref.free_ = true;
            size_--;
        }
    }

    bool contains(const Key& key) const
    {
        return arr_[get_index(key)].free_ == false;
    }

    Value& operator[](const Key& key)
    {
        node* ptr = arr_ + get_index(key);

        if (ptr->free_)
        {
            if (check_capacity())
                ptr = arr_ + get_index(key);

            *ptr = { false, {key, Value{}} };
            size_++;
        }
        return ptr->element_.second;
    }

    const Value& at(const Key& key) const
    {
        node* ptr = arr_ + get_index(key);

        if (ptr->free_)
            throw std::out_of_range("element not found");

        return ptr->element_.second;
    }

    int size() const
    {
        return size_;
    }

    int capacity() const
    {
        return capacity_;
    }

    bool operator==(const fast_hash_map& right) const
    {
        if (right.size_ != size_)
            return false;

        for (int i = 0; i < capacity_; ++i)
        {
            if (!arr_[i].free_)
            {
                auto& element = arr_[i].element_;
                if (!right.contains(element.first) || right.at(element.first) != element.second)
                    return false;
            }
        }
        return true;
    }

    bool operator!=(const fast_hash_map& right) const
    {
        return !(*this == right);
    }

    void print() const
    {
        for (int i = 0; i < capacity_; ++i)
        {
            if (!arr_[i].free_)
                std::cout << arr_[i].element_.first << " " << arr_[i].element_.second << std::endl;
            else
                std::cout << "- // - " << std::endl;
        }
        std::cout << std::endl;
    }

    void clear()
    {
        for (int i = 0; i < capacity_; ++i)
        {
            arr_[i].free_ = true;
        }
        size_ = 0;
    }

    ~fast_hash_map()
    {
        delete[] arr_;
    }

private:

    int get_index(const Key& key) const
    {
        int start_index = hash_function_(key) % capacity_;

        if (arr_[start_index].free_ || arr_[start_index].element_.first == key)
        {
            return start_index;
        }
        else
        {
            int index = start_index < capacity_ - 1 ? start_index + 1 : 0;
            while (index != start_index)
            {
                if (arr_[index].free_ || arr_[index].element_.first == key)
                    return index;
                else
                    index = index < capacity_ - 1 ? index + 1 : 0;
            }
        }
    }

    bool check_capacity()
    {
        if (size_ < capacity_ / 2)
            return false;

        int old_capacity = capacity_;
        capacity_ *= 2;
        node* temp = new node[capacity_];
        for (int i = 0; i < old_capacity; ++i)
        {
            if (!arr_[i].free_)
                temp[get_index(arr_[i].element_.first)] = arr_[i];
        }
        delete[] arr_;
        arr_ = temp;
        return true;
    }

    int size_;
    int capacity_;
    node* arr_;
    Hash hash_function_;
};

int main()
{
    fast_hash_map<int, int> fhm1;
    fhm1.insert({ 1,2 });
    fhm1.insert({ 3,4 });
    assert(fhm1[1] == 2);
    fhm1.erase(3);
    assert(fhm1.contains(1));
    assert(!fhm1.contains(3));

    fast_hash_map<int, int> fhm2;

    for (int i = 0; i < 10; ++i)
    {
        fhm1[i] = i + 30;
        fhm2[i] = i + 30;
    }

    fast_hash_map<int, int> fhm3{ {1,3}, {2,4}, {5,8} };
    fhm3.print();

    fast_hash_map<int, int> fhm4(fhm3);
    fhm1 = fhm3;
    assert(fhm1 == fhm4);
}