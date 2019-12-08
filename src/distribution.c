#include"header.h"
#include<stdio.h>
int Uniform(int rangeLow, int rangeHigh) {

 	int range = rangeHigh - rangeLow + 1;
    int secureMax = RAND_MAX - RAND_MAX % range;
    int x;
    do x = rand(); while (x >= secureMax);
    return rangeLow + x % range;

}

double Exponential(double l){  // stack overflow

  	double u;
    u = rand() / (RAND_MAX + 1.0);

    return  -log(1- u) / l;
 
}
