/* Hangman Kommandozeilen-Spiel Bibliothek fürs Dateisystem
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
#include "game.h"
#include "gui.h"
#include "fileio.h"



//globale Konstanten
#define LENGTH 255
#define WORDS 2048
#define KEY 5



/*
 * Schreibt den eingegebenen Spielernamen in eine .txt Datei
 * Es werden keine doppelten Einträge geschrieben
 */
void write_username_to_file(char playername[128]){
    char line[WORDS];
    bool name_already_exists = false;
    FILE *names_file = fopen("playernames.txt","r");
    while(fgets(line,LENGTH,names_file)) {
        if(strstr(line, playername) != NULL) {
            name_already_exists = true;
        }
    }
    fclose(names_file);
    if(name_already_exists == false) {
        FILE *WriteToFile = fopen("playernames.txt","a");
        fprintf(WriteToFile,"%s",playername);
        fclose(WriteToFile);
    }
}

/*
 * Verschlüsselung der Lösungswörterdatei
 */
void encrypt()
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

    while(1) {
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



/*
 * Entschlüsselung der Lösungswörterdatei
 */
void decrypt()
{
    char words;

    //Lesen der Wörterdatei
    FILE *solution_words = fopen("words.txt","r");

    //Schreiben in eine Temporäre Datei
    FILE *temp_words = fopen("tmp.txt","w");

    while(1)
    {
        words = fgetc(solution_words);

        //überprüfe ob am Ende der Datei
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


/*
 * Wählt ein zufälliges Wort aus der Lösungswortdatei aus und speichert es in einer globalen Variable
 */
void get_solution() {
    char words[WORDS][LENGTH];
    int count = 0;

    //Datei entschlüsseln
    decrypt();

    //Zufallsgenerator
    srand(time(NULL));
    int random_index = rand () % 57;

    FILE *word_list = fopen("words.txt","r");
    char input[57];

    while(fgets(input, 56, word_list))
    {
        sscanf(input, "%s", words[count]);
        count++;
    }

    for(int i = 0; i < strlen(words[random_index]); i++) {
        solution[i] = words[random_index][i];
    }

    fclose(word_list);

    //datei verschlüsseln
    encrypt();
}


/*
 * Zählt die aktuellen Einträge in der Highscoreliste und gibt diese zurück
 */
int get_highscore_count() {
    FILE* file;
    int maxc = 1024;
    char* tmp;
    int id;
    char *column;
    char filepath[18] = "Highscoreliste.csv";
    filepath[18] = '\0';
    char line[maxc];
    char delimiter[] = ";";

    file = fopen(filepath, "r+");

    //Wenn die Datei nicht geöffnet werden konnte gib einen Error aus
    if(file == NULL) {
        perror("Fehler beim Öffnen der Highscoreliste!");
        exit(-1);
    }

    //Lies Datei um die letzte ID zu finden
    while(fgets(line, maxc, file) != NULL) {
        tmp = strdup(line);

        //letzte ID finden
        column = strtok(tmp, delimiter);
        if(strcmp(column, "ID") == 0) {
            id = 1;
        } else {
            id = atoi(column); // atoi => string zu int
        }
    }

    fclose(file);

    return id;
}


/*
 * Speichert Statisken in einer Highscoreliste als CSV-Datei
 */
void save_statistic(Statistic statistic) {
    FILE* file;
    int id = 0;
    char filepath[18] = "Highscoreliste.csv";
    filepath[18] = '\0';
    time_t t;
    struct tm* ts;

    //falls keine Highscoreliste existiert erstelle eine
    if(access(filepath, F_OK) != 0) {
        printf("Es existiert keine Highscoreliste! Neue Liste wird erstellt..\n");

        //Datei erstellen und Überschriften reinschreiben
        file = fopen(filepath, "w+");
        fprintf(file, "ID; name; mode; input_count; success_count; error_count; time; solution; win; date\n");
        fclose(file);
    }

    file = fopen(filepath, "a+");

    //Wenn die Datei nicht geöffnet werden konnte gib einen Error aus
    if(file == NULL) {
        perror("Error opening file!");
        exit(-1);
    }

    //letzte ID herausfinden
    id = get_highscore_count() + 1;

    //aktuelle Zeit und Datum
    t = time(NULL);
    ts = localtime(&t);

    //Daten in die Dateischreiben
    fprintf(file, "%d; %s; %d; %d; %d; %d; %.2lf; %s; %d; %d-%d-%d\n",
            id,
            statistic.username,
            statistic.mode,
            statistic.error_count + statistic.success_count,
            statistic.success_count,
            statistic.error_count,
            statistic.time,
            statistic.solution,
            statistic.success,
            ts->tm_year+1900, ts->tm_mon, ts->tm_mday
    );
    fclose(file);

}



/*
 * Liest die Highscorelisten-datei ein und gibt die Statistiken zurück
 */
Statistic * read_highscorelist() {
    FILE* file;
    int maxc = 1024;
    int entry_count = 0;
    int statistic_index = 0;
    int i,j = 0;
    char* tmp;
    char delimiter[] = ";";
    char filepath[18] = "Highscoreliste.csv";
    filepath[18] = '\0';
    char line[maxc];

    //Anzahl an Einträgen der Highscoreliste
    entry_count = get_highscore_count() - 1;

    //Leeres Statistik-Array in passender Größe erzeugen
    Statistic *statistics = malloc(sizeof(Statistic) * entry_count);

    //Überprüfe ob die Datei existiert
    if(access(filepath, F_OK) != 0) {
        printf("\nFehler! Highscoreliste wurde nicht gefunden");
        exit(-1);
    }

    //Datei öffnen
    file = fopen(filepath, "r+");

    //Wenn die Datei nicht geöffnet werden konnte gib einen Fehler zurück
    if(file == NULL) {
        printf("\n Fehler beim öffnen der Highscoreliste!");
        exit(-2);
    }

    printf("\nStart reading lines\n");

    //Lies Datei
    while(fgets(line, maxc, file) != NULL) {

        tmp = strdup(line);

        //Zeile am trenner zerteilen
        tmp = strtok(tmp, delimiter);

        //Erste Zeile überspringen
        if(strstr(tmp, "ID") != NULL) {
            continue;
        }

        //Spaltenindex zurücksetzen
        i = 0;

        //Zeile durchgehen
        while(tmp != NULL) {
            switch(i){
                case 0: break; // ID

                case 1: //Username im Struct speichern
                    for(j = 0; j < strlen(tmp); j++) {
                        statistics[statistic_index].username[j] = tmp[j];
                    }
                    statistics[statistic_index].username[strlen(tmp)] = '\0';
                    break;

                case 2: //Modus
                    statistics[statistic_index].mode = atoi(tmp); //atoi => str zu int
                    break;

                case 3: break;//Anzahl der Eingaben
                case 4: //Anzahl der richtig geratenen Buchstaben
                    statistics[statistic_index].success_count = atoi(tmp); //atoi => str zu int
                    break;

                case 5: //Anzahl der falsch geratenen Buchstaben
                    statistics[statistic_index].error_count = atoi(tmp);
                    break;

                case 6: //gebrauchte Zeit
                    statistics[statistic_index].time = atof(tmp); //atof => str zu float
                    break;

                case 7: break; //Lösungswort

                case 8: //Spiel gewonnen
                    statistics[statistic_index].success = atoi(tmp);
                    break;

                default: break;
            }

            //Nächste Spalte
            tmp = strtok(NULL, delimiter);
            i++;
        }

        statistic_index++;
    }

    printf("\nreadhighscores finished\n");

    return statistics;

}
