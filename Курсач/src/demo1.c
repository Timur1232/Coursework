#ifdef _DEMO

#include <stdio.h>
#include <conio.h>
#include <locale.h>
#include <string.h>

#include "fec_note/fec_note.h"
#include "compare/compare.h"
#include "list/list.h"
#include "file_work/file_work.h"
#include "ref_array/ref_array.h"
#include "find/find_entries.h"

// Массив функций сравнения
int (* const COMPARE_FUNC_ARR[6]) (ConstValuePtr val1, ConstValuePtr val2) = {
	ser_num, fac_num, dir_name, eng_name, cons_plan, cons_real
};

enum Compare
{
	SER_NUM,
	FAC_NUM,
	DIR_NAME,
	ENG_NAME,
	CONS_PLAN,
	CONS_REAL
};

// Массив интерфейсов
const char* const INTERFACE_ARR[] = {
	"Меню:\n"
	" 1) Организация списка\n"
	" 2) Вывод списка\n"
	" 3) Добавление записи\n"
	" 4) Удаление записи\n"
	" 5) Изменить запись\n"
	" 6) Найти вхождения\n"
	" 7) Загрузить список\n"
	" 0) Выход\n"
	"\nВыберете действие\n",

	"0) Вернуться\n"
	"1) Сортировка\n",

	"Сортировка:\n"
	" 1) Номер записи\n"
	" 2) Номер завода\n"
	" 3) Имя директора\n"
	" 4) Имя инженера\n"
	" 5) План расхода\n"
	" 6) Реальный расход\n",

	"0) Добавить к списку\n"
	"1) Новый список (это удалит старый список!)\n"
	"Выбор >> ",

	"Поиск:\n"
	" 1) По номеру записки\n"
	" 2) По номеру завода\n"
	" 3) По имени директора\n"
	" 4) По имени главного инжинера\n"
	" 5) По плану расхода\n"
	" 6) По фактическому расходу\n"
	"Выберите поле поиска >> "
};

enum Interfaces
{
	MAIN = 0,
	PRINT = 1,
	SORT = 2,
	ORG = 3,
	FIND = 4
};

/*-----------------------------[Прототипы]-----------------------------*/

void print_interface(enum Interfaces interface_num);	// Печать интерфейса

void organization(ListPtr fecNotes);					// Организация списка
void print_notes(ListPtr fecNotes);						// Печать списка
void add_note(ListPtr fecNotes);						// Добавление записки
void delete_note(ListPtr fecNotes);						// Удаление записки
void change_note(ListPtr fecNotes);						// Изменение записки
void find(ListPtr fecNotes);							// Поиск вхождений
void load_notes(ListPtr fecNotes);						// Загрузка тестового примера

void wait_input();										// Ожидание ввода пользователя
void notes_renumber(ListPtr fecNotes);

/*-----------------------------[main]-----------------------------*/

int main()
{
	setlocale(LC_ALL, "");

	List fecNotes = init_list();
	int shouldClose = 0;

	while (!shouldClose)
	{
		print_interface(MAIN);
		switch (_getch() - '0')
		{
		case 1:
		{
			organization(&fecNotes);
			break;
		}
		case 2:
		{
			print_notes(&fecNotes);
			break;
		}
		case 3:
		{
			add_note(&fecNotes);
			break;
		}
		case 4:
		{
			delete_note(&fecNotes);
			break;
		}
		case 5:
		{
			change_note(&fecNotes);
			break;
		}
		case 6:
		{
			find(&fecNotes);
			break;
		}
		case 7:
		{
			load_notes(&fecNotes);
			break;
		}
		case 0:
		{
			clear(&fecNotes);
			shouldClose = 1;
			break;
		}
		default:
			break;
		}
	}
	return 0;
}

/*-----------------------------[Определения функций]-----------------------------*/

void print_interface(enum Interfaces interface_num)
{
	system("cls");
	printf(INTERFACE_ARR[interface_num]);
}

void organization(ListPtr fecNotes)
{
	int userInput = 1;
	int count = 1;

	print_interface(ORG);
	// Добавление к существующему списку или создание нового списка
	if (!(getchar() - '0'))
	{
		count = fecNotes->size;
	}
	else
	{
		clear(fecNotes);
	}

	// Ввод пока пользователь не введет 0
	while (userInput)
	{
		system("cls");
		FECNote note = input_note(count++);
		push_back(fecNotes, &note);

		printf("\nПродолжить ввод? (1 - да, 0 - нет) >> ");
		userInput = (getchar() - '0');
	}
	// Сортировка по порядковому номеру
	sort(fecNotes, COMPARE_FUNC_ARR[SER_NUM]);
}

void print_notes(ListPtr fecNotes)
{
	int sortMode = 1;
	while (1)
	{
		print_interface(PRINT);
		printf("Текущий режим: %d\n", sortMode);
		// Направление печати списка ((+) с начала, (-) с конца)
		if (sortMode > 0)
		{
			print_list(fecNotes);
		}
		else
		{
			print_list_backward(fecNotes);
		}

		if (_getch() - '0')
		{
			print_interface(SORT);
			printf(	"Текущий режим: %d\n"
					"Выбор >> ", sortMode);
			int userInput = getchar() - '0';
			if (userInput >= 1 && userInput <= 6)
			{
				if (userInput == abs(sortMode)) sortMode *= -1;
				else
				{
					sortMode = userInput;
					// Сортировка по выбранному полю
					sort(fecNotes, COMPARE_FUNC_ARR[sortMode - 1]);
				}
			}
		}
		else return;
	}
}

void add_note(ListPtr fecNotes)
{
	system("cls");
	FECNote note = input_note(fecNotes->size);
	push_back(fecNotes, &note);
}

void delete_note(ListPtr fecNotes)
{
	int userInput = -1;
	system("cls");
	printf("Введите номер записки для удаления >> ");
	scanf("%d", &userInput);

	// Удаление по индексу
	if (pop(fecNotes, userInput - 1))
	{
		puts("\nЗаписки не существует\n");
		wait_input();
	}
	else notes_renumber(fecNotes); // Пересчет порядковых нмеров записей
}

void change_note(ListPtr fecNotes)
{
	int userInput = -1;
	system("cls");
	printf("Введите номер записки для изменения >> ");
	scanf("%d", &userInput);

	// Поиск и изменение по индексу
	FECNote* note = get_at(fecNotes, userInput - 1);
	if (!note)
	{
		puts("\nЗаписки не существует\n");
		wait_input();
	}
	else
	{
		system("cls");
		*note = input_note(note->serialNumber);
	}
}

void find(ListPtr fecNotes)
{
	int findMode = 0;
	union {
		char stringInput[MAX_STRING_SIZE];
		int intInput;
		float floatInput;
	} userInput = {""};

	// Пользовательский ввод:

	system("cls");
	// Выбор поля поиска
	do
	{
		print_interface(FIND);
		scanf("%d", &findMode);
	}
	while (findMode <= 0 && findMode > 6);

	int correct = 0;
	// Ввод искомого значения
	do
	{
		system("cls");
		printf("Введите искомое значение >> ");
		switch (findMode)
		{
		// Ввод типа int
		case 1: case 2:
		{
			if (scanf("%d", &userInput.intInput) == 1) correct = 1;
			else puts("Ошибка ввода...");
			break;
		}
		// Ввод типа char* (строка)
		case 3: case 4:
		{
			getchar();
			if (fgets(userInput.stringInput, MAX_STRING_SIZE, stdin) != NULL)
			{
				correct = 1;
				userInput.stringInput[strlen(userInput.stringInput) - 1] = '\0';
			}
			else puts("Ошибка ввода...");
			fflush(stdin);
			break;
		}
		// Ввод типа float
		case 5: case 6:
		{
			if (scanf("%f", &userInput.floatInput) == 1) correct = 1;
			else puts("Ошибка ввода...");
			break;
		}
		default:
			break;
		}
	} while (!correct);

	// Создание структуры для поиска с нужным полем
	FECNote required = init_note();
	switch (findMode)
	{
	case 1: required.serialNumber = userInput.intInput; break;
	case 2: required.factoryNumber = userInput.intInput; break;
	case 3: strcpy(required.directorFullName, userInput.stringInput); break;
	case 4: strcpy(required.engineerFullName, userInput.stringInput); break;
	case 5: required.energyConsPlan = userInput.floatInput; break;
	case 6: required.energyConsReal = userInput.floatInput; break;
	}

	// Сам поиск вхождений
	RefArray entries = find_entries(fecNotes, &required, COMPARE_FUNC_ARR[findMode - 1]);

	print_entries(&entries);
	wait_input();

	clear_array(&entries);
}

void load_notes(ListPtr fecNotes)
{
	scan_note_list("input\\in.txt", fecNotes);
	notes_renumber(fecNotes);
}

void wait_input()
{
	puts("\nНажмите любую кнопку...");
	_getch();
}

void notes_renumber(ListPtr fecNotes)
{
	int num = 1;
	for (FOR_RANGE(iter, *fecNotes))
	{
		iter->data.serialNumber = num++;
	}
}

#endif // _DEMO