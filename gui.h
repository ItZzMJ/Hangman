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

#ifndef HANGMAN_GUI_H
#define HANGMAN_GUI_H

void print_progress();
void print_suggestions(int position, char line[5000]);
void print_header();
void set_console_color_active();
void reset_console_color();
void print_gamemode_selection(int selection);
void print_statistic(Statistic statistic);
void print_main_menu(int selection);
void print_highscorelist(Highscore *highscores, int count);
void print_game(int error_count, int input_count);
void print_time_and_tries(int input_count, int error_count);
void print_endscreen(int game_won);

#endif //HANGMAN_GUI_H