//Project 2 --- Written by Jonathan Dereje

#include "clock.h"
#include <stdio.h>

// Reads the time of day from the TIME_OF_DAY_PORT global variable. If
// the port's value is invalid (negative or larger than 16 times the
// number of seconds in a day) does nothing to tod and returns 1 to
// indicate an error. Otherwise, this function uses the port value to
// calculate the number of seconds from start of day (port value is
// 16*number of seconds from midnight). Rounds seconds up if there at
// least 8/16 have passed. Uses shifts and masks for this calculation
// to be efficient. Then uses division on the seconds since the
// begining of the day to calculate the time of day broken into hours,
// minutes, seconds, and sets the AM/PM designation with 1 for AM and
// 2 for PM. By the end, all fields of the `tod` struct are filled in
// and 0 is returned for success.
 // 
// CONSTRAINT: Uses only integer operations. No floating point
// operations are used as the target machine does not have a FPU.
// 
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 40 lines may be
// penalized for complexity.

int set_tod_from_ports(tod_t *tod){
    //checking if the TOD port is invalid
    if(TIME_OF_DAY_PORT < 0 || TIME_OF_DAY_PORT > 16 * 86400){
        return 1;
        //base case
    }

    //creating each hours, mins, secs variables
    tod->day_secs = TIME_OF_DAY_PORT >> 4;

    if((TIME_OF_DAY_PORT  / 16) > 0){
        if((TIME_OF_DAY_PORT % 16) >= 8)
        tod->day_secs++;
        /*using double nested loop, to check for rounding 
        numbers up and down 
        */
    }
    tod->time_hours = tod->day_secs / 3600;
     if (tod->day_secs >= 13 * 3600){
        tod->time_hours = tod->time_hours - 12;
    }
    else if(tod->day_secs < 3600){
        tod->time_hours = tod->time_hours + 12;
    }
    //adding or subtracting 12 depending on which half of the day it is

    tod->time_mins = (tod->day_secs % 3600) / 60;
    tod->time_secs = (tod->day_secs % 60);
    
    /*dividing each mins, secs, hours by their respective numbers
    
    and setting the variables to them. 
    */
    if(tod->day_secs < 43200){
        tod->ampm = 1;
    }
    else{
        tod->ampm = 2;
    }
    //showing pm or am depending 
    //on which half of the day, using seconds


return 0; //returning for success
}

// Accepts a tod and alters the bits in the int pointed at by display
// to reflect how the LCD clock should appear. If any time_** fields
// of tod are negative or too large (e.g. bigger than 12 for hours,
// bigger than 59 for min/sec) or if the AM/PM is not 1 or 2, no
// change is made to display and 1 is returned to indicate an
// error. The display pattern is constructed via shifting bit patterns
// representing digits and using logical operations to combine them.
// May make use of an array of bit masks corresponding to the pattern
// for each digit of the clock to make the task easier.  Returns 0 to
// indicate success. This function DOES NOT modify any global
// variables
// 
// CONSTRAINT: Limit the complexity of code as much as possible. Do
// not use deeply nested conditional structures. Seek to make the code
// as short, and simple as possible. Code longer than 85 lines may be
// penalized for complexity.


int set_display_from_tod(tod_t tod, int *display){
    if(tod.time_hours < 0 || tod.time_hours > 12 ||
       tod.time_mins < 0  || tod.time_mins > 59  || 
       tod.time_secs < 0  || tod.time_secs > 59){

        //base scase for each scenerio, based on the write up
        return 1;
    }                       //0     // 1        //2        //3        //4        //5         //6       //7
    int bit_mask[10] = {0b1110111, 0b0100100, 0b1011101, 0b1101101, 0b0101110, 0b1101011, 0b1111011, 0b0100101, 
       //8       //9
    0b1111111, 0b1101111};

    //bit masks for numbers 1-9, as stated in the write up 


    int hours_ones = tod.time_hours % 10;
    int hours_tens = tod.time_hours / 10;
    int mins_ones = tod.time_mins % 10;
    int mins_tens = (tod.time_mins / 10) % 10;

    //creating hours and mins variables

    *display = 0;
    //initalizing display variable

    if(tod.ampm == 1){ 
        *display = *display | (1 << 28);
    }
    else if(tod.ampm == 2){
        *display = *display | (1 << 29);

        //if else statement for the AM and PM 
    }
    if(hours_tens == 1){ 
        *display = *display | (bit_mask[1] << 21);
    }
    //bit shifting hours_tens with the bit mask by 21
    
    *display = *display | (bit_mask[hours_ones] << 14); 
    *display = *display | (bit_mask[mins_tens] << 7); 
    *display = *display | (bit_mask[mins_ones] << 0); 
    //bitshifting hours, mins_tens, and mins_ones by 0,7,14

    return 0; //returning for success 
    

}

// Examines the TIME_OF_DAY_PORT global variable to determine hour,
// minute, and am/pm.  Sets the global variable CLOCK_DISPLAY_PORT bits
// to show the proper time.  If TIME_OF_DAY_PORT appears to be in error
// (to large/small) makes no change to CLOCK_DISPLAY_PORT and returns 1
// to indicate an error. Otherwise returns 0 to indicate success.
//
// Makes use of the previous two functions: set_tod_from_ports() and
// set_display_from_tod().
// 
// CONSTRAINT: Does not allocate any heap memory as malloc() is NOT
// available on the target microcontroller.  Uses stack and global
// memory only.


int clock_update(){
    tod_t tod;
    //initalizing tod variable
    int set_tod = set_tod_from_ports(&tod);
    //as shown in the write up

    if(set_tod){
        return 1;
    }
    if(set_display_from_tod(tod, &CLOCK_DISPLAY_PORT) == 1){
        return 1;
    }
    //using set_display_from_tod variable from the clock_main.c to 
    //display the clock.
    
    return 0; //returning for success
}

