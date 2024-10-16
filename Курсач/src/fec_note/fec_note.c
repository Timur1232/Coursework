#include "fec_note.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"
#include "../proccess_fec/proccess_fec.h"

FECNote init_note()
{
    FECNote note = {
        0, 0, "", "", 0.0f, 0.0f
    };
    return note;
}

void print_note(FECNote* note)
{
    printf("| %-3d | %-3d | %-15s | %-15s | %-11.2f | %-11.2f | %11.2f |\n",
        note->serialNumber, note->factoryNumber, note->directorFullName,
        note->engineerFullName, note->energyConsPlan, note->energyConsReal,
        calc_diff_deviation(note));
}

FECNote input_note(uint_32 num)
{
    FECNote note = init_note();
    note.serialNumber = num;

    printf("Записка #%d\n"
        "Номер завода >> ", num);
    scanf("%u", &note.factoryNumber);
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
