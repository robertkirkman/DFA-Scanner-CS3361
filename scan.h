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
    *tokensToPrint);
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
    int *tokenFinalStatesArrIndex, bool *tokenComplete);
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
int getNextState(Automaton *automatonPtr, int currState, char transChar);
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
void recognizeTokens(Automaton *automatonPtr, int *tokenFinalStatesArr,
    int tokenFinalStatesArrIndex, char *tokensToPrint);
#endif
