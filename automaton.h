/* Filename: automaton.h
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This header file contains the definitons for structs and the
 * function prototypes necessary for parsing a formatted string and
 * constructing and storing a deterministic finite automaton from it.
 */
//preprocessor directives and constants
#ifndef AUTOMATON_H
#define AUTOMATON_H
#define TOKENSTR_MAXLEN 20
#define CHARARR_MAXLEN 100
#define STATEARR_MAXLEN 100
#define TRANSITIONARR_MAXLEN (CHARARR_MAXLEN * STATEARR_MAXLEN)
//struct definitons and typedefs for Automaton data type
typedef struct transitionTripleStruct
{
    int currentState, nextState;
    char transitionChar;
} transitionTriple;
typedef struct tokenPairStruct
{
    int finalState;
    char tokenStr[TOKENSTR_MAXLEN];
} tokenPair;
typedef struct AutomatonStruct
{
    char charArr[CHARARR_MAXLEN];
    int stateArr[STATEARR_MAXLEN], finalStateArr[STATEARR_MAXLEN], charCount,
        stateCount, finalStateCount, initialState, transitionCount, tokenCount;
    transitionTriple transitionArr[TRANSITIONARR_MAXLEN];
    tokenPair tokenArr[STATEARR_MAXLEN];
} Automaton;
//function prototypes
/*
 * Function: Automaton *newAutomaton(void)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure to be allocated
 * Return value: new pointer to the newly allocated automatonPtr
 * Description: This function handles memory allocation for the automatonPtr.
 */
Automaton *newAutomaton(void);
/*
 * Function: void deleteAutomaton(Automaton *automatonPtr)
 * Parameters:
 *     automatonPtr: a pointer to the Automaton data structure to be freed
 * Return value: none.
 * Description: This function handles memory deallocation for the automatonPtr.
 */
void deleteAutomaton(Automaton *automatonPtr);
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
void buildAutomaton(char *automatonFileStr, Automaton *automatonPtr);
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
    Automaton *automatonPtr);
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
    Automaton *automatonPtr);
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
void buildFinalStateArr(char *automatonFileStr,
    int *automatonFileFinalStateIndex, Automaton *automatonPtr);
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
void setInitialState(char *automatonFileStr,
    int *automatonFileInitialStateIndex, Automaton *automatonPtr);
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
    int *automatonFileTransitionsIndex, Automaton *automatonPtr);
/*
 * Function: void buildTransitionTriple(char *transitionTripleStr,
 *     Automaton *automatonPtr)
 * Parameters:
 *     tokenPairStr: a formatted string containing a transition for a DFA.
 *     automatonPtr: a pointer to the Automaton data structure in which to store
 *         the parsed data.
 * Return value: none.
 * Description: This function parses a string of the format (%d,%s,%d) where %d
 * are integers with possibly more than two digits and %s is a string that must
 * consist only of alphabetical characters and stores the parsed data in the
 * data structure pointed to by automatonPtr.
 */
void buildTokenArr(char *automatonFileStr, int *automatonFileTokensIndex,
    Automaton *automatonPtr);
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
void buildTransitionTriple(char *transitionTripleStr, Automaton *automatonPtr);
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
void buildTokenPair(char *tokenPairStr, Automaton *automatonPtr);
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
void getSubStr(char *str, char *subStr, int count, int startIndex);
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
void delSubstr(char *str, int count, int startIndex);
/*
 * Function: void initStr(char *str, int size)
 * Parameters:
 *     str: the string to initialize with NUL chars.
 *     size: the length of the string.
 * Return value: none.
 * Description: This function fills a string with null characters, effectively
 * creating a specialized kind of empty string.
 */
void delComments(char *str);
/*
 * Function: int getDigitCount(int n)
 * Parameters:
 *     n: the base-10 integer of which to compute the number of digits.
 * Return value: the number of digits in n.
 * Description: This function uses log() and floor() from math.h to calculate
 * the number of digits in an integer value, then returns that number as int.
 */
void initStr(char *str, int size);
/*
 * Function: void delComments(char *str)
 * Parameters:
 *     str: the string from which to remove comments.
 * Return value: none.
 * Description: This function removes C89-style comments from strings.
 */
int getDigitCount(int n);
#endif
