#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

//#include <cstdlib>

using namespace std;

class DATA
{
private:																	//объявление переменных

	int d, m;
	double p1, p2, p3, av;
	vector <int> DAYS_IN_MONTH{ 31,29,31,30,31,30,31,31,30,31,30,31 };		//вектор дней в каждом месяце


public:


	DATA() : d(), m(), p1(), p2(), p3(), av() {}							//пустой конструктор


	DATA(int dd, int mm, double pp1, double pp2, double pp3) :				//конструктор
		d(dd),
		m(mm),
		p1(pp1),
		p2(pp2),
		p3(pp3)
	{
		check();
	}

	int day() { return d; }													//гетеры
	int month() { return m; }
	double pressure1() { return p1; }
	double pressure2() { return p2; }
	double pressure3() { return p3; }
	double pressure_AV()
	{
		double av = (p1 + p2 + p3) / 3.0;
		return av;
	}

	void check()															//проверки на месяцы и дни
	{
		check_month();
		check_day();
	}

	void check_month()
	{
		if (m < 1 || m>12) {

			throw runtime_error("Некорректный месяц");
		}
	}


	void check_day()
	{
		if (d<1 || d>DAYS_IN_MONTH[m - 1]) {

			throw runtime_error("Некорректный день");
		}
	}

};


ostream& operator<<(ostream& out, DATA& data)								//перегруженный оператор вывода
{
	return out << setfill('0') << setw(2) << data.day() << '.' << setw(2) << data.month() <<
		' ' << data.pressure1() << ' ' << data.pressure2() << ' ' << data.pressure3();
}


istream& operator>>(istream& in, DATA& data)								//перегруженный оператор ввода
{

	int d, m;																//день и месяц

	int point;																//переменная-счётчик для количества точек в double числе,
																			//которое представлено в виде переменной типа string

	double p1, p2, p3;														//давление1 давление2 давление3

	string info = "";															//переменная для считывания строки

	getline(in, info, ' ');													//дату считываем из потока до пробела

	if (info[0] == '\n')
	{
		info.erase(info.find('\n'), 1);										//удаляем перенос строки, если он стоит перед словом, с помощью функции erase
	}

	if (info.length() != 5) {												//если у даты НЕКОРРЕКТНЫЙ формат (НЕ 01.01), то сразу выдаёт ошибку
		cin.clear();
		while (cin.get() != '\n');
		throw runtime_error("Некоректный формат даты!");
	}

	//проверка на то, чтобы каждый знак являлся числом, а 3 знак - обязательно точкой
	if (info[0] >= '0' && info[0] <= '9' && info[1] >= '0' && info[1] <= '9' &&
		info[3] >= '0' && info[3] <= '9' && info[4] >= '0' && info[4] <= '9' && info[2] == '.')
	{
		d = (info[0] - 48) * 10 + (info[1] - 48);							//используя таблицу кодировки ASCII, переводим символы цифр в int-овые их значения
		m = (info[3] - 48) * 10 + (info[4] - 48);
	}

	else
	{
		cin.clear();
		while (cin.get() != '\n');
		throw runtime_error("Некоректный формат даты!");
	}


	for (int r = 1; r <= 3; r++)
	{
		info = "";															//каждый раз обнуляем переменную для считывания строки
		point = 0;

		if (r == 3)
		{
			getline(in, info);												//если мы берем третье давление, то берем его до конца строки
		}

		else
		{
			getline(in, info, ' ');											//первое и второе давление берем ДО пробела
		}

		char* str = new char[info.length()];								//вводим динамический массив чаров, размера нашей строки(чтобы использовать функцию 
		if (info != "")
		{
			for (int i = 0; i < info.length(); i++)
			{
				if (info[i] > '9' || info[i] < '0')						//если знак явлеется НЕ цифрой, то мы проверяем
				{
					if (info[i] == '.')										//точка ли это
					{
						if (i == 0 || i == info.length() - 1)				//если она стоит на первом или же последнем месте, то сразу выводим ошибку неправильного формата давления
						{
							cin.clear();
							while (cin.get() != '\n');
							throw runtime_error("Некоректный формат давления!");
						}

						else												//если мы находим точку, то инкрементируем переменную-счётчик
						{
							point++;
							info[i] = ',';									//все точки меняем на запятые(функция strstod работает только с string-овым представлением числа double, записанным с использованием запятой(не точки!), иначе он переводит в int
						}
					}

					else													//если найден любой другой символ, кроме цифр и точек, то сразу выбрасываем ошибку
					{
						cin.clear();
						while (cin.get() != '\n');
						throw runtime_error("Некоректный формат давления!");
					}
				}
			}
		}

		else                                                                //если строка так и осталась пустой и в переменную из потока ничего не взялось, то выбрасываем ошибку
		{
			cin.clear();
			while (cin.get() != '\n');
			throw runtime_error("Некоректный формат давления!");
		}


		if (point == 1 || point == 0)										//если в числе 1 точка(или же их вообще нет)
		{
			for (int i = 0; i < info.length(); i++)
			{
				str[i] = info[i];											//копируем все элементы переменной string в массив char-ов
			}

			switch (r)
			{
			case 1:
				p1 = strtod(str, NULL); break;								//функция strtod преобразовывает строку string в double, NULL - до коцна переменной
			case 2:
				p2 = strtod(str, NULL); break;
			case 3:
				p3 = strtod(str, NULL); break;
			}
		}

		else
		{
			cin.clear();
			throw runtime_error("Некоректный формат давления!");
		}
	}



	data = DATA(d, m, p1, p2, p3);											//передаем все полученные и обработанные данные в конструктор класса

	return in;																//возвращаем поток
}


void check1(int& v)															//проверка на количество экземпляров класса
{
	while (!(cin >> v) || (cin.peek() != '\n') || v <= 0)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Вы ввели неккоректное значение количества экземпляров класса! Попробуйте снова >";
	}
}

void check2(int& v)															//проверка на корректность ответа
{
	while (!(cin >> v) || (cin.peek() != '\n') || v < 1 || v > 2)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "Вы ввели неккоректное значение! Ответом может служить либо 1, либо 2. Попробуйте снова >";
	}
}

int main()																	//начало MAIN
{
	try {
		setlocale(LC_ALL, "rus");

		ifstream fin;														//объект класса для взять из потока
		ofstream fout;														//объект класса для помещения в файл

		int first_answer;
		int second_answer;
		int num;															//количество экземляров класса
		string str;
		DATA data;															//объект написанного класса

		int i = 0;															//итератор


		cout << "Вы желаете считать данные с консоли или из файла?\n(1 - ввести данные самостоятельно; 2 - прочитать их из файла)> ";
		check2(first_answer);

		vector <DATA> correct_data;											//вектор для экземляров класса
		vector <double> average_p;											//вектор для результата работы класса
		bool f = true;


		if (first_answer == 1)
		{
			cout << "Введите количество экземпляров класса> ";
			check1(num);
			while (f)
			{
				cout << "Пожалуйста, введите данные ТОЛЬКО в данном формате:\n(ДД.ММ) (ДАВЛЕНИЕ1) (ДАВЛЕНИЕ2) (ДАВЛЕНИЕ3)" << endl;
				try															//создаем еще один блок try|catch для отлавливания ошибок и ВОЗМОЖНОСТИ вновь ввести данные, не стирая предыдущие
				{
					while (i < num)
					{
						cout << i + 1 << ") ";
						cin >> data;
						correct_data.push_back(data);						//push_back-ом помешаем экземляр класса в конец вектора
						average_p.push_back(data.pressure_AV());			//push_back-ом помешаем результат обработки данных в конец вектора
						i++;
					}
					if (i == num)
					{
						f = false;											//выходим из цикла, как только итератор станет равен количеству экземпляров класса
					}
				}

				catch (runtime_error& e)
				{
					cout << e.what() << endl;
				}
			}
		}


		else if (first_answer == 2)
		{

			fin.open("input.txt");

			if (fin.is_open())
			{
				getline(fin, str);											//забираем всю первую строку(количество экземпляров класса) и проверяем ее на корректность
				for (int k = 0; k < str.length(); k++)
				{
					if (str[k] < '0' || str[k]>'9')
					{
						throw runtime_error("Некорректное количество экземляров класса!");
					}
				}
				num = atof(str.c_str());									//строку с помощью функции atof переводим в int-овое число
				while (i < num)
				{
					fin >> data;
					correct_data.push_back(data);
					average_p.push_back(data.pressure_AV());
					i++;
				}
			}

			else
			{
				cin.clear();
				while (cin.get() != '\n');
				throw runtime_error("Невозможно корректно открыть данный файл!");

			}

			fin.close();
		}

		cout << "Полученные данные Вы желаете вывести в консоль или поместить в файл?\n (1 - вывести в консоль; 2 - поместить в файл)> ";
		check2(second_answer);

		if (second_answer == 1)
		{
			for (int i = 0; i < num; i++)									//последовательно выводим экземляры класса и результат работы(через ;)
			{
				cout << i + 1 << ") " << correct_data[i] << "; " << average_p[i] << endl;

			}
		}

		else if (second_answer == 2)
		{
			fout.open("output.txt");
			if (fout.is_open())
			{
				for (int i = 0; i < num; i++)								//последовательно помещаем в файл экземляры класса и результат работы(через ;)
				{
					fout << i + 1 << ") " << correct_data[i] << "; " << average_p[i] << endl;
				}
				cout << "Помещение данных в файл завершено успешно!";
			}
		}
	}

	catch (runtime_error& e)
	{
		cout << e.what();
	}

	return 0;
}