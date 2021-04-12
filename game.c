/* Hangman Kommandozeilen-Spiel
 * Author: Jannik Möbius
 * Erstellt am: 12.04.2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char progress[128];
char used_letters[30];

void print_progress() {
    printf("\n\t");
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
                printf("\tDieser Buchstabe wurde bereits gewählt! Versuch's nochmal: ");
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
            "\t  +---+\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t      |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t  |   |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t /|   |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t /|\\  |\n"
            "\t      |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t /|\\  |\n"
            "\t /    |\n"
            "\t      |\n"
            "\t=========\n",
            "\t  +---+\n"
            "\t  |   |\n"
            "\t  o   |\n"
            "\t /|\\  |\n"
            "\t / \\  |\n"
            "\t      |\n"
            "\t=========\n"
    };

    printf("\t*********** Hangman ***********\n\n");

    printf(hangman[error_count]);

    print_progress();


    // Wenn es bereits benutzte Buchstaben gibt, gib diese aus
    if(strlen(used_letters) > 0) {
        printf("\n\tBereits benutzte Buchstaben: ");
        for(int i = 0; i < strlen(used_letters); i++) {
            if(i == 0) {
                printf("%c", used_letters[i]);
            } else {
                printf(", %c", used_letters[i]);
            }
        }
    }
    printf("\n");


}

int run(char solution[]) {
    int game_finished = 0; // ist das Spiel beendet?
    char input_char;
    int error_count = 0;

    for (int i = 0; i < strlen(solution); i++) {
        if (progress[i] != ' ') {
            progress[i] = '_';
        } else {
            progress[i] = ' ';
        }
    }

    print_game(error_count);

    do {
        // Buchstabe eingeben
        printf("\tBuchstaben eingeben: ");
        input_char = tolower(get_char());

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
            printf("\tDu bist Tod! Spiel wird beendet.. \n");
            game_finished = 1;
        } else if(is_game_finished()) {
            printf("\tDu gewinnst!! Spiel wird beendet..\n");
            game_finished = 1;
        }


    } while (game_finished == 0);

    return 0;
}
