#include<csignal>
#include<cstdlib>
#include<unistd.h>


#include "kalmanfilter.h"
using namespace USU;

KalmanFilter kalmanFilter(5, 20000, "/dev/i2c-2", "/dev/i2c-3");


void endProgram(int s)
{
 kalmanFilter.stop();

 usleep(1000000);

 exit(1);

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


    return 0;
}
