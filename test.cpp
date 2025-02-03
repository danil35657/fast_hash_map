﻿#include <unordered_map>
#include <cassert>
#include <chrono>
#include <random>
#include "fast_hash_map.h"
#include "flat_hash_map.hpp"

template <typename Container>
void print(Container& cont)
{
   /* for (auto it = cont.begin(); it != cont.end(); ++it)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }*/

    for (auto it : cont)
    {
        std::cout << it.first << " " << it.second << std::endl;
    }
}


void functional_test() {
    using Key = std::string;
    using Value = int;

    // Конструктор по умолчанию
    fast_hash_map<Key, Value> map1;
    assert(map1.size() == 0);
    assert(map1.capacity() > 0);

    // Вставка элементов
    map1.insert({ "one", 1 });
    map1.insert({ "two", 2 });
    map1.insert({ "three", 3 });
    assert(map1.size() == 3);
    assert(map1.contains("one"));
    assert(map1.contains("two"));
    assert(map1.contains("three"));

    // Проверка оператора []
    map1["four"] = 4;
    assert(map1.size() == 4);
    assert(map1["four"] == 4);
    map1["four"] = 40;
    assert(map1["four"] == 40);

    // Проверка метода at
    assert(map1.at("one") == 1);
    assert(map1.at("two") == 2);
    try {
        map1.at("five"); // Должно вызывать исключение
        assert(false);
    }
    catch (const std::out_of_range&) {
        assert(true);
    }

    // Удаление элементов
    map1.erase("one");
    assert(!map1.contains("one"));
    assert(map1.size() == 3);

    // Проверка конструктора с начальной вместимостью
    fast_hash_map<Key, Value> map2(100);
    assert(map2.capacity() >= 100);
    assert(map2.size() == 0);

    // Конструктор копирования
    fast_hash_map<Key, Value> map3 = map1;
    assert(map3.size() == 3);
    assert(map3.contains("two"));
    assert(map3.contains("three"));
    assert(map3.contains("four"));

    // Оператор присваивания
    fast_hash_map<Key, Value> map4;
    map4 = map1;
    assert(map4.size() == 3);
    assert(map4.contains("two"));
    assert(map4.contains("three"));
    assert(map4.contains("four"));

    // Конструктор перемещения
    fast_hash_map<Key, Value> map5 = std::move(map1);
    assert(map5.size() == 3);
    assert(map5.contains("two"));
    assert(map5.contains("three"));
    assert(map5.contains("four"));
    assert(map1.size() == 0); // map1 должно быть пустым после перемещения

    // Оператор присваивания с перемещением
    fast_hash_map<Key, Value> map6;
    map6 = std::move(map3);
    assert(map6.size() == 3);
    assert(map6.contains("two"));
    assert(map6.contains("three"));
    assert(map6.contains("four"));
    assert(map3.size() == 0); // map3 должно быть пустым после перемещения

    // Операторы сравнения
    assert(map5 == map6);
    map6.erase("two");
    assert(map5 != map6);
    auto it = map5.find("two");
    assert(it->first == "two" && it->second == 2);
    it = map6.find("two");
    assert(it == map6.end());


    // Проверка clear
    map6.clear();
    assert(map6.size() == 0);
    assert(!map6.contains("three"));

    // Тестирование краевых случаев
    fast_hash_map<Key, Value> edge_map;
    edge_map.insert({ "key", 1 });
    edge_map.insert({ "key", 2 }); // Обновление значения
    assert(edge_map.size() == 1);
    assert(edge_map.at("key") == 2);
    

    //print(map5);

    std::cout << "All tests passed!" << std::endl;
}

void test_find()
{
    fast_hash_map<int, int> myMap;
    //std::unordered_map<int, int> myMap;
    //ska::flat_hash_map<int, int> myMap;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10000000);

    const int maxSize = 1000000;

    int targetSize = 10; // Начальный размер контейнера
    int incrementSize = 10; // Размер увеличения на каждом шаге   1 / 10

    // Изначально добавляем 10 элементов
    while (myMap.size() < targetSize) {
        //int randomNumber = abs(dis(gen) * dis(gen));
        int randomNumber = dis(gen);
        myMap[randomNumber] = 0; // Значение может быть любым, здесь 0
    }

    while (targetSize <= maxSize) {
        // Запоминаем одно из сгенерированных чисел
        //int keyToFind = std::next(myMap.begin(), dis(gen) % myMap.size())->first;

        // Измерение времени выполнения 1000000 операций поиска
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; ++i) {
            myMap.find(i);
        }
        auto end = std::chrono::high_resolution_clock::now();

        // Вычисление времени одной операции поиска
        std::chrono::duration<int, std::nano> duration = end - start;
        double timePerOperation = duration.count() / 1000000; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << /*targetSize << " - " <<  */timePerOperation  /*<< " ns"  */<< std::endl;

        if (targetSize == maxSize) // 10000000
            return;

        // Увеличение размера контейнера
        if (targetSize == incrementSize * 10) // 100 / 10
            incrementSize *= 10;

        // Увеличиваем targetSize на incrementSize
        targetSize += incrementSize;

        // Добавляем новые случайные числа в unordered_map
        while (myMap.size() < targetSize) {
            //int number = abs(dis(gen) * dis(gen));
            int number = dis(gen);
            myMap[number] = 0; // Значение может быть любым, здесь 0
        }
    }
    std::cout << "============" << std::endl;
}

void test_insert()
{
    //fast_hash_map<int, int> myMap;
    std::unordered_map<int, int> myMap;
    //ska::flat_hash_map<int, int> myMap;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10000000);

    const int maxSize = 1000000;

    int targetSize = 10;
    int incrementSize = 10; // Размер увеличения на каждом шаге   1 / 10

    while (targetSize <= maxSize) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < targetSize; ++i) {
            int number = dis(gen);
            //myMap[number] = 0;
            myMap.insert({ number, 0 });
        }
        auto end = std::chrono::high_resolution_clock::now();

        // Вычисление времени одной операции поиска
        std::chrono::duration<int, std::nano> duration = end - start;
        double timePerOperation = duration.count() / targetSize; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << /*targetSize << " - " <<  */timePerOperation  /*<< " ns"  */ << std::endl;

        if (targetSize == maxSize) // 1000000
            return;

        // Увеличение размера контейнера
        if (targetSize == incrementSize * 10) // 100 / 10
            incrementSize *= 10;

        // Увеличиваем targetSize на incrementSize
        targetSize += incrementSize;

        myMap.clear();
    }
    std::cout << "============" << std::endl;
}

void test_erase()
{
    //fast_hash_map<int, int> myMap;
    std::unordered_map<int, int> myMap;
    //ska::flat_hash_map<int, int> myMap;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 10000000);

    const int maxSize = 1000000;

    std::vector<int> elements(maxSize);

    for (int i = 0; i < maxSize; ++i)
    {
        elements[i] = dis(gen);
    }

    int targetSize = 10;
    int incrementSize = 10; // Размер увеличения на каждом шаге   1 / 10

    while (targetSize <= maxSize) {

        for (int i = 0; i < targetSize; ++i) {
            myMap[elements[i]] = 0;
        }

        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < targetSize; ++i) {
            myMap.erase(elements[i]);
        }
        auto end = std::chrono::high_resolution_clock::now();

        // Вычисление времени одной операции поиска
        std::chrono::duration<int, std::nano> duration = end - start;
        double timePerOperation = duration.count() / targetSize; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << /*targetSize << " - " <<  */timePerOperation  /*<< " ns"  */ << std::endl;

        if (targetSize == maxSize) // 1000000
            return;

        // Увеличение размера контейнера
        if (targetSize == incrementSize * 10) // 100 / 10
            incrementSize *= 10;

        // Увеличиваем targetSize на incrementSize
        targetSize += incrementSize;

        myMap.clear();
    }
    std::cout << "============" << std::endl;
}

int main()
{
    functional_test();
    test_find();
    //test_insert();
    //test_erase();
}