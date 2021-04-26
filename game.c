/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "game.h"

#define KEY 5

char progress[128];
char used_letters[30];
int time_limit = 0;
clock_t tstart;

//Umlaute
char ae = '\x84';
char oe = '\x94';
char ue = '\x81';
char ss = '\xE1';

//Verschlüsselung der Wörterdatei
void decyrption()
{
   char words;

   //Lesen der Wörterdatei
   FILE *fpdo = fopen("words.txt","r");

   //Schreiben in eine Temporäre Datei
   FILE *fpdotp = fopen("tmp.txt","w");

   while(1)
   {
       words = fgetc(fpdo);
       if(words == EOF)
       {
           break;
       }
       else
       {
           //Verschrieben der Buchstaben in den Wörtern um 5 nach vorne
           words = words+KEY;
           fputc(words,fpdotp);
       }
   }
   fclose(fpdo);
   fclose(fpdotp);

   fpdo=fopen("Words.txt","w");

   fpdotp=fopen("tmp.txt","r");

   while(1)
   {

   words = fgetc(fpdotp);
   if(words == EOF)
   {
       break;
   }
   else
   {
       fputc(words, fpdo);
   }
   }
   fclose(fpdo);
   fclose(fpdotp);
}


//Entschlüsselung der Wörter
void encyrption()
{
 char words;

  //Lesen der Wörterdatei
  FILE *solution_words = fopen("words.txt","r");

   //Schreiben in eine Temporäre Datei
   FILE *temp_words = fopen("tmp.txt","w");

   while(1)
   {
       words = fgetc(solution_words);
       if(words == EOF)
       {
           break;
       }
       else
       {
           //Einzelne Buchstaben werden um 5 Buchstaben nach hinten zurück verschoben
           words = words-KEY;
           fputc(words,temp_words);
       }
   }
   fclose(solution_words);
   fclose(temp_words);

   solution_words=fopen("Words.txt","w");

   temp_words=fopen("tmp.txt","r");

   while(1)
   {

   words = fgetc(temp_words);
   if(words == EOF)
   {
       break;
   }
   else
   {
       fputc(words, solution_words);
   }
   }
   fclose(solution_words);
   fclose(temp_words);
}

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

    time += clock() -tstart;
    time = time/CLOCKS_PER_SEC;

    //richtige Einrückung festlegen um beide Zeiten passend zu positionieren
    if(time < 10) {
        printf("\t\t\t\t Zeit:  %.2lfs\n", time);
    } else {
        printf("\t\t\t\t Zeit: %.2lfs\n", time);
    }


    if(time_limit != 0) {
        time_left = time_limit - time;
        if(time_left < 10 && time > 0) {
            printf("\t\t    verlbeibende Zeit:  %.2lfs\n", time_left);
        } else {
            printf("\t\t    verlbeibende Zeit: %.2lfs\n", time_left);

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
    time += clock() -tstart;
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


Statistic run(char solution[128], char username[], int with_time_limit) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int error_count = 0;
    int input_count = 0;
    double time = 0.0;
    Statistic statistic;
    time_limit = with_time_limit;
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

        //Überprüfe ob Zeitlimit überschritten wurde
        if(is_time_over()) {
            time += clock() -tstart;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu hast verloren! Zeitlimit wurde %cberschritten!\n", ue);
            //printf("\tGebrauchte Zeit: %.2lfs\n", time);
            game_finished = 1;
        }


        //check ob das spiel beendet ist
        if(error_count >= 7) {
            time += clock() -tstart;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu bist Tod! Spiel wird beendet.. \n");
            printf("\tGebrauchte Zeit: %.2lfs\n", time);
            game_finished = 1;

        } else if(is_game_finished()) {
            time += clock() -tstart;
            time = time/CLOCKS_PER_SEC;
            printf("\tDu gewinnst!! Spiel wird beendet..\n");
            printf("\tGebrauchte Zeit: %.2lfs\n", time);
            game_finished = 1;
        }


    } while (game_finished == 0);

    for(int i = 0; i < strlen(username); i++) {
        statistic.username[i] = username[i];
    }
    statistic.username[strlen(username)] = 0;

    statistic.success_count = input_count - error_count;
    statistic.error_count = error_count;
    statistic.time = time;


    return statistic;
}

Statistic run_2player(char solution[], char player1[], char player2[]) {
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
    int winner = 0;
    double time1 = 0.0;
    Statistic player1_statistic;
    Statistic player2_statistic;

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
            winner = 0; //Niemand gewinnt

            game_finished = 1;

        } else if(is_game_finished()) {
            time1 += clock() -tstart;
            time1 = time1/CLOCKS_PER_SEC;
            player1_success_count = player1_input_count - player1_error_count;
            player2_success_count = player2_input_count - player2_error_count;

            if(player1_success_count > player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player1);
                winner = 1; //Spieler 1 gewinnt

            } else if(player1_success_count < player2_success_count) {
                printf("\t%s gewinnt!! Spiel wird beendet..\n", player2);
                winner = 2; //Spieler 2 gewinnt

            } else {
                printf("\tUnentschieden!! Spiel wird beendet..\n");
                winner = 3; //Unentschieden  TODO: gewinner der das wort löst ??
            }

            game_finished = 1;
        }
    } while (game_finished == 0);

//    *statistics[0]->error_count = player1_error_count;
//    *statistics[0]->success_count = player1_success_count;
//    *statistics[0]->time = time1;
//    if(winner == 1) {
//        *statistics[0]->success = 1;
//    } else {
//        *statistics[0]->success = 1;
//    }


//    //Statistik Spieler 1
//    strcpy(player1_statistic.username, player1);
//    //player1_statistic.username = player1;
//    player1_statistic.error_count = player1_error_count;
//    player1_statistic.success_count = player1_input_count - player1_error_count;
//    player1_statistic.time = time1;
//    if(winner == 1) {
//        player1_statistic.success = 1;
//    } else {
//        player1_statistic.success = 0;
//    }
//
//    //Statistik Spieler 2
//    strcpy(player2_statistic.username, player2);
//    //player2_statistic.username = player2;
//    player2_statistic.error_count = player2_error_count;
//    player2_statistic.success_count = player2_input_count - player2_error_count;
//    player2_statistic.time = time1;
//    if(winner == 2) {
//        player2_statistic.success = 1;
//    } else {
//        player2_statistic.success = 0;
//    }
//
//    Statistic statistics[2] = {player1_statistic, player2_statistic};
//    return statistics;

}
