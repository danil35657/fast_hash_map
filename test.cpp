
#include <cassert>
#include <chrono>
#include <random>
#include "fast_hash_map.h"
#include "flat_hash_map.hpp"

void functional_test()
{
    fast_hash_map<int, std::string> map;

    // Тест вставки и доступа
    map.insert({ 1, "one" });
    map.insert({ 2, "two" });
    assert(map.contains(1) == true);
    assert(map.contains(2) == true);
    assert(map[1] == "one");
    assert(map[2] == "two");

    // Тест обновления значения
    map[1] = "uno";
    assert(map[1] == "uno");

    // Тест удаления
    map.erase(1);
    assert(map.contains(1) == false);
    assert(map.size() == 1); // должно остаться 1 элемент

    // Тест очистки
    map.clear();
    assert(map.size() == 0);
    assert(map.contains(2) == false);

    // Тест коллизий
    map.insert({ 3, "three" });
    map.insert({ 10, "ten" }); // Предположим, что 3 и 10 имеют одинаковый хэш
    assert(map.contains(3) == true);
    assert(map.contains(10) == true);
    assert(map[3] == "three");
    assert(map[10] == "ten");

    // Тест повторного удаления
    map.erase(10);
    assert(map.contains(10) == false);
    assert(map.size() == 1); // должно остаться 1 элемент

    for (int i = 0; i < 100; ++i)
    {
        map[i] = "hjbhbhbh";
    }

    fast_hash_map<int, std::string> map2(map);

    //map.print();
    //std::cout << "============" << std::endl;
    //map2.print();

    assert(map2 == map);

    fast_hash_map<int, std::string> map3 = std::move(map);
    assert(map2 == map3);
    assert(map2 != map);

    //map3.print();
    //std::cout << map3.size() << " " << map3.capacity() << std::endl;

    fast_hash_map<int, std::string> map4{ {1, "kbbjhb"}, {2,"kmklmkm"}, {4,"knljknjln"} };
    assert(map4.size() == 3);
    assert(map4.capacity() == 7);
    //map4.print();
    //std::cout << map3.size() << " " << map3.capacity() << std::endl;

    std::cout << "Ok" << std::endl << "============" << std::endl;
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
            myMap.contains(i);
        }
        auto end = std::chrono::high_resolution_clock::now();

        // Вычисление времени одной операции поиска
        std::chrono::duration<double, std::nano> duration = end - start;
        double timePerOperation = duration.count() / 1000000; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << targetSize << " - " << timePerOperation << " ns" << std::endl;

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
    fast_hash_map<int, int> myMap;
    //std::unordered_map<int, int> myMap;
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
        std::chrono::duration<double, std::nano> duration = end - start;
        double timePerOperation = duration.count() / targetSize; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << targetSize << " - " << timePerOperation << " ns" << std::endl;

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
    fast_hash_map<int, int> myMap;
    //std::unordered_map<int, int> myMap;
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
        std::chrono::duration<double, std::nano> duration = end - start;
        double timePerOperation = duration.count() / targetSize; // Время на одну операцию в наносекундах

        // Вывод результата
        std::cout << targetSize << " - " << timePerOperation << " ns" << std::endl;

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