#ifndef FEC_NOTE_H
#define FEC_NOTE_H

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
    int serialNumber;
    int factoryNumber;
    char directorFullName[16];
    char engineerFullName[16];
    float energyConsPlan;
    float energyConsReal;
} FECNote;

//_____________________________[Прототипы функций]_____________________________//

FECNote init_note();

// Временные функции
void print_note(FECNote* note);
FECNote input_note(int num);

#endif