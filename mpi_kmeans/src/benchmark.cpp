#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdio.h>

#include "../include/utils.h"
#include "../include/mpi_kmeans.h"

using namespace std;

void mark1(){
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    performance_t p;
    int it = 50;
    int reruns = 10;
    int k = 1<<4;
    int d = 1<<4;
    int N = 12;
    int ni = 10;
    int max_n = 1<<(N+ni-1);

    //initialise enough memory once
    point_t points = new element_t[ max_n*d ];
    point_t centroids = new element_t[k*d];

    init_test_points(points, max_n, d);

    if(pid==0){
        printf("Mark 1 (k=%d, d=%d)\n",k, d);
        printf("n, assign_to_centroid time, update_centroid time\n");
    }

    for(int x=ni; x<ni+N;x++){
        int n = 1<<x;
        p.clear();

        mpi_kmeans mk(points, centroids, k, n, d);
        
        for(int r=0;r<reruns;r++)mk.execute(it, p);
        p.runtime1/=reruns;
        p.runtime2/=reruns;
        p.runtime3/=reruns;

        if(pid==0)printf("%d, %.4f, %.4f, %.04f \n", n, p.runtime1, p.runtime2, p.runtime3);

    }

    delete[] points;
    delete[] centroids;
}

void mark2(){
    int pid;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    performance_t p;
    int it = 50;
    int reruns = 10;
    int k = 1<<4;
    int n = 1<<10;
    int D = 12;
    int di = 4;
    int max_d = 1<<(D+di-1);

    //initialise enough memory once
    point_t points = new element_t[ n*max_d];
    point_t centroids = new element_t[k*max_d];

    init_test_points(points, n, max_d);

    if(pid==0){
        printf("Mark 2 (k=%d, n=%d)\n",k, n);
        printf("d, assign_to_centroid time, update_centroid time\n");
    }

    for(int x=di; x<di+D;x++){
        int d = 1<<x;
        p.clear();

        mpi_kmeans mk(points, centroids, k, n, d);
        
        for(int r=0;r<reruns;r++)mk.execute(it, p);
        p.runtime1/=reruns;
        p.runtime2/=reruns;
        p.runtime3/=reruns;

        if(pid==0)printf("%d, %.4f, %.4f, %.04f \n", d, p.runtime1, p.runtime2, p.runtime3);
        
    }

    delete[] points;
    delete[] centroids;
}

int main( int argc, char **argv ) {
    MPI_Init(&argc, &argv);

    mark1();
    mark2();

    MPI_Finalize();
    return 0;
}