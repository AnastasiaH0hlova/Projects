#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//происходит возвращение объектов для того, чтобы совершать множественные арефметические операции a*c+b например

using namespace std;

class Set1
{
public:

	vector <int> elements;

	Set1() {}													//конструктор по умолчанию


	Set1(const Set1& set)										//конструктор копирования
	{
		for (unsigned int i = 0; i < set.elements.size(); ++i)
		{
			this->elements.push_back(set.elements[i]);
		}
	}

	Set1(vector <int> v)										//конструктор инициализации
	{
		for (unsigned int i = 0; i < v.size(); ++i)
		{
			this->elements.push_back(v[i]);
		}
	}

	~Set1() {};													//деструктор

	Set1 add(int a);											//добавление элемента
	Set1 remove(int a);											//удаление элемента
//--------Перегруженные функции--------//
	string operator== (const Set1&);							//эквивалентность
	string operator< (const Set1&);								//сравнение мощностей
	string operator> (const Set1&);
	string operator<= (const Set1&);							//выявление включенность множества
	string operator>= (const Set1&);

	Set1 operator= (const Set1&);								//присваивание
	Set1 operator* (const Set1&);								//пересечение множеств
	Set1 operator+ (const Set1&);								//объединение множеств
	Set1 operator/ (const Set1&);								//исключение множества
	Set1 operator- (const Set1& b);								//дополнение множества

	friend ofstream& operator<<(ofstream& fout, Set1&);			//дружественная функция помещения в файл
	friend ifstream& operator>>(ifstream& fin, Set1&);			//дружественная функция взятия из файла

};

class Set2 : public Set1
{
public:

	Set2& operator= (const Set1& x);//в дочернем классе дополнительно определена функция присваивания, т.к. необходимо вернуть объект дочернего класса, а не родительского
	void sort();//определен дополнительный метод сортировки только для дочерних объектов

};

//--------Проверка отношений между множествами--------//

string Set1::operator== (const Set1& b)							//проверяем сначала, чтобы мощности множеств были равны
{																//затем проверяем, чтобы каждый элемент из первого содержался во втором
	string str;
	bool flag = 0;

	if (this->elements.size() == b.elements.size())
	{
		for (unsigned int i = 0; i < b.elements.size(); ++i)
		{
			flag = 0;

			for (unsigned int j = 0; j < this->elements.size(); ++j)
			{
				if (this->elements[j] == b.elements[i])
				{
					flag = 1; break;
				}
			}

			if (!flag) break;									//если какого-то элемента из В нет в множестве А, то множества уже не эквивалентны (наоборот аналогично)
		}

		if (flag)
		{
			str = "The set A is equivalent to the set B";
		}

		else
		{
			str = "The set A is not equivalent to the set B";
		}

	}

	else
	{
		str = "The set A is not equivalent to the set B";
	}

	return str;
}

string Set1::operator< (const Set1& b)							//Отвечаем на вопрос:"Мощность множества А меньше мощности множества В?"
{
	string str;

	if (this->elements.size() < b.elements.size())
	{
		str = "There are more elements in set B than in set A";
	}

	else
	{
		str = "There are no more elements in set B than in set A";
	}

	return str;
}

string Set1::operator> (const Set1& b)							//Отвечаем на вопрос:"Мощность множества А больше мощности множества В?"
{
	string str;

	if (this->elements.size() > b.elements.size())
	{
		str = "There are more elements in set A than in set B";
	}

	else
	{
		str = "There are no more elements in set A than in set B";
	}

	return str;
}

string Set1::operator<= (const Set1& b)							//Отвечаем на вопрос:"Множество А содержится в B?"
{
	string str;
	bool flag = 0;

	if (this->elements.size() <= b.elements.size())
	{
		for (unsigned int i = 0; i < this->elements.size(); ++i)
		{
			flag = 0;

			for (unsigned int j = 0; j < b.elements.size(); ++j)
			{
				if (this->elements[i] == b.elements[j])
				{
					flag = 1; break;
				}
			}

			if (!flag) break;
		}

		if (flag)
		{
			str = "The set A is contained in the set B";
		}

		else
		{
			str = "The set A is not contained in the set B";
		}
	}

	else
	{
		str = "The set A is not contained in the set B";
	}

	return str;
}

string Set1::operator>= (const Set1& b)									//Отвечаем на вопрос:"Множество В содержится в А?"
{
	string str;
	bool flag = 0;

	if (this->elements.size() >= b.elements.size())
	{
		for (unsigned int i = 0; i < b.elements.size(); ++i)
		{
			flag = 0;

			for (unsigned int j = 0; j < this->elements.size(); ++j)
			{
				if (this->elements[j] == b.elements[i])
				{
					flag = 1; break;
				}
			}

			if (!flag) break;
		}

		if (flag)
		{
			str = "The set B is contained in the set A";
		}

		else
		{
			str = "The set B is not contained in the set A";
		}
	}

	else
	{
		str = "The set B is not contained in the set A";
	}

	return str;
}

//-------Добавление/удаление элемента/сортировка-------//

Set1 Set1::add(int a)
{
	Set1 new_set(this->elements);
	new_set.elements.push_back(a);
	return new_set;
}

Set1  Set1::remove(int a)												//находим индекс с элементом а
{																		//удаляем его из нового множества и возвращаем его
	Set1 new_set(this->elements);
	int p = -1;

	for (unsigned int i = 0; i < new_set.elements.size(); ++i)
	{
		if (new_set.elements[i] == a) { p = i; break; }
	}

	if (p != -1)
	{
		new_set.elements.erase(new_set.elements.begin() + p);
	}

	return new_set;
}

void Set2::sort()														//сортировка выбором для объектов дочернего класса
{
	for (unsigned int i = 0; i < this->elements.size() - 1; ++i)
	{
		int imin = i;
		for (unsigned int j = i + 1; j < this->elements.size(); ++j)
		{
			if (this->elements[j] < this->elements[imin])imin = j;
		}

		int temp = this->elements[i];
		this->elements[i] = this->elements[imin];
		this->elements[imin] = temp;
	}
}

//-------Арифметические операции-------//

Set1 Set1::operator= (const Set1& x)										//если что-то было в векторе объекта - чистим
{
	this->elements.clear();

	for (unsigned int i = 0; i < x.elements.size(); ++i)					//копируем все элементы из правого множества в левое
	{
		this->elements.push_back(x.elements[i]);
	}

	return *this;
}

Set2& Set2::operator= (const Set1& x)
{
	this->elements.clear();

	for (unsigned int i = 0; i < x.elements.size(); ++i)
	{
		this->elements.push_back(x.elements[i]);
	}

	return *this;
}

Set1 Set1::operator* (const Set1& b)									//находим одинаковые элементы в обоих множествах и создаем новое с этими элементами
{
	Set1 new_set;

	for (unsigned int i = 0; i < this->elements.size(); ++i)
	{
		for (unsigned int j = 0; j < b.elements.size(); ++j)
		{
			if (this->elements[i] == b.elements[j])
			{
				new_set.elements.push_back(this->elements[i]); break;
			}
		}
	}

	return new_set;
}

Set1 Set1::operator+ (const Set1& b)									//копируем первое множество в новое и добавляем в него те элементы, которые не встречаются в А, но есть в В
{
	Set1 new_set;

	for (unsigned int i = 0; i < this->elements.size(); ++i)
	{
		new_set.elements.push_back(this->elements[i]);
	}

	bool flag;

	for (unsigned int i = 0; i < b.elements.size(); ++i)
	{
		flag = 1;
		for (unsigned int j = 0; j < new_set.elements.size(); ++j)
		{
			if (new_set.elements[j] == b.elements[i])
			{
				flag = 0; break;
			}
		}

		if (flag)new_set.elements.push_back(b.elements[i]);
	}

	return new_set;
}

Set1 Set1::operator/ (const Set1& b)								//в новое множество добавляем все элементы, которые есть в А, но нет в В
{
	Set1 new_set;

	bool flag;

	for (unsigned int i = 0; i < this->elements.size(); ++i)
	{
		flag = 1;

		for (unsigned int j = 0; j < b.elements.size(); ++j)
		{
			if (this->elements[i] == b.elements[j])
			{
				flag = 0; break;
			}
		}

		if (flag)new_set.elements.push_back(this->elements[i]);
	}

	return new_set;
}

Set1 Set1::operator- (const Set1& b)											//проверяем сначала, что В является подмножеством А
{																				//затем в новое множество добавляем все элементы которые есть в А, но нет в В
	Set1 new_set;
	bool flag = 0;

	if (this->elements.size() >= b.elements.size())
	{
		for (unsigned int i = 0; i < b.elements.size(); ++i)
		{
			flag = 0;
			for (unsigned int j = 0; j < this->elements.size(); ++j)
			{
				if (this->elements[j] == b.elements[i])
				{
					flag = 1; break;
				}
			}
			if (!flag) break;
		}

		if (flag)
		{
			for (unsigned int i = 0; i < this->elements.size(); ++i)
			{
				flag = 1;
				for (unsigned int j = 0; j < b.elements.size(); ++j)
				{
					if (this->elements[i] == b.elements[j])
					{
						flag = 0; break;
					}
				}

				if (flag)new_set.elements.push_back(this->elements[i]);
			}
		}
	}

	else
	{
		for (unsigned int i = 0; i < this->elements.size(); ++i)
		{
			flag = 0;
			for (unsigned int j = 0; j < b.elements.size(); ++j)
			{
				if (this->elements[i] == b.elements[j])
				{
					flag = 1; break;
				}
			}

			if (!flag) break;
		}

		if (flag)
		{
			for (unsigned int i = 0; i < b.elements.size(); ++i)
			{
				flag = 1;
				for (unsigned int j = 0; j < this->elements.size(); ++j)
				{
					if (this->elements[j] == b.elements[i])
					{
						flag = 0; break;
					}
				}

				if (flag)new_set.elements.push_back(b.elements[i]);
			}
		}
	}

	return new_set;
}

//-------Потоки ввода и вывода-------//

ifstream& operator >> (ifstream& fin, Set1& x) // перегрузка оператора ввода
{
	fin.get();										//забираем перевод строки между ключом и множествами
	int val;
	while (fin.peek() > 0 && fin.peek() != '\n')	//идем либо до конца файла, либо до перевода строки
	{
		fin >> val;
		x.elements.push_back(val);
	}

	return fin;
}

ofstream& operator << (ofstream& fout, Set1& x)
{
	for (unsigned int i = 0; i < x.elements.size(); ++i)
	{
		fout << x.elements[i] << ' ';
	}

	return fout;
}


int main()
{
	ofstream fout("output.txt");
	ifstream fin("input.txt");

	if (fin.is_open() && fout.is_open())
	{
		int ans;							//ключ
		fin >> ans;

		if (ans == 0 || ans == 1)
		{
			Set1 a;
			Set1 b;
			fin >> a;
			fin >> b;

			if (!ans)
			{
				Set1 c;
				c = a + b;
				fout << c;
			}

			else
			{
				string str;
				str = a == b;
				fout << str;
			}
		}

		else if (ans == 2 || ans == 3)
		{
			Set2 a;
			Set2 b;

			fin >> a;
			fin >> b;

			if (ans == 2)
			{
				Set2 c;
				c = a + b;
				fout << c;
			}

			else
			{
				string str;
				str = a == b;
				fout << str;
			}
		}

		else return -1;
	}

	else return -1;

	return 0;
}