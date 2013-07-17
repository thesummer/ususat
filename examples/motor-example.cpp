#include <iostream>
using std::cout;
using std::endl;

#include "motorprogrammer.hpp"


int main(int argc, char *argv[])
{

    if (argc != 3 && argc != 2)
    {
        cout << "Wrong number of arguments" << endl;
        cout << "Usage:" <<endl;
        cout << "motor-test <input file> [<output file>]" << endl;
        cout << "Terminating" << endl;
        return 1;
    }

    const char * inputFile = argv[1];
    const char * outputFile;
    if (argc == 3)
    {
        outputFile = argv[2];
    }
    else
    {
        outputFile = "output.csv";
    }


    MotorProgrammer mp(6, inputFile, outputFile, 20000);

    mp.start();

    mp.join();

   return 0;
}



