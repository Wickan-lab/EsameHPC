#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "knn.h"

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