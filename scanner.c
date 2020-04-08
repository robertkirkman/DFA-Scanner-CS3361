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

int initFileBuf(FILE *filePtr, char **fileBufPtr, char *filename);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("error: invalid number of arguments.\n");
        return 2;
    }
    FILE *automatonFilePtr = NULL, *tokensFilePtr = NULL;
    char *automatonFileBuf = NULL, *tokensFileBuf = NULL;
    int automatonFileErr = initFileBuf(automatonFilePtr, &automatonFileBuf,
        argv[1]);
    if (automatonFileErr)
        return automatonFileErr;
    int tokensFileErr = initFileBuf(tokensFilePtr, &tokensFileBuf,
        argv[2]);
    if (tokensFileErr)
        return tokensFileErr;
    Automaton *automatonPtr = newAutomaton();
    if (!automatonPtr)
    {
        printf("error: memory allocation error.\n");
        return 1;
    }
    buildAutomaton(automatonFileBuf, automatonPtr);
    free(automatonFileBuf);
    free(tokensFileBuf);
    //char tokensToPrint[TOKENSTOPRINT_LEN] = "";
    //printf("%s\n", automatonFileBuf);
    //scanTokens(tokensFileBuf, automatonPtr, &tokensToPrint[0]);
    //printf("%s\n", tokensToPrint);

    deleteAutomaton(automatonPtr);
    return 0;
}
//this function induces UB included as a cheap hack for cross-platform file I/O.
//Rewrite.
int initFileBuf(FILE *filePtr, char **fileBufPtr, char *filename)
{
    int fileBufLen = 0;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL)
    {
        printf("error: invalid filename in first argument.\n");
        return 3;
    }
    //this line is UNDEFINED BEHAVIOR for BINARY STREAMS in the C Standard Spec
    if (fseek(filePtr, 0, SEEK_END))
    {
        printf("error: file read error.\n");
        return 4;
    }
    fileBufLen = ftell(filePtr);
    if (fileBufLen == -1)
    {
        printf("error: file read error.\n");
        return 4;
    }
    rewind(filePtr);
    *fileBufPtr = malloc(fileBufLen + 1);
    if (*fileBufPtr == NULL)
    {
        printf("error: memory allocation error.\n");
        return 1;
    }
    fread(*fileBufPtr, 1, fileBufLen, filePtr);
    if (ferror(filePtr))
    {
        printf("error: file read error.\n");
        free(*fileBufPtr);
        return 4;
    }
    fclose(filePtr);
    (*fileBufPtr)[fileBufLen] = '\0';
    return 0;
}
