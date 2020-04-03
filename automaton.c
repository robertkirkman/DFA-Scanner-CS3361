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
    int stateArr[20], finalStateArr[20], stateCount, finalStateCount,
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
    int automatonFileIndex = 0, automatonCharIndex = 0,
        automatonStateIndex = 0, automatonFinalStateIndex = 0, automatonTransitionIndex = 0, automatonTokenIndex = 0;
    delComments(automatonFileStr);
    buildCharArr(automatonFileStr, &automatonFileIndex, automatonPtr,
        &automatonCharIndex);
    buildStateArr(automatonFileStr, &automatonFileIndex, automatonPtr,
        &automatonStateIndex);
    buildFinalStateArr(automatonFileStr, &automatonFileIndex, automatonPtr,
        &automatonFinalStateIndex);
    setInitialState(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildTransitionArr(automatonFileStr, &automatonFileIndex, automatonPtr,
        &automatonTransitionIndex);
    buildTokenArr(automatonFileStr, &automatonFileIndex, automatonPtr,
        &automatonTokenIndex);
    //for (int i = 0; i < 6; i++)
    //    printf("%c ", (automatonPtr->charArr)[i]);
}
void buildCharArr(char *automatonFileStr, int *automatonFileCharIndex,
    Automaton *automatonPtr, int *automatonCharIndex)
{
    int automatonFileStrLen = strlen(automatonFileStr);
    while (automatonFileStr[*automatonFileCharIndex] != '}' &&
        *automatonCharIndex < 20 && *automatonFileCharIndex <
        automatonFileStrLen - 1)
    {
        if (automatonFileStr[*automatonFileCharIndex] == '{' ||
            automatonFileStr[*automatonFileCharIndex] == ',')
            (automatonPtr->charArr)[(*automatonCharIndex)++] =
            automatonFileStr[++(*automatonFileCharIndex)];
        else
            (*automatonFileCharIndex)++;
    }
}
void buildStateArr(char *automatonFileStr, int *automatonFileStateIndex,
    Automaton *automatonPtr, int *automatonStateIndex)
{
    int i = 0, automatonFileStrLen = strlen(automatonFileStr),
        stateStrIndex = ++(*automatonFileStateIndex);
    while (automatonFileStr[++(*automatonFileStateIndex)] != '}')
        i++;
    char automatonStateStr[i+1];
    getSubStr(auto)
}
void buildFinalStateArr(char *automatonFileStr,
    int *automatonFileFinalStateIndex, Automaton *automatonPtr,
    int *automatonFinalStateIndex)
{

}
void setInitialState(char *automatonFileStr,
    int *automatonFileInitialStateIndex, Automaton *automatonPtr)
{

}
void buildTransitionArr(char *automatonFileStr,
    int *automatonFileTransitionsIndex, Automaton *automatonPtr,
    int *automatonTransitionIndex)
{

}
void buildTokenArr(char *automatonFileStr, int *automatonFileTokensIndex,
    Automaton *automatonPtr, int *automatonTokenIndex)
{

}
void buildTransitionTriple(char *automatonFileStr,
    int *automatonFileTripleIndex, Automaton *automatonPtr)
{

}
void buildTokenPair(char *automatonFileStr, int *automatonFileTokenIndex,
    Automaton *automatonPtr)
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
