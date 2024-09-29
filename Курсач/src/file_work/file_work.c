#include "file_work.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "parser/parser.h"


static void ignore_line(FILE* file);
static ParserErrors get_int(char* buff, int* data);
static ParserErrors get_float(char* buff, float* data);
static ParserErrors get_str(char* buff, char* data);
static ParserErrors get_value(Token valueToken, ParserHandler* parser);
static ParserErrors load_var(ParserHandler* parser, FECNote* note, Token valueType);
static ParserErrors proccess_var_token(FILE* file, ParserHandler* parser, FECNote* note);


ParserErrors scan_note(FILE* file, FECNote* note)
{
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note()", "file = nullptr", LOG_FILE);
		return FILE_OPEN_ERR;
	}

	ParserHandler parser = init_parser();

	do
	{
		if (fgets(parser.buff, BUFFER_SIZE, file) == NULL)
		{
			return FILE_ENDS;
		}
		parser.token = get_token(parser.buff);
	} while (token_type(parser.token) == SPEC);

	if (parser.token != OPEN_BRACKET)
	{
		return NO_OPEN_BRACKET;
	}

	while (!feof(file) && !parser.shouldClose)
	{
		fgets(parser.buff, BUFFER_SIZE, file);
		int num = strlen(parser.buff);
		if (parser.buff[num - 1] != '\n' && num == BUFFER_SIZE)
		{
			ignore_line(file);
			parser.buffSizeExceeded = 1;
		}

		while (!eob(parser.buff))
		{
			parser.token = get_token(parser.buff);

			if (token_type(parser.token) == SPEC)
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
			else if (parser.token == OPEN_BRACKET)
			{
				return NO_CLOSE_BRACKET;
			}
		}
	}
	if (parser.buffSizeExceeded)
	{
		return BUFF_SIZE_EXCEEDED;
	}
	return ALL_GOOD;
}

ParserErrors scan_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to open input file", LOG_FILE);
		return FILE_OPEN_ERR;
	}
	clear(fecNotes);

	while (!feof(file))
	{
		FECNote note = init_note();
		ParserErrors err = scan_note(file, &note);

		if (err == FILE_ENDS)
		{
			break;
		}
		else if (err != ALL_GOOD)
		{
			fclose(file);
			char str[150];
			sprintf(str, "Unable to read formatter text, count=%d\n"
						 "%d %d %s %s %f %f", err, note.serialNumber, note.factoryNumber, note.directorFullName, note.engineerFullName, note.energyConsPlan, note.energyConsReal);
			LOG(ERR, "fec_note.c", "scan_note_list()", str, LOG_FILE);
			return err;
		}
		push_back(fecNotes, &note);
	}
	fclose(file);
	return ALL_GOOD;
}

ParserErrors scan_bin_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "rb");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to create output file", LOG_FILE);
		return FILE_OPEN_ERR;
	}
	clear(fecNotes);

	while (!feof(file))
	{
		FECNote note = init_note();
		if (fread(&note, sizeof(FECNote), 1, file) != 1)
		{
			break;
		}
		push_back(fecNotes, &note);
	}
	fclose(file);
	return ALL_GOOD;
}

ParserErrors save_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "w");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to create output file", LOG_FILE);
		return FILE_OPEN_ERR;
	}

	for (FOR_RANGE(iter, *fecNotes))
	{
		fprintf(file,
			"{\n"
			"\tSerialNumber = %d\n"
			"\tFactoryNumber = %d\n"
			"\tDirectorName = \"%s\"\n"
			"\tEngineerName = \"%s\"\n"
			"\tEnergyConsPlan = %f\n"
			"\tEnergyConsReal = %f\n"
			"}\n",
			iter->data.serialNumber, iter->data.factoryNumber,
			iter->data.directorFullName, iter->data.engineerFullName,
			iter->data.energyConsPlan, iter->data.energyConsReal
		);
	}
	fclose(file);
	return ALL_GOOD;
}

ParserErrors save_bin_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "wb");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to create output file", LOG_FILE);
		return FILE_OPEN_ERR;
	}

	for (FOR_RANGE(iter, *fecNotes))
	{
		fwrite(&iter->data, sizeof(FECNote), 1, file);
	}
	fclose(file);
	return ALL_GOOD;
}

/*=================================[Вспомогательные функции]=================================*/

static void ignore_line(FILE* file)
{
	int ch = fgetc(file);
	while (ch != '\n' && ch != EOF)
	{
		ch = fgetc(file);
	}
}

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
	data[i] = '\0';
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

static ParserErrors load_var(ParserHandler* parser, FECNote* note, Token valueType)
{
	switch (parser->token)
	{
	case SERIAL_NUM:
		if (parser->input.serialNumber)
			return MULTIPLE_VARS;
		if (valueType != INT_TYPE)
			return EXPECT_INT;
		note->serialNumber = parser->scanValue.intValue;
		parser->input.serialNumber = 1;
		break;

	case FACTORY_NUM:
		if (parser->input.factoryNumber)
			return MULTIPLE_VARS;
		if (valueType != INT_TYPE)
			return EXPECT_INT;
		note->factoryNumber = parser->scanValue.intValue;
		parser->input.factoryNumber = 1;
		break;

	case DIR_NAME:
		if (parser->input.directorFullName)
			return MULTIPLE_VARS;
		if (valueType != STRING_TYPE)
			return EXPECT_STR;
		strcpy(note->directorFullName, parser->scanValue.stringValue);
		parser->input.directorFullName = 1;
		break;

	case ENG_NAME:
		if (parser->input.engineerFullName)
			return MULTIPLE_VARS;
		if (valueType != STRING_TYPE)
			return EXPECT_STR;
		strcpy(note->engineerFullName, parser->scanValue.stringValue);
		parser->input.engineerFullName = 1;
		break;

	case CONS_PLAN:
		if (parser->input.energyConsPlan)
			return MULTIPLE_VARS;
		if (valueType != FLOAT_TYPE)
			return EXPECT_FLOAT;
		note->energyConsPlan = parser->scanValue.floatValue;
		parser->input.energyConsPlan = 1;
		break;

	case CONS_REAL:
		if (parser->input.energyConsReal)
			return MULTIPLE_VARS;
		if (valueType != FLOAT_TYPE)
			return EXPECT_FLOAT;
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

	err = load_var(parser, note, valueToken);
	if (err != ALL_GOOD)
	{
		return err;
	}
	return ALL_GOOD;
}