/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius, Armin Spöllmann, Mark Goldmann
 * Erstellt am: 04.05.2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#include "gui.h"
#include "fileio.h"
#include "game.h"

//globale Konstate
#define LENGTH 255



/*
 * Hilfsfunktion für get_suggestions_position
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


/*
 * Sucht die Postion eines Teilstrings heraus und gibt dann die Postition des Wortanfangs zurück
 */
int get_suggestions_position(char playername[128]) {
    int position = 0;
    char line[5000] = {};
    FILE *names_file = fopen("playernames.txt","r");

    //gehe durch jede Zeile in playernames.txt durch
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


/*
 * Liest den Namen des Spielers ein und schreibt diesen in eine Datei
 * Dabei werden Namesvorschläge anhand der Eingabe des Spielers gemacht
 * Rückgabewert ist ein char Array mit den Usernamen
 */
char *get_username(int player) {
    bool valid_playername = true;
    char playername[128] = {};
    char *username;
    int i = 0;
    char line[5000] = {};
    int position = -1;
    FILE *names_file = fopen("playernames.txt","r");
    fgets(line,LENGTH,names_file);
    fclose(names_file);

    while(1 == 1) {
        system("cls");
        if(valid_playername == false) {
            printf("Bitte gib einen g\x81ltigen Spielernamen ein!\n");
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

        if(playername[i] == 13) { //Wenn ENTER gedrückt wurde
            if(i == 0) {
                valid_playername = false;
                playername[i] = '\0';
                continue;
            } else {
                valid_playername = true;
                break;
            }
        }

        if(playername[i] == 32) { //Wenn SPACE gedrückt wurde
            valid_playername = false;
            position = -1;
            playername[i] = '\0';
            continue;
        }

        if(playername[i] == 9) { //Wenn TAB gedrückt wurde
            playername[i] = '\0';
            position = get_suggestions_position(playername);
            i = 0;

            for(int j = position; j < strlen(line);j++) {
                if(line[j] == 13) {
                    playername[i] = line[j];
                    break;
                }
                playername[i] = line[j];
                i++;
            }
            if(position == -1) {
                valid_playername = false;
                playername[i] = '\0';
                continue;

            } else {
                valid_playername = true;
                break;
            }
        }

        if(playername[i] == 8) { //Wenn BACKSPACE gedrückt wurde
            playername[i] = '\0';
            if(i != 0) {
                i--;
            }
            playername[i] = '\0';
        } else {
            i++;
        }
    }
    write_username_to_file(playername);
    playername[strlen(playername) -1] = '\0';

    username = malloc(sizeof(playername)+1); // Speicher bereitstellen
    strcpy(username, playername); //Funktion soll einen Pointer zurückgeben, also wird dort der Name rüberkopiert


    return username;
}


/*
 * Methode für die Spielmodusauswahl
 * Gibt den gewählten Spielmodus als Zahl zurück
 * 1 => 1 Spieler; 2 => 2 Spieler; 3 => 1 Spieler gegen Zeit
 */
int get_gamemode() {

    int ch1, ch2;
    int selection = 1;

    print_gamemode_selection(selection);

    //Menü-Schleife
    while(1 == 1) {
        ch1 = getch();
        ch2 = 0;
        if (ch1 == 0xE0) { // eine Scrolltaste wurde gedrückt
            ch2 = getch();
            //entscheide welche scrolltaste
            switch(ch2)
            {
                case 72: //Pfeiltaste nach oben
                    //printf("UP WAS PRESSED\n");
                    if(selection > 1) {
                        selection--;
                    }
                    print_gamemode_selection(selection);

                    break;

                case 80: //Pfeiltaste nach unten
                    //printf("DOWN WAS PRESSED\n");
                    if(selection < 4) {
                        selection++;
                    }
                    print_gamemode_selection(selection);
                    break;

                default:
                    break;
            }
        } else if(ch1 == 13) {
            break;
        }
    }
    return selection;
}



/*
 * Analysiert die gegebenen Statistiken
 */
Highscore * analyse_highscores(Statistic *statistics) {
    int i,j,k = 0;
    int player_count = 0;
    int max = 0;
    int tmp_index = 0;
    char *username;
    int entry_count = 0;
    Highscore * highscores;

    entry_count = get_highscore_count() - 1;
    Highscore * tmp = malloc(sizeof(Highscore) * entry_count); //temporäres Highscore-array


    //Satistiken analysen und in ungeordnetes und wahrscheinlich zu großes Highscore-array speichern
    for(i = 0; i < entry_count; i++) {
        username = statistics[i].username;

        tmp_index = 0;

        //überprüfe ob der Username bereits im tmp-array existiert
        for(j = 0; j < entry_count; j++) {
            if(strcmp(tmp[j].username, username) == 0) {
                tmp_index = j;//index merken
            }
        }

        //Username existiert bereits im tmp-array
        if(tmp_index != 0) {
            tmp[tmp_index].global_intput_count += statistics[i].success_count + statistics[i].error_count;

            if(statistics[i].success == 1) { //überprüfe ob das Spiel gewonnen oder verloren war
                tmp[tmp_index].games_won++;
            } else {
                tmp[tmp_index].games_lost++;
            }

            //durchschnittl. gebrauchte Zeit
            tmp[tmp_index].avg_time = (tmp[tmp_index].avg_time * (tmp[tmp_index].games_won + tmp[tmp_index].games_lost - 1) + statistics[i].time) / (tmp[tmp_index].games_won + tmp[tmp_index].games_lost);

        } else { //Username existiert noch nicht
            for(k = 0; k < strlen(username); k++) { // Username kopieren
                tmp[player_count].username[k] = username[k];
            }
            tmp[player_count].username[strlen(username)] = '\0';


            //weitere Daten im tmp-array speichern
            tmp[player_count].global_intput_count = statistics[i].success_count + statistics[i].error_count;
            tmp[player_count].avg_time = statistics[i].time;

            if(statistics[i].success == 1) { //überprüfe ob das Spiel gewonnen oder verloren war
                tmp[player_count].games_lost = 0;
                tmp[player_count].games_won = 1;

            } else {
                tmp[player_count].games_lost = 1;
                tmp[player_count].games_won = 0;
            }

            player_count++;
        }

    }

    printf("after for\n");

    highscores = malloc(sizeof(Highscore) * 3);

    //Highscore-array sortieren und die 3 besten in neues Array schreiben
    for(i = 0; i < 3; i++) {
        max = 0;

        //Spieler mit den meisten Siegen herausfinden
        for(j = 0; j < player_count; j++) {
            if(tmp[j].games_won > max) {
                max = tmp[j].games_won;
                tmp_index = j;
            }
        }

        //In Highscore-array schreiben und aus dem tmp-array "entfernen" indem man gewonnene Spiele auf 0 setzt
        highscores[i] = tmp[tmp_index];
        tmp[tmp_index].games_won = 0;
    }


    free(tmp); //Speicher wieder freigeben

    return highscores;
}


/*
 * Zeigt die aktuell 3 besten Spieler an mit Statistiken
 */
void show_highscores() {
    int count = 0;
    Highscore *highscores;
    Statistic *statistics;

    printf("\nStart reading\n");

    //Highscoreliste einlesen
    statistics = read_highscorelist();

    printf("start analysing\n");

    //Highscores analysieren
    highscores = analyse_highscores(statistics);

    printf("start printing\n");

    //Highscores ausgeben
    print_highscorelist(highscores, 3);


    //auf beliebige Tasteneingabe warten um zurück ins Hauptmenü zu gelangen
    getch();

    printf("reseting vars\n");


    //Array länge herausfinden
    count = get_highscore_count() - 1;

    //Speicher freigeben
    memset(statistics, 0, sizeof(Statistic) * count);
    printf("memset done\n");
    statistics = NULL;

    free(highscores);
    printf("Highscores freed\n");


    printf("Vars reseted!\n");
}


/*
 * Spielmethode
 * Startet erst die Spielmodus auswahl und dann den gewählten Spielmodus
 * Am Ende werden die Statistiken ausgegeben und gespeichert
 */
int run_game()
{
    char *username;
    char *player1;
    char *player2;
    int gamemode = 0;
    int time_limit = 0;
    Statistic statistic;
    Statistic *statistics; //für zwei Spieler Statistik

    //Auswahl des Spielmodus
    gamemode = get_gamemode();


    switch (gamemode) {
        case 1: //1 Spieler
            //Eingabe des Spielernamens
            username = get_username(0);

            //Auswahl des Lösungswortes
            get_solution();

            //Spiel start
            statistic = run(solution, username, time_limit);
            break;

        case 2: //2 Spieler
            //Eingabe der Spielernamen
            player1 = get_username(1);
            player2 = get_username(2);

            //Auswahl des Lösungswortes
            get_solution();

            //Spiel start
            statistics = run_2player(solution, player1, player2);

            //Ausgabe der Statistik
            print_statistic(statistics[0]);
            print_statistic(statistics[1]);

            //Speichern in der Highscoreliste
            save_statistic(statistics[0]);
            save_statistic(statistics[1]);

            memset(statistics, 0, sizeof(Statistic) * 2);
            statistics = NULL;
            break;

        case 3: //1 Spieler gegen Zeit
            //Eingabe der Spielernamen
            username = get_username(0);

            //Auswahl des Lösungswortes
            get_solution();

            time_limit = 60;

            //Spiel start
            statistic = run(solution, username, time_limit);
            break;

        case 4: //zurück
            return 0;
        default:
            break;
    }


    //Bei 1 Spieler Modi
    if(gamemode == 1 || gamemode == 3) {
        //Ausgabe der Staistik und des Highscores
        print_statistic(statistic);

        //speichern der aktualisierten Highscore datei
        save_statistic(statistic);

        //Variable freigeben
        memset(&statistic, 0, sizeof(Statistic));
    }

    printf("\n\nbeliebige Taste dr%ccken um zur%cck ins Hauptmen%c zu gelangen\n\n", ue, ue, ue);

    getch(); // Auf Eingabe des Spielers warten

    return 0;
}

/*
 * Hauptmenü
 * Ermöglicht Navigation im Hauptmenü und sorgt dafür, dass nach Beenden des Spiels wieder zurück ins Hauptmenü gelangt
 */
int main() {
    int selection = 0;
    int ch1, ch2;

    //Spiel-Schleife
    while(1) {
        print_main_menu(selection);

        //Menü-Schleife
        while(1) {
            ch1 = getch();
            ch2 = 0;
            if (ch1 == 0xE0) { // eine Scrolltaste wurde gedrückt
                ch2 = getch();

                //entscheide welche scrolltaste
                switch(ch2)
                {
                    case 72: //Pfeiltaste nach oben
                        if(selection > 0) {
                            selection--;
                        }
                        print_main_menu(selection);
                        break;

                    case 80: //Pfeiltaste nach unten
                        if(selection < 2) {
                            selection++;
                        }
                        print_main_menu(selection);
                        break;

                    default:
                        break;
                }
            } else if(ch1 == 13) {
                break;
            }
        }

        if(selection == 0) {
            //Spielstart
            if(run_game() == -1) {
                //beende Spiel, sonst neustart im Menü
                break;
            }

        } else if(selection == 1) {
            show_highscores();
            printf("showing highscores complete");

        } else if(selection == 2) { //Spiel beenden
            return 0;
        }
    }


    return 0;
}
