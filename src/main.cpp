#include<csignal>
#include<cstdlib>
#include<unistd.h>

#include <iostream>
#include <string>
using std::string;

#include "tclap/CmdLine.h"
#include "mainthread.h"
using namespace USU;

// Text to explain the different modes (more elegant way to split strings over several lines?)
const string modeText = string("Operation mode: \n\t") +
                                 string("- pololu: Collect data from Pololu IMU and print it in csv format\n\t") +
                                 string("- microstrain: Collect data from MicroStrain IMU and print it in csv format\n\t") +
                                 string("- collect: Collect data from both IMUs and print it in csv format\n\t") +
                                 string("- simpleControl: Run simple angular velocity control scheme");

// Parse the command line arguments
// Define possible arguments
TCLAP::CmdLine cmd("Program for the attitude determination and control of the USU simulation table",' ', "0.1");

TCLAP::ValueArg<string> trajFile("", "trajfile", "Input file for the trajectory the table should follow", false, "input.txt", "filename");
TCLAP::ValueArg<float> pgain("", "pgain", "The P-Gain for the simple proportional speed controller", false, 1.0, "float");
TCLAP::ValueArg<string> mode("", "mode",  modeText , true, string(), "mode name");

// Example for switching arg
//TCLAP::SwitchArg stats("s", "stats", "Print statistics (number of spots, number of identified spots, ratio");


MainThread mainThread(5, 20000 , "/dev/i2c-2", "/dev/i2c-3");
//bool run = true;

void endProgram(int s)
{
    std::cerr << "MAIN: Got signal for termination" << std::endl;
    std::cerr << "MAIN: Stopping kalman filter thread..." << std::endl;
    mainThread.stop();
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
            mainThread.initializeModeSimpleControl(trajFile.getValue(), pgain.getValue());
            mainThread.setMode(MainThread::SimpleControl);
        }
        else if (mode.getValue() == "control")
        {
            mainThread.setMode(MainThread::Control);
        }
        else if(mode.getValue() == "pololu")
        {
            mainThread.setMode(MainThread::CollectPololuData);
        }
        else if(mode.getValue() == "microstrain")
        {
            mainThread.setMode(MainThread::CollectMicroStrainData);
        }
        else if(mode.getValue() == "collect")
        {
            mainThread.setMode(MainThread::CollectData);
        }
        else
        {
            throw std::runtime_error("MAIN: Unknown mode selected. Terminating");
            return 1;
        }

        mainThread.start();

        if(mainThread.join() )
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
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        return 1;
    }

}
