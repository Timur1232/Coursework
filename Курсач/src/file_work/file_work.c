#include "file_work.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "parser/parser.h"

/** 
  * TODO: Улучшить оформление структуры в файле
  *		  Улучшить форматированный ввод
*/

static ParserErrors get_int(char* buff, int* data)
{
	if (sscanf(buff, "%d", data) == 1)
		return ALL_GOOD;
	return SCAN_INT_ERR;
}

static ParserErrors get_float(char* buff, float* data)
{
	if (sscanf(buff, "%f", data) == 1)
		return ALL_GOOD;
	return SCAN_FLOAT_ERR;
}

static ParserErrors get_str(char* buff, char* data)
{
	int i = 0;
	for (; buff[i] != '\"' && i < MAX_STRING_SIZE; i++)
	{
		data[i] = buff[i];
	}
	if (i >= MAX_STRING_SIZE && buff[i - 1] != '\"')
	{
		return SCAN_STR_ERR;
	}
	return ALL_GOOD;
}

static ParserErrors get_value(Token valueToken, ParserHandler* parser)
{
	ParserErrors err = ALL_GOOD;
	switch (valueToken)
	{
	case INT_TYPE:
		err = get_int(parser->buff, &parser->scanValue.intValue);
		break;
	case FLOAT_TYPE:
		err = get_float(parser->buff, &parser->scanValue.floatValue);
		break;
	case STRING_TYPE:
		err = get_str(parser->buff, parser->scanValue.stringValue);
		break;
	}
	parser->buff[0] = '\0';
	return err;
}

static ParserErrors load_var(ParserHandler* parser, FECNote* note)
{
	switch (parser->token)
	{
	case SERIAL_NUM:
		if (parser->input.serialNumber)
			return MULTIPLE_VARS;
		note->serialNumber = parser->scanValue.intValue;
		parser->input.serialNumber = 1;
		break;

	case FACTORY_NUM:
		if (parser->input.factoryNumber)
			return MULTIPLE_VARS;
		note->factoryNumber = parser->scanValue.intValue;
		parser->input.factoryNumber = 1;
		break;

	case DIR_NAME:
		if (parser->input.directorFullName)
			return MULTIPLE_VARS;
		strcpy(note->directorFullName, parser->scanValue.stringValue);
		parser->input.directorFullName = 1;
		break;

	case ENG_NAME:
		if (parser->input.engineerFullName)
			return MULTIPLE_VARS;
		strcpy(note->engineerFullName, parser->scanValue.stringValue);
		parser->input.engineerFullName = 1;
		break;

	case CONS_PLAN:
		if (parser->input.energyConsPlan)
			return MULTIPLE_VARS;
		note->energyConsPlan = parser->scanValue.floatValue;
		parser->input.energyConsPlan = 1;
		break;

	case CONS_REAL:
		if (parser->input.energyConsReal)
			return MULTIPLE_VARS;
		note->energyConsReal = parser->scanValue.floatValue;
		parser->input.energyConsReal = 1;
		break;
	}
	return ALL_GOOD;
}

static ParserErrors proccess_var_token(FILE* file, ParserHandler* parser, FECNote* note)
{
	if (get_token(parser->buff) != EQUAL_SIGN)
	{
		return EXPECT_ASSIGN;
	}
	Token valueToken = get_token(parser->buff);
	if (valueToken == QUOTE_MARK)
	{
		valueToken = STRING_TYPE;
	}
	if (token_type(valueToken) != VALUE_TYPE)
	{
		return EXPECT_VALUE;
	}

	ParserErrors err = get_value(valueToken, parser);
	if (err != ALL_GOOD)
	{
		return err;
	}

	err = load_var(parser, note);
	if (err != ALL_GOOD)
	{
		return err;
	}
	return ALL_GOOD;
}

ParserErrors scan_note(FILE* file, FECNote* note)
{
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note()", "file = nullptr", LOG_FILE);
		return FILE_OPEN_ERR;
	}

	ParserHandler parser = init_parser();

	fgets(parser.buff, BUFFER_SIZE, file);
	//printf(parser.buff);
	parser.token = get_token(parser.buff);

	if (parser.token != OPEN_BRACKET)
	{
		return NO_OPEN_BRACKET;
	}

	while (!feof(file) && !parser.shouldClose)
	{
		fgets(parser.buff, BUFFER_SIZE, file);
		//printf(parser.buff);
		if (parser.buff[strlen(parser.buff) - 1] != '\n')
		{
			ignore_line(file);
			parser.buffSizeExceeded = 1;
		}

		while (!eob(parser.buff))
		{
			parser.token = get_token(parser.buff);

			if (parser.token == COMMENT)
			{
				break;
			}
			else if (token_type(parser.token) == VAR)
			{
				ParserErrors err = proccess_var_token(file, &parser, note);
				if (err != ALL_GOOD)
				{
					return err;
				}
			}
			else if (parser.token == CLOSE_BRAKET)
			{
				parser.shouldClose = 1;
			}
		}
	}
	if (parser.buffSizeExceeded)
	{
		return BUFF_SIZE_EXCEEDED;
	}
	return ALL_GOOD;
}

void scan_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to open input file", LOG_FILE);
		return;
	}
	clear(fecNotes);
	while (!feof(file))
	{
		FECNote note = init_note();
		int count = fscanf(file,
			"%d %d %s %s %f %f",
			&note.serialNumber, &note.factoryNumber,
			note.directorFullName, note.engineerFullName,
			&note.energyConsPlan, &note.energyConsReal);

		if (count != 6)
		{
			fclose(file);
			char str[150];
			sprintf(str, "Unable to read formatter text, count=%d\n"
						 "%d %d %s %s %f %f", count, note.serialNumber, note.factoryNumber, note.directorFullName, note.engineerFullName, note.energyConsPlan, note.energyConsReal);
			LOG(ERR, "fec_note.c", "scan_note_list()", str, LOG_FILE);
			return;
		}
		push_back(fecNotes, &note);
	}
	fclose(file);
}

void ignore_line(FILE* file)
{
	int ch = fgetc(file);
	while (ch != '\n' && ch != EOF)
	{
		ch = fgetc(file);
	}
}