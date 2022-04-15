#include "task1.h"
#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <mutex>
#include <sstream>
#include <map>

#define DISTANCE 100

std::map <std::string,int> results;
std::mutex results_mutex;

class Swimmer
{
    double speed = 0;
    double distance = 0;
    std::string name;
    int time = 0;

public:

    Swimmer()
    {
        std::cout << "Enter name of Swimmer" <<std::endl;
        std::cin >> name;
        std::cout << "Enter average speed of "<< name <<" in m/s" << std::endl;
        std::cin >> speed;
        assert (speed > 0);
    }

    bool swimming(int sec)
    {
        distance = speed*sec;
        if (distance < DISTANCE) {
            std::stringstream outp;
            outp << "Swimmer " << name << " is on distance " << distance << " on " << sec << " second!" << std::endl;
            std::cout << outp.str();
            return false;
        } else {
            distance = DISTANCE;
            std::stringstream outp;
            outp << "Swimmer " << name << " finished race on " << sec << " second!" << std::endl;
            std::cout << outp.str();
            time = sec;
            results_mutex.lock();
            results.insert(std::pair <std::string,int> (name , time));
            results_mutex.unlock();
            return true;
        }
    }

    std::string getName()
    {
        return name;
    }

    int getTime()
    {
        return time;
    }
};


void oneThreadSwim (Swimmer* person)
{
    int i = 1;
    bool check;
    do {
        check = person->swimming(i);
        ++i;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } while (!check);
}

int comp (const void* a, const void* b)
{
    int arg1 = ((Swimmer*)a)->getTime();
    int arg2 = ((Swimmer*)b)->getTime();
    return arg1-arg2;
}

void task1()
{
    Swimmer* group = new Swimmer[6];
    std::vector<std::thread> swim;
    for (int i = 0; i<6; ++i)
    {
        swim.push_back(std::thread(oneThreadSwim, &group[i]));
    }
    for (int i = 0; i<6; ++i)
    {
        swim.at (i).join();
    }
    std::qsort(group,6,sizeof (Swimmer),   comp);
    std::cout << "Leader board:" <<std::endl;
    for (int i = 0; i<6; ++i) {
        std::cout << group[i].getName() << " " << group[i].getTime() <<std::endl;
    }
    delete group;
    group = nullptr;
    delete group;
}