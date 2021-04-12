/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include "game.h"

int MAX_STR_LENGTH = 128;

char *get_username() {
    char *username = malloc(MAX_STR_LENGTH);
    if(!username) {
        return NULL;
    }

    printf("Bitte geben Sie einen Username an: ");
    gets(username);
    fflush(stdin);

    return username;
}

char *get_solution() {
    return "Esel";
}

int main()
{
    char *username;
    char *solution;


    //Begrüßung
    printf("**** Hangman ****");
    printf("\n\n");

    //Eingabe des Spielernamens
    //username = get_username();


    //printf("\n Username ist: %s\n", username);

    //Auswahl des Lösungswortes
    solution = get_solution();

    printf("Lösungswort ist: %s \n", solution);

    //Spiel start
    run(solution);
        //extra Spiel methode run

    //Ausgabe der Staistik und des Highscores

    //speichern der aktualisierten Highscore datei




    return 0;
}
