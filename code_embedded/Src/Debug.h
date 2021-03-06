//Debug.h



#include <stdio.h>


#ifndef DEBUG_H
#define DEBUG_H

// define the different Debug regions here:
#define DEBUG_COMMAND_1     0x00000000000000000000000000000001
#define DEBUG_SCANNING_1    0x00000000000000000000000000000010
#define DEBUG_LADAR_MOTOR_1 0x00000000000000000000000000000100









// DEBUG_LEVEL should be equal to all the regions
// that are active or'ed together.
#define DEBUG_LEVEL DEBUG_COMMAND_1 | DEBUG_SCANNING_1 | DEBUG_LADAR_MOTOR_1


//#define DEBUG



#ifdef DEBUG

// dprint prints out whatever is passed in as if it were a printf
#define dprint(level,...) if(level&DEBUG_LEVEL) printf(__VA_ARGS__)

// whereis prints out the current line number and filename
#define whereis(level) if(level&DEBUG_LEVEL) printf("At line %d of file %s\n",__LINE__,__FILE__)

// whatis prints out the name and value of 
#define whatis(level,variable,formatArg) if(level&DEBUG_LEVEL) printf("%s is " #formatArg " on line %d of file %s\n",#variable, variable,__LINE__,__FILE__)




#else

#define dprint(level,args...) 
#define whereis(level) 
#define whatis(level,variable,type) 


#endif



#endif