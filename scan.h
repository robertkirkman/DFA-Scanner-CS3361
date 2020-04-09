/* Filename: scan.h
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This C file contains the declarations for functions
 * necessary for using an arbitrary deterministic finite automaton to scan a
 * formatted string and list the tokens detected in the string as long as all
 * tokens comply with the rules given in the DFA.
 */
//preprocessor directives and constants
#ifndef SCAN_H
#define SCAN_H
#include <stdbool.h>
#include "automaton.h"
#define MAX_TOKENS 100
#define TOKENSTOPRINT_LEN (3 * MAX_TOKENS + 2)
//function prototypes
void scanTokens(char *tokensFileStr, Automaton *automatonPtr, char
    *tokensToPrint);
void scan(char *tokensFileStr, int tokensFileStrLen, Automaton *automatonPtr,
    int *tokensFileIndex, int *currStatePtr, int *tokenFinalStatesArr,
    int *tokenFinalStatesArrIndex, bool *tokenComplete);
int getNextState(Automaton *automatonPtr, int currState, char transChar);
void recognizeTokens(Automaton *automatonPtr, int *tokenFinalStatesArr,
    int tokenFinalStatesArrIndex, char *tokensToPrint);
#endif
