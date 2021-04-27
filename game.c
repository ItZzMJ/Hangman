/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"

char progress[128];
char used_letters[30];
int time_limit = 0;
clock_t game_start;
//clock_t tstart;

//Umlaute
char ae = '\x84';
char oe = '\x94';
char ue = '\x81';
char ss = '\xE1';

void print_progress() {
    printf("\n\t\t");
    for(int i = 0; i < strlen(progress); i++) {
        printf("%c ", progress[i]);
    }
    printf("\n\n");
}

int add_to_used_letters(char letter) {
    used_letters[strlen(used_letters)] = letter;
}

int string_contains_char(char string[], char input_char){
    for(int i = 0; i < strlen(string); i++) {
        if(tolower(string[i]) == input_char) {
            return 1; // String enthählt den eingegebenen Buchstaben
        }
    }

    // String enthählt nicht den eingegebenen Buchstaben
    return 0;

}

/*
 * Überprüft ob das gegebene Zeichen ein valider Buchstabe ist
 */
int is_valid_char(char input_char) {
    printf("%c", input_char);

    if(input_char == ae || input_char == oe || input_char == ue || input_char == ss) {
        printf("\nUMLAUT\n");
        return 1;  // 0 = false, 1 = true
    } else if(input_char < 65) {
        return 0;
    } else if(input_char > 90 && input_char < 97) {
        return 0;
    } else if(input_char > 122) {
        return 0;
    } else {
        return 1;
    }
}

char get_char() {
    int success = 0;
    char input_char;


    do {
        success = 0;

        scanf("%c", &input_char);
        fflush(stdin);

        //Überprüfe ob der eingegebene Wert ein Buchstabe ist
        if(!is_valid_char(input_char)) {
            success = 1;
            printf("\tDie Eingabe muss ein Buchstabe sein! Versuch's nochmal: ");

        //Überprüfe ob der Eingegebene Buchstabe bereits eingegeben wurde.
        } else {
            if(string_contains_char(used_letters, input_char) != 0) {
                success = 1;
                printf("\tDieser Buchstabe wurde bereits gew%chlt! Versuch's nochmal: ", ae);
            } else {
                add_to_used_letters(input_char);
            }
        }
    } while(success != 0);

    return input_char;
}


void update_progress(char input_char, char solution[]) {
    for(int i = 0; i < strlen(solution); i++) {
        if (tolower(solution[i]) == input_char) {
            progress[i] = toupper(input_char);
        }
    }
}

int is_game_finished() {
    for(int i = 0; i < strlen(progress); i++) {
        if(progress[i] == '_') {
            return 0; // 0 = false
        }
    }
    return 1; // 1 = true
}

void print_game(error_count) {
    system("cls"); // clear console

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

    print_time();

    printf("%s", hangman[error_count]);

    print_progress();


    // Wenn es bereits benutzte Buchstaben gibt, gib diese aus
    if(strlen(used_letters) > 0) {
        printf("\n\tBereits benutzte Buchstaben:\n\t");
        for(int i = 0; i < strlen(used_letters); i++) {
            if(i == 0) {
                printf("%c", toupper(used_letters[i]));
            } else {
                printf(", %c", toupper(used_letters[i]));
            }
        }
    }
    printf("\n\n");


}

void print_time() {
    double time = 0.0;
    double time_left = 0.0;

    time += clock() - game_start;
    time = time/CLOCKS_PER_SEC;

    //richtige Einrückung festlegen um beide Zeiten passend zu positionieren
    if(time < 10) {
        printf("\t\t\t\t\t Zeit:  %.2lfs\n", time);
    } else {
        printf("\t\t\t\t\t Zeit: %.2lfs\n", time);
    }


    if(time_limit != 0) {
        time_left = time_limit - time;
        if(time_left < 10 && time > 0) {
            printf("\t\t\t    verlbeibende Zeit:  %.2lfs\n", time_left);
        } else {
            printf("\t\t\t    verlbeibende Zeit: %.2lfs\n", time_left);

        }
    }
}

void translate_to_progress(char solution[]) {
    for (int i = 0; i < strlen(solution); i++) {
        if (solution[i] == ' ') {
            progress[i] = ' ';
            printf("Solution == ' '\n");

        } else if (solution[i] == '-') {
            progress[i] = '-';
            printf("Solution == '-'\n");

        } else {
            progress[i] = '_';
        }

    }
}

int is_time_over(){
    double time = 0.0;
    time += clock() -game_start;
    time = time/CLOCKS_PER_SEC;

    //überprüfe ob es ein Zeitlimit gibt
    if(time_limit == 0) {
        return 0;

    } else if(time <= time_limit) {
        return 0;
    } else {
        return 1; // Zeit ist vorbei
    }

}

void print_endscreen(int game_won) {
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

    printf("\n\n\tbeliebigen Knopf dr%ccken\n", ue);
    getch();
}


Statistic run(char solution[128], char *username, int with_time_limit) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int error_count = 0;
    int input_count = 0;
    double time = 0.0;
    int game_won = 0;
    Statistic statistic;

    time_limit = with_time_limit; // Zeitlimit global setzen
    game_start = clock(); //Zeitmessung starten

    //Lösungswort in Unterstriche übersetzen
    translate_to_progress(solution);

    //Spielfeld ausgeben
    print_game(error_count);

    do {
        // Buchstabe eingeben
        printf("\tBuchstaben eingeben: ");
        input_char = tolower(get_char());

        //Anzahl der Züge erhöhen
        input_count++;

        //check ob der Buchstabe im Lösungswort existiert
        if (string_contains_char(solution, input_char)) {
            update_progress(input_char, solution);

        } else {
            printf("\tBuchstabe nicht vorhanden!\n");
            error_count++;
        }

        //Spiel layout anhand des Buchstabens ändern
        print_game(error_count);

        //Überprüfe ob Zeitlimit überschritten wurde
        if(is_time_over()) {
            time += clock() -game_start;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu hast verloren! Zeitlimit wurde %cberschritten!\n", ue);
            game_finished = 1;
        }


        //check ob das spiel beendet ist
        if(error_count >= 7) {
            time += clock() -game_start;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu bist Tod! Spiel wird beendet.. \n");
            game_finished = 1;

        } else if(is_game_finished()) {
            time += clock() -game_start;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu gewinnst!! Spiel wird beendet..\n");
            game_finished = 1;
            game_won = 1;
        }


    } while (game_finished == 0);


    //Statistik speichern und zurückgeben
    for(int i=0; i< strlen(username); i++) {
        statistic.username[i] = username[i];
    }
    statistic.username[strlen(username)] = '\0';

    statistic.success_count = input_count - error_count;
    statistic.error_count = error_count;
    statistic.time = time;
    statistic.success = game_won;
    statistic.mode = 1;

    //Lösungswort für Statistik speichern
    for(int i=0; i< strlen(solution); i++) {
        statistic.solution[i] = solution[i];
    }
    statistic.solution[strlen(solution)] = '\0';


    print_endscreen(game_won);

    return statistic;

}

Statistic * run_2player(char solution[], char player1[], char player2[]) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int i = 0;
    int player1_error_count = 0;
    int player2_error_count = 0;
    int error_count = 0;
    int player1_input_count = 0;
    int player2_input_count = 0;
    int input_count = 0;
    int player1_success_count = 0;
    int player2_success_count = 0;
    int active_player = 1;
    int winner = 0;
    double time = 0.0;
    double time_player1 = 0.0;
    double time_player2 = 0.0;
    Statistic *statistics = malloc(sizeof(Statistic) * 2);
    clock_t tstart; //Startet irgendwie nicht bei 0?


    //Lösungswort in Unterstriche übersetzen
    translate_to_progress(solution);

    game_start = clock(); //globale Zeitmessung starten

    //Spielfeld ausgeben
    print_game(error_count);



    do {
        tstart = clock(); //lokale Spieler Zeitmessung für einen Zug starten

        printf("\t");

        //aktiven Spieler ausgeben und Zug mitzählen
        if(active_player == 1) {
            printf("%s", player1);
            player1_input_count++;
        } else {
            printf("%s", player2);
            player2_input_count++;
        }

        //Buchstabe eingeben
        printf(" Buchstaben eingeben: ");
        input_char = tolower(get_char());


        //check ob der Buchstabe im Lösungswort existiert
        if (string_contains_char(solution, input_char)) {
            update_progress(input_char, solution);
        } else {
            printf("\tBuchstabe nicht vorhanden!\n");
            error_count++;

            //Bei falschem Buchstaben Spielerwechsel
            if(active_player == 1) {
                player1_error_count++;
                active_player = 2;

            } else {
                player2_error_count++;
                active_player = 1;
            }

        }

        //Zeit für den Zug stoppen und speichern
        if(active_player == 1) {
            //Zeit für Spieler 1 anhalten
            time += clock() - tstart;
            time = time/CLOCKS_PER_SEC;
            time_player1 += time;

        } else {
            //Zeit für Spieler 2 anhalten
            time += clock() - tstart;
            time = time/CLOCKS_PER_SEC;
            time_player2 += time;
        }

        //Spiel layout anhand des Buchstabens ändern
        print_game(error_count);

        //check ob das spiel beendet ist
        if(error_count >= 7) {
            time += clock() - tstart;
            time = time/CLOCKS_PER_SEC;

            //Zeit für den entsprechenden Spieler speichern
            if(active_player == 1) {
                time_player1 += time;
            } else {
                time_player2 += time;
            }


            printf("\tIhr seid Tod! Spiel wird beendet.. \n");
            printf("\tDas L%csungswort war '%s'\n", oe, solution);
            winner = 0; //Niemand gewinnt

            game_finished = 1;

        } else if(is_game_finished()) {
            time += clock() -tstart;
            time = time/CLOCKS_PER_SEC;

            //Zeit für den entsprechenden Spieler speichern
            if(active_player == 1) {
                time_player1 += time;
            } else {
                time_player2 += time;
            }

            player1_success_count = player1_input_count - player1_error_count;
            player2_success_count = player2_input_count - player2_error_count;

            //Entscheidung wer gewinnt nach richtrig geratenen Buchstaben
            if(player1_success_count > player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player1);
                winner = 1; //Spieler 1 gewinnt

            } else if(player1_success_count < player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player2);
                winner = 2; //Spieler 2 gewinnt

            } else { //Bei gleicher Punktzahl gewinnt der, der den letzten Buchstaben geraten hat
                if(active_player == 1) {
                    printf("\t%s gewinnt!! Spiel wird beendet..\n", player1);
                    winner = 1; //Spieler 1 gewinnt
                } else {
                    printf("\t%s gewinnt!! Spiel wird beendet..\n", player2);
                    winner = 2; //Spieler 2 gewinnt
                }
            }

            //Spiel ist beendet
            game_finished = 1;
        }
    } while (game_finished == 0);



    //Statistik Spieler 1

    //Username in Statistik-Array schreiben
    for(i = 0; i < strlen(player1); i++) {
        statistics[0].username[i] = player1[i];
    }
    statistics[0].username[strlen(player1)] = '\0';

    statistics[0].error_count = player1_error_count;
    statistics[0].success_count = player1_input_count - player1_error_count;
    statistics[0].time = time_player1;
    statistics[0].mode = 2;

    //Lösungswort für Statistik speichern
    for(i = 0; i < strlen(solution); i++) {
        statistics[0].solution[i] = solution[i];
    }
    statistics[0].solution[strlen(solution)] = '\0';

    //hat Spieler 1 gewonnen?
    if(winner == 1) {
        statistics[0].success = 1;
    } else {
        statistics[0].success = 0;
    }

    //Statistik Spieler 2

    //Username in Statistik-Array schreiben
    for(i = 0; i < strlen(player2); i++) {
        statistics[1].username[i] = player2[i];
    }
    statistics[1].username[strlen(player2)] = '\0';

    statistics[1].error_count = player2_error_count;
    statistics[1].success_count = player2_input_count - player2_error_count;
    statistics[1].time = time_player2;
    statistics[1].mode = 2;

    //Lösungswort für Statistik speichern
    for(i = 0; i < strlen(solution); i++) {
        statistics[1].solution[i] = solution[i];
    }
    statistics[1].solution[strlen(solution)] = '\0';

    //hat Spieler 2 gewonnen?
    if(winner == 2) {
        statistics[1].success = 1;
    } else {
        statistics[1].success = 0;
    }

    return statistics;

}
