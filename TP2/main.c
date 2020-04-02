// TP2 Simulation - 02/04/2020
// BAUDELET Conrad
// LE FEYER Aymeric

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define TEMPS 5000.0
#define SEED 31415

double a = 2.0;
double b = 6.0;
double M = 2.0;

double generer_inter_arrivee(void) {
    double z,x;
    z=(double)rand()/RAND_MAX;
    if(z!=0){
        x=(double)-(1.0/0.2)*log(z);
        return x;
    }
    return generer_inter_arrivee();
}

double generer_tmps_serviceB(void)
{
    double z,x;
    z=(double)rand()/RAND_MAX;
    if(z!=0){
        x=(double)-(3.0)*log(z);
        return x;
    }
    return generer_tmps_serviceB();
}


double generer_tmps_serviceA(void)
{
   double x,y;
   x = a+(b-a)*(double)rand()/RAND_MAX;
   y = M*(double)rand()/RAND_MAX;
   if( (x>=2) && (x<3) )
   {
      if(y < (1.0/2.0)*(x-2.0) )
      {
          return x;
      }
   }
   if( (x >= 3) && (x <= 6) )
   {
       if(y < (1.0/2.0)*(2-(x/3.0) ) )
       {
           return x;
       }
   }
   return generer_tmps_serviceA();
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
    srand(SEED);
    double deltaA = 0; // temps d'inter-evenements
    double deltaB = 0; // temps d'inter-evenements
    double tA = 0; // temps d'horloge
    double tB = 0; // temps d'horloge
    double t_depA = generer_tmps_serviceA() + tA; // pas de depart programmé
    double t_depB = generer_tmps_serviceB() + tB; // pas de depart programmé
    double t_arrA = 0; // arrivée de la pièce suivante à zéro
    double t_arrB = TEMPS; // arrivée de la pièce suivante à zéro
    double t_fin = TEMPS; // duree de la simulation
    double t_bloc = 0;
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
    int nouvellePiece = 0;
    int maxFileB = 5;
    int maxFileA = 99999;
    
    int ssA = 0;
    int ssB = 0;
    int ssTotal = 0;

    while (tA < t_fin) {
        // printf("--------\n");
        // printf("Temps A : %lf, Temps B : %lf\n", tA, tB);
        // printf("Prochaine arrivee B : %lf\n", t_arrB);
        // printf("Prochain depart B : %lf\n", t_depB);
        // printf("Pieces dans B : %d\n", sA);

        ssA += sA;
        ssB += sB;
        ssTotal += (sA + sB);

        
        if (sA != maxFileA) {
            if (sB != maxFileB) {
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
                }
            else {
                printf("Bloqués B !\n");
                t_depB = t_arrB;
                
                t_bloc += deltaA;
                

            }
        }
        else {
            printf("Bloqués A !\n");
            t_depA = t_arrA;
        }
        if (t_arrA >= t_depA) {
          
            // Evenement de depart A
            deltaA = t_depA - tA;
            t_cumA = t_cumA + sA * deltaA;
            t_occA += bA * deltaA;
            tA = t_depA;
            sA--;
            if (sA > 0) {
                t_depA = tA + generer_tmps_serviceA();                
            } else {
                bA = 0;
                t_depA = __INT_MAX__;
            }
            t_arrB = tA;
            if (sB < maxFileB) nouvellePiece = 1;
            
            
        }
            
    
        if ((t_arrB < t_depB) && (nouvellePiece)) {
            // Evenement d'arrivee B
            nouvellePiece = 0;
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
            
            
        }
        else if (t_arrB >= t_depB) {
            // Evenement de depart B
            deltaB = t_depB - tB;
            t_cumB += sB * deltaB;
            t_occB += bB * deltaB;
            tB = t_depB;
            sB--;
            if (sB > 0) {
                t_depB = tB + generer_tmps_serviceB();                
            } else {
                bB = 0;
                t_depB = __INT_MAX__;
            }
        }
        else if (t_arrB < t_depB) {
            t_occB += bB * deltaB;
        }
        printf("%d\n", sB);
    }
    deltaA = t_fin - tA;
    deltaB = t_fin - tB;
    t_cumA = t_cumA + sA * deltaA;
    t_cumB = t_cumB + sB * deltaB;
    t_occA = t_occA + bA * deltaA;
    t_occB = t_occB + bB * deltaB;

    printf("----------------\n");

    printf("Nombre de pièces : %d\n", nbA);
    printf("Periode de simulation : %lf\n", tA);
    printf("Nombre moyen de pieces dans l'atelier : %lf\n", ssTotal/tA + ssTotal/tB);
    printf("Temps moyen passe par une piece dans l'atelier : %lf\n", (t_cumA) / (nbB));
    printf("Pourcentage de blocage du serveur A : %.4lf%%\n", 100*(t_bloc/t_fin));

    printf("Pourcentage d'utilisation de la machine A : %.2lf%%\n", 100*(t_occA/t_fin));
    printf("Pourcentage d'utilisation de la machine B : %.2lf%%\n", 100*(t_occB/t_fin));
    printf("Lambda : %lf\n", nbA/tA );
    // printf("----------------\n");
    // printf("%d\n%lf\n%lf\n%.2lf%%\n%.2lf%%\n%.2lf%%\n%lf", nbA, ssTotal/tA + ssTotal/tB, (t_cumA) / (nbB), 100*(t_bloc/t_fin), 100*(t_occA/t_fin), 100*(t_occB/t_fin), nbA/tA);
}

int main() {
    serverAB();
}