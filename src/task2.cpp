#include "task2.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <cassert>
#include <syncstream>
#include <vector>
#include <sstream>

bool station_is_lock;
std::mutex station_is_lock_mutex;

void TrainMoving (int seconds, char train)
{
    std::stringstream output1;
    output1 << "Train " << train << " is departing to destination station. Travel time " << seconds / 3600 << " hours "
              << (seconds % 3600) / 60 << " minutes " << seconds % 60 << " seconds." <<std::endl;
    std::cout << output1.str();
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    if ( station_is_lock ) {
        std::stringstream output2;
        output2 << "Train " << train <<" is waiting clear platform in destination station." << std::endl;
        std::cout << output2.str();
        while (station_is_lock)
            std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    station_is_lock_mutex.lock();
    station_is_lock = true;
    std::stringstream output3;
    output3 << "Train " << train << " is on platform and waiting command for depart..." << std::endl;
    std::cout << output3.str();


    while (station_is_lock) {

        std::string command;
        std::cin >> command;
        if (command == "depart")
        {
            std::stringstream output4;
            output4 << "Train " << train << " is moving out." << std::endl;
            std::cout << output4.str();
            station_is_lock_mutex.unlock();
            station_is_lock  =  false;
            break;
        }
        else
        {
            std::cout << " Wrong command!" << std::endl;
        }
    }

}

void task2() {
    int times[3];
    char firstTrain = 'A';
    for (int i = 0; i < 3; ++i)
    {
        std:: cout << "Enter  time for train " << (char)(firstTrain+i)<< std::endl;
        std::cin >> times[i];
        assert(times[i] >0);
     }
    std::vector<std::thread> trains;
    for (int i = 0; i < 3; ++i)
    {
        trains.push_back(std::thread(TrainMoving,times[i],(char)(firstTrain+i)));
    }
    for (int i = 0; i < 3; ++i)
    {
        trains.at (i).join();
    }

}