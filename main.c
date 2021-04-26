/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"

#define LENGTH 255
#define WORDS 2048
#define KEY 5


/* Liest den Namen des Spielers ein und schreibt diesen in eine Datei
 * Dabei werden Namesvorschläge anhand der Eingabe des Spielers gemacht
 * Rückgabewert ist ein char Array mit den Usernamen
 */
char *get_username(int player) {
    bool valid_playername = true;
    char playername[128] = {};
    int i = 0;
    char line[5000] = {};
    int position = 0;
    FILE *names_file = fopen("playernames.txt","r");
    fgets(line,LENGTH,names_file);
    fclose(names_file);

    while(1 == 1) {
        system("cls");
        if(valid_playername == false) {
            printf("Spielername darf nicht leer sein!\n");
        }
        if(player != 0) {
            printf("Spieler %i, bitte gib deinen Namen ein ", player);
        } else {
            printf("Bitte gib deinen Namen ein ");
        }

        print_suggestions(position, line);
        printf("\nName: %s", playername);

        playername[i] = getch();
        position = get_suggestions_position(playername);

        if(playername[i] == 13) {
            if(i == 0) {
                valid_playername = false;
                playername[i] = NULL;
                continue;
            } else {
                valid_playername = true;
                break;
            }
        }
        if(playername[i] == 8) {
            playername[i] = NULL;
            if(i != 0) {
                i--;
            }
            playername[i] = NULL;
        } else {
            i++;
        }
    }
    write_username_to_file(playername);
    return playername;
}

/* Hilfsfunktion für get_suggestions_position
 * Sucht die Postion eines Teilstrings heraus und gibt diese zurück
 */
int strpos(char *src, char *search) {
    char max[strlen(src)];
    strncpy(max, src, strlen(src));
    char *position = strstr(max, search);
    if (position) {
        return position - max;
    }
    return 0;
}

/* Sucht die Postion eines Teilstrings heraus und gibt dann die Postition des Wortanfangs zurück
 *
 */
int get_suggestions_position(char playername[128]) {
    int position = 0;
    char line[5000] = {};
    FILE *names_file = fopen("playernames.txt","r");
    while(fgets(line,LENGTH,names_file)) {
        if(strstr(line, playername) != NULL) {
            position = strpos(line, playername);
            for(int i = position; line[i-1] != 13; i--) {
                if(i == 0) {
                    position = 0;
                    fclose(names_file);
                    return position;
                }
                position = i-1;
            }
        } else {
            position = -1;
        }
    }
    fclose(names_file);
    return position;
}

/* Zeigt Vorschläge anhand der Eingabe des Spielers an
 *
 */
void print_suggestions(int position, char line[5000]) {
    printf("\nVorschlag: ");
    if(position != -1) {
        for(int j = position; j < strlen(line);j++) {
            if(line[j] == 13) {
                break;
            }
            printf("%c", line[j]);
        }
    }
}

/* Schreibt den eingegebenen Spielernamen in eine .txt Datei
 * Es werden keine doppelten Einträge geschrieben
 */
void write_username_to_file(char playername[128]){
    char line[LENGTH][WORDS];
    bool name_already_exists = false;
    FILE *names_file = fopen("playernames.txt","r");
    while(fgets(line,LENGTH,names_file)) {
        if(strstr(line, playername) != NULL) {
            name_already_exists = true;
        }
    }
    fclose(names_file);
    if(name_already_exists == false) {
        FILE *WriteToFile = fopen("playernames.txt","a");
        fprintf(WriteToFile,"%s",playername);
        fclose(WriteToFile);
    }
}

void get_solution() {
    char words[WORDS][LENGTH];
    int count = 0;

    //Zufallsgenerator
    srand(time(NULL));
    int random_index = rand () % 67;

    FILE *word_list = fopen("Words.txt","r");
    char input[64];

    while(fgets(input, 63, word_list))
    {
      sscanf(input, "%s", words[count]);
      count++;
    }
    for(int i = 0; i < strlen(words[random_index]); i++) {
        solution[i] = words[random_index][i];
    }
    fclose(word_list);
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

            //Entschlüsselung der Wörterliste
            encyrption();

            //Auswahl des Lösungswortes
            get_solution(&solution);

            //Verschlüsselung der Wörterliste
            decyrption();

            //Spiel start
            statistic = run(solution, username, 0);

            //Ausgabe der Staistik und des Highscores
            print_statistic(statistic);
            break;
        case 2:
            //Eingabe der Spielernamen
            player1 = get_username(1);
            player2 = get_username(2);

            //Entschlüsselung der Wörterliste
            encyrption();

            //Auswahl des Lösungswortes
            get_solution(&solution);

            //Verschlüsselung der Wörterliste
            decyrption();

            //Spiel start
//            statistics = run_2player(solution, player1, player2);
//            print_statistic(statistics[0]);
//            print_statistic(statistics[1]);
            break;
        case 3:
            //Eingabe der Spielernamen
            username = get_username(0);

            //Entschlüsselung der Wörterliste
            encyrption();

            //Auswahl des Lösungswortes
            get_solution(&solution);

            //Verschlüsselung der Wörterliste
            decyrption();

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
