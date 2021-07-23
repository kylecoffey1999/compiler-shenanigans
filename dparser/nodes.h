#ifndef NODES_H
#define NODES_H

typedef enum
{
    ntQvalue,
    ntSqvalue,
    ntCommandLine,
    ntCommandLineGlobal,
    ntCommandLineNonGlobal,
    ntCommand,
    ntOption,
    ntName,
    ntValue,
} NodeType;

#endif // NODES_H
