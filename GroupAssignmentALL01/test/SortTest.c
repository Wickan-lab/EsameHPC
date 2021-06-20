#include <stdio.h>
#include <stdlib.h>
#include "TestLib.h"
#include "sort.h"
#include "point.h"
#include "knn_omp.h"
#include "assert.h"

int main(int argc, char** argv){
    if(argc != 3){
        printf("Usage : [nc] number of clusters, [k] number of neighbours\n");
        exit(EXIT_FAILURE);
    }

    int N;
    int l = 9;
    printf("l : %d\n",l);
    int nc = atoi(argv[1]);
    int k = atoi(argv[2]);
    Point* dataset;
    printf("============================================\n");
    printf("============================================\n\n");
    printf("Generating points\n");
    dataset = Test_Point_GeneratePoints(dataset, l, &N, nc, make_square);    
    printf("Generating points OK with N : %d\n\n", N);



    printf("Test Point is the middle of the sqaure\n\n");
    Point test_point;
    test_point.x = (float)l/2;
    test_point.y = (float)l/2;
    //test point is in the middle of the square

    printf("Calculating Distances\n");
    Test_Points_EuclideanDistance(dataset, N, test_point);
    printf("Calculating Distances OK\n\n");
    // Quick Sort    
    printf("Testing Quick sort\n");
    QuickSortIterative(dataset, N);
    if (validate_sort(dataset, N) == 0){
        printf("Quick Sort failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Quick sort OK\n\n");

    int test_cluster = Test_classify(dataset, N, k, nc);
    int cluster = OMP_ClassifyPoint(dataset, test_point, k, N, 0, OMP_Facade_QuickSort);
    printf("Test point belongs to %d\n", test_cluster);
    printf("[Classify under test] Point belongs to %d\n\n", cluster);
    assert(test_cluster == cluster);
    printf("Asserted cluster_num equality\n");

    printf("============================================\n");
    printf("============================================\n\n");

    printf("Regenerating points\n");
    dataset = Test_Point_GeneratePoints(dataset,l, &N, nc, make_square);
    printf("Regenerating points OK with N : %d\n\n", N);

    test_point.x = (float)-2*l;
    test_point.y = (float)l/2;

    printf("Test Point is at x : %f, y : %f\n",test_point.x, test_point.y);


    printf("Calculating Distances\n");
    Test_Points_EuclideanDistance(dataset, N, test_point);
    printf("Calculating Distances OK\n");

    //Bitonic Sort
    printf("Testing Bitonic Sort\n");
    //0, n / size - 1, local_a, 0
    bitonicSequenceGenerator(0, N , dataset, 0);
    if (validate_sort(dataset, N) == 0){
        printf("Bitonic Sort failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Testing Bitonic Sort OK\n\n");

    test_cluster = Test_classify(dataset, N, k, nc);
    cluster = OMP_ClassifyPoint(dataset, test_point, k, N, 0, OMP_Facade_BitonicSort);

    printf("Test point belongs to %d\n", test_cluster);

    printf("[Classify under test] Point belongs to %d\n\n", cluster);

    assert(test_cluster == cluster);
    printf("Asserted cluster_num equality\n");

    printf("============================================\n");
    printf("============================================\n\n");

    
    return 0;
	
}