/* Filename: scan.c
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This C file contains the definitions for functions
 * necessary for using an arbitrary deterministic finite automaton to scan a
 * formatted string and list the tokens detected in the string as long as all
 * tokens comply with the rules given in the DFA.
 */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "automaton.h"
#include "scan.h"
/*
 * Comments and newlines are ignored using completely separate methods
 * in completely different functions (delComments(), buildAutomaton() and
 * scan()). Rewriting these could increase clarity and improve style.
 */
void scanTokens(char *tokensFileStr, Automaton *automatonPtr, char
     *tokensToPrint)
{
    //shared index incremented by all the scanning functions to step through
    //the buffer
    int tokensFileIndex = 0, tokenFinalStatesArrIndex = 0,
        currState = automatonPtr->initialState;
    bool tokenComplete = false;
    //completely remove comments from the buffer before doing anything else
    //to ensure comments to not interfere with the scanning
    delComments(tokensFileStr);
    int tokenFinalStatesArr[MAX_TOKENS];
    int tokensFileStrLen = strlen(tokensFileStr);
    scan(tokensFileStr, tokensFileStrLen, automatonPtr,
        &tokensFileIndex, &currState, tokenFinalStatesArr,
        &tokenFinalStatesArrIndex, &tokenComplete);
    recognizeTokens(automatonPtr, tokenFinalStatesArr,
        tokenFinalStatesArrIndex, tokensToPrint);
}
//This function is poor style because it has too many parameters. Combine some
//of the parameters together into a data structure, and pass a pointer to an
//instance of one instead of a bunch of separate pointers.
void scan(char *tokensFileStr, int tokensFileStrLen, Automaton *automatonPtr,
    int *tokensFileIndex, int *currStatePtr, int *tokenFinalStatesArr,
    int *tokenFinalStatesArrIndex, bool *tokenComplete)
{
    if (*tokensFileIndex < tokensFileStrLen)
    {
        int nextState = getNextState(automatonPtr, *currStatePtr,
            tokensFileStr[*tokensFileIndex]);
        if (nextState >= 0)
        {
            *tokenComplete = false;
            (*tokensFileIndex)++;
            *currStatePtr = nextState;
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete);
        }
        else if (nextState == -1 && *tokenFinalStatesArrIndex < MAX_TOKENS)
        {
            *tokenComplete = true;
            tokenFinalStatesArr[(*tokenFinalStatesArrIndex)++] = *currStatePtr;
            *currStatePtr = automatonPtr->initialState;
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete);
        }
        /* account for the newline characters (and for text files saved by
         * Windows, carriage return characters...) at the end of text files for
         * DFAs which do not include \n or \r as a token. This also skips
         * extraneous newline characters.
         */
        else if (tokensFileStr[*tokensFileIndex] == '\n' ||
            tokensFileStr[*tokensFileIndex] == '\r')
        {
            (*tokensFileIndex)++;
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete);
        }
        else
        {
            *tokenFinalStatesArr = -2;
            if (!(*tokenFinalStatesArrIndex))
                (*tokenFinalStatesArrIndex)++;
        }
    }
    else if (!(*tokenComplete))
    {
        *tokenFinalStatesArr = -3;
        if (!(*tokenFinalStatesArrIndex))
            (*tokenFinalStatesArrIndex)++;
    }
}

int getNextState(Automaton *automatonPtr, int currState, char transChar)
{
    for (int i = 0; i < automatonPtr->transitionCount; i++)
    {
        if (currState == automatonPtr->transitionArr[i].currentState &&
            transChar == automatonPtr->transitionArr[i].transitionChar)
            return automatonPtr->transitionArr[i].nextState;
    }
    for (int i = 0; i < automatonPtr->finalStateCount; i++)
    {
        if (currState == automatonPtr->finalStateArr[i])
            return -1;
    }
    return -2;
}

void recognizeTokens(Automaton *automatonPtr, int *tokenFinalStatesArr,
    int tokenFinalStatesArrIndex, char *tokensToPrint)
{
    if (*tokenFinalStatesArr >= 0)
    {
        *tokensToPrint = '(';
        int tokensToPrintIndex = 1;
        for (int i = 0; i < tokenFinalStatesArrIndex; i++)
        {
            for(int j = 0; j < automatonPtr->tokenCount; j++)
            {
                if (tokenFinalStatesArr[i] ==
                    automatonPtr->tokenArr[j].finalState &&
                    tokensToPrintIndex < TOKENSTOPRINT_LEN)
                {
                    strcpy(&tokensToPrint[tokensToPrintIndex],
                        automatonPtr->tokenArr[j].tokenStr);
                    tokensToPrintIndex +=
                        strlen(automatonPtr->tokenArr[j].tokenStr);
                    strcpy(&tokensToPrint[tokensToPrintIndex], ", ");
                    tokensToPrintIndex += 2;
                }
            }
        }
        if (tokensToPrintIndex < TOKENSTOPRINT_LEN)
            strcpy(&tokensToPrint[tokensToPrintIndex - 2], ")");
    }
    else
        strcpy(tokensToPrint, "error.");
}
