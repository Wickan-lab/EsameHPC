#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct 
{	
	int cluster_number;   /* Cluster to which the point belongs */
	float x, y; 	      /* Coordinate x and y of the point    */
	float distance; 	  /* Distance from test point           */
} Point;


float Q_rsqrt(float number)
{  
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv  = {.f = number};
	
	conv.i  = 0x5f3759df - (conv.i >> 1);
	conv.f  *= threehalfs - (x2 * conv.f * conv.f);
	
	return conv.f;
}

float euclidean_distance(Point a, Point b){
	float square_difference_x = (b.x - a.x) * (b.x - a.x);
    float square_difference_y = (b.y - a.y) * (b.y - a.y);
    float sum = square_difference_x + square_difference_y;
    return Q_rsqrt(sum);
}

int distance_comparison(Point a, Point b){
	return (a.distance >= b.distance) ? 1 : 0; 
}

int main(int argc, char const *argv[])
{
	



	return 0;
}