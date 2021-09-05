#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Lex  //структура лексемы
{
	char* pal;
};

enum states { S, A, B, C, D, G, H, E, F }; //состо€ни€ перехода

states matrix[3][9] //матрица состо€ний
{
	//			S  A  B  C  D  G  H  E  F
	/*alpha*/	A, B, C, D, G, H, E, E, F,
	/*empty*/	S, F, F, F, F, F, F, E, F,
	/*other*/	E, E, E, E, E, E, E, E, F
};

const char* alphabet = "абвгдеЄжзийклмнопрстуфхцчшщъыьэю€јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЏџ№Ёёя";

int place_in_the_matrix(char cur_char) // получение значени€ строки матрицы
{

	if (cur_char == ' ' || cur_char == '\t' || cur_char == '\n' || cur_char == '\0')
	{
		return 1;
	}

	for (int i = 0; i < 66; ++i)
	{
		if (cur_char == alphabet[i])
		{
			return 0;
		}
	}

	return 2;
}

vector <Lex> search_for_palindromes(char* text) //лексический анализатор
{
	int running = 0;
	int beginning;
	char cur_char;

	char a, b, c, d, g, h;

	Lex word;
	vector<Lex> result;

	states state = S;
	states last;

	bool ok = false;

	bool O = false;
	while (!O)
	{
		cur_char = text[running];
		if (cur_char == '\0')
			O = true;

		cur_char = text[running];

		if (state == S || state == F)
		{
			beginning = running;
		}

		last = state;
		state = matrix[place_in_the_matrix(cur_char)][state];

		switch (state)
		{
		case A: a = cur_char; break;
		case B: b = cur_char; break;
		case C: c = cur_char; break;
		case D: d = cur_char; break;
		case G: g = cur_char; break;
		case H: h = cur_char; break;
		}

		if (state == E && cur_char == ' ' || state == E && cur_char == '\n' || state == E && cur_char == '\t')
		{
			state = S;
		}

		if (state == F)
		{
			switch (last)
			{
			case A: ok = true; break;
			case B: if (a == b)ok = true; break;
			case C: if (a == c)ok = true; break;
			case D: if (a == d && b == c)ok = true; break;
			case G: if (a == g && b == d)ok = true; break;
			case H: if (a == h && b == g && c == d)ok = true; break;
			}

			if (ok)
			{
				int length = running - beginning;

				word.pal = new char[length + 1];

				for (int i = 0; i < length; i++, beginning++)
				{
					word.pal[i] = text[beginning];
				}

				word.pal[length] = '\0';
				result.push_back(word);

				ok = false;
			}

			state = S;
		}

		else
		{
			running++;
		}


	}

	return result;
}

void output(ostream& fout, vector <Lex>& lexeme)
{
	int i;
	for (i = 0; i < lexeme.size() - 1; i++)
	{
		cout << lexeme.at(i).pal << " ";
		fout << lexeme.at(i).pal << " ";
	}
	cout << lexeme.at(i).pal;
	fout << lexeme.at(i).pal;

}

void delet(vector <Lex>& lexeme)
{
	for (int i = 0; i < lexeme.size(); i++)
	{
		delete lexeme.at(i).pal;
	}
}

int main()

{
	setlocale(LC_ALL, "rus");

	ifstream fin("input.txt");
	ofstream fout("output.txt");

	char* text;							//все символы в файле
	int n = 0;							//их количество

	if (fin.is_open() && fout.is_open())

	{
		while (fin.get() > 0) n++;

		if (n)

		{

			text = new char[n + 1];			//массив на размера n+1 (в конце хранитс€ терминирующий ноль)

			fin.clear();					//переходим в начало файла
			fin.seekg(0);

			for (int i = 0; i < n; i++)		//заполн€ем исходный массив
			{
				text[i] = fin.get();
			}

			text[n] = '\0';					//последнему присваиваем ноль

			vector<Lex> lexeme = search_for_palindromes(text);

			output(fout, lexeme);

			delet(lexeme);

			delete[] text;
		}

		fin.close();
		fout.close();
	}

	else return -1;

	return 0;
}