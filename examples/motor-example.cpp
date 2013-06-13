#include <ncurses.h>
#include <fstream>
#include <sys/time.h>
#include <sched.h>

#include "motorcontrol.h"
#include "periodicrtthread.h"
#include "Lock.h"

using namespace USU;

#define LENGTH 31 // (Maximum) length of the bar chart
int MOT=0;        // which motor is used [0..3]

int dutyCycle = 0;  // global variable duty cycle
char row[LENGTH+1];          /*!< Array containing LENGTH '#' which represents the maximum length of the bar chart*/
int err;

// function to substract two timeval structures
int timeval_subtract (struct timeval * result, struct timeval * x, struct timeval * y)
{
    /* Perform the carry for the later subtraction by updating y. */
    if (x->tv_usec < y->tv_usec) {
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if (x->tv_usec - y->tv_usec > 1000000) {
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

/*!
 \brief  Update the ncurses ui-screen

*/
void printScreen()
{
   // clear screen
   erase();

   // print label
   mvprintw(2, 2, "Channel 1:");

   //calculate the length of each bar and draw it next to the labels
   attron(COLOR_PAIR(1) | A_INVIS);
   if (dutyCycle > 0)
   {
       int barLength = LENGTH - LENGTH*dutyCycle/100;
       mvprintw(4, 5 + LENGTH, "%s", &row[barLength]);
   }
   else
   {
       int barLength = LENGTH + LENGTH*dutyCycle/100;
       mvprintw(4, 5 + barLength, "%s", &row[barLength]);
   }
   attroff(COLOR_PAIR(1) | A_INVIS);

   // print value of duty cycle
   mvprintw(4, 63, ":%4d", dutyCycle);
//   mvprintw(6, 2, "Analog 1: %f\t\tAnalog 2: %f", a1, a2);

   //Print the information how to use the program
   mvprintw(18, 2, "+/-: Switch to increase or decrease duty cycle, h/l for steps of 10");
   mvprintw(21, 2, "q: Quit");

   refresh();
}


// Simple class which manages the motors and collects data at an periodic intervall
// Inherited from PeriodicRtThread
class DataCollector: public PeriodicRtThread
{
public:

    DataCollector(int priority, unsigned int period_us);

    int getDutyCycle();
    void setDutyCycle(int value);

    virtual void run();


    volatile bool mKeepRunning;
    char * mFilename;

private:
    int mDutyCycle;   // DutyCycle of the motor
    Lock mLockDutyCycle; // Protect mDutyCycle from concurrent access using a mutex
    MotorControl mMotors;   // Motor object

};

DataCollector::DataCollector::DataCollector(int priority, unsigned int period_us)
    :PeriodicRtThread(priority, period_us)
{}

int DataCollector::getDutyCycle()
{
    ScopedLock scLock(mLockDutyCycle);
    return mDutyCycle;
}

void DataCollector::setDutyCycle(int value)
{
    ScopedLock scLock(mLockDutyCycle);
    mDutyCycle = value;
    mMotors.setMotor(MOT, mDutyCycle);
}

void DataCollector::run()
{
    mKeepRunning = true;

    struct timeval start, now, elapsed;

    // open output file
    std::fstream file;
    file.open(mFilename, std::ios_base::out);

    // get initial clock value
    gettimeofday(&start, NULL);

    while(mKeepRunning)
    {
        // read analog sensor
        float a1, a2;
        mMotors.getAnalog(MOT, a1, a2);

        // take time stamp
        gettimeofday(&now, NULL);
        timeval_subtract(&elapsed, &now, &start);
        unsigned long long timestamp = elapsed.tv_sec * 1000 + elapsed.tv_usec / 1000; // in ms since start

        // make a copy of the current dutyCycle.
        mLockDutyCycle.lock();
            int temp = mDutyCycle;
        mLockDutyCycle.unlock();

        //print line of data to output file
        file << timestamp << "," << a1 << "," << a2 << "," << temp << std::endl;

        waitPeriod();
    }

    file.close();
}


int main(int argc, char *argv[])
{

//    struct sched_param param;

//    param.__sched_priority = 10;

//    if( sched_setscheduler( 0, SCHED_FIFO, &param ) == -1 )
//    {
//        perror("sched_setscheduler");
//    }

    // Initialize data collector
    DataCollector dc(5, 20000);

    // first commandline argument (if any) determines which motor (default=0)
    if (argc >1)
    {
        switch (*argv[1])
        {
        case '0': MOT = 0; break;
        case '1': MOT = 1; break;
        case '2': MOT = 2; break;
        case '3': MOT = 3; break;
        }
    }

    // 2nd commandline argument sets output file (default: output.csv)
    dc.mFilename = "output.csv";
    if (argc > 2)
    {
        dc.mFilename = argv[2];
    }


   int ch = 0;
   int i;
   //initialize an array of '#' which determines the maximum length of a bar
   for (i = 0; i<LENGTH+1; i++)
     row[i] = '#';
   row[0] = 'a';
   row[LENGTH] = '\0';

   //Initialize ncurses
   initscr();
   if(has_colors() == FALSE)
    {	endwin();
        printf("Your terminal does not support color\n");
        return 1;
    }
   start_color();			/* Start color 			*/
   init_pair(1, COLOR_BLUE, COLOR_BLUE);
   cbreak();
   noecho();

   //Initialize the duty cycles of motor and start data collection
   dc.setDutyCycle(dutyCycle);
   dc.start();

   printScreen();

   int inc = 1;

   //Wait for user input
   while(ch != 'q')
   {
     ch = getch();
     switch(ch)
     {
         case '+':dutyCycle += inc; break;
         case '-':dutyCycle -= inc; break;
         case 'h': dutyCycle += 10*inc; break;
         case 'l': dutyCycle -= 10*inc; break;
     }

     dc.setDutyCycle(dutyCycle);

     printScreen();
   }
   endwin();  //Stop ncurses

   // stop motors correctly
   dc.setDutyCycle(0);
   dc.mKeepRunning = false;

   dc.join(2000);

   return 0;
}



