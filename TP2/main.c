#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define TEMPS 5000.0

double generer_inter_arrivee(void) {
    double esperance = 3.3333;
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

void serverA() {
    double delta = 0; // temps d'inter-evenements
    double t = 0; // temps d'horloge
    double t_dep = generer_tmps_serviceA() + t; // pas de depart programmé
    double t_arr = 0; // arrivée de la pièce suivante à zéro
    double t_fin = TEMPS; // duree de la simulation
    int b = 0; // etat du serveur, busy or not
    int s = 0; // nb de pieces presentes dans le systeme
    int nb = 0; // compteur pour le nombre d'arrivee
    double t_cum = 0; // temps total de sejour dans le systeme
    double t_occ = 0; // temps total d'occupation du serveur

    while (t < t_fin) {
        if (t_arr < t_dep) {
            // Evenement d'arrivee
            delta = t_arr - t;
            t_cum = t_cum + s * delta;
            t_occ = t_occ + b * delta;
            nb++;
            t = t_arr;
            s++;
            if (!b) {
                b = 1;
                t_dep = t + generer_tmps_serviceA();
            }
            t_arr = t + generer_inter_arrivee() ;
        }
        else {
            // Evenement de depart
            delta = t_dep - t;
            t_cum = t_cum + s * delta;
            t_occ = t_occ + b * delta;
            t = t_dep;
            s--;
            if (s > 0) {
                t_dep = t + generer_tmps_serviceA();                
            } else {
                b = 0;
                t_dep = __INT_MAX__;
            }
        }
    }
    delta = t_fin - t;
    t_cum = t_cum + s * delta;
    t_occ = t_occ + b * delta;
    printf("Temps de simulation : %lf\n", t);
    printf("Le nb de pieces dans le systeme est : %d\n", nb);
    printf("Le taux d'occupation du serveur est : %lf\n", t_occ/t_fin);
}

void serverB() {
    double delta = 0; // temps d'inter-evenements
    double t = 0; // temps d'horloge
    double t_dep = generer_tmps_serviceB() + t; // pas de depart programmé
    double t_arr = 0; // arrivée de la pièce suivante à zéro
    double t_fin = TEMPS; // duree de la simulation
    int b = 0; // etat du serveur, busy or not
    int s = 0; // nb de pieces presentes dans le systeme
    int nb = 0; // compteur pour le nombre d'arrivee
    double t_cum = 0; // temps total de sejour dans le systeme
    double t_occ = 0; // temps total d'occupation du serveur

    while (t < t_fin) {
        if (t_arr < t_dep) {
            // Evenement d'arrivee
            delta = t_arr - t;
            t_cum = t_cum + s * delta;
            t_occ = t_occ + b * delta;
            nb++;
            t = t_arr;
            s++;
            if (!b) {
                b = 1;
                t_dep = t + generer_tmps_serviceB();
            }
            t_arr = t + generer_inter_arrivee() ;
        }
        else {
            // Evenement de depart
            delta = t_dep - t;
            t_cum = t_cum + s * delta;
            t_occ = t_occ + b * delta;
            t = t_dep;
            s--;
            if (s > 0) {
                t_dep = t + generer_tmps_serviceB();                
            } else {
                b = 0;
                t_dep = __INT_MAX__;
            }
        }
    }
    delta = t_fin - t;
    t_cum = t_cum + s * delta;
    t_occ = t_occ + b * delta;
    printf("Temps de simulation : %lf\n", t);
    printf("Le nb de pieces dans le systeme est : %d\n", nb);
    printf("Le taux d'occupation du serveur est : %lf\n", t_occ/t_fin);
}

void serverAB() {
    double deltaA = 0; // temps d'inter-evenements
    double deltaB = 0; // temps d'inter-evenements
    double tA = 0; // temps d'horloge
    double tB = 0; // temps d'horloge
    double t_depA = generer_tmps_serviceA() + tA; // pas de depart programmé
    double t_depB = generer_tmps_serviceB() + tB; // pas de depart programmé
    double t_arrA = 0; // arrivée de la pièce suivante à zéro
    double t_arrB = 0; // arrivée de la pièce suivante à zéro
    double t_fin = TEMPS; // duree de la simulation
    int bA = 0; // etat du serveur, busy or not
    int bB = 0;
    int sA = 0; // nb de pieces presentes dans le systeme
    int sB = 0;
    int nb = 0; // compteur pour le nombre d'arrivee
    int nbA = 0;
    int nbB = 0;
    double t_cumA = 0; // temps total de sejour dans le systeme
    double t_cumB = 0; // temps total de sejour dans le systeme
    double t_occA = 0; // temps total d'occupation du serveur
    double t_occB = 0; // temps total d'occupation du serveur

    while (tA < t_fin) {
        if (t_arrA < t_depA) {
            // Evenement d'arrivee A
            deltaA = t_arrA - tA;
            t_cumA = t_cumA + sA * deltaA;
            t_occA = t_occA + bA * deltaA;
            nbA++;
            nb++;
            tA = t_arrA;
            sA++;
            if (!bA) {
                bA = 1;
                t_depA = tA + generer_tmps_serviceA();
            }
            t_arrA = tA + generer_inter_arrivee() ;
        }
        else {
            // Evenement de depart
            deltaA = t_depA - tA;
            t_cumA = t_cumA + sA * deltaA;
            t_occA = t_occA + bA * deltaA;
            tA = t_depA;
            sA--;
            if (sA > 0) {
                t_depA = tA + generer_tmps_serviceA();                
            } else {
                bA = 0;
                t_depA = __INT_MAX__;
            }
        }

        if (t_arrB < t_depB) {
            // Evenement d'arrivee B
            deltaB = t_arrB - tB;
            t_cumB = t_cumB + sB * deltaB;
            t_occB = t_occB + bB * deltaB;
            nbB++;
            nb++;
            tB = t_arrB;
            sB++;
            if (!bB) {
                bB = 1;
                t_depB = tB + generer_tmps_serviceB();
            }
            t_arrB = tB + generer_inter_arrivee() ;
        }
        else {
            // Evenement de depart
            deltaB = t_depB - tB;
            t_cumB = t_cumB + sB * deltaB;
            t_occB = t_occB + bB * deltaB;
            tB = t_depB;
            sB--;
            if (sB > 0) {
                t_depB = tB + generer_tmps_serviceB();                
            } else {
                bB = 0;
                t_depB = __INT_MAX__;
            }
        }
    }
    deltaA = t_fin - tA;
    deltaB = t_fin - tB;
    t_cumA = t_cumA + sA * deltaA;
    t_cumB = t_cumB + sB * deltaB;
    t_occA = t_occA + bA * deltaA;
    t_occB = t_occB + bB * deltaB;
    printf("Temps de simulation : %lf\n", tA);
    printf("Le nb de pieces dans le systeme A est : %d\n", nbA);
    printf("Le nb de pieces dans le systeme B est : %d\n", nbB);
    printf("Le taux d'occupation du serveur A est : %lf\n", t_occA/t_fin);
    printf("Le taux d'occupation du serveur B est : %lf\n", t_occB/t_fin);
}

int main() {
    serverAB();
}