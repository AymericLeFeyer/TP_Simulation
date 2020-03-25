#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

double generer_inter_arrivee(void) {
    double esperance = 0.3;
    double lambda = 1 / esperance;
    double z = 0;
    while (z == 0) 
        z = (double) rand() / (double) RAND_MAX;

    return ((-1/lambda) * log(z));
}

double generer_tmps_serviceA(void) {
    double esperance = 3;
    double lambda = 1 / esperance;
    double z = 0;
    while (z == 0) 
        z = (double) rand() / (double) RAND_MAX;
        
    return ((-1/lambda) * log(z));
}


double generer_tmps_serviceB(void){
    double x = ((double) rand() / (double) RAND_MAX)*4+2;
    double y;

    if (x >=2 && x<3) {
        y=(1.0/2.0)*(x-2);
    }
    else if (x>=3 && x<6) {
        y=(1.0/2.0)*(2-((double)x/3));
    }
    else {
        y=0;
    }

    return y;
}


int main() {
    double s = 0;
    for (int i = 0; i < 10000; i++) {
        s += generer_tmps_serviceB();
    }
    printf("%lf\n", s/10000);

    return 0;
}