#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "../../list/list.h"

ErrorHandler parse_tokens(TokenQueue* tokens, ListPtr fecNotes);

#endif // PARSER_H