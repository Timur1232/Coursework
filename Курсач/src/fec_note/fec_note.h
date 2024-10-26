#ifndef FEC_NOTE_H
#define FEC_NOTE_H

#define MAX_STRING_SIZE 16

#include <types.h>

/**
 * порядковый номер
 * номер завода
 * ФИО директора (15 символов)
 * ФИО главного энергетика (15 символов)
 * расход энергии по плану (в тыс. КВт∙ч)
 * израсходовано фактически (в тыс. КВт∙ч)
 */
 // Factory Electricity Consumption Note struct
typedef struct FECNote
{
    uint_32 serialNumber;
    uint_32 factoryNumber;
    char directorFullName[MAX_STRING_SIZE];
    char engineerFullName[MAX_STRING_SIZE];
    float energyConsPlan;
    float energyConsReal;
} FECNote;

typedef FECNote* FECNotePtr;

//_____________________________[Прототипы функций]_____________________________//

FECNote init_note();

// Временные функции
//void print_note(FECNote* note);
FECNote input_note(uint_32 num);

#endif // FEC_NOTE_H