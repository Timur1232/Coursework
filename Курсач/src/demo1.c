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

// ������ ������� ���������
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

// ������ �����������
const char* const INTERFACE_ARR[] = {
	"����:\n"
	" 1) ����������� ������\n"
	" 2) ����� ������\n"
	" 3) ���������� ������\n"
	" 4) �������� ������\n"
	" 5) �������� ������\n"
	" 6) ����� ���������\n"
	" 7) ��������� ������\n"
	" 0) �����\n"
	"\n�������� ��������\n",

	"0) ���������\n"
	"1) ����������\n",

	"����������:\n"
	" 1) ����� ������\n"
	" 2) ����� ������\n"
	" 3) ��� ���������\n"
	" 4) ��� ��������\n"
	" 5) ���� �������\n"
	" 6) �������� ������\n",

	"0) �������� � ������\n"
	"1) ����� ������ (��� ������ ������ ������!)\n"
	"����� >> ",

	"�����:\n"
	" 1) �� ������ �������\n"
	" 2) �� ������ ������\n"
	" 3) �� ����� ���������\n"
	" 4) �� ����� �������� ��������\n"
	" 5) �� ����� �������\n"
	" 6) �� ������������ �������\n"
	"�������� ���� ������ >> "
};

enum Interfaces
{
	MAIN = 0,
	PRINT = 1,
	SORT = 2,
	ORG = 3,
	FIND = 4
};

/*-----------------------------[���������]-----------------------------*/

void print_interface(enum Interfaces interface_num);	// ������ ����������

void organization(ListPtr fecNotes);					// ����������� ������
void print_notes(ListPtr fecNotes);						// ������ ������
void add_note(ListPtr fecNotes);						// ���������� �������
void delete_note(ListPtr fecNotes);						// �������� �������
void change_note(ListPtr fecNotes);						// ��������� �������
void find(ListPtr fecNotes);							// ����� ���������
void load_notes(ListPtr fecNotes);						// �������� ��������� �������

void wait_input();										// �������� ����� ������������
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

/*-----------------------------[����������� �������]-----------------------------*/

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
	// ���������� � ������������� ������ ��� �������� ������ ������
	if (!(getchar() - '0'))
	{
		count = fecNotes->size;
	}
	else
	{
		clear(fecNotes);
	}

	// ���� ���� ������������ �� ������ 0
	while (userInput)
	{
		system("cls");
		FECNote note = input_note(count++);
		push_back(fecNotes, &note);

		printf("\n���������� ����? (1 - ��, 0 - ���) >> ");
		userInput = (getchar() - '0');
	}
	// ���������� �� ����������� ������
	sort(fecNotes, COMPARE_FUNC_ARR[SER_NUM]);
}

void print_notes(ListPtr fecNotes)
{
	int sortMode = 1;
	while (1)
	{
		print_interface(PRINT);
		printf("������� �����: %d\n", sortMode);
		// ����������� ������ ������ ((+) � ������, (-) � �����)
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
			printf(	"������� �����: %d\n"
					"����� >> ", sortMode);
			int userInput = getchar() - '0';
			if (userInput >= 1 && userInput <= 6)
			{
				if (userInput == abs(sortMode)) sortMode *= -1;
				else
				{
					sortMode = userInput;
					// ���������� �� ���������� ����
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
	printf("������� ����� ������� ��� �������� >> ");
	scanf("%d", &userInput);

	// �������� �� �������
	if (pop(fecNotes, userInput - 1))
	{
		puts("\n������� �� ����������\n");
		wait_input();
	}
	else notes_renumber(fecNotes); // �������� ���������� ������ �������
}

void change_note(ListPtr fecNotes)
{
	int userInput = -1;
	system("cls");
	printf("������� ����� ������� ��� ��������� >> ");
	scanf("%d", &userInput);

	// ����� � ��������� �� �������
	FECNote* note = get_at(fecNotes, userInput - 1);
	if (!note)
	{
		puts("\n������� �� ����������\n");
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

	// ���������������� ����:

	system("cls");
	// ����� ���� ������
	do
	{
		print_interface(FIND);
		scanf("%d", &findMode);
	}
	while (findMode <= 0 && findMode > 6);

	int correct = 0;
	// ���� �������� ��������
	do
	{
		system("cls");
		printf("������� ������� �������� >> ");
		switch (findMode)
		{
		// ���� ���� int
		case 1: case 2:
		{
			if (scanf("%d", &userInput.intInput) == 1) correct = 1;
			else puts("������ �����...");
			break;
		}
		// ���� ���� char* (������)
		case 3: case 4:
		{
			getchar();
			if (fgets(userInput.stringInput, MAX_STRING_SIZE, stdin) != NULL)
			{
				correct = 1;
				userInput.stringInput[strlen(userInput.stringInput) - 1] = '\0';
			}
			else puts("������ �����...");
			fflush(stdin);
			break;
		}
		// ���� ���� float
		case 5: case 6:
		{
			if (scanf("%f", &userInput.floatInput) == 1) correct = 1;
			else puts("������ �����...");
			break;
		}
		default:
			break;
		}
	} while (!correct);

	// �������� ��������� ��� ������ � ������ �����
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

	// ��� ����� ���������
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
	puts("\n������� ����� ������...");
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