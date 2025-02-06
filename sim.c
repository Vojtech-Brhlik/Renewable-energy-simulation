/*********************************
**  IMS Project                 **
**                              **
**  author: Vojtech Brhlik      **
**  xbrhli02@stud.fit.vutbr.cz  **
**  VUT FIT, 2022               **
**                              **
**********************************/

#include "sim.h"

int main(int argc, char* argv[]) {
    printf("################# START #################\n\n");
    if (arg_check(argc, argv) < 1) {
        return 1;
    }

    if (BAT == true) {
        Battery();
        Battery();
        printf("################## END ##################\n\n");
        return 0;
    }
    
    if (CNT == true) {
        Total_run();
        return 0;
    }

    Run();
    return 0;
}

int arg_check(int argc, char* argv[]) {
    /*help check*/
    if (argc == 2 && strcmp(argv[1], "-h") == 0) {
        help();
        return 1;
    }

    int caplen = 1;             //caplen to increment through the argumencaplen
    char* param = "";           //string to represent the parameter currently parsed
    char* param_value = "";     //string to represent the parameter value currently proccessed

    /*parsing argumencaplen*/
    while (argv[caplen] != NULL) {
        param = argv[caplen];
        param_value = argv[caplen + 1];
        char* buffer = "";
    
        /*-u parameter*/
        if (strcmp(param, "-u") == 0) {
            totalUsage = strtof(param_value, &buffer);
            if (strcmp(buffer, "") != 0) {
                printf("ERROR: %s is not a float\n", param_value);
                return 0;
            } 
            printf("# total elecrticity usage set to %.2f kWh\n", totalUsage);
            caplen += 2;
        }

        /*-p parameter*/
        else if (strcmp(param, "-p") == 0) {
            panelProduction = strtof(param_value, &buffer);
            if (strcmp(buffer, "") != 0) {
                printf("ERROR: %s is not a float\n", param_value);
                return 0;
            } 
            printf("# panel production set to %.2f Wp\n", panelProduction);
            caplen += 2;
        }

        /*-l parameter*/
        else if (strcmp(param, "-l") == 0) {
            totalLoss = strtof(param_value, &buffer);
            if (strcmp(buffer, "") != 0) {
                printf("ERROR: %s is not a float\n", param_value);
                return 0;
            } 
            printf("# total loss set to %.2f %%\n", totalLoss);
            totalLoss = totalLoss/100;
            caplen += 2;
        }

        /*-c parameter*/
        else if (strcmp(param, "-c") == 0) {
            count = strtof(param_value, &buffer);
            count = ceil(count);
            CNT = true;
            if (strcmp(buffer, "") != 0) {
                printf("ERROR: %s is not a float\n", param_value);
                return 0;
            } 
            printf("# total panel count set to %3.f\n", count);
            caplen += 2;
        }

        /*-b parameter*/
        else if (strcmp(param, "-b") == 0) {
            capacity = strtof(param_value, &buffer);
            BAT = true;
            if (strcmp(buffer, "") != 0) {
                printf("ERROR: %s is not a float\n", param_value);
                return 0;
            } 
            printf("# battery set to set to %.2f kWh\n", battery);
            caplen += 2;
        }

        /*catches unknown parameters, also errors when parameter value is missing*/
        else {
            printf("ERROR: unknown parameter. Print help with \"./sim -h\"\n");
            return 0;
        }
        
    }
    return 1;
}

void Run() {
    float sum = 0.0;            //variable used to count the full sum of the production
    float production = 0.0;     //variable used to store the production value during the loop 
    printf("Program parametres:\n");
    printf("# Loss = %.2f Wp\n", Loss());
    printf("# panel production = %.2f Wp\n", panelProduction);
    printf("# total usage = %.2f kWh\n\n", totalUsage);

    printf("__________________________________________\n");
    printf("| month:| kWh produced:\t| sunlight hours:|\n");
    printf("__________________________________________\n");

    /* Loop through all the months to calculate the table */
    for (int i = 0; i < 12; i++) {
        printf("|");

        /* prints additional space for months first 10 months (for aligning purpose) */
        if (i + 1 < 10) {
            printf(" ");
        }
        production =  get_production(i) / 1000.0;                       //convert to kWh
        sum += production;
        printf("   %i\t| \t%.2f\t| \t%.2f\t | \n", i + 1, production, monthHours[i]);
    }
    printf("__________________________________________\n\n");

    count = ceil(totalUsage/sum);

    printf("Total yearly production from a single %.2f Wp panel: %.4f kWh\n", panelProduction, sum);
    printf("For yearly production of %.2f kWh, your system would need %2.f solar panels installed.\n", totalUsage, count);
    printf("\nThis would result in a following monthly production:\n\n");

    /* now do the calculations for all the panels */
    Total_run(ceil(totalUsage/sum));
}

void Total_run() {
    float production = 0.0;
    float sum = 0.0;
    printf("_________________________________________\n");
    printf("| month:| kWh produced:\t| daily avg:\t|\n");
    printf("_________________________________________\n");

    /* Loop through all the months to calculate the table */
    for (int i = 0; i < 12; i++) {
        printf("|");

        /* prints additional space for months first 10 months (for aligning purpose) */
        if (i + 1 < 10) {
            printf(" ");
        }
        production = count * get_production(i) / 1000.0;                //convert to kWh
        sum += production;
        printf("   %i\t| \t%.2f\t| \t%.3f\t|\n", i + 1, production, production / (sunnyDays[i] + semiDays[i] + cloudyDays[i]));
    }
    printf("_________________________________________\n\n");
    printf("Total production value: %.2f kWh\n", sum);
    printf("################## END ##################\n\n");
}

void Battery() {
    float production = 0.0;
    float avg = 0.0;
    
    printf("BATTERY MAX CAPACITY: %2.f kWh\n\n", capacity);
    printf("_________________________________________________________\n");
    printf("| month:| kWh produced:\t| daily avg:\t| battery:\t|\n");
    printf("_________________________________________________________\n");

    /* Loop through all the months to calculate the table */
    for (int i = 0; i < 12; i++) {
        printf("|");

        /* prints additional space for months first 10 months (for aligning purpose) */
        if (i + 1 < 10) {
            printf(" ");
        }
        production = count * get_production(i) / 1000.0;                //convert to kWh
        avg = production / (sunnyDays[i] + semiDays[i] + cloudyDays[i]);
        printf("   %i\t| \t%.2f\t| \t%.3f\t|", i + 1, production, avg);
        /* calculate battery status for given month */
        for (int j = 0; j < (sunnyDays[i] + semiDays[i] + cloudyDays[i]); j++) {
            if (avg > DAILY) {
                if (battery <= capacity) {
                    battery += (avg - DAILY);
                }
                if (battery > capacity) {
                    battery = capacity;
                }
            }
            if (avg < DAILY) {
                if (battery > 0) {
                    battery -= (DAILY - avg);
                }
                if (battery < 0) {
                    printf("d%i", j);
                    battery = 0;
                }
            }
        }
        printf("\t%2.f\t|", battery);
        printf("\n");
    }
    printf("_________________________________________________________\n\n");
}

float Loss() {
    return panelProduction * totalLoss;
}

float get_production(int index) {
    float production = (panelProduction - Loss()) * (get_hours(index, SUNNY) + get_hours(index, SEMI) + get_hours(index, CLOUDY));
    return production;
}

float get_hours(int index, int type) {
    float temp = sunnyDays[index] + semiDays[index] + cloudyDays[index];
    /* Check what type of hours we're calculating */
    switch(type) {

        /* in sunny weather the production is  100% effective, so we just return the calculated value */
        case SUNNY:
            temp = sunnyDays[index] / temp;
            return temp * monthHours[index];
            break;

        /* in semi-cloudy weather the production is only 50% effective, so we divide the return value by 2 */
        case SEMI:
            temp = semiDays[index] / temp;
            return temp * monthHours[index] / 2;
            break;

        /* in cloudy weather the production is only 10% effective, so we divide the return value by 10 */
        case CLOUDY:
            temp = cloudyDays[index] / temp;
            return temp*monthHours[index] / 10;
            break;
        
        default:
            exit(1);
    }
}

void help() {
    printf("\nThis program calcualtes the amount of solar panels a given system needs to be fully self-sustainable.\n");
    printf("Use program the following way:\n\n");
    printf("\t./sim -u [uVALUE] -c [cVALUE] -p [pVALUE] -l [lVALUE] -b [bVALUE]\n\n");
    printf("\t-u sets the total electricity usage to uVALUE (in kWh)\n");
    printf("\t-c sets the count of panels to cVALUE (and rounds it up)\n");
    printf("\t   using this parameter also jumps straight to the full production calculation\n");
    printf("\t-p sets the panel production to pVALUE (in Wp)\n");
    printf("\t-l sets the total loss to lVALUE (in %%)\n");
    printf("\t-B sets the program into battery mode and setes battery capacity to bVALUE (in kWh)\n\n");
    printf("All arguments are optional\n");
    exit(0);
}