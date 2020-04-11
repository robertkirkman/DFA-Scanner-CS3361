/* Filename: automaton.c
 * Author: Robert Kirkman
 * Author of delComments() function: Connor Irvine
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This C file contains the definitions for functions
 * necessary for parsing a formatted string and constructing and storing a
 * deterministic finite automaton from it.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "automaton.h"
//Function definitons for parsing the buffer of the automaton file into the
//Automaton data structure
/*
 * Function: Automaton *newAutomaton(void)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure to be allocated
 * Return value: new pointer to the newly allocated automatonPtr
 * Description: This function handles memory allocation for the automatonPtr.
 */
Automaton *newAutomaton(void)
{
    Automaton *newAutomatonPtr = malloc(sizeof(Automaton));
    return newAutomatonPtr;
}
/*
 * Function: void deleteAutomaton(Automaton *automatonPtr)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure to be freed
 * Return value: none.
 * Description: This function handles memory deallocation for the automatonPtr.
 */
void deleteAutomaton(Automaton *automatonPtr)
{
    free(automatonPtr);
}
/*
 * Function: void buildAutomaton(char *automatonFileStr,
 *     Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a text-based
 *         representation of an arbitrary deterministic finite automaton.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {%c,...}\n{%d,...}
 * \n{%d}\n{%d,...}\n{(%d,%s,%d),...}\n{(%d,%s),...}
 * where %c are characters, %d are integers with possibly more than two digits,
 * and %s are strings consisting only of alphabetical characters and stores the
 * parsed data in the data structure pointed to by automatonPtr.
 */
void buildAutomaton(char *automatonFileStr, Automaton *automatonPtr)
{
    //shared index incremented by all the parsing functions to step through the
    //buffer
    int automatonFileIndex = 0;
    //completely remove comments from the buffer before doing anything else
    //to ensure comments to not interfere with the parsing
    delComments(automatonFileStr);
    //the functions must be called in this order to correctly build the
    //Automaton, since the six data sets they are associated with are given in
    //this order by the syntax shown in the specification.
    buildCharArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildStateArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    setInitialState(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildFinalStateArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildTransitionArr(automatonFileStr, &automatonFileIndex, automatonPtr);
    buildTokenArr(automatonFileStr, &automatonFileIndex, automatonPtr);
}
/* !!!!!
 * FIXME: IMPORTANT: buildTokenPair() and buildTransitionTriple() ONLY support
 * alphabetical characters. The specification states that transition characters
 * may be "all characters that a programmer can use EXCEPT the THREE characters
 * '{’, ’}’ and ’,’." Please rewrite to support this.
 * !!!!!
 */
/*
 * Function: void buildCharArr(char *automatonFileStr,
 *     int *automatonFileCharIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a set of transition
 *         characters for a DFA at the offset indicated by
 *         automatonFileCharIndex.
 *     automatonFileCharIndex: the index of automatonFileStr indicating
 *         the offset at which the set of transition characters begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {%c,...}
 * at the indicated index where %c are characters and stores the parsed data in
 * the data structure pointed to by automatonPtr. It also advances
 * automatonFileCharIndex to the end of the set.
 */
void buildCharArr(char *automatonFileStr, int *automatonFileCharIndex,
    Automaton *automatonPtr)
{
    automatonPtr->charCount = 0;
    int automatonFileStrLen = strlen(automatonFileStr);
    while (automatonFileStr[*automatonFileCharIndex] != '}' &&
        automatonPtr->charCount < CHARARR_MAXLEN && *automatonFileCharIndex <
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
//These functions, like the Automata data structure, only support up to
//STATEARR_MAXLEN states. This is out of specification, which states "Each
//state is a number with possibly more than two digits." Please rewrite.
/*
 * Function: void buildStateArr(char *automatonFileStr,
 *     int *automatonFileStateIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a set of states for
 *         a DFA at the offset indicated by automatonFileStateIndex.
 *     automatonFileStateIndex: the index of automatonFileStr indicating
 *         the offset at which the set of states begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {%d,...}
 * at the indicated index where %d are integers with possibly more than two
 * digits and stores the parsed data in the data structure pointed to by
 * automatonPtr. It also advances automatonFileStateIndex to the end of the set.
 */
void buildStateArr(char *automatonFileStr, int *automatonFileStateIndex,
    Automaton *automatonPtr)
{
    int i = 0, j = 0, automatonFileStrLen = strlen(automatonFileStr),
        stateStrIndex = ++(*automatonFileStateIndex);
    int stateSubStrLen = getDigitCount(STATEARR_MAXLEN);
    char currStateSubStr[stateSubStrLen];
    initStr(currStateSubStr, stateSubStrLen);
    while (automatonFileStr[++(*automatonFileStateIndex)] != '}' &&
        *automatonFileStateIndex < automatonFileStrLen)
        i++;
    (*automatonFileStateIndex)++;
    i = i + 2;
    char stateStr[i + 1];
    getSubStr(automatonFileStr, stateStr, i, stateStrIndex);
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
                if (automatonPtr->stateCount < STATEARR_MAXLEN)
                    automatonPtr->stateArr[(automatonPtr->stateCount)++] =
                        atoi(currStateSubStr);
                else break;
                initStr(currStateSubStr, stateSubStrLen);
            }
        }
    }
}
/*
 * Function: void setInitialState(char *automatonFileStr,
 *     int *automatonFileInitialStateIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing an initial state for
 *         a DFA at the offset indicated by automatonFileFinalStateIndex.
 *     automatonFileInitialStateIndex: the index of automatonFileStr indicating
 *         the offset at which the final state begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {%d}
 * at the indicated index where %d is an integer with possibly more than two
 * digits and stores the parsed data in the data structure pointed to by
 * automatonPtr. It also advances automatonFileInitialStateIndex to the end of
 * the set.
 */
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
/*
 * Function: void buildFinalStateArr(char *automatonFileStr,
 *     int *automatonFileFinalStateIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a set of final states for
 *         a DFA at the offset indicated by automatonFileFinalStateIndex.
 *     automatonFileFinalStateIndex: the index of automatonFileStr indicating
 *         the offset at which the set of FinalStates begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {%d,...}
 * at the indicated index where %d are integers with possibly more than two
 * digits and stores the parsed data in the data structure pointed to by
 * automatonPtr. It also advances automatonFileFinalStateIndex to the end of
 * the set.
 */
void buildFinalStateArr(char *automatonFileStr,
    int *automatonFileFinalStateIndex, Automaton *automatonPtr)
{
    int i = 0, j = 0, automatonFileStrLen = strlen(automatonFileStr),
        finalStateStrIndex = ++(*automatonFileFinalStateIndex);
    int stateSubStrLen = getDigitCount(STATEARR_MAXLEN);
    char currStateSubStr[stateSubStrLen];
    initStr(currStateSubStr, stateSubStrLen);
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
                if (automatonPtr->finalStateCount < STATEARR_MAXLEN)
                    automatonPtr->finalStateArr[
                        (automatonPtr->finalStateCount)++] =
                        atoi(currStateSubStr);
                else break;
                initStr(currStateSubStr, stateSubStrLen);
            }
        }
    }
}
//this function is very similar to buildTokenArr. Please refactor.
/*
 * Function: void buildTransitionArr(char *automatonFileStr,
 *     int *automatonFileTransitionsIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a set of transitions for
 *         a DFA at the offset indicated by automatonFileTransitionsIndex.
 *     automatonFileTransitionsIndex: the index of automatonFileStr indicating
 *         the offset at which the set of transitions begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {(%d,%s,%d),...}
 * at the indicated index where %d are integers with possibly more than two
 * digits and %s are strings that must consist only of alphabetical characters
 * and stores the parsed data in the data structure pointed to by automatonPtr.
 * It also advances automatonFileTransitionsIndex to the end of the set.
 */
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
    int transitionsStrLen = strlen(transitionsStr);
    for (int j = 0; j < transitionsStrLen; j++)
    {
        if (transitionsStr[j] == ')' && automatonPtr->transitionCount <
            TRANSITIONARR_MAXLEN)
        {
            k++;
            char transitionTripleStr[k + 1];
            getSubStr(transitionsStr, transitionTripleStr, k,
                transitionTripleStrIndex);
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
/*
 * Function: void buildTransitionTriple(char *transitionTripleStr,
 *     Automaton *automatonPtr)
 * Parameters:
 *     tokenPairStr: a formatted string containing a transition for a DFA.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string of the format (%d,%c,%d) where %d
 * are integers with possibly more than two digits and %c is a transition
 * character and stores the parsed data in the
 * data structure pointed to by automatonPtr.
 */
void buildTransitionTriple(char *transitionTripleStr,  Automaton *automatonPtr)
{
    int j = 0, transitionTripleStrLen = strlen(transitionTripleStr);
    bool currentStateStored = false;
    int stateSubStrLen = getDigitCount(STATEARR_MAXLEN);
    char currStateSubStr[stateSubStrLen];
    initStr(currStateSubStr, stateSubStrLen);
    for (int i = 0; i <= transitionTripleStrLen; i++)
    {
        if (isdigit(transitionTripleStr[i]) && j < 3)
            currStateSubStr[j++] = transitionTripleStr[i];
        else if (transitionTripleStr[i] == ',' && i > 0)
        {
            j = 0;
            if (currentStateStored)
            {
                automatonPtr->transitionArr[
                    automatonPtr->transitionCount].transitionChar =
                    transitionTripleStr[i - 1];
            }
            else
            {
                automatonPtr->transitionArr[
                    automatonPtr->transitionCount].currentState =
                    atoi(currStateSubStr);
                currentStateStored = true;
            }
            initStr(currStateSubStr, stateSubStrLen);
        }
        else if (transitionTripleStr[i] == ')')
            automatonPtr->transitionArr[
                automatonPtr->transitionCount].nextState =
                atoi(currStateSubStr);
    }
}
//this function is very similar to buildTransitionArr. Please refactor.
/*
 * Function: void buildTokenArr(char *automatonFileStr,
 *     int *automatonFileTokensIndex, Automaton *automatonPtr)
 * Parameters:
 *     automatonFileStr: a formatted string containing a set of token pairs for
 *         a DFA at the offset indicated by automatonFileTokensIndex.
 *     automatonFileTokensIndex: the index of automatonFileStr indicating the
 *         offset at which the set of token pairs begins.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string with the format {(%d,%s),...}
 * at the indicated index where %d are integers with possibly more than two
 * digits and %s are strings that must consist only of alphabetical characters
 * and stores the parsed data in the data structure pointed to by automatonPtr.
 * It also advances automatonFileTokensIndex to the end of the set.
 */
void buildTokenArr(char *automatonFileStr, int *automatonFileTokensIndex,
    Automaton *automatonPtr)
{
    int i = 0, k = 0, tokensStrIndex = 0, automatonFileStrLen =
        strlen(automatonFileStr), tokenPairStrIndex = 0;
    automatonPtr->tokenCount = 0;
    while (automatonFileStr[++(*automatonFileTokensIndex)] != '(' &&
        *automatonFileTokensIndex < automatonFileStrLen) {}
    tokensStrIndex = *automatonFileTokensIndex;
    while (automatonFileStr[++(*automatonFileTokensIndex)] != '}' &&
        *automatonFileTokensIndex < automatonFileStrLen)
        i++;
    (*automatonFileTokensIndex)++;
    i++;
    char tokensStr[i + 1];
    getSubStr(automatonFileStr, tokensStr, i, tokensStrIndex);
    int tokensStrLen = strlen(tokensStr);
    for (int j = 0; j < tokensStrLen; j++)
    {
        if (tokensStr[j] == ')' && automatonPtr->tokenCount <
            TRANSITIONARR_MAXLEN)
        {
            k++;
            char tokenPairStr[k + 1];
            getSubStr(tokensStr, tokenPairStr, k, tokenPairStrIndex);
            buildTokenPair(tokenPairStr, automatonPtr);
            (automatonPtr->tokenCount)++;
        }
        else if (tokensStr[j] == '(')
        {
            tokenPairStrIndex = j;
            k = 1;
        }
        else
            k++;
    }
}
/*
 * Function: void buildTokenPair(char *tokenPairStr, Automaton *automatonPtr)
 * Parameters:
 *     tokenPairStr: a formatted string containing a token pair for a DFA.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string of the format (%d,%s) where %d is
 * an integer with possibly more than two digits and %s is a string that must
 * consist only of alphabetical characters and stores the parsed data in the
 * data structure pointed to by automatonPtr.
 */
void buildTokenPair(char *tokenPairStr, Automaton *automatonPtr)
{
    int j = 0, k = 0, tokenPairStrLen = strlen(tokenPairStr);
    int stateSubStrLen = getDigitCount(STATEARR_MAXLEN);
    char currStateSubStr[stateSubStrLen];
    initStr(currStateSubStr, stateSubStrLen);
    initStr(automatonPtr->tokenArr[automatonPtr->tokenCount].tokenStr,
        TOKENSTR_MAXLEN);
    for (int i = 0; i <= tokenPairStrLen; i++)
    {
        if (isdigit(tokenPairStr[i]) && j < 3)
            currStateSubStr[j++] = tokenPairStr[i];
        else if (tokenPairStr[i] == ',')
        {
            j = 0;
            automatonPtr->tokenArr[automatonPtr->tokenCount].finalState =
                atoi(currStateSubStr);
            initStr(currStateSubStr, stateSubStrLen);
        }
        else if (isalpha(tokenPairStr[i]) && k < TOKENSTR_MAXLEN)
        {
            automatonPtr->tokenArr[automatonPtr->tokenCount].tokenStr[k] =
                tokenPairStr[i];
            k++;
        }
    }
}
//This function is inefficient because it wastes memory, copying more of the
//string than absolutely necessary. Please rewrite
/*
 * Function: void getSubStr(char *str, char *subStr, int count, int startIndex)
 * Parameters:
 *     str: the string to copy a substring from.
 *     subStr: the string in which to store the substring.
 *     count: the length of the substring to copy.
 *     startIndex: the starting index in str from which to copy the substring.
 * Return value: none.
 * Description: This function copies a substring of a given length into another
 * string. It is assumed that the memory allocated for subStr is greater than
 * or equal to count * sizeof(char).
 */
void getSubStr(char *str, char *subStr, int count, int startIndex)
{
    memcpy(subStr, &str[startIndex], count);
    subStr[count] = '\0';
}
/*
 * Function: void delSubStr(char *str, int count, int startIndex)
 * Parameters:
 *     str: the string from which to remove the substring.
 *     count: the number of characters to remove from the string.
 *     startIndex: the starting index of the substring to remove.
 * Return value: none.
 * Description: This function removes a given number of characters from a
 * string, starting from a given index in the string, and shortens the length
 * to account for the removal.
 */
void delSubStr(char *str, int count, int startIndex)
{
    if ((count + startIndex) <= (int)strlen(str))
        strcpy(&str[startIndex], &str[count + startIndex]);
}
/*
 * Function: void initStr(char *str, int size)
 * Parameters:
 *     str: the string to initialize with NUL chars.
 *     size: the length of the string.
 * Return value: none.
 * Description: This function fills a string with null characters, effectively
 * creating a specialized kind of empty string.
 */
void initStr(char *str, int size)
{
    for (int i = 0; i < size; i++)
        str[i] = '\0';
}
//with the right technique, it might be possible to replace this function
//with a preprocessor macro, to optimize runtime efficiency and eliminate
//the need for math.h.
/*
 * Function: int getDigitCount(int n)
 * Parameters:
 *     n: the base-10 integer of which to compute the number of digits.
 * Return value: the number of digits in n.
 * Description: This function uses log() and floor() from math.h to calculate
 * the number of digits in an integer value, then returns that number as int.
 */
int getDigitCount(int n)
{
    return floor(log(n) / log(10)) + 1;
}
//This function is inefficient because it iterates through the same characters
//repeatedly. It is also poor style because it breaks a for loop. Please rewrite
//based on pseudocode by Connor Irvine
/*
 * Function: void delComments(char *str)
 * Parameters:
 *     str: the string from which to remove comments.
 * Return value: none.
 * Description: This function removes C89-style comments from strings.
 */
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
                delSubStr(str, j, commentIndex);
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
