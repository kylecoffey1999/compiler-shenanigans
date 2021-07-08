#include <queue.h>
#include <sds.h>
#include <set.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef sds String;
typedef size_t Size;

typedef enum
{
    ancNone = 0,
    ancBol = (1 << 0),
    ancEol = (1 << 1),
    ancBoth = ancBol | ancEol,
} NondeterministicFiniteAutomatonAnchor;

typedef enum
{
    edgeEmpty = -3,
    edgeCharacterClass,
    edgeEpsilon,
} NondeterministicFiniteAutomatonTransitionType;

typedef struct _NondeterministicFiniteAutomatonNode
{
    struct _NondeterministicFiniteAutomatonNode *next[2];
    NondeterministicFiniteAutomatonTransitionType transitionType;
    Set *transitionSet;
    NondeterministicFiniteAutomatonAnchor anchor;
} * NondeterministicFiniteAutomatonNode;

typedef enum
{
    nfapecProcessCouldNotAllocateMemory,
    nfapecRegularExpressionWasMalformed,
    nfapecMissingCloseParenthesis,
    nfapecMissingOpenBracketInCharacterClass,
    nfapecEncounteredCaratSymbolInAnInvalidPosition,
    nfapecEncounteredClosureSymbolInAnInvalidPosition,
    nfapecEncounteredNewLineCharacterWithinQuotedString,
    nfapecCount,
} NondeterministicFiniteAutomatonParseErrorCode;

static const char *const nondeterministicFiniteAutomatonParseErrorMessages[nfapecCount] = {
    "the process failed to allocate memory",
    "the regular expression was malformed",
    "a close parenthesis was expected, but it was not present",
    "an open bracket was expected to begin a character class, but it was not present",
    "the parser encountered a carat symbol in an invalid position; a carat symbol represents the beginning of a line "
    "and cannot occur mid-expression",
    "the parser encountered a symbol representing a closure, but it did not follow a valid expression",
    "while parsing a quoted string, a literal newline was encountered in the source text",
};

typedef struct
{
    Queue *nondeterministicFiniteAutomaton;
    NondeterministicFiniteAutomatonParseErrorCode nondeterministicFiniteAutomatonParseErrorCode;
} * NondeterministicFiniteAutomaton;

static NondeterministicFiniteAutomaton performThompsonsConstruction(String input, Size *out_startStateIndex);
static void printNondeterministicFiniteAutomaton(NondeterministicFiniteAutomaton nondeterministicFiniteAutomaton,
                                                 Size startStateIndex);

#ifdef TRACE_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER
static Size nondeterministicFiniteAutomatonParserDepth = 0;
#define PRINT_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_TRACE_INDENT()                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        for (Size nondeterministicFiniteAutomatonParserTraceIndent = 0;                                                \
             nondeterministicFiniteAutomatonParserTraceIndent < nondeterministicFiniteAutomatonParserDepth;            \
             nondeterministicFiniteAutomatonParserTraceIndent++)                                                       \
        {                                                                                                              \
            printf("    ");                                                                                            \
        }                                                                                                              \
    } while (0)
#define ENTER_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_FUNCTION(functionName)                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        PRINT_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_TRACE_INDENT();                                                 \
        nondeterministicFiniteAutomatonParserDepth++;                                                                  \
        printf("enter %s\n", functionName);                                                                            \
    } while (0)
#define LEAVE_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_FUNCTION(functionName)                                          \
    do                                                                                                                 \
    {                                                                                                                  \
        nondeterministicFiniteAutomatonParserDepth--;                                                                  \
        PRINT_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_TRACE_INDENT();                                                 \
        printf("leave %s\n", functionName);                                                                            \
    } while (0)
#else
#define ENTER_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_FUNCTION(unused_functionName)
#define LEAVE_NONDETERMINISTIC_FINITE_AUTOMATON_PARSER_FUNCTION(unused_functionName)
#endif

static void displayNondeterministicFiniteAutomatonParseError(String source,
                                                             NondeterministicFiniteAutomatonParseErrorCode code)
{
    fprintf(stderr, "while attempting to parse '%.*s' as a regular expression, %s.\n", (int)sdslen(source), source,
            nondeterministicFiniteAutomatonParseErrorMessages[code]);
}

typedef struct
{
    String source;
    NondeterministicFiniteAutomaton automaton;
    Queue *discardStack;
} * NondeterministicFiniteAutomatonParserState;

static NondeterministicFiniteAutomatonNode allocateNondeterministicFiniteAutomatonNode(
    NondeterministicFiniteAutomatonParserState state)
{
    if (queue_is_empty(state->discardStack))
    {
        NondeterministicFiniteAutomatonNode node = malloc(sizeof(*node));
        if (node == NULL)
        {
            state->automaton->nondeterministicFiniteAutomatonParseErrorCode = nfapecProcessCouldNotAllocateMemory;
            return NULL;
        }
        node->anchor = ancNone;
        node->next[0] = NULL;
        node->next[1] = NULL;
        node->transitionSet = NULL;
        node->transitionType = edgeEmpty;
        queue_push_tail(state->automaton->nondeterministicFiniteAutomaton, node);
        return node;
    }

    int *discardedIndex = queue_pop_tail(state->discardStack);
    NondeterministicFiniteAutomatonNode node = state->automaton->nondeterministicFiniteAutomaton;
    free(discardedIndex);
}

int main(void)
{
}
