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


#ifndef HANGMAN_FILEIO_H
#define HANGMAN_FILEIO_H

void write_username_to_file(char playername[128]);
void encrypt();
void decrypt();
void get_solution();
void save_statistic(Statistic statistic);
int get_highscore_count();
Statistic * read_highscorelist();

#endif //HANGMAN_FILEIO_H
