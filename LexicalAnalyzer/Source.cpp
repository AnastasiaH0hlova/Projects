#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

struct Lex																									//��������� �������
{
	char* lex;																								//������ �������
	int type;																								//������ ��� �������
};

//--------------------------------------����������� ����������---------------------------------//

enum LexStates { S, vl_, id_, ao_, eq_, co1, co2, co3, sc_, F, E };											//��������� ��������
enum TypesOfLex { vl, id, fo, ao, eq, co, ne, to, sc, lo, wl, no };											//������������ ����� ������

const char Types_name[12][3]{ "vl", "id", "fo", "ao", "eq", "co", "ne","to", "sc",  "lo","wl", "no" };		//�������� ����� ������ (���������� ��������� ������, ����� ����� �������� ��)

LexStates matrix_lex[9][11]																					//������� �������� ������������ �����������
{																											//S-��������� ���������
//					S		vl_		id_		ao_		eq_		co1		co2		co3		sc_		F		E		//id_-��������� ��������������
	/*0)alpha*/		id_,	E,		id_,	F,		F,		F,		F,		F,		F,		F,		E,		//vl_-��������� ���������
	/*1)0...9*/		vl_,	vl_,	id_,	F,		F,		F,		F,		F,		F,		F,		E,		//ao_-��������� + ��� -
	/*2 + -/ */		ao_,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//eq_-��������� =
	/*3)  <  */		co1,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//co1-���������, �� �������� ���������� <>, <= ��� <
	/*4)  >  */		co2,	F,		F,		F,		F,		co3,	F,		F,		F,		F,		F,		//co2-���������, �� �������� ���������� > ��� >=
	/*5)  =  */		eq_,	F,		F,		F,		F,		co3,	co3,	F,		F,		F,		F,		//co3-��������� <>, <=, <,> ��� >=
	/*6)  ;  */		sc_,	F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//co3-��������� <>, <=, <,> ��� >=
	/*7)empty*/		S,		F,		F,		F,		F,		F,		F,		F,		F,		F,		F,		//sc_-��������� ;
	/*8)other*/		E,		E,		E,		F,		F,		F,		F,		F,		F,		F,		E		//F-�������� ���������
																											//E-��������� ���������
};


int place_in_the_matrix_lex(char cur_char)																	// ��������� �������� ������ �������
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


void add(char*& str, vector<Lex>& v, unsigned int beginning, unsigned int running, LexStates last)			//���������� ������� � ������
{
	Lex token;																								//������ ��� �������������� �� ���������� � �������� �������

	char* word = new char[running - beginning + 1];

	unsigned int i = 0;

	for (unsigned int j = beginning; j < running; ++i, ++j)
	{
		word[i] = str[j];
	}

	word[i] = '\0';

	token.lex = word;																						//��������� ���� �������

	if (last == id_)																						//���� ������� �������� ��������������� 
	{
		if ((running - beginning) > 5)																		//���������, ����� ��� ��������� � ���� �� ����� 5 ��������
		{
			token.type = wl;																				//���� ������� �� �����������, �� ������� ������ ���� ��� �� ������������
		}

		else if (!strcmp(token.lex, "for"))		token.type = fo;
		else if (!strcmp(token.lex, "to"))		token.type = to;
		else if (!strcmp(token.lex, "next"))	token.type = ne;											//���� ������� �������� �������� ������

		else if (!strcmp(token.lex, "and") || !strcmp(token.lex, "or")) token.type = lo;
		else if (!strcmp(token.lex, "not"))		token.type = no;

		else token.type = id;																				//���� ������� �� �������� �������� ������ � �� �������� ����� 5 ��������, �� ��� �������� ���������������
	}

	else if (last == vl_)																					//���� ������� �������� ����������
	{
		if (atoi(token.lex) > 32768)																		//���������, �������� �� ��� ������, ������� 32767
		{
			token.type = wl;																				//���� ������� �����������, �� ������� ������ ���� ��� �� ������������
		}

		else token.type = vl;																				//���� ������� �� ������ 32767, �� ��� �������� ����������
	}

	else if (last == co1 || last == co2 || last == co3)														//<> > < <= >=
	{
		token.type = co;
	}

	else token.type = last;																	//���� �� ������� �� ���� �� ���������������, �� ����������, �� ��� �������� �� ����� �����
																							//������� ��� ��������� ��� ��� ������ ��������
	v.push_back(token);
}


void LexAnalysis(char*& str, vector<Lex>& v)												 //�������� ������ ����� � ������, � ������� ����� ��������� �������, �� ������
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

		last = state;																		//���������� ��������� ���������� ���������, ������ ����� ���������� ��� �������, ����� ��������� ������ ��������
		state = matrix_lex[place_in_the_matrix_lex(cur_char)][state];

		if (state == F)																		//���� �� �������� ������� � ���������� ������ ��������, ��
		{
			add(str, v, beginning, running, last);

			state = S;																		//����� ��������� ������� � ��������� �� � ������, ������ ��������� �������� �� ��������� 
																							//� ���������� ���������� �������, ���� �� ��������� �� ������������� ����			
		}

		else ++running;																		//���� �� ��� �� �������� ��������� ���������, ��������� �������

	}
}

//--------------------------------------�������������� ����������---------------------------------//

enum SynStates { _S, _id, _eq, aex0, aex1, aex2, _E };										//��������� ��������

SynStates matrix_syn[10][7]																	//������� �������� ��������������� �����������
{																							//S-��������� ���������
	//				_S		_id		_eq		aex0	aex1	aex2	_E						//id-��������� �������� ���������
	/*0) +/* */		_E,		_E,		_E,		_E,		aex2,	_E,		_E,						//eq-��������� �������� �����
	/*1)  -  */		_E,		_E,		_E,		aex2,	aex2,	_E,		_E,						//aex0-��������� �������� ������, ���������� ��� ���������
	/*2  id  */		_E,		_eq,	_E,		aex1,	_eq,	aex1,	_E,						//aex1-��������� ��������, ������� ������� �� �������� �����
	/*3) vl  */		_E,		_E,		_E,		aex1,	_E,		aex1,	_E,						//���� flag = 0, �� ������� �������������� �������� ��� to
	/*4)  =  */		_E,		_E,		aex0,	_E,		_E,		_E,		_E,						//���� flag = 1, �� ������� �������������� �������� ��� ����������
	/*5)  ;  */		_E,		_E,		_E,		_E,		_id,	_E,		_E,						//���� flag = 2, �� ������� �������������� ��������, ����� � ������� ��� next
	/*6  fo  */		_id,	_E,		_E,		_E,		_E,		_E,		_E,						//aex2-��������� �������� ���������� ��� ���������					
	/*7) to  */		_E,		_E,		_E,		_E,		aex0,	_E,		_E,						//E-��������� ���������					
	/*8) next*/		_E,		_E,		_E,		_E,		_S,		_E,		_E,
	/*9) other*/	_E,		_E,		_E,		_E,		_E,		_E,		_E

};


int place_in_the_matrix_syn(char* cur_type, unsigned int& flag, SynStates last)				//��������� �������� ������ �������
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

void SynAnalysis(ofstream& fout, vector<Lex>& v)											//� �������������� ���������� �������� ����� ��������� � ����
{
	unsigned int running = 0;
	unsigned int flag = 0;																	//���� ��� ����������� �������� ���������
	char cur_type[3];

	SynStates state = _S;
	SynStates last = state;


	while (running != v.size() && state != _E)
	{

		last = state;

		strcpy_s(cur_type, Types_name[v[running].type]);

		if (!strcmp(cur_type, "ao"))														//����� ���� ��������, ����� ���������� ������������� ��������� 
		{
			if (!strcmp(v[running].lex, "-"))strcpy_s(cur_type, "_a");
		}

		state = matrix_syn[place_in_the_matrix_syn(cur_type, flag, last)][state];

		++running;
	}


	if (state != _S)																		//�� �������� � ��������� ���������, ���� ��� ����� � �������
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

		else																					//���� ������ ���������� � �� �� ����� � ��������� ���������
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
		fout << v[i].lex << "[" << Types_name[v[i].type] << "] ";							//��� ������ ���� ���������� � const char Types_name �� �������, ������� ��� ������ ���� �������
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

	fin.seekg(0, ios::end);																//������������� ������� ������ �� �����
	n = fin.tellg();																	//� ���������� n ���������� ���������� ��������� � �����
	fin.seekg(0, ios::beg);																//������� ��������� ������� �� ������

	if (n)																				//���� ���� �� ������!
	{
		char* str;																		//������� ������ ��� �������� �� ��������� �����

		str = new char[n + 1];															//�������� ������ ��� ������� �� n+1 ��������� (� ����� �������� ������������� ����)

		fin.getline(str, n + 1, '\0');													//��������� �������� �� �����

		str[n] = '\0';																	//��������� ������� �������� �����

		vector <Lex> lexeme;															//������� ������ ������

		LexAnalysis(str, lexeme);														//�������� � ������ � �������� ������, � ������ ������, ������� ����� ����� �����������, � ����������� ����������

		print(fout, lexeme);															//������� �������� �������

		SynAnalysis(fout, lexeme);														//�������� ������ ������ � �������������� ����������

		delet(lexeme);																	//������� ��� ��������� ������ � ������� ������ ��� ������� �������� � ���

		delete[] str;																	//������� ���������� ��� ������ ������
	}

	fin.close();
	fout.close();

	return 0;
}