#include <iostream>
#include <fstream>
#include <vector>
#include <string>
//���������� ����������� �������� ��� ����, ����� ��������� ������������� �������������� �������� a*c+b ��������

using namespace std;

class Set1
{
public:

	vector <int> elements;

	Set1() {}													//����������� �� ���������


	Set1(const Set1& set)										//����������� �����������
	{
		for (unsigned int i = 0; i < set.elements.size(); ++i)
		{
			this->elements.push_back(set.elements[i]);
		}
	}

	Set1(vector <int> v)										//����������� �������������
	{
		for (unsigned int i = 0; i < v.size(); ++i)
		{
			this->elements.push_back(v[i]);
		}
	}

	~Set1() {};													//����������

	Set1 add(int a);											//���������� ��������
	Set1 remove(int a);											//�������� ��������
//--------������������� �������--------//
	string operator== (const Set1&);							//���������������
	string operator< (const Set1&);								//��������� ���������
	string operator> (const Set1&);
	string operator<= (const Set1&);							//��������� ������������ ���������
	string operator>= (const Set1&);

	Set1 operator= (const Set1&);								//������������
	Set1 operator* (const Set1&);								//����������� ��������
	Set1 operator+ (const Set1&);								//����������� ��������
	Set1 operator/ (const Set1&);								//���������� ���������
	Set1 operator- (const Set1& b);								//���������� ���������

	friend ofstream& operator<<(ofstream& fout, Set1&);			//������������� ������� ��������� � ����
	friend ifstream& operator>>(ifstream& fin, Set1&);			//������������� ������� ������ �� �����

};

class Set2 : public Set1
{
public:

	Set2& operator= (const Set1& x);//� �������� ������ ������������� ���������� ������� ������������, �.�. ���������� ������� ������ ��������� ������, � �� �������������
	void sort();//��������� �������������� ����� ���������� ������ ��� �������� ��������

};

//--------�������� ��������� ����� �����������--------//

string Set1::operator== (const Set1& b)							//��������� �������, ����� �������� �������� ���� �����
{																//����� ���������, ����� ������ ������� �� ������� ���������� �� ������
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

			if (!flag) break;									//���� ������-�� �������� �� � ��� � ��������� �, �� ��������� ��� �� ������������ (�������� ����������)
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

string Set1::operator< (const Set1& b)							//�������� �� ������:"�������� ��������� � ������ �������� ��������� �?"
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

string Set1::operator> (const Set1& b)							//�������� �� ������:"�������� ��������� � ������ �������� ��������� �?"
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

string Set1::operator<= (const Set1& b)							//�������� �� ������:"��������� � ���������� � B?"
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

string Set1::operator>= (const Set1& b)									//�������� �� ������:"��������� � ���������� � �?"
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

//-------����������/�������� ��������/����������-------//

Set1 Set1::add(int a)
{
	Set1 new_set(this->elements);
	new_set.elements.push_back(a);
	return new_set;
}

Set1  Set1::remove(int a)												//������� ������ � ��������� �
{																		//������� ��� �� ������ ��������� � ���������� ���
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

void Set2::sort()														//���������� ������� ��� �������� ��������� ������
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

//-------�������������� ��������-------//

Set1 Set1::operator= (const Set1& x)										//���� ���-�� ���� � ������� ������� - ������
{
	this->elements.clear();

	for (unsigned int i = 0; i < x.elements.size(); ++i)					//�������� ��� �������� �� ������� ��������� � �����
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

Set1 Set1::operator* (const Set1& b)									//������� ���������� �������� � ����� ���������� � ������� ����� � ����� ����������
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

Set1 Set1::operator+ (const Set1& b)									//�������� ������ ��������� � ����� � ��������� � ���� �� ��������, ������� �� ����������� � �, �� ���� � �
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

Set1 Set1::operator/ (const Set1& b)								//� ����� ��������� ��������� ��� ��������, ������� ���� � �, �� ��� � �
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

Set1 Set1::operator- (const Set1& b)											//��������� �������, ��� � �������� ������������� �
{																				//����� � ����� ��������� ��������� ��� �������� ������� ���� � �, �� ��� � �
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

//-------������ ����� � ������-------//

ifstream& operator >> (ifstream& fin, Set1& x) // ���������� ��������� �����
{
	fin.get();										//�������� ������� ������ ����� ������ � �����������
	int val;
	while (fin.peek() > 0 && fin.peek() != '\n')	//���� ���� �� ����� �����, ���� �� �������� ������
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
		int ans;							//����
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