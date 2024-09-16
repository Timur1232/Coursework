#include "file_work.h"

#include <stdio.h>
#include "../log/log.h"
#include "../list/list.h"
#include "../fec_note/fec_note.h"

/** 
  * TODO: Улучшить оформление структуры в файле
  *		  Улучшить форматированный ввод
*/

void scan_note(const char* fileName, FECNote* note)
{
	FILE* file = fopen(fileName, "r");
	if (!file)
	{
		LOG(ERR, "fec_note.c", "scan_note()", "Unable to open input file", LOG_FILE);
		return;
	}

	int count = fscanf(file,
		"serialNumber=%d\n"
		"factoryNumber=%d\n"
		"directorFullName=%s\n"
		"engineerFullName=%s\n"
		"energyConsPlan=%f\n"
		"energyConsReal=%f\n",
		&note->serialNumber, &note->factoryNumber,
		note->directorFullName, note->engineerFullName,
		&note->energyConsPlan, &note->energyConsReal);

	if (count != 6)
	{
		fclose(file);
		LOG(ERR, "fec_note.c", "scan_note()", "Unable to read formated input", LOG_FILE);
		return;
	}
	fclose(file);
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