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
    std::cerr << "MAIN: Got signal for termination" << std::endl;
    std::cerr << "MAIN: Stopping kalman filter thread..." << std::endl;
    kalmanFilter.stop();
}


int main()
{
    // Register endProgram function as
    // signal handler for the kill signal (ctrl+c)
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = endProgram;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    kalmanFilter.start();

    kalmanFilter.join();

    std::cerr << "MAIN: Kalman filter thread joined" << std::endl;
    std::cerr << "MAIN: Terminating now..." << std::endl;

    return 0;
}
