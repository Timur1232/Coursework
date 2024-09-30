#include "parser.h"

#include "../../fec_note/fec_note.h"
#include "../../list/list.h"

ErrorHandler parse_tokens(TokenQueue* tokens, ListPtr fecNotes)
{
	ErrorHandler error = { 0, ALL_GOOD };
	while (!empty_tokens(tokens))
	{
		FECNote note = init_note();
		TokenVar* token = get_token(tokens);

		if (token->type == OPEN_BRACKET)
		{
			pop_token(tokens);
			while (1)
			{
				token = get_token(tokens);
				if (token->type == CLOSE_BRAKET)
				{
					pop_token(tokens);
					break;
				}
				error.err = load_var(token, &note);
				if (error.err != ALL_GOOD)
				{
					error.line = -1;
					clear_tokens(tokens);
					return error;
				}
				pop_token(tokens);
			}
			push_back(fecNotes, &note);
		}
		else
		{
			error.err = UNEXPECTED_ERROR;
			error.line = -1;
			clear_tokens(tokens);
			return error;
		}
	}
	clear_tokens(tokens);
	return error;
}

/*=================================[Вспомогательные функции]=================================*/

TokenizerErrors load_var(TokenVar* token, FECNote* note)
{
	switch (token->type)
	{
	case SERIAL_NUM:
		note->serialNumber = token->value.intValue;
		break;

	case FACTORY_NUM:
		note->factoryNumber = token->value.intValue;
		break;

	case DIR_NAME:
		strcpy(note->directorFullName, token->value.stringValue);
		break;

	case ENG_NAME:
		strcpy(note->engineerFullName, token->value.stringValue);
		break;

	case CONS_PLAN:
		note->energyConsPlan = token->value.floatValue;
		break;

	case CONS_REAL:
		note->energyConsReal = token->value.floatValue;
		break;
	default:
		return UNEXPECTED_ERROR;
	}
	return ALL_GOOD;
}