#include<csignal>
#include<cstdlib>
#include<unistd.h>

#include <iostream>

#include "kalmanfilter.h"
using namespace USU;

KalmanFilter kalmanFilter(5, 20000, "/dev/i2c-2", "/dev/i2c-3");
//bool run = true;

void endProgram(int s)
{
    std::cout << "MAIN: Got signal for termination" << std::endl;
    std::cout << "MAIN: Stopping kalman filter thread..." << std::endl;
    kalmanFilter.stop();
}


int main()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = endProgram;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    kalmanFilter.start();

    kalmanFilter.join();

    std::cout << "MAIN: Kalman filter thread joined" << std::endl;
    std::cout << "MAIN: Terminating now..." << std::endl;

    return 0;
}
