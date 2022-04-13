#include "task3.h"
#include <cstdlib>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <sstream>

enum ORDER {
    PIZZA,
    SOUP,
    STEAK,
    SALAD,
    SUSI
};

std::vector<ORDER> ordersIn;
std::mutex ordersIn_mutex;

std::vector<ORDER> ordersDone;
std::mutex ordersDone_mutex;

int ordersCount = 0;

std::string getDish(ORDER dish)
{
    switch ((int)dish)
    {
        case 0:
            return "pizza";
        case 1:
            return "soup";
        case 2:
            return "steak";
        case 3:
            return "salad";
        case 4:
            return "susi";
    }
}

void waiter()
{
    while (ordersCount<10) {
        ordersIn_mutex.lock();
        ORDER dish = (ORDER) (std::rand() % 5);
        ordersIn.push_back(dish);
        std::stringstream output;
        output << "Order with " << getDish(dish) << " accepted" << std::endl;
        std::cout << output.str();
        int time = std::rand() % 6 + 5;
        ordersIn_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(time));
    }
}

void kitchen()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (ordersCount<10) {
        int time = std::rand() % 11 + 5;
        if (!ordersIn.empty()) {
            ordersIn_mutex.lock();
            ordersDone_mutex.lock();
            ORDER dish = ordersIn.front();
            ordersDone.push_back(dish);
            std::stringstream output;
            output << "Order with " << getDish(dish) << " in work" << std::endl;
            std::cout << output.str();
            for (int i = 0; i < ordersIn.size() - 1; ++i) {
                ordersIn[i] = ordersIn[i + 1];
            }
            ordersIn.resize(ordersIn.size() - 1);
            ordersIn_mutex.unlock();
            ordersDone_mutex.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(time));
            std::stringstream output1;
            output1 << "Order with " << getDish(dish) << " ready for courier" << std::endl;
            std::cout << output1.str();
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void courier()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    while (ordersCount<10) {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        while (!ordersDone.empty())
        {
            if (!ordersDone.empty()) {
                ordersDone_mutex.lock();
                std::stringstream output;
                output << "Order with " << getDish(ordersDone.back()) << " done" << std::endl;
                std::cout << output.str();
                ordersDone.pop_back();
                ++ordersCount;
                ordersDone_mutex.unlock();
            }
        }
    }
}



void task3()
{
    std::srand(std::time(nullptr));
    std::thread  waiter1 (waiter);
    std::thread  kitchen1 (kitchen);
    std::thread courier1 (courier);
    waiter1.detach();
    kitchen1.detach();
    courier1.detach();
    while (ordersCount < 10)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}