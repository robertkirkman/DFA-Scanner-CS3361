/* Filename: automaton.h
 * Author: Robert Kirkman
 * Class: CS 3361-001
 * Professor: Dr. Yuanlin Zhang
 * Assignment: Project 1
 * Description: This header file contains the declarations for structs and
 * functions necessary for parsing a formatted text file and constructing and
 * storing a deterministic finite automaton from it.
 */

#define TOKENSTOPRINT_LEN 100

#define MOVE 1
#define RECOGNIZE 2
#define STUCK 3

struct transitionTripleStruct;
typedef struct transitionTripleStruct transitionTriple;
struct tokenPairStruct;
typedef struct tokenPairStruct tokenPair;
struct AutomatonStruct;
typedef struct AutomatonStruct Automaton;

Automaton *newAutomaton(void);
void deleteAutomaton(Automaton *automatonPtr);
void buildAutomaton(char *automatonFileStr, Automaton *automatonPtr);
void buildCharArr(char *automatonFileStr, int *automatonFileCharIndex,
    Automaton *automatonPtr);
void buildStateArr(char *automatonFileStr, int *automatonFileStateIndex,
    Automaton *automatonPtr);
void buildFinalStateArr(char *automatonFileStr,
    int *automatonFileFinalStateIndex, Automaton *automatonPtr);
void setInitialState(char *automatonFileStr,
    int *automatonFileInitialStateIndex, Automaton *automatonPtr);
void buildTransitionArr(char *automatonFileStr,
    int *automatonFileTransitionsIndex, Automaton *automatonPtr);
void buildTokenArr(char *automatonFileStr, int *automatonFileTokensIndex,
    Automaton *automatonPtr);
void buildTransitionTriple(char *transitionTripleStr, Automaton *automatonPtr);
void buildTokenPair(char *tokenPairStr, Automaton *automatonPtr);
void getSubStr(char *str, char *subStr, int count, int startIndex);
void delSubstr(char *str, int count, int startIndex);
void delComments(char *str);
