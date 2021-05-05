/* Hangman Kommandozeilen-Spiel Bibliothek für das Userinterface
 * Author: Jannik Möbius, Armin Spöllmann, Mark Goldmann
 * Erstellt am: 04.05.2021
 */

#include <conio.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <windows.h>
#include "game.h"
#include "fileio.h"

char progress[128];
char used_letters[30];
int time_limit = 0;
clock_t game_start;


/*
 * Gibt den aktuellen Fortschritt in Form von Unterstrichen und geratenen Buchstaben aus
 */
void print_progress() {
    printf("\n\t\t");
    for(int i = 0; i < strlen(progress); i++) {
        printf("%c ", progress[i]);
    }
    printf("\n\n");
}


/*
 * Zeigt Namensvorschläge anhand der Eingabe des Spielers an
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
        printf("   (mit TAB Vorschlag benutzen)");
    }
}


/*
 * Gibt die Hangman Überschrift aus
 */
void print_header() {
    printf(" _                                             \n"
           "| |                                            \n"
           "| |__   __ _ _ __   __ _ _ __ ___   __ _ _ __  \n"
           "| '_ \\ / _` | '_ \\ / _` | '_ ` _ \\ / _` | '_ \\ \n"
           "| | | | (_| | | | | (_| | | | | | | (_| | | | |\n"
           "|_| |_|\\__,_|_| |_|\\__, |_| |_| |_|\\__,_|_| |_|\n"
           "                    __/ |                      \n"
           "                   |___/                       \n");
}


/*
 * ändert die Farbe der Konsolenausgabe
 */
void set_console_color_active()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}


/*
 * setzt die Farbe der Konsolenausgabe zurück
 */
void reset_console_color()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}


/*
 * Gibt das Menü aus
 */
void print_gamemode_selection(int selection) {
    system("cls");

    print_header();
    printf("Navigation mit den Pfeiltasten\n");
    printf("\n\n\t");

    if(selection == 1) {
        set_console_color_active();
    }
    printf("1 Spieler\n\t");
    reset_console_color();

    if(selection == 2) {
        set_console_color_active();
    }
    printf("2 Spieler\n\t");
    reset_console_color();

    if(selection == 3) {
        set_console_color_active();
    }
    printf("1 Spieler gegen Zeit\n\n\n\t");
    reset_console_color();

    if(selection == 4) {
        set_console_color_active();
    }
    printf("<- Zur%cck\n", ue);
    reset_console_color();

}


/*
 * Gib Statistiken aus
 */
void print_statistic(Statistic statistic) {

    printf("\n\t******* Statistik f%cr %s", ue, statistic.username);
    printf(" *******\n");
    printf("\t\tBen%ctigte Zeit:   %.2lfs\n", oe, statistic.time);
    printf("\t\tRichtig geraten:  %d\n", statistic.success_count);
    printf("\t\tFalsch geraten:   %d\n", statistic.error_count);
    printf("\t\tZ%cge insgesamt:   %d\n\t", ue, statistic.error_count + statistic.success_count);

    //passende Anzahl an Sternchen ausgeben um die gleiche Länge der Überschrift haben
    for(int i = 0; i < strlen(statistic.username) + 30; i++) {
        printf("*");
    }
    printf("\n");
}


/*
 * Gibt das Hauptmenü anhand der aktuell ausgewählten Option aus
 */
void print_main_menu(int selection) {
    system("cls");
    print_header();
    printf("\nNavigation mit den Pfeiltasten\n\n\t");

    if(selection == 0) {
        set_console_color_active();
    }
    printf("Spielen\n\t");
    reset_console_color();


    if(selection == 1) {
        set_console_color_active();
    }
    printf("Highscoreliste anzeigen\n\n\t");
    reset_console_color();

    if(selection == 2) {
        set_console_color_active();
    }
    printf("Spiel verlassen\n");
    reset_console_color();
}


/*
 * Gibt die gegebenen Highscores aus
 */
void print_highscorelist(Highscore *highscores, int count) {
    system("cls");
    int i = 0;
    double winrate = 0.0;

    printf("********************** Highscores **********************\n\n");

    for(i = 0; i < count; i++) {
        winrate = (double)highscores[i].games_won / (double)(highscores[i].games_won + highscores[i].games_lost);

        printf("%d. Platz: %s\n", i+1, highscores[i].username);
        printf("\tSpiele gewonnen: %d\n", highscores[i].games_won);
        printf("\tSpiele gespielt: %d\n", highscores[i].games_won + highscores[i].games_lost);
        printf("\tDurchschnittliche Zeit: %.2lfs\n", highscores[i].avg_time);
        printf("\tDurchschnittliche Siegquote: %d%%\n", (int)(winrate * 100));
        printf("\tInsgesamt geratene Buchstaben: %d\n", highscores[i].global_intput_count);
        printf("\n\n");
    }

    set_console_color_active();
    printf("   <- Zur%cck\n", ue);
    reset_console_color();

}


/*
 * Gibt die Anzahl bereits gebrauchten Züge und richtig geratenen Züge aus, sowie die aktuelle Zeit im Spiel aus
 * falls ein Zeitlimit gesetzt ist zeigt die Methode auch die verbleibende Zeit an
 */
void print_time_and_tries(int input_count, int error_count) {
    double time = 0.0;
    double time_left = 0.0;
    int success_count = input_count - error_count;

    //Zeit seit Spielstart
    time += clock() - game_start;
    time = time/CLOCKS_PER_SEC;

    //richtige Einrückung
    if(input_count < 10) {
        printf("Z%cge insgesamt:  %d\t", ue, input_count);
    } else {
        printf("Z%cge insgesamt: %d\t", ue, input_count);
    }

    //richtige Einrückung festlegen um beide Zeiten passend zu positionieren
    if(time < 10) {
        printf("\t\t Zeit:  %.2lfs\n", time);
    } else {
        printf("\t\t Zeit: %.2lfs\n", time);
    }

    //richtige Einrückung
    if(success_count < 10){
        printf("Davon richtig:   %d\t", success_count);
    } else {
        printf("Davon richtig:  %d\t", success_count);
    }



    if(time_limit != 0) {
        time_left = time_limit - time;
        if(time_left < 10 && time > 0) {
            printf("    verlbeibende Zeit:  %.2lfs\n", time_left);
        } else {
            printf("    verlbeibende Zeit: %.2lfs\n", time_left);

        }
    }
    printf("\n");
}


/*
 * Gibt das Spielfeld anhand des aktuellen Stands des Spiels aus
 */
void print_game(int error_count, int input_count) {
    system("cls"); // Konsole bereinigen
    int i = 0;

    char hangman[8][150] = {
            "\t\t  +---+\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t  |   |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t /|   |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t /|\\  |\n"
            "\t\t      |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t /|\\  |\n"
            "\t\t /    |\n"
            "\t\t      |\n"
            "\t\t=========\n",
            "\t\t  +---+\n"
            "\t\t  |   |\n"
            "\t\t  o   |\n"
            "\t\t /|\\  |\n"
            "\t\t / \\  |\n"
            "\t\t      |\n"
            "\t\t=========\n"
    };

    printf("********************** Hangman **********************\n");

    print_time_and_tries(input_count, error_count);

    printf("%s", hangman[error_count]);

    print_progress();


    // Wenn es bereits benutzte Buchstaben gibt, gib diese aus
    if(strlen(used_letters) > 0) {
        printf("\n\tBereits benutzte Buchstaben:\n\t");
        for(i = 0; i < strlen(used_letters); i++) {
            if(i == 0) {
                printf("%c", toupper(used_letters[i]));
            } else {
                printf(", %c", toupper(used_letters[i]));
            }
        }
    }
    printf("\n\n");
}


/*
 * Gibt den End-Bildschirm aus
 */
void print_endscreen(int game_won, char *username) {
    system("cls");
    if(game_won == 1) {
        printf("           ________________\n"
               "      .---'::'             `---.\n"
               "     (::::::'                   )\n"
               "     |`-----.____________.-----'|\n"
               "     |                   :::::::|\n"
               "    .|                    ::::::!-.\n"
               "    \\|                    :::::/|/\n"
               "     |                      ::::|\n"
               "     |   Special Flonk Award :::|\n"
               "     |      for Silliness   ::::|\n"
               "     |                    ::::::|\n"
               "     |                   .::::::|\n"
               "     J                   :::::::F\n"
               "      \\                 :::::::/\n"
               "       `.             .:::::::'\n"
               "         `-._       .::::::-'\n"
               "             |       \"\"\"|\"\n"
               "             |       :::|\n"
               "             F        ::J\n"
               "            /          ::\\\n"
               "       __.-'           :::`-.__\n"
               "      (_                ::::::_)\n"
               "        `\"\"\"--------------\"\"\"'\n");

        if(strlen(username) != 0) {
            printf("\t\t %s \n", username);
        }

        printf("__     ______  _    _  __          _______ _   _   _ \n"
               "\\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | | | |\n"
               " \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| | | |\n"
               "  \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` | | |\n"
               "   | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  | |_|\n"
               "   |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_| (_)\n"
               "\n");

    } else if(game_won == 0) {
        printf("               ...\n"
               "             ;::::;\n"
               "           ;::::; :;\n"
               "         ;:::::'   :;\n"
               "        ;:::::;     ;.\n"
               "       ,:::::'       ;           OOO\\\n"
               "       ::::::;       ;          OOOOO\\\n"
               "       ;:::::;       ;         OOOOOOOO\n"
               "      ,;::::::;     ;'         / OOOOOOO\n"
               "    ;:::::::::`. ,,,;.        /  / DOOOOOO\n"
               "  .';:::::::::::::::::;,     /  /     DOOOO\n"
               " ,::::::;::::::;;;;::::;,   /  /        DOOO\n"
               ";`::::::`'::::::;;;::::: ,#/  /          DOOO\n"
               ":`:::::::`;::::::;;::: ;::#  /            DOOO\n"
               "::`:::::::`;:::::::: ;::::# /              DOO\n"
               "`:`:::::::`;:::::: ;::::::#/               DOO\n"
               " :::`:::::::`;; ;:::::::::##                OO\n"
               " ::::`:::::::`;::::::::;:::#                OO\n"
               " `:::::`::::::::::::;'`:;::#                O\n"
               "  `:::::`::::::::;' /  / `:#\n"
               "   ::::::`:::::;'  /  /   `#\n");

        printf("__     ______  _    _   _      ____   _____ ______ _ \n"
               "\\ \\   / / __ \\| |  | | | |    / __ \\ / ____|  ____| |\n"
               " \\ \\_/ / |  | | |  | | | |   | |  | | (___ | |__  | |\n"
               "  \\   /| |  | | |  | | | |   | |  | |\\___ \\|  __| | |\n"
               "   | | | |__| | |__| | | |___| |__| |____) | |____|_|\n"
               "   |_|  \\____/ \\____/  |______\\____/|_____/|______(_)\n"
               "\n");
    }

    printf("\n\n\tbeliebigen Knopf f%cr die Statistik dr%ccken\n\n", ue, ue);

    getch(); //auf Eingabe des Spielers warten
}

