#ifndef DOTPROD_H_   /* Include guard */
#define DOTPROD_H_

/** macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

#include <time.h>

#define STARTTIME(id) clock_t start_time_42_id,end_time_42_id; start_time_42_id = clock()

#define ENDTIME(id,x) end_time_42_id=clock(); x=((double) (end_time_42_id-start_time_42_id)) /  CLOCKS_PER_SEC

void init_structures(double**, double **, double **, int , int , int);
void dot_product(double *, double *, double *, int , int , int );
void init_structures_upgr(double**, double **, double **, int , int , int);
void dot_product_upgr(double *, double *, double *, int , int , int );

#endif