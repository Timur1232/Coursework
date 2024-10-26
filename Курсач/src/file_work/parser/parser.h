#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "../../list/list.h"

ErrorHandler parse_tokens(TokenQueue* tokens, ListPtr fecNotes);
TokenizerErrors load_var(TokenQueue* tokens, FECNote* note, RepeatObserver* observer);		// Загрузка считанного значения в соответствующее поле структуры

#endif // PARSER_H