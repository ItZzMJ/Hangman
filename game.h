//
// Created by Jannik Möbius on 15.02.2021.
//

#ifndef HANGMAN_GAME_H
#define HANGMAN_GAME_H

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

char solution[128];
//Umlaute
char ae;
char oe;
char ue;
char ss;

Statistic run(char solution[], char username[],  int with_time_limit);
Statistic * run_2player(char solution[], char player1[], char player2[]);
void load_game(char solution[]);

#endif //HANGMAN_GAME_H
