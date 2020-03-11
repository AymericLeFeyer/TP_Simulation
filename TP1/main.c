// Aymeric LE FEYER
// Conrad BAUDELET

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SEED 33
#define TEMPS 30.0

//generation des temps aleatoires d'inter-arrivee
int generer_inter_arrivee(void) {
    double x;
    do {
        x = (double) rand() / (double) RAND_MAX;
    } while (x == 0);

    if (x <= 0.25) {
        return 1;
    } else if (x <= 0.65) {
        return 2;
    } else if (x <= 0.85) {
        return 3;
    } else if (x <= 0.95) {
        return 4;
    } else {
        return 5;
    }
}

//generation des temps aleatoires de service de A
int generer_tmps_serviceA(void) {
    double x;
    do {
        x = (double) rand() / (double) RAND_MAX;
    } while (x == 0);

    if (x <= 0.55) {
        return 3;
    } else if (x <= 0.90) {
        return 2;
    } else {
        return 1;
    }
}


//generation des temps aleatoires de service de B
int generer_tmps_serviceB(void) {
    double x;
    do {
        x = (double) rand() / (double) RAND_MAX;
    } while (x == 0);

    if (x <= 0.55) {
        return 4;
    } else if (x <= 0.90) {
        return 3;
    } else {
        return 2;
    }
}

int main(int argc, char **argv) {
    double t = 0.0; // temps d'horloge
    double t_arr = 0.0; //instant de la prochaine arrivee
    double t_max = TEMPS; // duree maximale de la simulation (en jours)
    if (argc > 2) t_max = atoi(argv[2]);
    int long_file = 0; //nombre de tankers en attente dans la file, avant d'etre traites
    double t_finA = (double) RAND_MAX; //instant du prochain depart de tanker depuis le terminal A
    double t_finB = (double) RAND_MAX; //instant du prochain depart de tanker depuis le terminal B
    int etat_serveurA = 0; //etat du serveur A : 1 occupe, 0 libre
    int etat_serveurB = 0; //etat du serveur B : 1 occupe, 0 libre
    double t_cum = 0.0; //somme de tous les temps de sejour des tankers dans le systeme
    double t_inoccA = 0.0; //somme des temps d'inoccupation de A
    double t_inoccB = 0.0; //somme des temps d'inoccupation de B
    double T_moy; // variable pour calculer le temps moyen passe par un tanker dans le systeme
    double N_moy; // variable pour calculer le nombre moyen de tankers dans le systeme a un instant quelconque
    double pour_inoccA; //variable pour calculer l'inoccupation de A
    double pour_inoccB; //variable pour calculer l'inoccupation de B
    double delta; //variable pour calculer le temps entre deux evenements successifs
    int q = 0; //compteur du nombre de tankers arrives dans le systeme
    int modeAffichage = 1;

    /*
    printf("%d\n", RAND_MAX);
    exit(1);
    */


    if (argc > 2) srand(atoi(argv[1]));
    else srand(SEED);
    t_arr = t + generer_inter_arrivee(); //generation du temps pour la premiere arrivee de tanker dans le systeme

    if (modeAffichage ==1) printf("#\thorloge\tfile\tetatA\tetatB\tq\n");
    if (modeAffichage ==1) printf("init:\t%3.1lf\t%d\t%d\t%d\t%d\n", t, long_file, etat_serveurA, etat_serveurB,
           q); //affichage de l'etat global du systeme

    while (t < t_max) //fin de la simulation?
    {
        if (t_arr < t_finA && t_arr < t_finB) //si c'est un evenement d'arrivee
        {
            q++; //incrementation du compteur des arrivees
            delta = t_arr - t; //calcul du delta entre les deux evenements
            t = t_arr; //mise a jour du temps d'horloge
            t_arr = t + generer_inter_arrivee(); //programmation de l'arrivee suivante dans le systeme
            t_cum = t_cum + (delta * (long_file + etat_serveurA +
                                      etat_serveurB)); //mise a jour du temps total de sejour de tous les tankers dans le systeme

            if (etat_serveurB == 0) //si le serveur B est libre - il a la priorite, car il est plus rapide
            {
                etat_serveurB = 1;
                t_finB = t + generer_tmps_serviceB();
                t_inoccB = t_inoccB + delta;
                if (etat_serveurA == 0) {
                    t_inoccA = t_inoccA + delta;
                }
            } else {
                if (etat_serveurA == 0) {
                    etat_serveurA = 1;
                    t_finA = t + generer_tmps_serviceA();
                    t_inoccA = t_inoccA + delta;
                } else {
                    long_file++;
                }
            }
            if (modeAffichage ==1) printf("arr_: \t%3.1lf\t%d\t%d\t%d\t%d\n", t, long_file, etat_serveurA, etat_serveurB, q);
        } else {
            if (t_finA <= t_arr && t_finA < t_finB) {
                delta = t_finA - t;
                t = t_finA;
                t_cum = t_cum + (delta * (long_file + etat_serveurA +
                                          etat_serveurB)); //mise a jour du temps total de sejour de tous les tankers dans le systeme
                if (etat_serveurB == 0) {
                    t_inoccB = t_inoccB + delta;
                }
                if (long_file > 0) {
                    long_file--;
                    t_finA = t + generer_tmps_serviceA();
                } else {
                    etat_serveurA = 0;
                    t_finA = INT_MAX;
                }
                if (modeAffichage ==1) printf("finA: \t%3.1lf\t%d\t%d\t%d\t%d\n", t, long_file, etat_serveurA, etat_serveurB, q);
            }
            if (t_finB <= t_arr && t_finB <= t_finA) {
                delta = t_finB - t;
                t = t_finB;
                t_cum = t_cum + (delta * (long_file + etat_serveurA +
                                          etat_serveurB)); //mise a jour du temps total de sejour de tous les tankers dans le systeme
                if (etat_serveurA == 0) {
                    t_inoccA = t_inoccA + delta;
                }
                if (long_file > 0) {
                    long_file--;
                    t_finB = t + generer_tmps_serviceB();
                } else {
                    etat_serveurB = 0;
                    t_finB = INT_MAX;
                }
                if (modeAffichage ==1) printf("finB: \t%3.1lf\t%d\t%d\t%d\t%d\n", t, long_file, etat_serveurA, etat_serveurB, q);
            }
        }
    } //fin de la boucle de simulation

    //Fin de la simulation
    N_moy = t_cum / t; //calcul du nombre moyen de tankers dans le systeme
    T_moy = t_cum / q; //calcul du temps moyen passe par un tanker dans le systeme

    pour_inoccA = (t_inoccA / t) * 100.0;
    pour_inoccB = (t_inoccB / t) * 100.0;
    
    if (modeAffichage == 1) {
        printf("# Resultats de la simulation\n# -------\n");
        printf("# Periode de simulation : %lf\n", t);
        printf("# Nombre moyen de tankers dans le port : %lf\n", N_moy);
        printf("# Temps moyen passe par un tanker dans le port : %lf\n", T_moy);
        printf("# Pourcentage d'occupation du serveur A : %lf\n", 100 - pour_inoccA);
        printf("# Pourcentage d'occupation du serveur B : %lf\n", 100 - pour_inoccB);
        printf("# Lambda : %lf\n", q / t);
        printf("# q : %d\n", q);
    } else {
        printf("%lf\n%lf\n%lf\n%lf\n%lf\n", N_moy, T_moy, 100 - pour_inoccA, 100 - pour_inoccB, q / t);
    }

    
    

/*
double s=0;
double s1=0;
double s2=0;
    for (int i = 0; i<1000000;i++){
        s=s+generer_inter_arrivee();
    }
    printf("%lf\n",s/1000000);
    for (int i = 0; i<1000000;i++){
        s1=s1+generer_tmps_serviceA();
    }
    printf("%lf\n",s1/1000000);
    for (int i = 0; i<1000000;i++){
        s2=s2+generer_tmps_serviceB();
    }
    printf("%lf\n",s2/1000000);*/
    return 0;
}
