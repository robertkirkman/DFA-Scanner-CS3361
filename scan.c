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
#include <ctype.h>
#include "automaton.h"
#include "scan.h"
/*
 * Comments and newlines are ignored using completely separate methods
 * in completely different functions (delComments(), buildAutomaton() and
 * scan()). Rewriting these could increase clarity and improve style.
 */
/*
 * Function: void scanTokens(char *tokensFileStr, Automaton *automatonPtr,
 *     char *tokensToPrint)
 * Parameters:
 *     tokensFileStr: the string containg the tokens for scanning.
 *     automatonPtr: a pointer to the Automaton data structure containing the
 *         DFA to use as a ruleset for possible tokens.
 *     tokensToPrint: a string initialized using the initStr() function that
 *         the string of token names to print will be stored in.
 * Return value: none.
 * Description: This function scans a string containing tokens compatible with
 * the DFA stored at the data structure pointed to by automatonPtr. If only
 * valid tokens are present in the string, then it stores a list of the token
 * names with the format (%s, ...) where %s are token names in order as
 * detected in the source file. If any token is invalid or no valid tokens are
 * present, it stores the string "error."
 */
void scanTokens(char *tokensFileStr, Automaton *automatonPtr, char
     detectedTokensArr[MAX_TOKENS][TOKENCONTENT_LEN], int *detectedTokensCount)
{
    //shared index incremented by all the scanning functions to step through
    //the buffer
    int tokensFileIndex = 0, tokenFinalStatesArrIndex = 0,
        tokenContentIndex = -1, tokenContentStrLen = 0,
        currState = automatonPtr->initialState;
    bool tokenComplete = false;
    //completely remove comments from the buffer before doing anything else
    //to ensure comments to not interfere with the scanning
    delComments(tokensFileStr);
    int tokenFinalStatesArr[MAX_TOKENS];
    int tokensFileStrLen = strlen(tokensFileStr);
    scan(tokensFileStr, tokensFileStrLen, automatonPtr,
        &tokensFileIndex, &currState, tokenFinalStatesArr,
        &tokenFinalStatesArrIndex, &tokenComplete, &tokenContentIndex,
        &tokenContentStrLen);
    recognizeTokens(automatonPtr, tokenFinalStatesArr,
        tokenFinalStatesArrIndex, detectedTokensArr, detectedTokensCount,
        tokensFileStr, tokensFileStrLen);
}
//This function is poor style because it has too many parameters. Combine some
//of the parameters together into a data structure, and pass a pointer to an
//instance of one instead of a bunch of separate pointers.
/*
 * FIXME: scan() provides information about the tokens by populating the array
 * tokenFinalStatesArr. According to the specifcation, the output of
 * scan must be one token instead of a sequence of tokens. Please refactor,
 * for example, into two functions, one which satisfies the requirements of the
 * specification for the function scan(), and one perhaps named scanDriver() to
 * manipulate that function into the existing structure of this application.
 */
/*
 * Function: void scan(char *tokensFileStr, int tokensFileStrLen,
 *     Automaton *automatonPtr, int *tokensFileIndex, int *currStatePtr,
 *     int *tokenFinalStatesArr, int *tokenFinalStatesArrIndex,
 *     bool *tokenComplete)
 * Parameters:
 *     tokensFileStr: the string containg the tokens for scanning.
 *     tokensFileStrLen: the length of tokensFileStr
 *     automatonPtr: a pointer to the Automaton data structure containing the
 *         DFA to use as a ruleset for possible tokens.
 *     tokensFileIndex: the index of the token currently being scanned in
 *         tokensFileStr, at the location within the token currently being
 *         scanned.
 *     currStatePtr: a pointer to the current state of the DFA specified at
 *         automatonPtr as determined by the transition rules.
 *     tokenFinalStatesArr: an array of final states corresponding to their
 *         respective tokens. If any token before the final token is invalid,
 *         the first element will contain -2. If the last token is invalid,
 *         the first element will contain -3.
 *     tokenFinalStatesArrIndex: the index in tokenFinalStatesArr immediately
 *         after the last token identified in tokensFileStr. This is also equal
 *         to the number of tokens that have been identified so far.
 *     tokenComplete: a boolean value indicating whether a token is currently
 *         being scanned, or is complete.
 * Return value: none.
 * Description: This function scans a string containing tokens compatible with
 * the DFA stored at the data structure pointed to by automatonPtr. If only
 * valid tokens are present in the string, then it stores the final states
 * associated with the respective tokens at automatonPtr->tokensArr in order as
 * detected in tokensFileStr. If any token is invalid or no valid tokens are
 * present, it stores a negative value in the first element of
 * tokenFinalStatesArr.
 */
void scan(char *tokensFileStr, int tokensFileStrLen, Automaton *automatonPtr,
    int *tokensFileIndex, int *currStatePtr, int *tokenFinalStatesArr,
    int *tokenFinalStatesArrIndex, bool *tokenComplete, int *tokenContentIndex,
    int *tokenContentStrLen)
{
    if (*tokensFileIndex < tokensFileStrLen)
    {
        int nextState = getNextState(automatonPtr, *currStatePtr,
            tokensFileStr[*tokensFileIndex], *tokensFileIndex,
            tokenContentIndex, tokenContentStrLen);
        if (nextState >= 0)
        {
            *tokenComplete = false;
            (*tokensFileIndex)++;
            *currStatePtr = nextState;
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete, tokenContentIndex,
                tokenContentStrLen);
        }
        else if (nextState == -1 && *tokenFinalStatesArrIndex < MAX_TOKENS - 2)
        {
            *tokenComplete = true;
            tokenFinalStatesArr[(*tokenFinalStatesArrIndex)++] = *currStatePtr;
            *currStatePtr = automatonPtr->initialState;
            if (*tokenContentIndex >= 0)
            {
                tokenFinalStatesArr[(*tokenFinalStatesArrIndex)++] =
                    *tokenContentStrLen;
                tokenFinalStatesArr[(*tokenFinalStatesArrIndex)++] =
                    *tokenContentIndex;
                *tokenContentIndex = -1;
                *tokenContentStrLen = 0;
            }
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete, tokenContentIndex,
                tokenContentStrLen);
        }
        /*
         * Account for the newline characters (and for text files saved by
         * Windows, carriage return characters...) at the end of text files for
         * DFAs which do not include \n or \r as a token. This also skips
         * extraneous newline characters.
         * NOTE: As the scanner is written currently, for whitespace characters
         * to be accepted, they must be accounted for in the DFA at automatonPtr
         */
        else if (tokensFileStr[*tokensFileIndex] == '\n' ||
            tokensFileStr[*tokensFileIndex] == '\r')
        {
            (*tokensFileIndex)++;
            scan(tokensFileStr, tokensFileStrLen, automatonPtr,
                tokensFileIndex, currStatePtr, tokenFinalStatesArr,
                tokenFinalStatesArrIndex, tokenComplete, tokenContentIndex,
                tokenContentStrLen);
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
/*
 * Function: int getNextState(Automaton *automatonPtr, int currState,
 *     char transChar)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure containing the
 *         DFA to use as a ruleset for possible tokens.
 *     currState: the current state in the DFA from which to check for next
 *         states to which to transition.
 *     transChar: the current character being scanned for transitions from
 *         the currState it matches.
 * Return value: an integer equal to the next state detected given currState.
 *     If no valid next state exists and the current state is a final state,
 *     returns -1, but if it is not a final state, returns -2.
 * Description: This function takes a current state and transition character
 * and checks automatonPtr->transitionArr for a transition to a next state
 * matching these two parameters, and returns the next state if one exists.
 */
int getNextState(Automaton *automatonPtr, int currState, char transChar,
    int tokensFileIndex, int *tokenContentIndex, int *tokenContentStrLen)
{
    for (int i = 0; i < automatonPtr->transitionCount; i++)
    {
        //return the next state if the character matches or the requirements
        //of one of the special wildcard transition characters are fulfilled
        if (currState == automatonPtr->transitionArr[i].currentState)
        {
            if (transChar == automatonPtr->transitionArr[i].transitionChar &&
                transChar != '~' && transChar != '`' && transChar != '&')
                return automatonPtr->transitionArr[i].nextState;
            if ((automatonPtr->transitionArr[i].transitionChar == '~' &&
                isdigit(transChar)) ||
                (automatonPtr->transitionArr[i].transitionChar == '`' &&
                isalpha(transChar)) ||
                (automatonPtr->transitionArr[i].transitionChar == '&' &&
                isalnum(transChar)) || transChar == '.')
            {
                if (*tokenContentIndex < 0)
                    *tokenContentIndex = tokensFileIndex;
                (*tokenContentStrLen)++;
                return automatonPtr->transitionArr[i].nextState;
            }
        }
    }
    for (int i = 0; i < automatonPtr->finalStateCount; i++)
    {
        if (currState == automatonPtr->finalStateArr[i])
            return -1;
    }
    return -2;
}
/*
 * Function: void recognizeTokens(Automaton *automatonPtr,
 *     int *tokenFinalStatesArr, int tokenFinalStatesArrIndex,
 *     char *tokensToPrint)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure containing the
 *         DFA to use as a ruleset for possible tokens.
 *     tokenFinalStatesArr: an array of final states corresponding to their
 *         respective tokens. If any token before the final token is invalid,
 *         the first element will contain -2. If the last token is invalid,
 *         the first element will contain -3.
 *     tokenFinalStatesArrIndex: the index in tokenFinalStatesArr immediately
 *         after the last token identified in tokensFileStr. This is also equal
 *         to the number of tokens that have been identified so far.
 * Return value: none.
 * Description: This function takes an array of final states and matches them
 * to token names existing for those states in automatonPtr->tokenArr. It
 * populates the tokensToPrint string with a list of these token
 * names with the format (%s, ...) where %s are token names in order as
 * detected in the source file. If any token is invalid or no valid tokens are
 * present, it stores the string "error."
 */
/*
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
            strcpy(&tokensToPrint[tokensToPrintIndex - 2], ").");
    }
    else
        strcpy(tokensToPrint, "error.");
}
*/
void recognizeTokens(Automaton *automatonPtr, int *tokenFinalStatesArr,
    int tokenFinalStatesArrIndex,
    char detectedTokensArr[MAX_TOKENS][TOKENCONTENT_LEN],
    int *detectedTokensCount, char *tokensFileStr, int tokensFileStrLen)
{
    if (*tokenFinalStatesArr >= 0)
    {
        int i = 0;
        while (i < tokenFinalStatesArrIndex)
        {
            for(int j = 0; j < automatonPtr->tokenCount; j++)
            {
                if (tokenFinalStatesArr[i] ==
                    automatonPtr->tokenArr[j].finalState &&
                    *detectedTokensCount < MAX_TOKENS - 1)
                {
                    strcpy(detectedTokensArr[(*detectedTokensCount)++],
                        automatonPtr->tokenArr[j].tokenStr);
                    if (strcmp(detectedTokensArr[*detectedTokensCount - 1],
                        "id") == 0 ||
                        strcmp(detectedTokensArr[*detectedTokensCount - 1],
                        "number") == 0)
                    {
                        char tokenContentStr[TOKENCONTENT_LEN];
                        initStr(tokenContentStr, TOKENCONTENT_LEN);
                        if (tokenFinalStatesArr[i + 1] +
                            tokenFinalStatesArr[i + 2] < tokensFileStrLen)
                        {
                            getSubStr(tokensFileStr, tokenContentStr,
                                tokenFinalStatesArr[i + 1],
                                tokenFinalStatesArr[i + 2]);
                            i += 2;
                            strcpy(detectedTokensArr[(*detectedTokensCount)++],
                            tokenContentStr);
                        }
                    }
                }
            }
            i++;
        }
    }
    else
        strcpy(detectedTokensArr[0], "Error.");
}
