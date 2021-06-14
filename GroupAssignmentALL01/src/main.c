#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define UP 0
#define DOWN 1

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
	return 1/Q_rsqrt(sum);
}

int distance_comparison(Point a, Point b){
	return (a.distance >= b.distance) ? 1 : 0; 
}

void generate_points(Point *dataset, int n){
	for (int i = 0; i < n; ++i)
	{	
		Point new_point;
		new_point.x = rand() % 50;               /*  HARDCODED  */
		new_point.y = rand() % 50;			     /*  HARDCODED  */
		new_point.cluster_number = rand() % 2;   /*  HARDCODED  */
		dataset[i] = new_point;
	}
}
 
void merge_up(Point *arr, int n) {
	int step = n/2, i, j, k;
	int old_step = n;
	Point temp;
	
	while (step > 0) {
		for (i=0; i < n; i+=old_step) {
			for(j = i ; j < i+step ; j++){
				if (arr[j].distance > arr[j+step].distance ) {
					// swap
					temp = arr[j];
					arr[j]=arr[j+step];
					arr[j+step]=temp;
				}
			}
		}
		old_step = step;
		step /= 2;
	}
}

void merge_down(Point *arr, int n) {
	int step=n/2,i,j,k;
	int old_step = n;
	Point temp;
	
	while (step > 0) {
		for (i=0; i < n; i+=old_step) {
			for(j = i ; j < i+step ; j++){
				if (arr[j].distance < arr[j+step].distance ) {
					// swap
					temp = arr[j];
					arr[j]=arr[j+step];
					arr[j+step]=temp;
				}
			}	
		}
		old_step = step;
		step /= 2;
	}
}

int classify_point(Point *dataset, Point test_point, int k, int n){
	int counter_cluster_0 = 0, counter_cluster_1 = 0;
	float arr[n];
	for (int i = 0; i < n; ++i)
	{
		dataset[i].distance = euclidean_distance(dataset[i], test_point);
	}

	for (int s = 2; s < n; s *= 2) {
		for (int i = 0; i < n ; i += s*2) {
			merge_up((dataset + i), s);
			merge_down((dataset + i + s), s);
      }
  	}
	merge_up(dataset, n);

	for (int i = 0; i < n; ++i)
	{
		printf("indice %d - cluster number %d - distance %f \n", i, dataset[i].cluster_number, dataset[i].distance);
	}
	
	for (int i = 0; i < k; ++i)
	{
		if (dataset[i].cluster_number == 0)
			counter_cluster_0 += 1;
		else
			counter_cluster_1 += 1;
	}

	return (counter_cluster_1 >= counter_cluster_0) ? 1 : 0; 

}

int main(int argc, char const *argv[])
{	

	if(argc != 5){
		fprintf(stderr,"Usage:\n\t%s [n : points to generate] [x: Coordinate x of the point to classify] [y: Coordinate y of the point to classify] [k]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	int k = atoi(argv[4]);

	Point dataset[n];
	Point test_point;

	test_point.x = x;
	test_point.y = y;
	
	srand((unsigned int)time(NULL)); 

	generate_points(dataset, n);

	test_point.cluster_number = classify_point(dataset, test_point, k, n);

	printf("Point belongs to cluster: %d\n", test_point.cluster_number);

	return EXIT_SUCCESS;
}
