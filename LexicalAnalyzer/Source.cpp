#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

struct Lex																									//структура лексемы
{
	char* lex;																								//хранит лексему
	int type;																								//хранит тип лексемы
};

//--------------------------------------Лексический анализатор---------------------------------//

enum LexStates { S, vl_, id_, ao_, eq_, co1, co2, co3, sc_, F, E };											//состояния перехода
enum TypesOfLex { vl, id, fo, ao, eq, co, ne, to, sc, lo, wl, no };											//перечисление типов лексем

const char Types_name[12][3]{ "vl", "id", "fo", "ao", "eq", "co", "ne","to", "sc",  "lo","wl", "no" };		//названия типов лексем (используем двумерный массив, чтобы затем выводить их)

LexStates matrix_lex[9][11]																					//таблица автомата лексического анализатора
{																											//S-начальное состояние
//					S		vl_		id_		ao_		eq_		co1		co2		co3		sc_		F		E		//id_-состояние идентификатора
	/*0)alpha*/		id_,	E,		id_,	F,		F,		F,		F,		F,		F,		F,		E,		//vl_-состояние константы
	/*1)0...9*/		vl_,	vl_,	id_,	F,		F,		F,		F,		F,		F,		F,		E,		//ao_-состояние + или -
	/*2 + -/ */		ao_,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//eq_-состояние =
	/*3)  <  */		co1,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//co1-состояние, из которого получается <>, <= или <
	/*4)  >  */		co2,	F,		F,		F,		F,		co3,	F,		F,		F,		F,		F,		//co2-состояние, из которого получается > или >=
	/*5)  =  */		eq_,	F,		F,		F,		F,		co3,	co3,	F,		F,		F,		F,		//co3-состояние <>, <=, <,> или >=
	/*6)  ;  */		sc_,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//co3-состояние <>, <=, <,> или >=
	/*7)empty*/		S,		F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//sc_-состояние ;
	/*8)other*/		E,		E,		E,		F,		F,		F,		F,		F,		F,		F,		E		//F-конечное состояние
																											//E-ошибочное состояние
};


int place_in_the_matrix_lex(char cur_char)																	// получение значения строки матрицы
{
	if (isalpha(cur_char))
	{
		return 0;
	}

	else if (cur_char >= '0' && cur_char <= '9')
	{
		return 1;
	}

	else if (cur_char == '+' || cur_char == '-' || cur_char == '*' || cur_char == '/')
	{
		return 2;
	}

	else if (cur_char == '<')
	{
		return 3;
	}

	else if (cur_char == '>')
	{
		return 4;
	}

	else if (cur_char == '=')
	{
		return 5;
	}

	else if (cur_char == ';')
	{
		return 6;
	}

	else if (iscntrl(cur_char) || isspace(cur_char))
	{
		return 7;
	}

	return 8;
}


void add(char*& str, vector<Lex>& v, unsigned int beginning, unsigned int running, LexStates last)			//добавление лексемы в вектор
{
	Lex token;																								//объект для взаимодействия со структурой и хранения лексемы

	char* word = new char[running - beginning + 1];

	unsigned int i = 0;

	for (unsigned int j = beginning; j < running; ++i, ++j)
	{
		word[i] = str[j];
	}

	word[i] = '\0';

	token.lex = word;																						//заполняем поле лексемы

	if (last == id_)																						//если лексема является идентификатором 
	{
		if ((running - beginning) > 5)																		//проверяем, чтобы она содержала в себе не более 5 символов
		{
			token.type = wl;																				//если условие не выполняется, то лексема меняет свой тип на некорректную
		}

		else if (!strcmp(token.lex, "for"))		token.type = fo;
		else if (!strcmp(token.lex, "to"))		token.type = to;
		else if (!strcmp(token.lex, "next"))	token.type = ne;											//если лексема является ключевым словом

		else if (!strcmp(token.lex, "and") || !strcmp(token.lex, "or")) token.type = lo;
		else if (!strcmp(token.lex, "not"))		token.type = no;

		else token.type = id;																				//если лексема не является ключевым словом и не содержит более 5 символов, то она остается идентификатором
	}

	else if (last == vl_)																					//если лексема является константой
	{
		if (atoi(token.lex) > 32768)																		//проверяем, является ли она числом, большим 32767
		{
			token.type = wl;																				//если условие выполняется, то лексема меняет свой тип на некорректную
		}

		else token.type = vl;																				//если лексема не больше 32767, то она остается константой
	}

	else if (last == co1 || last == co2 || last == co3)														//<> > < <= >=
	{
		token.type = co;
	}

	else token.type = last;																	//если же лексема не была ни идентификатором, ни константой, то она остается со своим типом
																							//который был определен еще при работе автомата
	v.push_back(token);
}


void LexAnalysis(char*& str, vector<Lex>& v)												 //передаем строку файла и вектор, в который будет добавлять лексемы, по ссылке
{
	unsigned int running = 0;
	unsigned int beginning;

	char cur_char = str[0];

	LexStates state = S;
	LexStates last;

	while (cur_char != '\0')
	{

		cur_char = str[running];

		if (state == S)
		{
			beginning = running;
		}

		last = state;																		//необходимо запомнить предыдущее состояние, чтоыбы затем определить тип лексемы, после остановки работы автомата
		state = matrix_lex[place_in_the_matrix_lex(cur_char)][state];

		if (state == F)																		//если мы выделили лексему и остановили работу автомата, то
		{
			add(str, v, beginning, running, last);

			state = S;																		//после обработки лексемы и помещения ее в вектор, меняем состояние автомата на начальное 
																							//и продолжаем отыскивать лексемы, пока не наткнемся на терминирующий ноль			
		}

		else ++running;																		//если мы еще не достигли конечного состояния, итерируем счетчик

	}
}

//--------------------------------------Синтаксический анализатор---------------------------------//

enum SynStates { _S, _id, _eq, aex0, aex1, aex2, _E };										//состояния перехода

SynStates matrix_syn[10][7]																	//таблица автомата синтаксического анализатора
{																							//S-начальное состояние
	//				_S		_id		_eq		aex0	aex1	aex2	_E						//id-состояние ожидания константы
	/*0) +/* */		_E,		_E,		_E,		_E,		aex2,	_E,		_E,						//eq-состояние ожидания равно
	/*1)  -  */		_E,		_E,		_E,		aex2,	aex2,	_E,		_E,						//aex0-состояние ожидания минуса, переменной или константы
	/*2  id  */		_E,		_eq,	_E,		aex1,	_eq,	aex1,	_E,						//aex1-состояние ожидания, которое зависит от знаечния флага
	/*3) vl  */		_E,		_E,		_E,		aex1,	_E,		aex1,	_E,						//если flag = 0, то ожидает арифметическую операцию или to
	/*4)  =  */		_E,		_E,		aex0,	_E,		_E,		_E,		_E,						//если flag = 1, то ожидает арифметическую операцию или переменную
	/*5)  ;  */		_E,		_E,		_E,		_E,		_id,	_E,		_E,						//если flag = 2, то ожидает арифметическую операцию, точку с запятой или next
	/*6  fo  */		_id,	_E,		_E,		_E,		_E,		_E,		_E,						//aex2-состояние ожидания переменной или константы					
	/*7) to  */		_E,		_E,		_E,		_E,		aex0,	_E,		_E,						//E-ошибочное состояние					
	/*8) next*/		_E,		_E,		_E,		_E,		_S,		_E,		_E,
	/*9) other*/	_E,		_E,		_E,		_E,		_E,		_E,		_E

};


int place_in_the_matrix_syn(char* cur_type, unsigned int& flag, SynStates last)				//получение значения строки матрицы
{
	if (!strcmp(cur_type, "ao"))
	{
		return 0;
	}

	else if (!strcmp(cur_type, "_a"))
	{
		return 1;
	}

	else if (!strcmp(cur_type, "id"))
	{
		if (last == aex1)
		{
			if (flag == 1)
			{
				flag = 2; return 2;
			}

			return 9;
		}
		return 2;
	}

	else if (!strcmp(cur_type, "vl"))
	{
		return 3;
	}

	else if (!strcmp(cur_type, "eq"))
	{
		return 4;
	}

	else if (!strcmp(cur_type, "sc"))
	{
		if (flag == 2)
		{
			return 5;
		}
		return 9;
	}

	else if (!strcmp(cur_type, "fo"))
	{
		return 6;
	}

	else if (!strcmp(cur_type, "to"))
	{
		if (flag == 0)
		{
			flag = 1; return 7;
		}
		return 9;
	}

	else if (!strcmp(cur_type, "ne"))
	{
		if (flag == 2)
		{
			flag = 0; return 8;
		}
		return 9;
	}

	return 9;
}

void SynAnalysis(ofstream& fout, vector<Lex>& v)											//в синтаксический анализатор передаем поток помещения в файл
{
	unsigned int running = 0;
	unsigned int flag = 0;																	//флаг для корректного перехода состояний
	char cur_type[3];

	SynStates state = _S;
	SynStates last = state;


	while (running != v.size() && state != _E)
	{

		last = state;

		strcpy_s(cur_type, Types_name[v[running].type]);

		if (!strcmp(cur_type, "ao"))														//минус ищем отдельно, чтобы отработать отрицательные константы 
		{
			if (!strcmp(v[running].lex, "-"))strcpy_s(cur_type, "_a");
		}

		state = matrix_syn[place_in_the_matrix_syn(cur_type, flag, last)][state];

		++running;
	}


	if (state != _S)																		//мы вернемся в начальное состояние, если все будет в порядке
	{
		if (state != _E)
		{
			fout << running << ' ';
			cout << running << ' ';
			flag++;

			switch (state)
			{
			case _S: fout << "fo"; cout << "fo"; break;
			case _id: fout << "id"; cout << "id"; break;
			case _eq: fout << "eq"; cout << "eq"; break;
			case aex0:
			case aex2: fout << "id vl"; cout << "id vl"; break;
			case aex1:

				if (!flag)
				{
					fout << "ao to";
					cout << "ao to";
				}

				else if (flag == 1)
				{
					fout << "ao id";
					cout << "ao id";
				}

				else
				{
					fout << "ao ne sc";
					cout << "ao ne sc";
				}
			case _E: break;
			}

		}

		else																					//если вектор закончился и мы не зашли в ошибочное состояние
		{
			fout << running - 1 << ' ';
			cout << running - 1 << ' ';

			switch (last)
			{
			case _S: fout << "fo"; cout << "fo"; break;
			case _id: fout << "id"; cout << "id"; break;
			case _eq: fout << "eq"; cout << "eq"; break;
			case aex0:
			case aex2: fout << "id vl"; cout << "id vl"; break;
			case aex1:

				if (!flag)
				{
					fout << "ao to";
					cout << "ao to";
				}

				else if (flag == 1)
				{
					fout << "ao id";
					cout << "ao id";
				}

				else
				{
					fout << "ao ne sc";
					cout << "ao ne sc";
				}
			case _E: break;
			}
		}

	}

	else { fout << "OK"; cout << "OK"; }

}





void print(ofstream& fout, vector<Lex>& v)
{
	for (unsigned int i = 0; i < v.size(); ++i)
	{
		fout << v[i].lex << "[" << Types_name[v[i].type] << "] ";							//при выводе типа обращаемся к const char Types_name по индексу, который уже хранит сама лексема
		cout << v[i].lex << "[" << Types_name[v[i].type] << "] ";
	}

	fout << endl;
	cout << endl;

}

void delet(vector<Lex>& v)
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		delete[] v[i].lex;
	}
}

int main()
{
	ifstream fin("input.txt", ios::binary);
	ofstream fout("output.txt");

	long unsigned int n;

	fin.seekg(0, ios::end);																//устанавливаем позицию чтения на конец
	n = fin.tellg();																	//в переменную n записываем количество элементов в файле
	fin.seekg(0, ios::beg);																//позицию переносим обратно на начало

	if (n)																				//если файл не пустой!
	{
		char* str;																		//создаем массив для значений из исходного файла

		str = new char[n + 1];															//выделяем память для массива на n+1 элементов (в конце хранится терминирующий ноль)

		fin.getline(str, n + 1, '\0');													//гетлайном забираем всё сразу

		str[n] = '\0';																	//последний элемент является нулем

		vector <Lex> lexeme;															//создаем вектор лексем

		LexAnalysis(str, lexeme);														//передаем и массив с исходным файлом, и вектор лексем, который будет затем заполняться, в лексический анализатор

		print(fout, lexeme);															//выводим элементы вектора

		SynAnalysis(fout, lexeme);														//передаем вектор лексем в синтаксический анализатор

		delet(lexeme);																	//очищаем все созданные массив в векторе лексем для каждого элемента в нем

		delete[] str;																	//очищаем выделенную под массив память
	}

	fin.close();
	fout.close();

	return 0;
}