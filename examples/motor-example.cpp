
#include<iostream>
using namespace std;

#include <ncurses.h>

#include "motorcontrol.h"
using namespace USU;

#define LENGTH 31
int MOT=0;

int dutyCycle = 0;
char row[LENGTH+1];          /*!< Array containing LENGTH '#' which represents the maximum length of the bar chart*/
int err;
float a1 = 0, a2 = 0;

/*!
 \brief  Update the ncurses ui-screen

*/
void printScreen()
{
   erase();

   mvprintw(2, 2, "Channel 1:");

   //calculate the length of each bar an draw it next to the labels
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

   mvprintw(4, 63, ":%4d", dutyCycle);
   mvprintw(6, 2, "Analog 1: %f\t\tAnalog 2: %f", a1, a2);

   //Print the information how to use the program
   mvprintw(18, 2, "+/-: Switch to increase or decrease mode");
   mvprintw(21, 2, "q: Quit");

   refresh();
}

int main(int argc, char *argv[])
{
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

   int ch = 0;
   int i;
   //initialize an array of '#' which determines the maximum length of a bar
   for (i = 0; i<LENGTH+1; i++)
     row[i] = '#';
   row[0] = 'a';
   row[LENGTH] = '\0';

   // Initialize Motors
   MotorControl motors;

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

   //Initialize the duty cycles of motor
   motors.setMotor(MOT, dutyCycle);
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

     motors.setMotor(MOT,dutyCycle);
     motors.printAnalog(MOT, a1, a2);

     printScreen();
   }
   endwin();  //Stop ncurses

   motors.setMotor(MOT, 0);

   return 0;
}


