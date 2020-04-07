/* Filename: automaton.c
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This C file contains the definitions for structs and functions
 * necessary for parsing a formatted text file and constructing and storing a
 * deterministic finite automaton from it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "automaton.h"

struct transitionTripleStruct
{
    int currentState, nextState;
    char transitionChar;
};
struct tokenPairStruct
{
    int finalState;
    char tokenStr[20];
};
struct AutomatonStruct
{
    char charArr[20];
    int stateArr[20], finalStateArr[20], charCount, stateCount, finalStateCount,
        initialState, transitionCount, tokenCount;
    transitionTriple transitionArr[400];
    tokenPair tokenArr[20];
};
Automaton *newAutomaton(void)
{
    Automaton *newAutomatonPtr = malloc(sizeof(Automaton));
    return newAutomatonPtr;
}
void deleteAutomaton(Automaton *automatonPtr)
{
    free(automatonPtr);
}
void buildAutomaton(char *automatonFileStr, Automaton *automatonPtr)
{
    int automatonFileIndex = 0;
    delComments(automatonFileStr);
    buildCharArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildStateArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    setInitialState(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildFinalStateArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildTransitionArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildTokenArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    /*for (int i = 0; i < automatonPtr->charCount; i++)
        printf("%c ", (automatonPtr->charArr)[i]);
    //printf("\n%d %d\n", automatonPtr->charCount, automatonPtr->stateCount);
    for (int i = 0; i < automatonPtr->stateCount; i++)
        printf("%d ", (automatonPtr->stateArr)[i]);
    printf(". %d . ", automatonPtr->initialState);
    for (int i = 0; i < automatonPtr->finalStateCount; i++)
        printf("%d ", (automatonPtr->finalStateArr)[i]);*/
    for (int i = 0; i < automatonPtr->transitionCount; i++)
        printf("\n%d %c %d", automatonPtr->transitionArr[i].currentState,
                            automatonPtr->transitionArr[i].transitionChar,
                            automatonPtr->transitionArr[i].nextState);
}
void buildCharArr(char *automatonFileStr, int *automatonFileCharIndex,
    Automaton *automatonPtr)
{
    automatonPtr->charCount = 0;
    int automatonFileStrLen = strlen(automatonFileStr);
    while (automatonFileStr[*automatonFileCharIndex] != '}' &&
        automatonPtr->charCount < 20 && *automatonFileCharIndex <
        automatonFileStrLen)
    {
        if (automatonFileStr[*automatonFileCharIndex] == '{' ||
            automatonFileStr[*automatonFileCharIndex] == ',')
            (automatonPtr->charArr)[(automatonPtr->charCount)++] =
            automatonFileStr[++(*automatonFileCharIndex)];
        else
            (*automatonFileCharIndex)++;
    }
    (*automatonFileCharIndex)++;
}
//These functions, like the Automata data structure, only support up to 20
//states.
//This is out of specification, which states "Each state is a number with
//possibly more than two digits." Please rewrite.
void buildStateArr(char *automatonFileStr, int *automatonFileStateIndex,
    Automaton *automatonPtr)
{
    int i = 0, j = 0, automatonFileStrLen = strlen(automatonFileStr),
        stateStrIndex = ++(*automatonFileStateIndex);
    char currStateSubStr[3] = {'\0', '\0', '\0'};
    while (automatonFileStr[++(*automatonFileStateIndex)] != '}' &&
        *automatonFileStateIndex < automatonFileStrLen)
        i++;
    (*automatonFileStateIndex)++;
    i = i + 2;
    char stateStr[i + 1];
    getSubStr(automatonFileStr, stateStr, i, stateStrIndex);
    //printf("%d: %s\n", i, stateStr);
    automatonPtr->stateCount = 0;
    //place within separate function? (see buildFinalStateArr())
    for (int k = 0; k <= i; k++)
    {
        if (stateStr[k] != '{' && stateStr[k] != '\n' && stateStr[k] != ' ')
        {
            if (stateStr[k] != ',' && stateStr[k] != '}' && j < 3)
                currStateSubStr[j++] = stateStr[k];
            else
            {
                j = 0;
                if (automatonPtr->stateCount < 20)
                    automatonPtr->stateArr[(automatonPtr->stateCount)++] =
                        atoi(currStateSubStr);
                else break;
                //printf("%d \n", automatonPtr->stateCount);
                char resetStateSubStr[3] = {'\0', '\0', '\0'};
                memcpy(currStateSubStr, resetStateSubStr,
                    sizeof(resetStateSubStr));
            }
        }
    }
}
void setInitialState(char *automatonFileStr,
    int *automatonFileInitialStateIndex, Automaton *automatonPtr)
{
    int i = 0, initialStateStrIndex = ++(*automatonFileInitialStateIndex),
        automatonFileStrlen = strlen(automatonFileStr);
    while (automatonFileStr[++(*automatonFileInitialStateIndex)] != '}' &&
        *automatonFileInitialStateIndex < automatonFileStrlen)
        i++;
    i++;
    char initialStateStr[i + 1];
    getSubStr(automatonFileStr, initialStateStr, i, initialStateStrIndex);
    if (initialStateStr[0] == '{')
        initialStateStr[0] = '0';
    automatonPtr->initialState = atoi(initialStateStr);
}
//This function is mostly identical to buildStateArr(). Please refactor.
void buildFinalStateArr(char *automatonFileStr,
    int *automatonFileFinalStateIndex, Automaton *automatonPtr)
{
    int i = 0, j = 0, automatonFileStrLen = strlen(automatonFileStr),
        finalStateStrIndex = ++(*automatonFileFinalStateIndex);
    char currStateSubStr[3] = {'\0', '\0', '\0'};
    while (automatonFileStr[++(*automatonFileFinalStateIndex)] != '}' &&
        *automatonFileFinalStateIndex < automatonFileStrLen)
        i++;
    (*automatonFileFinalStateIndex)++;
    i = i + 2;
    char stateStr[i + 1];
    getSubStr(automatonFileStr, stateStr, i, finalStateStrIndex);
    automatonPtr->finalStateCount = 0;
    for (int k = 0; k <= i; k++)
    {
        if (stateStr[k] != '{' && stateStr[k] != '\n' && stateStr[k] != ' ')
        {
            if (stateStr[k] != ',' && stateStr[k] != '}' && j < 3)
                currStateSubStr[j++] = stateStr[k];
            else
            {
                j = 0;
                if (automatonPtr->finalStateCount < 20)
                    automatonPtr->finalStateArr[
                        (automatonPtr->finalStateCount)++] =
                        atoi(currStateSubStr);
                else break;
                char resetStateSubStr[3] = {'\0', '\0', '\0'};
                memcpy(currStateSubStr, resetStateSubStr,
                    sizeof(resetStateSubStr));
            }
        }
    }
}
void buildTransitionArr(char *automatonFileStr,
    int *automatonFileTransitionsIndex, Automaton *automatonPtr)
{
    int i = 0, k = 0, transitionsStrIndex = 0, automatonFileStrLen =
        strlen(automatonFileStr), transitionTripleStrIndex = 0;
    automatonPtr->transitionCount = 0;
    while (automatonFileStr[++(*automatonFileTransitionsIndex)] != '(' &&
        *automatonFileTransitionsIndex < automatonFileStrLen) {}
    transitionsStrIndex = *automatonFileTransitionsIndex;
    while (automatonFileStr[++(*automatonFileTransitionsIndex)] != '}' &&
        *automatonFileTransitionsIndex < automatonFileStrLen)
        i++;
    (*automatonFileTransitionsIndex)++;
    i++;
    char transitionsStr[i + 1];
    getSubStr(automatonFileStr, transitionsStr, i, transitionsStrIndex);
    printf("%s. ", transitionsStr);
    int transitionsStrLen = strlen(transitionsStr);
    for (int j = 0; j < transitionsStrLen; j++)
    {
        if (transitionsStr[j] == ')' && automatonPtr->transitionCount < 400)
        {
            k++;
            char transitionTripleStr[k + 1];
            getSubStr(transitionsStr, transitionTripleStr, k,
                transitionTripleStrIndex);
            printf("%s. ", transitionTripleStr);
            buildTransitionTriple(transitionTripleStr, automatonPtr);
            (automatonPtr->transitionCount)++;
        }
        else if (transitionsStr[j] == '(')
        {
            transitionTripleStrIndex = j;
            k = 1;
        }
        else
            k++;
    }
}
void buildTransitionTriple(char *transitionTripleStr,  Automaton *automatonPtr)
{
    int j = 0, transitionTripleStrLen = strlen(transitionTripleStr);
    bool charStored = false;
    char currStateSubStr[3] = {'\0', '\0', '\0'};
    for (int i = 0; i <= transitionTripleStrLen; i++)
    {
        if (isdigit(transitionTripleStr[i]) && j < 3)
            currStateSubStr[j++] = transitionTripleStr[i];
        else if ((transitionTripleStr[i] == ',' ||
            transitionTripleStr[i] == ')') && i > 0)
        {
            if(!isalpha(transitionTripleStr[i - 1]))
            {
                j = 0;
                if (charStored)
                    automatonPtr->transitionArr[
                        automatonPtr->transitionCount].nextState =
                        atoi(currStateSubStr);
                else
                    automatonPtr->transitionArr[
                        automatonPtr->transitionCount].currentState =
                        atoi(currStateSubStr);
                char resetStateSubStr[3] = {'\0', '\0', '\0'};
                memcpy(currStateSubStr, resetStateSubStr,
                    sizeof(resetStateSubStr));
            }
        }
        else if (isalpha(transitionTripleStr[i]))
        {
            automatonPtr->transitionArr[
                automatonPtr->transitionCount].transitionChar =
                transitionTripleStr[i];
            charStored = true;
        }
    }
}
void buildTokenArr(char *automatonFileStr, int *automatonFileTokensIndex,
    Automaton *automatonPtr)
{
    automatonPtr->tokenCount = 0;

}
void buildTokenPair(char *tokenPairStr, Automaton *automatonPtr)
{

}
//This function is inefficient because it wastes memory, copying more of the
//string than absolutely necessary. Please rewrite
void getSubStr(char *str, char *subStr, int count, int startIndex)
{
    memcpy(subStr, &str[startIndex], count);
    subStr[count] = '\0';
}
void delSubstr(char *str, int count, int startIndex)
{
    if ((count + startIndex) <= strlen(str))
        strcpy(&str[startIndex], &str[count + startIndex]);
}
//This function is inefficient because it iterates through the same characters
//repeatedly. It is also poor style because it breaks a for loop. Please rewrite
void delComments(char *str)
{
    bool isComment = false;
    int commentIndex = 0;
    int j = 0;
    int strLen = strlen(str);
    for (int i = 0; i < strLen - 1; i++)
    {
        if (isComment)
        {
            j++;
            if(str[i] == '*' && str[i + 1] == '/')
    		{
                j++;
                delSubstr(str, j, commentIndex);
                delComments(str);
                break;
            }
        }
        else if (str[i] == '/' && str[i + 1] == '*')
        {
            isComment = true;
            commentIndex = i;
            j++;
        }
    }
}
