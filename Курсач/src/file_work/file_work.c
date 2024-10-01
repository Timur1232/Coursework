#include "file_work.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "parser/tokenizer.h"
#include "parser/parser.h"

/*=====================================[Функции]=====================================*/

ErrorHandler scan_note_list(const char* fileName, ListPtr fecNotes)
{
	ErrorHandler error = init_error_handler();
	TokenQueue tokens = init_token_queue();
	FILE* file = fopen(fileName, "r");

	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to open input file", LOG_FILE);
		error.err = FILE_OPEN_ERR;
		return error;
	}
	clear(fecNotes);

	error = tokenize(&tokens, file);
	fclose(file);

	if (error.err != ALL_GOOD)
	{
		clear_tokens(&tokens);
		return error;
	}

	error = parse_tokens(&tokens, fecNotes);
	if (error.err != ALL_GOOD)
	{
		clear(fecNotes);
	}
	clear_tokens(&tokens);
	return error;
}

TokenizerErrors scan_bin_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "rb");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to open input file", LOG_FILE);
		return -1;
	}
	clear(fecNotes);

	FECNote note = init_note();
	while (fread(&note, sizeof(FECNote), 1, file) == 1)
	{
		if (ferror(file))
		{
			fclose(file);
			return -1;
		}
		push_back(fecNotes, &note);
		note = init_note();
	}
	fclose(file);
	return 0;
}

TokenizerErrors save_note_list(const char* fileName, ListPtr fecNotes)
{
	FILE* file = fopen(fileName, "w");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note_list()", "Unable to create output file", LOG_FILE);
		return -1;
	}

	for (FOR_RANGE(iter, *fecNotes))
	{
		fprintf(file,
			"{\n"
			"\tserialNumber = %d;\n"
			"\tfactoryNumber = %d;\n"
			"\tdirectorName = \"%s\";\n"
			"\tengineerName = \"%s\";\n"
			"\tenergyConsPlan = %f;\n"
			"\tenergyConsReal = %f;\n"
			"}\n",
			iter->data.serialNumber, iter->data.factoryNumber,
			iter->data.directorFullName, iter->data.engineerFullName,
			iter->data.energyConsPlan, iter->data.energyConsReal
		);
	}
	fclose(file);
	return 0;
}

TokenizerErrors save_bin_note_list(const char* fileName, ListPtr fecNotes)
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