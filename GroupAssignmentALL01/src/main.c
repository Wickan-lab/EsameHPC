#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

typedef struct 
{	
	int cluster_number;   /* Cluster to which the point belongs */
	float x, y; 	      /* Coordinate x and y of the point    */
	float distance; 	  /* Distance from test point           */
} Point;

/* Function to merge the two haves arr[l..m]
 and arr[m+1..r] of array arr[] */
void merge(Point arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Point L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].distance <= R[j].distance)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements
    of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements
    of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

 
/* l is for left index and r is
 right index of the sub-array
  of arr to be sorted */
void mergeSort(Point arr[], int l, int r)
{
   if (l < r)
   {
      // Same as (l+r)/2 but avoids
      // overflow for large l & h
      int m = l+(r-l)/2;
      mergeSort(arr, l, m);
      mergeSort(arr, m+1, r);
      merge(arr, l, m, r);
   }
}
 

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
 
 
int classify_point(Point *dataset, Point test_point, int k, int n){
	int counter_cluster_0 = 0, counter_cluster_1 = 0;

	for (int i = 0; i < n; ++i)
	{
		dataset[i].distance = euclidean_distance(dataset[i], test_point);
	}

	mergeSort(dataset, 0, n - 1);

	for (int i = 0; i < n; ++i)
	{
		printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", dataset[i].cluster_number, dataset[i].x, dataset[i].y, dataset[i].distance);
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
	
	srand(time(NULL)); 

	generate_points(dataset, n);

	test_point.cluster_number = classify_point(dataset, test_point, k, n);

	printf("Point belongs to cluster: %d\n", test_point.cluster_number);

	return 0;
}