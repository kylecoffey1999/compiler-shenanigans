#include "nodes.h"
#include <stdio.h>
#include <vec.h>

#include "dparse.h"

extern D_ParserTables parser_tables_gram;

typedef vec_t(D_ParseNode *) vec_parsenode_t;

int main(void)
{
    D_Parser *parser = new_D_Parser(&parser_tables_gram, sizeof(D_ParseNode_User));
    D_ParseNode *node = dparse(parser, "test set", 8);
    if (!node || parser->syntax_errors)
    {
        fprintf(stderr, "oh no\n");
        exit(1);
    }
    vec_parsenode_t nodes;
    vec_init(&nodes);
    while (d_get_number_of_children(node) > 0) {
        
    }
    vec_deinit(&nodes);
    free_D_Parser(parser);
}
