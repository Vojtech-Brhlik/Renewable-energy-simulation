/*********************************
**  IMS Project                 **
**                              **
**  author: Vojtech Brhlik      **
**  xbrhli02@stud.fit.vutbr.cz  **
**  VUT FIT, 2022               **
**                              **
**********************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define SUNNY 1
#define SEMI 2
#define CLOUDY 3
#define DAILY 11.715                //average daily electricity consumption

/* can be changed with arguments */
float totalUsage = 4276.0;          //Represents the total electricity usage of the system (in kWh)
float panelProduction = 300.0;      //Represents the amount of electricity one panel can produce (in W)                                        
float totalLoss = 0.15;             //Represents the loss of direction and angle    
float count = 0.0;                  //variable used to count the full sum of the production
float capacity = 0.0;               //Represents the maximum battery capacity

/* can not be changed with arguments */
float monthHours[12] = {47.5, 67, 134, 158, 217.5, 212.5, 211, 219.5, 157.5, 124, 56, 50};
float sunnyDays[12] = {2, 2, 4.3, 5.7, 5.9, 5, 7.4, 8.6, 7.5, 8, 2.8, 2.7};
float semiDays[12] = {10.8, 11.2, 13.6, 15.3, 16.6, 18.1, 18.5, 16.4, 14.3, 12.7, 10.3, 10.1};
float cloudyDays[12] = {18.2, 15.1, 13.1, 9, 8.4, 6.9, 5, 6, 8.2, 10.3, 16.9, 18.1};
float battery = 0.0;
bool BAT = false;
bool CNT = false;

/* Puts corresponding values into global variables, looks for errors in arguments */
int arg_check(int argc, char* argv[]);

/* Starts the calculations*/
void Run();

/* Does calculations for a specific number of panels in count */
void Total_run();

/* Does calculations for a specific number of panels in count, includes battery calculations */
void Battery();

/* Function calculates the production loss gained by variable causes and returns it */
float Loss();

/* Function calculates and returns the production gained in a given month's hours */
float get_production(int index);

/* Function calculates and returns hours for either SUNNY, SEMI or CLOUDY sky */
float get_hours(int index, int type);

/* Prints help */
void help();