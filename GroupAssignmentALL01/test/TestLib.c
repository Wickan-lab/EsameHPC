#include "point.h"
#include <stdio.h>
#include <stdlib.h>


int validate_sort(Point *dataset, int n){
    for(int i = 0; i < n - 1; i++){
        if(dataset[i].distance > dataset[i+1].distance || dataset[i].distance == 0){
            printf("[Validate Sort] Error in sort index %d and %d & elem1: %f elem2: %f\n", i, i + 1, dataset[i].distance, dataset[i+1].distance);
            return 0;
        }
    }
    return 1;
}

Point* make_square(Point* dataset, int l, int nc, int*n){
    
    int N = 4;
    dataset = calloc(N, sizeof(Point));
    printf("Allocated Dataset Space\n");

    for(int i = 0; i < N; i++){
        dataset[i].x = ((i % 2) == 0) ? 0 : l;   //0 l 0 l
        dataset[i].y = (i < N/2) ? 0 : l;       //0 0 l l
        dataset[i].cluster_number = rand() % nc;
        printf("Point %f %f of cluster %d\n",dataset[i].x,dataset[i].y,dataset[i].cluster_number);
    }
    *n = N;
    return dataset;
}

Point* Test_Point_GeneratePoints(Point*dataset, int l, int* n, int nc, Point *(*shape)(Point*, int, int, int*)){
    dataset = shape(dataset, l, nc, n);
    return dataset;
}

void Test_Points_EuclideanDistance(Point* dataset, int N, Point test_point){
    
    for(int i = 0; i < N; i++){
        dataset[i].distance = Point_EuclideanDistance(dataset[i], test_point);
        printf("Distance from test point is %f @index : %d\n", dataset[i].distance,i);
    }
}

int Test_classify(Point* ordered_dataset, int N, int k, int nc){
    if(nc > N){
        printf("Number of clusters more then size\n");
        exit(EXIT_FAILURE);
    }

    int* clusters = calloc(nc,sizeof(int));
    printf("Counting knn points cluster_number\n");
    for(int i = 0; i < k; i++){
        clusters[ordered_dataset[i].cluster_number]++;
        printf("Considering point with x: %f, y: %f, cluster: %d\n",ordered_dataset[i].x,ordered_dataset[i].y,ordered_dataset[i].cluster_number);
    }
    printf("Counting knn points cluster_number OK\n");

    printf("Calculating most frequent cluster_number\n");
    int pos = 0, max = 0;
    for(int i = 0; i < k; i++){
        if(max < clusters[i]){
            max = clusters[i];
            pos = i;
        }
    }
    printf("Calculating most frequent cluster_number OK\n");
    return pos;
}