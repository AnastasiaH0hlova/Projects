#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

//#include <cstdlib>

using namespace std;

class DATA
{
private:																	//���������� ����������

	int d, m;
	double p1, p2, p3, av;
	vector <int> DAYS_IN_MONTH{ 31,29,31,30,31,30,31,31,30,31,30,31 };		//������ ���� � ������ ������


public:


	DATA() : d(), m(), p1(), p2(), p3(), av() {}							//������ �����������


	DATA(int dd, int mm, double pp1, double pp2, double pp3) :				//�����������
		d(dd),
		m(mm),
		p1(pp1),
		p2(pp2),
		p3(pp3)
	{
		check();
	}

	int day() { return d; }													//������
	int month() { return m; }
	double pressure1() { return p1; }
	double pressure2() { return p2; }
	double pressure3() { return p3; }
	double pressure_AV()
	{
		double av = (p1 + p2 + p3) / 3.0;
		return av;
	}

	void check()															//�������� �� ������ � ���
	{
		check_month();
		check_day();
	}

	void check_month()
	{
		if (m < 1 || m>12) {

			throw runtime_error("������������ �����");
		}
	}


	void check_day()
	{
		if (d<1 || d>DAYS_IN_MONTH[m - 1]) {

			throw runtime_error("������������ ����");
		}
	}

};


ostream& operator<<(ostream& out, DATA& data)								//������������� �������� ������
{
	return out << setfill('0') << setw(2) << data.day() << '.' << setw(2) << data.month() <<
		' ' << data.pressure1() << ' ' << data.pressure2() << ' ' << data.pressure3();
}


istream& operator>>(istream& in, DATA& data)								//������������� �������� �����
{

	int d, m;																//���� � �����

	int point;																//����������-������� ��� ���������� ����� � double �����,
																			//������� ������������ � ���� ���������� ���� string

	double p1, p2, p3;														//��������1 ��������2 ��������3

	string info = "";															//���������� ��� ���������� ������

	getline(in, info, ' ');													//���� ��������� �� ������ �� �������

	if (info[0] == '\n')
	{
		info.erase(info.find('\n'), 1);										//������� ������� ������, ���� �� ����� ����� ������, � ������� ������� erase
	}

	if (info.length() != 5) {												//���� � ���� ������������ ������ (�� 01.01), �� ����� ����� ������
		cin.clear();
		while (cin.get() != '\n');
		throw runtime_error("����������� ������ ����!");
	}

	//�������� �� ��, ����� ������ ���� ������� ������, � 3 ���� - ����������� ������
	if (info[0] >= '0' && info[0] <= '9' && info[1] >= '0' && info[1] <= '9' &&
		info[3] >= '0' && info[3] <= '9' && info[4] >= '0' && info[4] <= '9' && info[2] == '.')
	{
		d = (info[0] - 48) * 10 + (info[1] - 48);							//��������� ������� ��������� ASCII, ��������� ������� ���� � int-���� �� ��������
		m = (info[3] - 48) * 10 + (info[4] - 48);
	}

	else
	{
		cin.clear();
		while (cin.get() != '\n');
		throw runtime_error("����������� ������ ����!");
	}


	for (int r = 1; r <= 3; r++)
	{
		info = "";															//������ ��� �������� ���������� ��� ���������� ������
		point = 0;

		if (r == 3)
		{
			getline(in, info);												//���� �� ����� ������ ��������, �� ����� ��� �� ����� ������
		}

		else
		{
			getline(in, info, ' ');											//������ � ������ �������� ����� �� �������
		}

		char* str = new char[info.length()];								//������ ������������ ������ �����, ������� ����� ������(����� ������������ ������� 
		if (info != "")
		{
			for (int i = 0; i < info.length(); i++)
			{
				if (info[i] > '9' || info[i] < '0')						//���� ���� �������� �� ������, �� �� ���������
				{
					if (info[i] == '.')										//����� �� ���
					{
						if (i == 0 || i == info.length() - 1)				//���� ��� ����� �� ������ ��� �� ��������� �����, �� ����� ������� ������ ������������� ������� ��������
						{
							cin.clear();
							while (cin.get() != '\n');
							throw runtime_error("����������� ������ ��������!");
						}

						else												//���� �� ������� �����, �� �������������� ����������-�������
						{
							point++;
							info[i] = ',';									//��� ����� ������ �� �������(������� strstod �������� ������ � string-���� �������������� ����� double, ���������� � �������������� �������(�� �����!), ����� �� ��������� � int
						}
					}

					else													//���� ������ ����� ������ ������, ����� ���� � �����, �� ����� ����������� ������
					{
						cin.clear();
						while (cin.get() != '\n');
						throw runtime_error("����������� ������ ��������!");
					}
				}
			}
		}

		else                                                                //���� ������ ��� � �������� ������ � � ���������� �� ������ ������ �� �������, �� ����������� ������
		{
			cin.clear();
			while (cin.get() != '\n');
			throw runtime_error("����������� ������ ��������!");
		}


		if (point == 1 || point == 0)										//���� � ����� 1 �����(��� �� �� ������ ���)
		{
			for (int i = 0; i < info.length(); i++)
			{
				str[i] = info[i];											//�������� ��� �������� ���������� string � ������ char-��
			}

			switch (r)
			{
			case 1:
				p1 = strtod(str, NULL); break;								//������� strtod ��������������� ������ string � double, NULL - �� ����� ����������
			case 2:
				p2 = strtod(str, NULL); break;
			case 3:
				p3 = strtod(str, NULL); break;
			}
		}

		else
		{
			cin.clear();
			throw runtime_error("����������� ������ ��������!");
		}
	}



	data = DATA(d, m, p1, p2, p3);											//�������� ��� ���������� � ������������ ������ � ����������� ������

	return in;																//���������� �����
}


void check1(int& v)															//�������� �� ���������� ����������� ������
{
	while (!(cin >> v) || (cin.peek() != '\n') || v <= 0)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "�� ����� ������������ �������� ���������� ����������� ������! ���������� ����� >";
	}
}

void check2(int& v)															//�������� �� ������������ ������
{
	while (!(cin >> v) || (cin.peek() != '\n') || v < 1 || v > 2)
	{
		cin.clear();
		while (cin.get() != '\n');
		cout << "�� ����� ������������ ��������! ������� ����� ������� ���� 1, ���� 2. ���������� ����� >";
	}
}

int main()																	//������ MAIN
{
	try {
		setlocale(LC_ALL, "rus");

		ifstream fin;														//������ ������ ��� ����� �� ������
		ofstream fout;														//������ ������ ��� ��������� � ����

		int first_answer;
		int second_answer;
		int num;															//���������� ���������� ������
		string str;
		DATA data;															//������ ����������� ������

		int i = 0;															//��������


		cout << "�� ������� ������� ������ � ������� ��� �� �����?\n(1 - ������ ������ ��������������; 2 - ��������� �� �� �����)> ";
		check2(first_answer);

		vector <DATA> correct_data;											//������ ��� ���������� ������
		vector <double> average_p;											//������ ��� ���������� ������ ������
		bool f = true;


		if (first_answer == 1)
		{
			cout << "������� ���������� ����������� ������> ";
			check1(num);
			while (f)
			{
				cout << "����������, ������� ������ ������ � ������ �������:\n(��.��) (��������1) (��������2) (��������3)" << endl;
				try															//������� ��� ���� ���� try|catch ��� ������������ ������ � ����������� ����� ������ ������, �� ������ ����������
				{
					while (i < num)
					{
						cout << i + 1 << ") ";
						cin >> data;
						correct_data.push_back(data);						//push_back-�� �������� �������� ������ � ����� �������
						average_p.push_back(data.pressure_AV());			//push_back-�� �������� ��������� ��������� ������ � ����� �������
						i++;
					}
					if (i == num)
					{
						f = false;											//������� �� �����, ��� ������ �������� ������ ����� ���������� ����������� ������
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
				getline(fin, str);											//�������� ��� ������ ������(���������� ����������� ������) � ��������� �� �� ������������
				for (int k = 0; k < str.length(); k++)
				{
					if (str[k] < '0' || str[k]>'9')
					{
						throw runtime_error("������������ ���������� ���������� ������!");
					}
				}
				num = atof(str.c_str());									//������ � ������� ������� atof ��������� � int-���� �����
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
				throw runtime_error("���������� ��������� ������� ������ ����!");

			}

			fin.close();
		}

		cout << "���������� ������ �� ������� ������� � ������� ��� ��������� � ����?\n (1 - ������� � �������; 2 - ��������� � ����)> ";
		check2(second_answer);

		if (second_answer == 1)
		{
			for (int i = 0; i < num; i++)									//��������������� ������� ��������� ������ � ��������� ������(����� ;)
			{
				cout << i + 1 << ") " << correct_data[i] << "; " << average_p[i] << endl;

			}
		}

		else if (second_answer == 2)
		{
			fout.open("output.txt");
			if (fout.is_open())
			{
				for (int i = 0; i < num; i++)								//��������������� �������� � ���� ��������� ������ � ��������� ������(����� ;)
				{
					fout << i + 1 << ") " << correct_data[i] << "; " << average_p[i] << endl;
				}
				cout << "��������� ������ � ���� ��������� �������!";
			}
		}
	}

	catch (runtime_error& e)
	{
		cout << e.what();
	}

	return 0;
}