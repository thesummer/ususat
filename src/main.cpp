#include<csignal>
#include<cstdlib>
#include<unistd.h>

#include <iostream>
#include <string>
using std::string;

#include "tclap/CmdLine.h"
#include "kalmanfilter.h"
using namespace USU;

// Parse the command line arguments
// Define possible arguments
TCLAP::CmdLine cmd("Program for the attitude determination and control of the USU simulation table",' ', "0.1");

TCLAP::ValueArg<string> trajFile("", "trajfile", "Input file for the trajectory the table should follow", false, "input.txt", "filename");
TCLAP::ValueArg<float> pgain("", "pgain", "The P-Gain for the simple proportional speed controller", false, 1.0, "float");
TCLAP::ValueArg<string> mode("", "mode", "Operation mode", true, string(), "mode name");

// Example for switching arg
//TCLAP::SwitchArg stats("s", "stats", "Print statistics (number of spots, number of identified spots, ratio");


KalmanFilter kalmanFilter(5, 20000 , "/dev/i2c-2", "/dev/i2c-3");
//bool run = true;

void endProgram(int s)
{
    std::cerr << "MAIN: Got signal for termination" << std::endl;
    std::cerr << "MAIN: Stopping kalman filter thread..." << std::endl;
    kalmanFilter.stop();
}


int main(int argc, char **argv)
{
    // Register endProgram function as
    // signal handler for the kill signal (ctrl+c)
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = endProgram;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    try
    {
        // Register commandline options to parser
        cmd.add(trajFile);
        cmd.add(pgain);
        cmd.add(mode);

        cmd.parse(argc, argv);

        // Evaluate command line options
        if(mode.getValue() == "simpleControl")
        {
            KalmanFilter.initializeModeSimpleControl(trajFile.getValue(), pgain.getValue());
        }
        /// TODO add more modes here
        else
        {
            throw std::runtime_error("MAIN: Unknown mode selected. Terminating");
            return 1;
        }

        kalmanFilter.start();

        if(kalmanFilter.join() )
        {
            std::cerr << "MAIN: Kalman filter thread joined" << std::endl;
            std::cerr << "MAIN: Terminating now..." << std::endl;
            return 0;
        }
        else
        {
            std::cerr << "MAIN: Joining Kalman filter thread failed" << std::endl;
            std::cerr << "MAIN: Terminating now..." << std::endl;
            return 1;
        }
    } catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
        return 1;
    }

}
