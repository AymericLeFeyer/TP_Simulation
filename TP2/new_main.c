#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define TEMPS 5000.0

double a = 2.0;
double b = 6.0;
double M = 2.0;

double generer_inter_arrivee(void) {
    double z,x;
    z=(double)rand()/RAND_MAX;
    if(z!=0){
        x=(double)-(1.0/0.3)*log(z);
        return x;
    }
    return generer_inter_arrivee();
}

double generer_tmps_serviceA(void)
{
    double z,x;
    z=(double)rand()/RAND_MAX;
    if(z!=0){
        x=(double)-(3.0)*log(z);
        return x;
    }
    return generer_tmps_serviceA();
}


double generer_tmps_serviceB(void)
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
   return generer_tmps_serviceB();
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
    double delta = 0; // temps d'inter-evenements
    double t = 0; // temps d'horloge
    double t_depA = generer_tmps_serviceA() + t; // pas de depart programmé
    double t_depB = TEMPS; // pas de depart programmé
    double t_arrA = 0; // arrivée de la pièce suivante à zéro
    double t_arrB = t_depA; // arrivée de la pièce suivante à zéro
    double t_fin = TEMPS; // duree de la simulation
    double t_bloc = 0;
    int bA = 0; // etat du serveur, busy or not
    int bB = 0;
    int sA = 0; // nb de pieces presentes dans le systeme
    int sB = 0;
    int nb = 0; // compteur pour le nombre d'arrivee
    int nbA = 0;
    int nbB = 0;
    double t_cum = 0; // temps total de sejour dans le systeme
    double t_occA = 0; // temps total d'occupation du serveur
    double t_occB = 0; // temps total d'occupation du serveur
    int nouvellePiece = 0;
    int maxFileB = 3;
    int ssA = 0;
    int ssB = 0;
    int ssTotal = 0;

    while (t < t_fin) {
        printf("--------\n");
        printf("Temps t : %lf\n", t);
        printf("Prochaine arrivee A : %lf\n", t_arrA);
        printf("Prochain depart A : %lf\n", t_depA);
        printf("Prochaine arrivee B : %lf\n", t_arrB);
        printf("Prochain depart B : %lf\n", t_depB);
        printf("Pieces dans B : %d\n", sB);

        ssA += sA;
        ssB += sB;
        ssTotal += (sA + sB);

        if (sB != maxFileB) {
                if ((t_arrA <= t_depA) && (t_arrA <= t_arrB) && (t_arrA <= t_depB)) {
                    printf("Arrivee dans A ...\n");
                // Evenement d'arrivee A
                delta = t_arrA - t;
                t_cum = t_cum + sA * delta + sB * delta;
                t_occA = t_occA + bA * delta;
                nbA++;
                nb++;
                t = t_arrA;
                sA++;
                if (!bA) {
                    bA = 1;
                    t_depA = t + generer_tmps_serviceA();
                }
                t_arrA = t + generer_inter_arrivee() ;
            }
        }
        else {
            printf("Bloqués !\n");
            t_depB = t_arrB;
            t_bloc += delta;
            

        }
            if ((t_depA <= t_arrA) && (t_depA <= t_arrB) && (t_depA <= t_depB)) {
                printf("Depart de B ...\n");
                
                // Evenement de depart A
                delta = t_depA - t;
                t_cum = t_cum + sA * delta + sB * delta;
                t_occA = t_occA + bA * delta;
                t = t_depA;
                sA--;
                if (sA > 0) {
                    t_depA = t + generer_tmps_serviceA();                
                } else {
                    bA = 0;
                    t_depA = __INT_MAX__;
                }
                t_arrB = t;
                if (sB < maxFileB) nouvellePiece = 1;
                
                
            }
            

        if (((t_arrB <= t_depB) && (t_arrB <= t_arrA) && (t_arrB <= t_depA))) {
            printf("Arrivee dans B ...\n");
            if ((nouvellePiece)) {
                // Evenement d'arrivee B
                nouvellePiece = 0;
                delta = t_arrB - t;
                t_cum = t_cum + sA * delta + sB * delta;
                t_occB = t_occB + bB * delta;
                nbB++;
                nb++;
                t = t_arrB;
                sB++;
                if (!bB) {
                    bB = 1;
                    t_depB = t + generer_tmps_serviceB();
                }

            } else {
                t = t_arrA;
                t_arrB = t_depA;
                
            }
            
            
            
        }
        if ((t_depB <= t_arrB) && (t_depB <= t_depA) && (t_depB <= t_arrA)) {
            printf("Depart de B ...\n");
            // Evenement de depart B
            delta = t_depB - t;
            t_cum = t_cum + sA * delta + sB * delta;
            t_occB = t_occB + bB * delta;
            t = t_depB;
            sB--;
            if (sB > 0) {
                t_depB = t + generer_tmps_serviceB();                
            } else {
                bB = 0;
                t_depB = __INT_MAX__;
            }
        }
    }
    delta = t_fin - t;

    t_cum = t_cum + sA * delta + sB * delta;

    t_occA = t_occA + bA * delta;
    t_occB = t_occB + bB * delta;

    printf("----------------\n");

    printf("Nombre de pièces : %d\n", nbA);
    printf("Nombre de pièces sorties de B : %d\n", nbB);
    printf("Periode de simulation : %lf\n", t);
    printf("Nombre moyen de pieces dans l'atelier : %lf\n", ssTotal/t + ssTotal/t);
    printf("Temps moyen passe par une piece dans l'atelier : %lf\n", (t_cum) / (t));
    printf("Pourcentage de blocage du serveur A : %.4lf%%\n", 100*(t_bloc/t_fin));

    printf("Pourcentage d'utilisation de la machine A : %.2lf%%\n", 100*(t_occA/t_fin));
    printf("Pourcentage d'utilisation de la machine B : %.2lf%%\n", 100*(t_occB/t_fin));
    printf("Lambda : %lf\n", nbA/t );
}

int main() {
    serverAB();
}