/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "game.h"

#define LENGTH 255
#define WORDS 2048
#define KEY 5

char solution[128]; //Lösungswort

/* Liest den Namen des Spielers ein und schreibt diesen in eine Datei
 * Dabei werden Namesvorschläge anhand der Eingabe des Spielers gemacht
 * Rückgabewert ist ein char Array mit den Usernamen
 */
char *get_username(int player) {
    bool valid_playername = true;
    char playername[128] = {};
    char *username;
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
    playername[strlen(playername) -1] = '\0';
    write_username_to_file(playername);

    username = malloc(sizeof(playername)+1); // Speicher bereitstellen
    strcpy(username, playername); //Funktion soll einen Pointer zurückgeben, also wird dort der Name rüberkopiert


    return username;
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
    int random_index = rand () % 57;

    FILE *word_list = fopen("Words.txt","r");
    char input[57];

    while(fgets(input, 56, word_list))
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

    //int selection = 0;
    //print_gamemode_selection(selection);
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

    return gamemode;
}



/*
 * Gib Statistiken aus
 */
void print_statistic(Statistic statistic) {

    printf("\n******* Statistik f%cr %s", ue, statistic.username);
    printf(" *******\n");
    printf("\tBen%ctigte Zeit:   %.2lfs\n", oe, statistic.time);
    printf("\tRichtig geraten:  %d\n", statistic.success_count);
    printf("\tFalsch geraten:   %d\n", statistic.error_count);
    printf("\tZ%cge insgesamt:   %d\n", ue, statistic.error_count + statistic.success_count);

    //passende Anzahl an Sternchen ausgeben um die gleiche Länge der Überschrift haben
    for(int i = 0; i < strlen(statistic.username) + 30; i++) {
        printf("*");
    }
    printf("\n");
}

/*
 * Speichert Statisken in einer Highscoreliste als CSV-Datei
 */
void save_statistic(Statistic statistic) {
    FILE* file;
    int maxc = 1024;
    int last_id = 0;
    int id = 0;
    char delimiter[] = ";";
    char* tmp;
    char* splitted;
    char filepath[18] = "Highscoreliste.csv";
    filepath[18] = '\0';
    char line[maxc];
    time_t t;
    struct tm* ts;

    //falls keine Highscoreliste existiert erstelle eine
    if(access(filepath, F_OK) != 0) {
        printf("Es existiert keine Highscoreliste! Neue Liste wird erstellt..\n");

        //Datei erstellen und Überschriften reinschreiben
        file = fopen(filepath, "w+");
        fprintf(file, "ID; name; mode; input_count; success_count; error_count; time; solution; win; date\n");
        fclose(filepath);
    }

    file = fopen(filepath, "a+");

    //Wenn die Datei nicht geöffnet werden konnte gib einen Error aus
    if(file == NULL) {
        perror("Error opening file!");
        exit(-1);
    }

    //Lies Datei um die letzte ID zu finden
    while(fgets(line, maxc, file) != NULL) {
        tmp = strdup(line);

        //letzte ID finden
        id = strtok(tmp, delimiter);
        if(id == "ID") {
            id = 1;
        } else {
            id = atoi(id) + 1; // atoi => string zu int
        }
    }

    //Wenn die Datei nicht geöffnet werden konnte gib einen Error aus
    if(file == NULL) {
        perror("Error opening file!");
        exit(-1);
    }

    //aktuelle Zeit und Datum
    t = time(NULL);
    ts = localtime(&t);

    fprintf(file, "%d; %s; %d; %d; %d; %d; %.2lf; %s; %d; %d-%d-%d\n",
            id,
            statistic.username,
            statistic.mode,
            statistic.error_count + statistic.success_count,
            statistic.success_count,
            statistic.error_count,
            statistic.time,
            statistic.solution,
            statistic.success,
            ts->tm_year+1900, ts->tm_mon, ts->tm_mday
            );
    fclose(file);

    printf("Statistik erfolgreich gespeichert!\n");

}

int main()
{

    char *username;
    char *player1;
    char *player2;
    int gamemode;
    int time_limit;
    Statistic statistic;
    Statistic *statistics; //für zwei Spieler Statistik

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
            get_solution();

            //Spiel start
            statistic = run(solution, username, 0);

            break;
        case 2:
            //Eingabe der Spielernamen
            player1 = get_username(1);
            player2 = get_username(2);

            //Auswahl des Lösungswortes
            get_solution();

            //Spiel start
            statistics = run_2player(solution, player1, player2);
            print_statistic(statistics[0]);
            print_statistic(statistics[1]);

            //Speichern der Statistik
            save_statistic(statistics[0]);
            save_statistic(statistics[1]);

            break;
        case 3:
            //Eingabe der Spielernamen
            username = get_username(0);

            //Auswahl des Lösungswortes
            get_solution();

            time_limit = 60;

            //Spiel start
            statistic = run(solution, username, time_limit);

            //Ausgabe der Staistik und des Highscores
            print_statistic(statistic);

            break;
        default:
            break;
    }



    if(gamemode == 1 ||gamemode == 3) {
        //Ausgabe der Staistik und des Highscores
        print_statistic(statistic);

        //speichern der aktualisierten Highscore datei
        save_statistic(statistic);
    }



    return 0;
}
