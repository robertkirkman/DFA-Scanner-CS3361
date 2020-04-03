/* Filename: scanner.c
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This program reads two files given as arguments, constructs and
 * stores a representation of an arbitrary deterministic finite automaton,
 * assuming a specific syntax, from the first file, then uses that automaton to
 * scan the second file for tokens defined by the automaton. If only valid
 * tokens are present, their names are printed to the terminal in a specifically
 * formatted list, but if any token is invalid, this program prints only
 * "error.".
 */

#include <stdio.h>
#include <stdlib.h>
#include "automaton.h"
//#include "scan.h"

int main(int argc, char **argv)
{
    FILE *automatonFilePtr, *tokensFilePtr;
    char *automatonFileBuf = NULL;
    int automatonFileBufLen = 0;
    if (argc != 3)
    {
        printf("error: invalid number of arguments.\n");
        return 2;
    }
    automatonFilePtr = fopen(argv[1], "rb");
    if (automatonFilePtr == NULL)
    {
        printf("error: invalid filename in first argument.\n");
        return 3;
    }
    if (fseek(automatonFilePtr, 0, SEEK_END) != 0)
    {
        printf("error: file read error.\n");
        return 4;
    }
    automatonFileBufLen = ftell(automatonFilePtr);
    if (automatonFileBufLen == -1)
    {
        printf("error: file read error.\n");
        return 4;
    }
    rewind(automatonFilePtr);
    automatonFileBuf = malloc(automatonFileBufLen + 1);
    if (automatonFileBuf == NULL)
    {
        printf("error: memory allocation error.\n");
        return 1;
    }
    fread(automatonFileBuf, 1, automatonFileBufLen, automatonFilePtr);
    if (ferror(automatonFilePtr))
    {
        printf("error: file read error.\n");
        free(automatonFileBuf);
        return 4;
    }
    fclose(automatonFilePtr);
    automatonFileBuf[automatonFileBufLen] = '\0';
    Automaton *automatonPtr = newAutomaton();
    if (!automatonPtr)
    {
        printf("error: memory allocation error.\n");
        return 1;
    }
    buildAutomaton(automatonFileBuf, automatonPtr);
    //printf("%s\n", automatonFileBuf);


    free(automatonFileBuf);
    deleteAutomaton(automatonPtr);
    return 0;
}
