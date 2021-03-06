//
// Created by Jannik Möbius, Armin Spöllmann, Mark Goldmann on 15.02.2021.
//

#ifndef HANGMAN_GAME_VARS
#define HANGMAN_GAME_VARS

char progress[128];
char used_letters[30];
int time_limit;

/*
 * Struct für die Statistik
 */
typedef struct
{
    char username[128];
    int success_count;
    int error_count;
    double time;
    int success;
    int mode; //1 = 1 Spieler, 2 = 2 Spieler, 3 = Gegen Zeit
    char solution[128];
} Statistic;

/*
 * Struct zum Ausgeben der Highscoreliste
 */
typedef struct
{
    char username[128];
    int global_intput_count;
    double avg_time;
    int games_won;
    int games_lost;
} Highscore;

char solution[128];
//Umlaute
char ae;
char oe;
char ue;
char ss;

#endif //HANGMAN_GAME_VARS

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

void add_to_used_letters(char letter);
int string_contains_char(char string[], char input_char);
int is_valid_char(char input_char);
char get_char();
void update_progress(char input_char, char solution[]);
int is_game_finished();
void translate_to_progress(char solution[]);
int is_time_over();
Statistic run(char solution[], char username[],  int with_time_limit);
Statistic * run_2player(char solution[], char player1[], char player2[]);

#endif //HANGMAN_GAME_H
