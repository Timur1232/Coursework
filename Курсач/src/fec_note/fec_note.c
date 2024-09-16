#include "fec_note.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"

FECNote init_note()
{
	FECNote note;
	note.serialNumber = 0;
	note.factoryNumber = 0;
	note.directorFullName[0] = '\0';
	note.engineerFullName[0] = '\0';
	note.energyConsPlan = 0.0f;
	note.energyConsReal = 0.0f;
	return note;
}

void print_note(FECNote* note)
{
	printf("| %-3d | %-3d | %15s | %15s | %8.2f | %8.2f |\n",
		note->serialNumber, note->factoryNumber, note->directorFullName,
		note->engineerFullName, note->energyConsPlan, note->energyConsReal);
}

FECNote input_note(int num)
{
	FECNote note = init_note();
	note.serialNumber = num;

	printf("Записка #%d\n"
		"Номер завода >> ", num);
	scanf("%d", &note.factoryNumber);
	printf("ФИО директора завода >> ");
	getchar();
	fgets(note.directorFullName, 16, stdin);
	fflush(stdin);
	note.directorFullName[strlen(note.directorFullName) - 1] = '\0';
	printf("ФИО главного инжинера >> ");
	fgets(note.engineerFullName, 16, stdin);
	fflush(stdin);
	note.engineerFullName[strlen(note.engineerFullName) - 1] = '\0';
	printf("Расход энергии по плану >> ");
	scanf("%f", &note.energyConsPlan);
	printf("Израсходовано энергии фактически >> ");
	scanf("%f", &note.energyConsReal);

	getchar();
	return note;
}
