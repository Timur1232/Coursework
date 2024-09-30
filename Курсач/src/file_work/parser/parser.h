#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "../../list/list.h"

ErrorHandler parse_tokens(TokenQueue* tokens, ListPtr fecNotes);
TokenizerErrors load_var(TokenVar* token, FECNote* note);		// Загрузка считанного значения в соответствующее поле структуры

#endif