/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			   Севастопольский государственный университет
					Кафедра «Информационные системы»
		  Программа для организации и обработки данных о студентах

																						РАЗРАБОТАЛ
																						Студент гр. ИТ/б-23-6-о
																									 Пушкарев Д.С.
									2024
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Основные функции программы:
1. Начальное создание таблицы.
2. Просмотр таблицы.
3. Добавление новой записи в таблицу.
4. Удаление записи.
5. Корректировка записи в таблице.
6. Поиск в таблице.
7. Сортировка таблицы.
8. Сохранение таблицы в файле.
9. Чтение данных из файла.
10. Вывод пяти студентов с максимальными долгами за общежитие
11. Выход.

	  Вариант задания 21. Утверждено 02.09.2024
	  Среда программирования Visual Studio 2022
	  Дата последней коррекции: 01.12.2024
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <windows.h>

struct Student
{
	int book_number;
	char surname[11];
	char name[12];
	char patronymic[15];
	char studying_direction[14];
	char groop[14];
	int hostel_number;
	int room_number;
	double hoster_debts;
};

struct Student_list
{
	struct Student this_student;
	struct Student_list* left, * right;
};

struct Student_list* Create_list(struct Student_list* this_list);
struct Student_list* Add_element(struct Student_list* this_list);
void Output_list(struct Student_list* this_list);
struct Student_list* Delete_element(struct Student_list* this_list);
struct Student_list* Change_element(struct Student_list* this_list);
struct Student_list* Sort_list(struct Student_list* this_list);
void Search_in_list(struct Student_list* this_list);
void file_save(struct Student_list* this_list);
struct Student_list* file_load(struct Student_list* this_list);
void Five_rooms_debts(struct Student_list* this_list);
struct Student_list* Delete_list(struct Student_list* this_list);

struct Student_list* Memory_input(struct Student_list* this_list);
struct Student_list* Add_to_right(struct Student_list* this_list, struct Student_list* this_element);
struct Student_list* Add_to_left(struct Student_list* this_list, struct Student_list* this_element);
void Output_all(struct Student_list* this_list);
void Output_by_elements(struct Student_list* this_list);
void Output_by_pages(struct Student_list* this_list);
void Element_output(struct Student_list* this_list);
void Element_output_for_page(struct Student_list* this_list);
int Output_page(struct Student_list* this_list, int page);
void search_by_book_number(struct Student_list* this_list);
void search_by_surname(struct Student_list* this_list);
void search_by_name(struct Student_list* this_list);
void search_by_patronym(struct Student_list* this_list);
void search_by_direction(struct Student_list* this_list);
void search_by_groop(struct Student_list* this_list);
void search_by_hostel(struct Student_list* this_list);
void search_by_room(struct Student_list* this_list);
void search_by_debts(struct Student_list* this_list);
int equal_book_number(struct Student_list*, int);

void press_any_key_function();
void Menu_fuctionction();
void header_output();

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("color F0");
	setlocale(LC_ALL, "RUSSIAN");
	struct Student_list* list = NULL;
	int menu_point = 0;
	while (1)
	{
		Menu_fuctionction();
		while (scanf_s("%d", &menu_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (menu_point)
		{
		case 1:
		{
			list = Create_list(list);
			break;
		}
		case 2:
		{
			list = Add_element(list);
			break;
		}
		case 3:
		{
			Output_list(list);
			break;
		}
		case 4:
		{
			list = Delete_element(list);
			break;
		}
		case 5:
		{
			list = Change_element(list);
			break;
		}
		case 6:
		{
			list = Sort_list(list);
			break;
		}
		case 7:
		{
			Search_in_list(list);
			break;
		}
		case 8:
		{
			file_save(list);
			break;
		}
		case 9:
		{
			list = file_load(list);
			break;
		}
		case 10:
		{
			Five_rooms_debts(list);
			break;
		}
		case 11:
		{
			list = Delete_list(list);
			break;
		}
		case 12:
		{
			list = Delete_list(list);
			return 0;
		}
		default:
		{
			break;
		}
		}
	}
}

void press_any_key_function()
{
	printf("\nНажмите любую клавишу для продолжения\n");
	_getch();
}

void header_output()
{
	printf("\n Номер    |            |             |                |  Направление  |               | Номер    | Номер   | Долги по");
	printf("\n Зачетной |  Фамилия   |     Имя     |   Отчество     |   Обучения    |     Группа    | Общежит. | Комнаты | общежитию");
	printf("\n Книжки   |            |             |                |               |               |          |         |\n");
}

void Menu_fuctionction()
{
	system("cls");
	printf("\n-------------------------------------------------------------------------\n\t1  - Создать список студентов\n\t2  - Добавить элемент в список\n\t3  - Вывести список на экран\n\t4  - Удалить элемент из списка\n\t5  - Изменить элемент\n\t6  - Сортировать список\n");
	printf("\t7  - Поиск в списке\n\t8  - Сохранение в файл\n\t9  - Загрузка из файла\n\t10 - Вывести пять студентов с максимальными долгами за общежитие\n\t11 - Очистить список\n\t12 - Выход из программы\n-------------------------------------------------------------------------\n");
}

void Element_output(struct Student_list* this_list)
{
	printf("\n_____________________________________________________________________________________________________________\n");
	printf("\n %-8d | %-10s | %-11s | %-14s | %-13s | %-13s | %-8d | %-7d | %-11.2lf", this_list->this_student.book_number, this_list->this_student.surname, this_list->this_student.name, this_list->this_student.patronymic, this_list->this_student.studying_direction, this_list->this_student.groop, this_list->this_student.hostel_number, this_list->this_student.room_number, this_list->this_student.hoster_debts);
	printf("\n_____________________________________________________________________________________________________________\n");
}

void Element_output_for_page(struct Student_list* this_list)
{
	printf("\n %-8d | %-10s | %-11s | %-14s | %-13s | %-13s | %-8d | %-7d | %-11.2lf", this_list->this_student.book_number, this_list->this_student.surname, this_list->this_student.name, this_list->this_student.patronymic, this_list->this_student.studying_direction, this_list->this_student.groop, this_list->this_student.hostel_number, this_list->this_student.room_number, this_list->this_student.hoster_debts);
}

struct Student_list* Create_list(struct Student_list* this_list)
{
	system("cls");
	if (this_list)
	{
		printf("\nСписок не пуст\nНажмите 1 для очистки списка\nЛюбую другую цифру для продолжения\n");
		int clearlist = 0;
		while (1)
		{
			while (scanf_s("%d", &clearlist) != 1)
			{
				printf("\nОшибка, введите целое число\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			if (clearlist == 1)
			{
				this_list = Delete_list(this_list);
				break;
			}
			else
				break;
		}
	}
	int submenu_point = 1;
	while (1)
	{
		this_list = Add_element(this_list);
		printf("\nЗавершить ввод - 0\n");
		while (scanf_s("%d", &submenu_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		if (submenu_point == 0) break;
		else system("cls");
	}
	return this_list;
}

struct Student_list* Add_element(struct Student_list* this_list)
{
	system("cls");
	struct Student_list* temp = Memory_input(this_list);
	if (this_list == NULL) this_list = temp;
	else
	{
		printf("\n1 - Добавить в начало списка\n2 - Добавить в конец списка\n");
		int submenu_point = 0;
		while (1)
		{
			while (scanf_s("%d", &submenu_point) != 1)
			{
				printf("\nError\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			switch (submenu_point)
			{
			case 1:
			{
				return Add_to_left(this_list, temp);
			}
			case 2:
			{
				return Add_to_right(this_list, temp);
			}
			default:
			{
				break;
			}
			}
		}
	}
	return this_list;
}

int equal_book_number(struct Student_list* this_list, int book_number)
{
	if (!this_list)
		return book_number;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
		if (temp->this_student.book_number == book_number)
		{
			int min = this_list->this_student.book_number;
			for (struct Student_list* temp_min = this_list; temp_min != NULL; temp_min = temp_min->right)
				if (temp_min->this_student.book_number < min)
					min = temp_min->this_student.book_number;
			int count_exit = 0;
			while (1)
			{
				count_exit = 0;
				min++;
				for (struct Student_list* temp2 = this_list; temp2 != NULL; temp2 = temp2->right)
					if (temp2->this_student.book_number == min)
						count_exit = 1;
				if (!count_exit)
					return min;
			}
		}
	return book_number;
}

struct Student_list* Memory_input(struct Student_list* this_list)
{
	printf("\nВведите информацию о студенте:\n");
	struct Student_list* temp = (struct Student_list*)malloc(sizeof(struct Student_list));
	printf("\nНомер зачетной книжки:\n");
	while (scanf_s("%8d", &temp->this_student.book_number) != 1)
	{
		printf("\nОшибка, введите целое число\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	if (temp->this_student.book_number < 0)
	{
		temp->this_student.book_number = (-1) * temp->this_student.book_number;
	}
	temp->this_student.book_number = equal_book_number(this_list, temp->this_student.book_number);
	printf("\nФамилия:\n");
	while (scanf_s("%10s", temp->this_student.surname, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nИмя:\n");
	while (scanf_s("%11s", temp->this_student.name, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nОтчество:\n");
	while (scanf_s("%14s", temp->this_student.patronymic, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nНаправление обучения:\n");
	while (scanf_s("%13s", temp->this_student.studying_direction, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nГруппа:\n");
	while (scanf_s("%13s", temp->this_student.groop, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	printf("\nНомер общежития:\n");
	while (scanf_s("%8d", &temp->this_student.hostel_number) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	if (temp->this_student.hostel_number < 0)
	{
		temp->this_student.hostel_number = (-1) * temp->this_student.hostel_number;
	}
	printf("\nНомер комнаты:\n");
	while (scanf_s("%7d", &temp->this_student.room_number) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	if (temp->this_student.room_number < 0)
	{
		temp->this_student.room_number = (-1) * temp->this_student.room_number;
	}
	printf("\nДолги за общежитие:\n");
	while (scanf_s("%8lf", &temp->this_student.hoster_debts) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	if (temp->this_student.hoster_debts < 0)
	{
		temp->this_student.hoster_debts = (-1.f) * temp->this_student.hoster_debts;
	}
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

struct Student_list* Add_to_right(struct Student_list* this_list, struct Student_list* this_element)
{
	struct Student_list* kon = this_list;
	for (; kon->right != NULL; kon = kon->right);
	kon->right = this_element;
	this_element->left = kon;
	return this_list;
}

struct Student_list* Add_to_left(struct Student_list* this_list, struct Student_list* this_element)
{
	struct Student_list* kon = this_list;
	for (; kon->left != NULL; kon = kon->left);
	kon->left = this_element;
	this_element->right = kon;
	return this_element;
}

void Output_list(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return;
	}
	int submenu_point = 4;
	printf("\n1 - Поэлементный вывод\n2 - Вывести весь список\n3 - Постраничный вывод\n");
	while (1)
	{
		while (scanf_s("%d", &submenu_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (submenu_point)
		{
		case 1:
		{
			Output_by_elements(this_list);
			return;
		}
		case 2:
		{
			Output_all(this_list);
			return;
		}
		case 3:
		{
			Output_by_pages(this_list);
			return;
		}
		default:
		{
			break;
		}
		}
	}
}

int Output_page(struct Student_list* this_list, int page)
{
	struct Student_list* temp = this_list;
	for (int point = 1; point < page * 10 - 9; point++)
	{
		if (temp)
			temp = temp->right;
		else
			return 1;
	}
	if (!temp) return 1;
	header_output();
	printf("\n_______________________________________________________________________________________________________________");
	for (int count_elements = 1; count_elements <= 10; count_elements++)
	{
		if (temp)
		{
			Element_output_for_page(temp);
			temp = temp->right;
		}
		else
		{
			printf("\n_______________________________________________________________________________________________________________\n");
			return 0;
		}
	}
	printf("\n_______________________________________________________________________________________________________________\n");
	return 0;
}

void Output_by_pages(struct Student_list* this_list)
{
	system("cls");
	int page_number = 1;
	Output_page(this_list, page_number);
	int left_right_point = 0;
	while (1)
	{
		printf("\n1 - Налево\n2 - Направо\n3 - Выход из вывода\n");
		while (scanf_s("%d", &left_right_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (left_right_point)
		{
		case 1:
		{
			system("cls");
			if (page_number != 1) page_number--;
			Output_page(this_list, page_number);
			break;
		}
		case 2:
		{
			system("cls");
			page_number++;
			int error_count = Output_page(this_list, page_number);
			if (error_count == 1)
			{
				system("cls");
				page_number--;
				Output_page(this_list, page_number);
			}
			break;
		}
		case 3:
		{
			system("cls");
			return;
		}
		default:
		{
			break;
		}
		}
	}
	return;
}

void Output_all(struct Student_list* this_list)
{
	system("cls");
	printf("\nВывести список\n1 - Слева направо\n2 - Справа налево\n");
	int left_right_point;
	while (1)
	{
		while (scanf_s("%d", &left_right_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (left_right_point)
		{
		case 1:
		{
			system("cls");
			struct Student_list* temp = this_list;
			header_output();
			while (temp != NULL)
			{
				Element_output_for_page(temp);
				temp = temp->right;
			}
			press_any_key_function();
			return;
		}
		case 2:
		{
			system("cls");
			struct Student_list* temp = this_list;
			for (; temp->right != NULL; temp = temp->right);
			header_output();
			while (temp != NULL)
			{
				Element_output_for_page(temp);
				temp = temp->left;
			}
			press_any_key_function();
			return;
		}
		default:
		{
			break;
		}
		}
	}
}

void Output_by_elements(struct Student_list* this_list)
{
	struct Student_list* temp = this_list;
	system("cls");
	header_output();
	Element_output(temp);
	int left_right_point = 0;
	while (1)
	{
		printf("\n1 - Следующий налево\n2 - Следующий направо\n3 - Выход из вывода\n");
		while (scanf_s("%d", &left_right_point) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (left_right_point)
		{
		case 1:
		{
			if (temp->left != NULL)
			{
				temp = temp->left;
				system("cls");
				header_output();
				Element_output(temp);
			}
			else
			{
				printf("\nЭто последний элемент слева\n");
			}
			break;
		}
		case 2:
		{
			if (temp->right != NULL)
			{
				temp = temp->right;
				system("cls");
				header_output();
				Element_output(temp);
			}
			else
			{
				printf("\nЭто последний элемент справа\n");
			}
			break;
		}
		case 3:
		{
			system("cls");
			return;
		}
		default:
		{
			break;
		}
		}
	}
	return;
}

struct Student_list* Delete_element(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return this_list;
	}
	printf("\nВведите номер зачетной книжки студента, которого необходимо удалить\n");
	int deleted_book_number;
	while (scanf_s("%d", &deleted_book_number) != 1)
	{
		printf("\nОшибка, введите целое число\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	if (this_list->this_student.book_number == deleted_book_number)
	{
		struct Student_list* temp = this_list->right;
		if (temp != NULL) temp->left = NULL;
		free(this_list);
		printf("\nЭлемент был успешно удален\n");
		press_any_key_function();
		return temp;
	}
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.book_number == deleted_book_number)
		{
			temp->left->right = temp->right;
			if (temp->right != NULL)
			{
				temp->right->left = temp->left;
			}
			else
			{
				temp->left->right = NULL;
			}
			free(temp);
			printf("\nЭлемент был успешно удален\n");
			press_any_key_function();
			return this_list;
		}
	}
	printf("\nЭлемент не найден\n");
	press_any_key_function();
	return this_list;
}

struct Student_list* Change_element(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return this_list;
	}
	printf("\nВведите номер зачетной книжки студента, которого нужно изменить\n");
	int change_book_number;
	while (scanf_s("%d", &change_book_number) != 1)
	{
		printf("\nОшибка, введите целое число\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.book_number == change_book_number)
		{
			printf("\nНайденный элемент:\n");
			Element_output(temp);
			printf("\n1 - Найти следующий\n2 - Изменить этот\n");
			int next_element = 0;
			while (scanf_s("%d", &next_element) != 1)
			{
				printf("\nОшибка, введите целое число\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			switch (next_element)
			{
			case 1:
			{
				system("cls");
				continue;
			}
			case 2:
			{
				int changing_element = 0;
				printf("\nКакой элемент необходимо изменить?\n1 - Номер зачетной книжки\n2 - Фамилия\n3 - Имя\n4 - Отчество\n5 - Направление обучения\n6 - Группа\n7 - Номер общежития\n8 - Номер комнаты\n9 - Долги за общежитие\n");
				while (1)
				{
					while (scanf_s("%d", &changing_element) != 1)
					{
						printf("\nОшибка, введите целое число\n");
						fseek(stdin, 0, SEEK_END);
					}
					fseek(stdin, 0, SEEK_END);
					system("cls");
					switch (changing_element)
					{
					case 1:
					{
						printf("\nВведите новый номер зачетной книжки:\n");
						while (scanf_s("%8d", &temp->this_student.book_number) != 1)
						{
							printf("\nОшибка, введите целое число\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						temp->this_student.book_number = equal_book_number(this_list, temp->this_student.book_number);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 2:
					{
						printf("\nВведите новую фамилию:\n");
						while (scanf_s("%10s", &temp->this_student.surname, 10) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 3:
					{
						printf("\nВведите новое имя:\n");
						while (scanf_s("%11s", &temp->this_student.name, 11) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 4:
					{
						printf("\nВведите новое отчество:\n");
						while (scanf_s("%14s", &temp->this_student.patronymic, 14) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 5:
					{
						printf("\nВведите новое направление обучения:\n");
						while (scanf_s("%13s", &temp->this_student.studying_direction, 13) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 6:
					{
						printf("\nВведите новую группу:\n");
						while (scanf_s("%13s", &temp->this_student.groop, 13) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 7:
					{
						printf("\nВведите новый номер общежития:\n");
						while (scanf_s("%8d", &temp->this_student.hostel_number) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 8:
					{
						printf("\nВведите новый номер комнаты:\n");
						while (scanf_s("%7d", &temp->this_student.room_number) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					case 9:
					{
						printf("\nВведите новые долги за общежитие:\n");
						while (scanf_s("%8lf", &temp->this_student.hoster_debts) != 1)
						{
							printf("\nОшибка\n");
							fseek(stdin, 0, SEEK_END);
						}
						fseek(stdin, 0, SEEK_END);
						printf("\nНовый элемент:\n");
						Element_output(temp);
						press_any_key_function();
						return this_list;
					}
					default:
					{
						break;
					}
					}
				}
			}
			default:
				break;
			}
		}
	}
	printf("\nСтудент не найден\n");
	press_any_key_function();
	return this_list;
}

struct Student_list* Sort_list(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return this_list;
	}
	if (this_list->right == NULL)
	{
		printf("\nСписок содержит только один элемент\n");
		press_any_key_function();
		return this_list;
	}
	int sort_type = 0;
	while (1)
	{
		printf("\n1 - Сортировать от меньшего к большему\n2 - Сортировать от большего к меньшему\n3 - Выход в меню\n");
		while (scanf_s("%d", &sort_type) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		system("cls");
		switch (sort_type)
		{
		case 1:
		{
			for (struct Student_list* temp1 = this_list; temp1 != NULL; temp1 = temp1->right)
			{
				for (struct Student_list* temp2 = temp1->right; temp2 != NULL; temp2 = temp2->right)
				{
					if (temp1->this_student.book_number > temp2->this_student.book_number)
					{
						if (temp1->right == temp2)
						{
							struct Student_list* temp3 = temp1->left, * temp4 = temp2->right;
							temp2->left = temp3;
							if (temp3) temp3->right = temp2;
							temp1->left = temp2;
							temp2->right = temp1;
							temp1->right = temp4;
							if (temp4) temp4->left = temp1;
							struct Student_list* start_value = this_list;
							for (; start_value->left != NULL; start_value = start_value->left);
							temp1 = start_value;
							temp2 = start_value;
						}
						else
						{
							struct Student_list* temp3 = temp1->left, * temp4 = temp2->left, * temp5 = temp1->right, * temp6 = temp2->right;
							temp2->left = temp3;
							if (temp3) 	temp3->right = temp2;
							temp2->right = temp5;
							if (temp5) temp5->left = temp2;
							temp1->left = temp4;
							if (temp4) temp4->right = temp1;
							temp1->right = temp6;
							if (temp6) temp6->left = temp1;
							struct Student_list* start_value = this_list;
							for (; start_value->left != NULL; start_value = start_value->left);
							temp1 = start_value;
							temp2 = start_value;
						}
					}
				}
			}
			printf("\nСортировка была успешно завершена\n");
			press_any_key_function();
			struct Student_list* return_value = this_list;
			for (; return_value->left != NULL; return_value = return_value->left);
			return return_value;
		}
		case 2:
		{
			for (struct Student_list* temp1 = this_list; temp1 != NULL; temp1 = temp1->right)
			{
				for (struct Student_list* temp2 = temp1->right; temp2 != NULL; temp2 = temp2->right)
				{
					if (temp1->this_student.book_number < temp2->this_student.book_number)
					{
						if (temp1->right == temp2)
						{
							struct Student_list* temp3 = temp1->left, * temp4 = temp2->right;
							temp2->left = temp3;
							if (temp3) temp3->right = temp2;
							temp1->left = temp2;
							temp2->right = temp1;
							temp1->right = temp4;
							if (temp4) temp4->left = temp1;
							struct Student_list* start_value = this_list;
							for (; start_value->left != NULL; start_value = start_value->left);
							temp1 = start_value;
							temp2 = start_value;
						}
						else
						{
							struct Student_list* temp3 = temp1->left, * temp4 = temp2->left, * temp5 = temp1->right, * temp6 = temp2->right;
							temp2->left = temp3;
							if (temp3) 	temp3->right = temp2;
							temp2->right = temp5;
							if (temp5) temp5->left = temp2;
							temp1->left = temp4;
							if (temp4) temp4->right = temp1;
							temp1->right = temp6;
							if (temp6) temp6->left = temp1;
							struct Student_list* start_value = this_list;
							for (; start_value->left != NULL; start_value = start_value->left);
							temp1 = start_value;
							temp2 = start_value;
						}
					}
				}
			}
			printf("\nСортировка была успешно завершена\n");
			press_any_key_function();
			struct Student_list* return_value = this_list;
			for (; return_value->left != NULL; return_value = return_value->left);
			return return_value;
		}
		case 3:
		{
			return this_list;
		}
		default:
		{
			break;
		}
		}
	}
	return this_list;
}

void search_by_book_number(struct Student_list* this_list)
{
	printf("\nВведите номер зачетной книжки\n");
	int this_book_number = 0;
	while (scanf_s("%d", &this_book_number) != 1)
	{
		printf("\nОшибка, введите целое число\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.book_number == this_book_number)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
}

void search_by_surname(struct Student_list* this_list)
{
	printf("\nВведите фамилию\n");
	char this_surname[20];
	while (scanf_s("%s", &this_surname, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (strcmp(temp->this_student.surname, this_surname) == 0)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
}

void search_by_name(struct Student_list* this_list)
{
	printf("\nВведите имя\n");
	char this_name[20];
	while (scanf_s("%s", &this_name, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (strcmp(temp->this_student.name, this_name) == 0)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_patronym(struct Student_list* this_list)
{
	printf("\nВведите отчество\n");
	char this_patronymic[20];
	while (scanf_s("%s", &this_patronymic, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (strcmp(temp->this_student.patronymic, this_patronymic) == 0)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_direction(struct Student_list* this_list)
{
	printf("\nВведите направление обучения\n");
	char this_studying_direction[20];
	while (scanf_s("%s", &this_studying_direction, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (strcmp(temp->this_student.studying_direction, this_studying_direction) == 0)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_groop(struct Student_list* this_list)
{
	printf("\nВведите группу\n");
	char this_groop[20];
	while (scanf_s("%s", &this_groop, 20) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (strcmp(temp->this_student.groop, this_groop) == 0)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_hostel(struct Student_list* this_list)
{
	printf("\nВведите номер общежития\n");
	int this_hostel_number = 0;
	while (scanf_s("%d", &this_hostel_number) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.hostel_number == this_hostel_number)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_room(struct Student_list* this_list)
{
	printf("\nВведите номер комнаты\n");
	int this_room_number = 0;
	while (scanf_s("%d", &this_room_number) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.room_number == this_room_number)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void search_by_debts(struct Student_list* this_list)
{
	printf("\nВведите долги за общежитие\n");
	double this_hoster_debts = 0;
	while (scanf_s("%lf", &this_hoster_debts) != 1)
	{
		printf("\nОшибка\n");
		fseek(stdin, 0, SEEK_END);
	}
	fseek(stdin, 0, SEEK_END);
	int found_count = 0;
	for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
	{
		if (temp->this_student.hoster_debts == this_hoster_debts)
		{
			if (!found_count)
			{
				header_output();
				printf("----------------------------------------------------------------------------------------------------------------");
			}
			Element_output_for_page(temp);
			found_count = 1;
		}
	}
	if (!found_count) printf("\nНе найдено\n");
	else
		printf("\n----------------------------------------------------------------------------------------------------------------\n");
	return;
}

void Search_in_list(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return;
	}
	int search_number = 0;
	while (1)
	{
		printf("\nИскать по какому полю?\n1 - Номер зачетной книжки\n2 - Фамилия\n3 - Имя\n4 - Отчество\n5 - Направление обучения\n6 - Группа\n7 - Номер общежития\n8 - Номер комнаты\n9 - Долги за общежитие\n\n");
		while (scanf_s("%d", &search_number) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		system("cls");
		switch (search_number)
		{
		case 1:
		{
			search_by_book_number(this_list);
			press_any_key_function();
			return;
		}
		case 2:
		{
			search_by_surname(this_list);
			press_any_key_function();
			return;
		}
		case 3:
		{
			search_by_name(this_list);
			press_any_key_function();
			return;
		}
		case 4:
		{
			search_by_patronym(this_list);
			press_any_key_function();
			return;
		}
		case 5:
		{
			search_by_direction(this_list);
			press_any_key_function();
			return;
		}
		case 6:
		{
			search_by_groop(this_list);
			press_any_key_function();
			return;
		}
		case 7:
		{
			search_by_hostel(this_list);
			press_any_key_function();
			return;
		}
		case 8:
		{
			search_by_room(this_list);
			press_any_key_function();
			return;
		}
		case 9:
		{
			search_by_debts(this_list);
			press_any_key_function();
			return;
		}
		default:
		{
			break;
		}
		}
	}
}

void file_save(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return;
	}
	int use_file;
	while (1)
	{
		printf("\n1 - Использовать бинарный файл\n2 - Использовать текстовый файл\n");
		while (scanf_s("%d", &use_file) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		system("cls");
		switch (use_file)
		{
		case 1:
		{
			FILE* in;
			char file_name[255];
			printf("\nВведите полный путь файла (используйте два \\\\ вместо одного \\)\n");
			while (scanf_s("%s", file_name, 255) != 1)
			{
				printf("\nОшибка чтения, вероятно название слишком длинное\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			int dot_position = 0, count_dot = 0;
			for (int i = 0; file_name[i] != '\0'; i++)
			{
				if (file_name[i] == '.')
				{
					dot_position = i;
					count_dot++;
				}
			}
			if (count_dot == 1 && (sizeof(file_name) - (dot_position + 1) >= 4))
			{
				file_name[dot_position + 1] = 'd';
				file_name[dot_position + 2] = 'a';
				file_name[dot_position + 3] = 't';
				file_name[dot_position + 4] = '\0';
			}
			else if (count_dot == 0 && (sizeof(file_name) - strlen(file_name) > 5))
			{
				strcat(file_name, ".dat\0");
			}
			else
			{

				printf("\nНедоступное имя файла\n");
				press_any_key_function();
				return;
			}
			int submenu_point = 0;
			printf("\n1 - Открыть файл\n2 - Создать файл или Очистить содержимое уже существующего\n");
			while (1)
			{
				while (scanf_s("%d", &submenu_point) != 1)
				{
					printf("\nОшибка, введите целое число\n");
					fseek(stdin, 0, SEEK_END);
				}
				fseek(stdin, 0, SEEK_END);
				if (submenu_point == 1)
				{
					fopen_s(&in, file_name, "r+b");
					break;
				}
				if (submenu_point == 2)
				{
					fopen_s(&in, file_name, "w+b");
					break;
				}
			}
			if (!in)
			{
				printf("\nОшибка открытия файла\n");
				press_any_key_function();
				return;
			}
			printf("\nФайл был успешно открыт\n");
			struct Student_list* temp = this_list;
			fseek(in, 0, SEEK_END);
			while (temp != NULL)
			{
				fwrite(&temp->this_student, sizeof(struct Student), 1, in);
				temp = temp->right;
			}
			fclose(in);
			printf("\nЗапись в файл прошла успешно\n");
			press_any_key_function();
			return;
		}
		case 2:
		{
			FILE* in;
			char file_name[255];
			printf("\nВведите полный путь файла (используйте два \\\\ вместо одного \\)\n");
			while (scanf_s("%s", file_name, 255) != 1)
			{
				printf("\nОшибка чтения, вероятно название слишком длинное\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			int dot_position = 0, count_dot = 0;
			for (int i = 0; file_name[i] != '\0'; i++)
			{
				if (file_name[i] == '.')
				{
					dot_position = i;
					count_dot++;
				}
			}
			if (count_dot == 1 && (sizeof(file_name) - (dot_position + 1) >= 4))
			{
				file_name[dot_position + 1] = 't';
				file_name[dot_position + 2] = 'x';
				file_name[dot_position + 3] = 't';
				file_name[dot_position + 4] = '\0';
			}
			else if (count_dot == 0 && (sizeof(file_name) - strlen(file_name) > 5))
			{
				strcat(file_name, ".txt\0");
			}
			else
			{

				printf("\nНедоступное имя файла\n");
				press_any_key_function();
				return;
			}
			int submenu_point = 0;
			printf("\n1 - Открыть файл\n2 - Создать файл или Очистить содержимое уже существующего\n");
			while (1)
			{
				while (scanf_s("%d", &submenu_point) != 1)
				{
					printf("\nОшибка, введите целое число\n");
					fseek(stdin, 0, SEEK_END);
				}
				fseek(stdin, 0, SEEK_END);
				if (submenu_point == 1)
				{
					fopen_s(&in, file_name, "r+");
					break;
				}
				if (submenu_point == 2)
				{
					fopen_s(&in, file_name, "w+");
					break;
				}
			}
			if (!in)
			{
				printf("\nОшибка открытия файла\n");
				press_any_key_function();
				return;
			}
			printf("\nФайл был успешно открыт\n");
			struct Student_list* temp = this_list;
			fseek(in, 0, SEEK_END);
			while (temp != NULL) {
				fprintf(in, "\n%d\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n%lf\n", temp->this_student.book_number, temp->this_student.surname, temp->this_student.name, temp->this_student.patronymic, temp->this_student.studying_direction, temp->this_student.groop, temp->this_student.hostel_number, temp->this_student.room_number, temp->this_student.hoster_debts);
				temp = temp->right;
			}
			fclose(in);
			printf("\nЗапись в файл прошла успешно\n");
			press_any_key_function();
			return;
		}
		default:
		{
			break;
		}
		}
	}
}

struct Student_list* file_load(struct Student_list* this_list)
{
	system("cls");
	if (this_list)
	{
		printf("\nСписок не пуст\nНажмите 1 чтобы очистить список\nЛюбое другое число для продолжения\n");
		int clearlist = 0;
		while (1)
		{
			while (scanf_s("%d", &clearlist) != 1)
			{
				printf("\nОшибка, введите целое число\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			if (clearlist == 1)
			{
				this_list = Delete_list(this_list);
				break;
			}
			else
				break;
		}
	}
	system("cls");
	int use_file;
	printf("\n1 - Использовать бинарный файл\n2 - Использовать текстовый фалй\n3 - Выход в меню\n");
	while (1)
	{
		while (scanf_s("%d", &use_file) != 1)
		{
			printf("\nОшибка, введите целое число\n");
			fseek(stdin, 0, SEEK_END);
		}
		fseek(stdin, 0, SEEK_END);
		switch (use_file)
		{
		case 1:
		{
			system("cls");
			FILE* in;
			char file_name[255];
			printf("\nВведите полный путь файла (используйте два \\\\ вместо одного \\)\n");
			while (scanf_s("%s", file_name, 255) != 1)
			{
				printf("\nОшибка, вероятно имя файла слишком длинное\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			int dot_position = 0, count_dot = 0;
			for (int i = 0; file_name[i] != '\0'; i++)
			{
				if (file_name[i] == '.')
				{
					dot_position = i;
					count_dot++;
				}
			}
			if (count_dot == 1 && (sizeof(file_name) - (dot_position + 1) >= 4))
			{
				file_name[dot_position + 1] = 'd';
				file_name[dot_position + 2] = 'a';
				file_name[dot_position + 3] = 't';
				file_name[dot_position + 4] = '\0';
			}
			else if (count_dot == 0 && (sizeof(file_name) - strlen(file_name) > 5))
			{
				strcat(file_name, ".dat\0");
			}
			else
			{

				printf("\nНедоступное имя файла\n");
				press_any_key_function();
				return this_list;
			}
			fopen_s(&in, file_name, "r+b");
			if (!in) fopen_s(&in, file_name, "w+b");
			if (!in)
			{
				printf("\nОшибка открытия файла\n");
				press_any_key_function();
				return this_list;
			}
			printf("\nФайл был успешно открыт\n");
			size_t end_of_file = 2;
			rewind(in);
			while (1)
			{
				struct Student_list* temp = (struct Student_list*)malloc(sizeof(struct Student_list));
				end_of_file = fread(&temp->this_student, sizeof(struct Student), 1, in);
				if (end_of_file < 1)
				{
					free(temp);
					break;
				}
				temp->this_student.book_number = equal_book_number(this_list, temp->this_student.book_number);
				temp->left = NULL;
				temp->right = NULL;
				if (this_list == NULL) this_list = temp;
				else
				{
					struct Student_list* kon = this_list;
					for (; kon->right != NULL; kon = kon->right);
					kon->right = temp;
					temp->left = kon;
				}
			}
			fclose(in);
			printf("\nЗапись из файла прошла успешно\n");
			press_any_key_function();
			return this_list;
		}
		case 2:
		{
			system("cls");
			FILE* in;
			char file_name[255];
			printf("\nВведите полный путь файла (используйте два \\\\ вместо одного \\)\n");
			while (scanf_s("%s", file_name, 255) != 1)
			{
				printf("\nОшибка, вероятно имя файла слишком длинное\n");
				fseek(stdin, 0, SEEK_END);
			}
			fseek(stdin, 0, SEEK_END);
			int dot_position = 0, count_dot = 0;
			for (int i = 0; file_name[i] != '\0'; i++)
			{
				if (file_name[i] == '.')
				{
					dot_position = i;
					count_dot++;
				}
			}
			if (count_dot == 1 && (sizeof(file_name) - (dot_position + 1) >= 4))
			{
				file_name[dot_position + 1] = 't';
				file_name[dot_position + 2] = 'x';
				file_name[dot_position + 3] = 't';
				file_name[dot_position + 4] = '\0';
			}
			else if (count_dot == 0 && (sizeof(file_name) - strlen(file_name) > 5))
			{
				strcat(file_name, ".txt\0");
			}
			else
			{

				printf("\nНедоступное имя файла\n");
				press_any_key_function();
				return this_list;
			}
			fopen_s(&in, file_name, "r+b");
			if (!in) fopen_s(&in, file_name, "w+b");
			if (!in)
			{
				printf("\nОшибка открытия файла\n");
				press_any_key_function();
				return this_list;
			}
			printf("\nФайл был успешно открыт\n");
			rewind(in);
			while (1)
			{
				struct Student_list* temp = (struct Student_list*)malloc(sizeof(struct Student_list));
				if (fscanf_s(in, "%8d", &temp->this_student.book_number) != 1)
				{
					free(temp);
					break;
				}
				temp->this_student.book_number = equal_book_number(this_list, temp->this_student.book_number);
				if (fscanf_s(in, "%10s", temp->this_student.surname, 20) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%11s", temp->this_student.name, 20) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%14s", temp->this_student.patronymic, 20) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%13s", temp->this_student.studying_direction, 20) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%13s", temp->this_student.groop, 20) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%8d", &temp->this_student.hostel_number) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%7d", &temp->this_student.room_number) != 1)
				{
					free(temp);
					break;
				}
				if (fscanf_s(in, "%8lf", &temp->this_student.hoster_debts) != 1)
				{
					free(temp);
					break;
				}
				temp->left = NULL;
				temp->right = NULL;
				if (this_list == NULL) this_list = temp;
				else
				{
					struct Student_list* kon = this_list;
					for (; kon->right != NULL; kon = kon->right);
					kon->right = temp;
					temp->left = kon;
				}
			}
			fclose(in);
			printf("\nЗапись из файла прошла успешно\n");
			press_any_key_function();
			return this_list;
		}
		case 3:
		{
			return this_list;
		}
		default:
		{
			break;
		}
		}
	}
	return this_list;
}

void Five_rooms_debts(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return;
	}
	struct Student_list* min = this_list, * this_min = this_list;
	double imin = this_list->this_student.hoster_debts;
	for (; min != NULL; min = min->right)
	{
		if (min->this_student.hoster_debts < imin)
		{
			imin = min->this_student.hoster_debts;
			this_min = min;
		}
	}
	printf("\nПять студентов с максимальными долгами по общежитию:\n");
	printf("\n------------------------------------------------------\n");
	struct Student_list* last_max = NULL;
	for (int count_five = 0; count_five < 5; count_five++)
	{
		struct Student_list* temp_max = this_min;
		for (struct Student_list* temp = this_list; temp != NULL; temp = temp->right)
		{
			if (count_five < 1)
			{
				if (temp->this_student.hoster_debts > temp_max->this_student.hoster_debts)
				{
					temp_max = temp;
				}
			}
			else
			{
				if (temp->this_student.hoster_debts < last_max->this_student.hoster_debts && temp->this_student.hoster_debts > temp_max->this_student.hoster_debts)
				{
					temp_max = temp;
				}
			}
		}
		if (last_max == temp_max)
		{
			printf("\n------------------------------------------------------\n");
			printf("\nВ списке нет пяти студентов\n");
			press_any_key_function();
			return;
		}
		last_max = temp_max;
		Element_output_for_page(temp_max);
	}
	printf("\n------------------------------------------------------\n");
	press_any_key_function();
}

struct Student_list* Delete_list(struct Student_list* this_list)
{
	system("cls");
	if (this_list == NULL)
	{
		printf("\nСписок пуст\n");
		press_any_key_function();
		return this_list;
	}
	while (this_list != NULL)
	{
		struct Student_list* temp = this_list;
		this_list = temp->right;
		free(temp);
	}
	printf("\n\nСписок был успешно очищен\n\n");
	press_any_key_function();
	return this_list;
}
