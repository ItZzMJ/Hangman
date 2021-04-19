/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int MAX_STR_LENGTH = 128;

char *get_username(int player) {
    char *username = malloc(MAX_STR_LENGTH);
    int success;
    if(!username) {
        return NULL;
    }
    printf("\t");
    if(player == 1) {
        printf("Spieler 1 bitte ");
    } else if(player == 2) {
        printf("Spieler 2 bitte ");
    } else {
        printf("Bitte ");
    }

    printf("geben Sie einen Usernamen an: ");
    do {
        success = 0;
        gets(username);
        fflush(stdin);
        if(!username) {
            success = 1;
            printf("\tDer Username darf nicht leer sein! Versuch's nochmal: ");
        }
    } while(success == 1);


    return username;
}

char *get_solution() {
    return "Cooler Typ";
}

int get_gamemode() {
    int gamemode,success = 0;

    printf("\t1 => 1 Spieler\n");
    printf("\t2 => 2 Spieler\n");
    printf("\t3 => 1 Spieler gegen Zeit\n");
    printf("\tBitte w%chlen Sie einen Spielmodus: ", ae);
    do {
        success = 0;
        scanf("%d", &gamemode);
        fflush(stdin);

        if(gamemode < 1 ||gamemode > 3) {
            success = 1;
            printf("\tDer Spielmodus muss zwischen 1 und 3 liegen! Versuch's nochmal: ");
        }

    } while(success == 1);

}

void print_statistic(Statistic statistic) {

    printf("\n******* Statistik f%cr %s *******\n", ue, statistic.username);
    printf("\tBen%ctigte Zeit:   %.2lfs\n", oe, statistic.time);
    printf("\tRichtig geraten:  %d\n", statistic.success_count);
    printf("\tFalsch geraten:   %d\n", statistic.error_count);
    printf("\tZ%cge insgesamt:   %d\n", ue, statistic.error_count + statistic.success_count);


}

int main()
{
    char *username;
    char *solution;
    char *player1;
    char *player2;
    int gamemode;
    int time_limit;
    Statistic statistic;
    Statistic statistics[2];//für zwei Spieler Statistik

    //Begrüßung
    printf("****************** Hangman ******************\n");
    printf("\n\n");

    //Auswahl des Spielmodus
    gamemode = get_gamemode();

    switch (gamemode) {
        case 1:
            //Eingabe des Spielernamens
            username = get_username(0);

            //Auswahl des Lösungswortes
            solution = get_solution();

            //Spiel start
            statistic = run(solution, username, 0);

            //Ausgabe der Staistik und des Highscores
            print_statistic(statistic);
            break;
        case 2:
            //Eingabe der Spielernamen
            player1 = get_username(1);
            player2 = get_username(2);

            //Auswahl des Lösungswortes
            solution = get_solution();

            //Spiel start
//            statistics = run_2player(solution, player1, player2);
//            print_statistic(statistics[0]);
//            print_statistic(statistics[1]);
            break;
        case 3:
            //Eingabe der Spielernamen
            username = get_username(0);

            //Auswahl des Lösungswortes
            solution = get_solution();

            time_limit = 60;

            //Spiel start
            statistic = run(solution, username, time_limit);

            //Ausgabe der Staistik und des Highscores
            print_statistic(statistic);
            break;
        default:
            break;
    }



    //speichern der aktualisierten Highscore datei




    return 0;
}
