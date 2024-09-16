#ifdef _DEMO

#include <stdio.h>
#include <conio.h>
#include <locale.h>

#include "fec_note/fec_note.h"
#include "list/list.h"
#include "list/list_sort.h"
#include "file_work/file_work.h"

/**
* TODO:
*	Доделать поиск
*	Наделать тестовых примеров (в файл)
*	Накоментить
*	? Почистить код
*/

int (* const SORT_FUNC_ARR[6]) (ConstValuePtr val1, ConstValuePtr val2) = {
	ser_num, fac_num, dir_name, eng_name, cons_plan, cons_real
};

void print_interface();

void organization(ListPtr fecNotes);
void print_notes(ListPtr fecNotes);
void add_note(ListPtr fecNotes);
void delete_note(ListPtr fecNotes);
void change_note(ListPtr fecNotes);
void find_entries(ListPtr fecNotes);
void load_notes(ListPtr fecNotes);

int main()
{
	setlocale(LC_ALL, "");

	List fecNotes = init_list();
	int shouldClose = 0;

	while (!shouldClose)
	{
		print_interface();
		switch (getch() - '0')
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
			find_entries(&fecNotes);
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
			printf("\nНеправильный ввод...\n");
		}
	}
	return 0;
}

void print_interface()
{
	system("cls");
	puts(	"Меню:\n"
			" 0) Организация списка\n"
			" 1) Вывод списка\n"
			" 2) Добавление записи\n"
			" 3) Удаление записи\n"
			" 4) Изменить запись\n"
			" 5) Найти вхождения\n"
			" 6) Загрузить список\n"
			"\nВыберете действие\n");
}

void organization(ListPtr fecNotes)
{
	int userInput = 1;
	int count = 1;

	system("cls");
	printf(	"0) Добавить к списку\n"
			"1) Новый список (это удалит старый список!)\n"
			"Выбор >> ");
	if (!(getchar() - '0'))
	{
		count = fecNotes->size;
	}
	else
	{
		clear(fecNotes);
	}

	while (userInput)
	{
		system("cls");
		FECNote note = input_note(count++);
		push_back(fecNotes, &note);

		printf("\nПродолжить ввод? (1 - да, 0 - нет) >> ");
		userInput = (getchar() - '0');
	}
}

void print_notes(ListPtr fecNotes)
{
	int sortMode = 1;
	while (1)
	{
		system("cls");
		printf(	"0) Вернуться\n"
				"1) Сортировка\n"
				"Текущий режим: %d\n", sortMode);
		if (sortMode > 0)
			print_list(fecNotes);
		else
			print_list_backward(fecNotes);

		switch (getch() - '0')
		{
		case 0:
			return;
		case 1:
		{
			system("cls");
			printf(	"Сортировка:\n"
					" 1) Номер записи\n"
					" 2) Номер завода\n"
					" 3) Имя директора\n"
					" 4) Имя инженера\n"
					" 5) План расхода\n"
					" 6) Реальный расход\n"
					"Текущий режим: %d\n"
					"Выбор >> ", sortMode);
			int userInput = getchar() - '0';
			getchar();
			if (userInput >= 1 && userInput <= 6)
			{
				if (userInput == abs(sortMode)) sortMode *= -1;
				else
				{
					sortMode = userInput;
					sort(fecNotes, SORT_FUNC_ARR[sortMode - 1]);
				}
			}
		}
		}
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

	if (pop(fecNotes, userInput))
	{
		puts(	"\nЗаписки не существует\n"
				"Нажмите Enter...");
		getchar();
	}
}

void change_note(ListPtr fecNotes)
{
	int userInput = -1;
	system("cls");
	printf("Введите номер записки для изменения >> ");
	scanf("%d", &userInput);

	FECNote* note = get_at(fecNotes, userInput);
	if (!note)
	{
		puts("\nЗаписки не существует\n"
			"Нажмите Enter...");
		getchar();
	}
	else
	{
		system("cls");
		*note = input_note(note->serialNumber);
	}
}

void find_entries(ListPtr fecNotes)
{
}

void load_notes(ListPtr fecNotes)
{
	scan_note_list("input\\in.txt", fecNotes);
}

#endif // _DEMO