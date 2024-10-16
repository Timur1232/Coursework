#include "parser.h"

#include <string.h>

#include "../../fec_note/fec_note.h"
#include "../../list/list.h"

ErrorHandler parse_tokens(TokenQueue* tokens, ListPtr fecNotes)
{
    ErrorHandler error = init_error_handler();
    RepeatObserver observer = init_observer();

    while (!empty_tokens(tokens))
    {
        FECNote note = init_note();

        if (get_token(tokens)->type == END)
        {
            pop_token(tokens);
            break;
        }
        else if (get_token(tokens)->type != OPEN_BRACKET)
        {
            error.err = NO_OPEN_BRACKET;
            error.line = get_token(tokens)->line;
            return error;
        }

        pop_token(tokens);
        while (1)
        {
            if (get_token(tokens)->type == CLOSE_BRAKET)
            {
                pop_token(tokens);
                observer = init_observer();
                break;
            }
            else if (get_token(tokens)->type == OPEN_BRACKET || get_token(tokens)->type == END)
            {
                error.err = NO_CLOSE_BRACKET;
                error.line = get_token(tokens)->line;
                return error;
            }

            error.err = load_var(tokens, &note, &observer);

            if (error.err != ALL_GOOD)
            {
                error.line = get_token(tokens)->line;
                return error;
            }
        }
        push_back(fecNotes, &note);
    }
    return error;
}

/*=================================[Вспомогательные функции]=================================*/

TokenizerErrors load_var(TokenQueue* tokens, FECNote* note, RepeatObserver* observer)
{
    Token varToken = get_token(tokens)->type;

    if (token_type(varToken) != VAR)
    {
        return EXPECT_VAR;
    }
    if (check_repeat(observer, varToken))
    {
        return MULTIPLE_VARS;
    }
    pop_token(tokens);
    if (get_token(tokens)->type != EQUAL_SIGN)
    {
        return EXPECT_ASSIGN;
    }
    pop_token(tokens);

    switch (varToken)
    {
    case SERIAL_NUM_VAR:
        note->serialNumber = get_token(tokens)->value.intValue;
        break;

    case FACTORY_NUM_VAR:
        note->factoryNumber = get_token(tokens)->value.intValue;
        break;

    case DIR_NAME_VAR:
        strcpy(note->directorFullName, get_token(tokens)->value.stringValue);
        break;

    case ENG_NAME_VAR:
        strcpy(note->engineerFullName, get_token(tokens)->value.stringValue);
        break;

    case CONS_PLAN_VAR:
        note->energyConsPlan = get_token(tokens)->value.floatValue;
        break;

    case CONS_REAL_VAR:
        note->energyConsReal = get_token(tokens)->value.floatValue;
        break;
    default:
        return UNEXPECTED_ERROR;
    }
    pop_token(tokens);
    return ALL_GOOD;
}