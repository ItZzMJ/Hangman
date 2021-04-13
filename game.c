/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char progress[128];
char used_letters[30];
clock_t tstart;

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


int is_valid_char(char input_char) {

    if(input_char < 65) {
        return 0; // 0 = false, 1 = true
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
    double time = 0.0;
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

    printf("****************** Hangman ******************\n");

    time += clock() -tstart;
    time = time/CLOCKS_PER_SEC;
    printf("\t\t\t\t Zeit: %.2lfs\n", time);

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


int run(char solution[]) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int error_count = 0;
    int input_count = 0;
    double time1 = 0.0;
    tstart = clock(); //Zeitmessung starten

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


        //check ob das spiel beendet ist
        if(error_count >= 7) {
            time1 += clock() -tstart;
            time1 = time1/CLOCKS_PER_SEC;
            printf("\tDu bist Tod! Spiel wird beendet.. \n");
            printf("\tGebrauchte Zeit: %.2lfs\n", time1);
            game_finished = 1;

        } else if(is_game_finished()) {
            time1 += clock() -tstart;
            time1 = time1/CLOCKS_PER_SEC;
            printf("\tDu gewinnst!! Spiel wird beendet..\n");
            printf("\tGebrauchte Zeit: %.2lfs\n", time1);
            game_finished = 1;
        }


    } while (game_finished == 0);


    return 0;
}

int run_2player(char solution[], char player1[], char player2[]) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int player1_error_count = 0;
    int player2_error_count = 0;
    int error_count = 0;
    int player1_input_count = 0;
    int player2_input_count = 0;
    int input_count = 0;
    int player1_success_count = 0;
    int player2_success_count = 0;
    int active_player = 1;
    double time1 = 0.0;
    tstart = clock(); //Zeitmessung starten

    //Lösungswort in Unterstriche übersetzen
    translate_to_progress(solution);

    //Spielfeld ausgeben
    print_game(error_count);

    do {
        // Buchstabe eingeben
        printf("\t");
        if(active_player == 1) {
            printf("%s", player1);
            player1_input_count++;
        } else {
            printf("%s", player2);
            player2_input_count++;
        }

        printf(" Buchstaben eingeben: ");
        input_char = tolower(get_char());

        //Anzahl der Züge erhöhen
        input_count++;

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

        //Spiel layout anhand des Buchstabens ändern
        print_game(error_count);


        //check ob das spiel beendet ist
        if(error_count >= 7) {
            time1 += clock() -tstart;
            time1 = time1/CLOCKS_PER_SEC;


            printf("\tIhr seid Tod! Spiel wird beendet.. \n");

            game_finished = 1;

        } else if(is_game_finished()) {
            time1 += clock() -tstart;
            time1 = time1/CLOCKS_PER_SEC;
            player1_success_count = player1_input_count - player1_error_count;
            player2_success_count = player2_input_count - player2_error_count;

            if(player1_success_count > player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player1);
            } else if(player1_success_count < player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player2);
            } else {
                printf("\tUnentschieden!! Spiel wird beendet..\n");
            }

            game_finished = 1;
        }
    } while (game_finished == 0);

    //Statistik
    printf("\tGebrauchte Zeit: %.2lfs\n\n", time1);

    printf("\tSpielerstatistik: \n");
    printf("\t%s:\n", player1);
    printf("\tFalsche Buchstaben: %d\n", player1_error_count);
    printf("\tRichtige Buchstaben: %d\n\n", player1_input_count - player1_error_count);

    printf("\t%s:\n", player2);
    printf("\tFalsche Buchstaben: %d\n", player2_error_count);
    printf("\tRichtige Buchstaben: %d\n", player2_input_count - player2_error_count);

}
