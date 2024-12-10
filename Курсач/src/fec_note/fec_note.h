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

// Массив функций сравнения
extern int (* const COMPARE_FUNC_ARRAY[6]) (const FECNote* val1, const FECNote* val2);

typedef enum Compare
{
    SER_NUM = 0,
    FAC_NUM = 1,
    DIR_NAME = 2,
    ENG_NAME = 3,
    CONS_PLAN = 4,
    CONS_REAL = 5
} Compare;

// Функции сравнения структур по их полям
int ser_num(const FECNote* val1, const FECNote* val2);
int fac_num(const FECNote* val1, const FECNote* val2);
int dir_name(const FECNote* val1, const FECNote* val2);
int eng_name(const FECNote* val1, const FECNote* val2);
int cons_plan(const FECNote* val1, const FECNote* val2);
int cons_real(const FECNote* val1, const FECNote* val2);

int my_strcmp(const char* str1, const char* str2);

//_____________________________[Прототипы функций]_____________________________//

FECNote init_note();

#endif // FEC_NOTE_H