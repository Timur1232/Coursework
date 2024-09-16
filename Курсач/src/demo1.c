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
*	�������� �����
*	�������� �������� �������� (� ����)
*	�����������
*	? ��������� ���
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
			printf("\n������������ ����...\n");
		}
	}
	return 0;
}

void print_interface()
{
	system("cls");
	puts(	"����:\n"
			" 0) ����������� ������\n"
			" 1) ����� ������\n"
			" 2) ���������� ������\n"
			" 3) �������� ������\n"
			" 4) �������� ������\n"
			" 5) ����� ���������\n"
			" 6) ��������� ������\n"
			"\n�������� ��������\n");
}

void organization(ListPtr fecNotes)
{
	int userInput = 1;
	int count = 1;

	system("cls");
	printf(	"0) �������� � ������\n"
			"1) ����� ������ (��� ������ ������ ������!)\n"
			"����� >> ");
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

		printf("\n���������� ����? (1 - ��, 0 - ���) >> ");
		userInput = (getchar() - '0');
	}
}

void print_notes(ListPtr fecNotes)
{
	int sortMode = 1;
	while (1)
	{
		system("cls");
		printf(	"0) ���������\n"
				"1) ����������\n"
				"������� �����: %d\n", sortMode);
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
			printf(	"����������:\n"
					" 1) ����� ������\n"
					" 2) ����� ������\n"
					" 3) ��� ���������\n"
					" 4) ��� ��������\n"
					" 5) ���� �������\n"
					" 6) �������� ������\n"
					"������� �����: %d\n"
					"����� >> ", sortMode);
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
	printf("������� ����� ������� ��� �������� >> ");
	scanf("%d", &userInput);

	if (pop(fecNotes, userInput))
	{
		puts(	"\n������� �� ����������\n"
				"������� Enter...");
		getchar();
	}
}

void change_note(ListPtr fecNotes)
{
	int userInput = -1;
	system("cls");
	printf("������� ����� ������� ��� ��������� >> ");
	scanf("%d", &userInput);

	FECNote* note = get_at(fecNotes, userInput);
	if (!note)
	{
		puts("\n������� �� ����������\n"
			"������� Enter...");
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