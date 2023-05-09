#include <iostream>
#include <string>
#include <time.h>
#include <locale.h>
#include <fstream>
#include <vector>

using namespace std;

vector<string> formation(int* w, int* c, int maxW, int N, int answer, int answer2)
{
	vector<string> form;
	int rnd;
	if (answer == 1) //Жадный алгоритм Данцига
	{
		for (int i = 0; i < answer2; i++)
		{
			int size = 0;
			int elem;
			string str = "";
			for (int j = 0; j < N; j++) str = str + '0';
			rnd = rand() % N;
			str[rnd] = '1';
			size = w[rnd];
			for (int j = 0; j < N - 1; j++)
			{
				int max = 0;
				elem = -1;
				for (int k = 0; k < N; k++)
					if (c[k] / w[k] > max && str[k] == '0')
					{
						max = c[k] / w[k];
						elem = k;
					}
				if (size + w[elem] < maxW)
				{
					size = size + w[elem];
					str[elem] = '1';
				}
				else break;
			}
			form.push_back(str);
		}
	}
	else if (answer == 2) //Случайно
	{
		for (int i = 0; i < answer2; i++)
		{
			string str = "";
			for (int j = 0; j < N; j++)
			{
				rnd = rand() % 100 + 1;
				if (rnd < 25) str = str + '1';
				else str = str + '0';
			}
			form.push_back(str);
		}
	}
	return form;
}
string kros(string par1, string par2)
{
	string child = "";
	int rnd = rand() % 2;
	if (rnd == 0) //Одноточечный кроссовер
	{
		int rnd2 = rand() % par1.size();
		for (int i = 0; i < rnd2; i++)
			child = child + par1[i];
		for (int i = rnd2; i < par2.size(); i++)
			child = child + par2[i];
	}
	else //Двухточечный кроссовер
	{
		int rnd2 = rand() % (par1.size() / 2);
		int rnd3 = rand() % (par1.size() / 2) + 7;
		for (int i = 0; i < rnd2; i++)
			child = child + par1[i];
		for (int i = rnd2; i < rnd3; i++)
			child = child + par2[i];
		for (int i = rnd3; i < par2.size(); i++)
			child = child + par1[i];
	}
	return child;
}
string mutation(string str)
{
	int rnd = rand() % 2;
	if (rnd == 0) //Точечная
	{
		int rnd2 = rand() % str.size();
		if (str[rnd2] == '1') str[rnd2] = '0';
		else str[rnd2] = '1';
	}
	else //Сальтация
	{
		int rnd2 = rand() % (str.size() / 2) + 1;
		char tmp;
		tmp = str[rnd2];
		str[rnd2] = str[str.size() - rnd2];
		str[str.size() - rnd2] = tmp;
	}
	return str;
}
int getWQ(string str, int* arg) //Возвращаем вес и цену
{
	int WQ = 0;
	for (int i = 0; i < str.size(); i++)
		if (str[i] == '1') WQ = WQ + arg[i];
	return WQ;
}
int main()
{
	setlocale(0, "");
	srand(time(0));
	int N = 15, maxW = 80, answer, answer2, answer3;
	vector<int> Q;
	vector<int> W;
	vector<string> population;
	int q_max = 0, weight_max = 0;
	string str_max;
	int* S, * w, * c;
	S = new int[N];
	w = new int[N];
	c = new int[N];
	ifstream in("New.txt");
	if (in.is_open())
	{
		for (int i = 0; i < N; i++)
		{
			in >> c[i];
			in >> w[i];
			S[i] = 0;
		}
	}
	else exit(1);
	in.close();
	cout << "Предметы, их веса и стоимости: " << endl;
	for (int i = 0; i < N; i++)
		cout << "Предмет №" << i + 1 << " Вес: " << w[i] << " Стоимость: " << c[i] << endl;
	cout << "Максимальный вес: " << maxW << endl;
	cout << "\nВыберите нужное действие:\n1) Создание начальной популяции жадным алгоритмом Данцига\n2) Создание начальной популяции случайным образом\n";
	cin >> answer;
	cout << "Введите кол-во особей в популяции: ";
	cin >> answer2;
	population = formation(w, c, maxW, N, answer, answer2);
	cout << "\nСгенерированная начальная популяция: \n";
	for (int i = 0; i < population.size(); i++)
	{
		Q.push_back(0);
		cout << population[i];
		int weight = getWQ(population[i], w);
		if (weight > maxW) Q[i] = 0;
		else Q[i] = getWQ(population[i], c);
		W.push_back(weight);
		if (Q[i] > q_max)
		{
			q_max = Q[i];
			weight_max = W[i];
			str_max = population[i];
		}
		cout << " Критерий: " << Q[i] << " Вес: " << W[i] << endl;
	}
	cout << "Введите кол-во поколений: ";
	cin >> answer3;
	int stop = 0;
	for (int i = 0; i < answer3; i++)
	{
		stop++;
		cout << "Номер поколения: " << i + 1 << "\nТекущее оптимальное решение: " << str_max << " Значение критерия: " << q_max << " Вес: " << weight_max << endl;
		vector<string> child;
		vector<int> childQ;
		vector<int> childW;
		vector<int> mutants;
		cout << "Текущая популяция: " << endl;
		for (int j = 0; j < population.size(); j++)
			cout << population[j] << " критерий: " << Q[j] << " вес: " << W[j] << endl;
		cout << "Родительские пары: " << endl;
		for (int j = 0; j < rand() % 3 + population.size(); j++)
		{
			string par1 = "", par2 = "";
			int rnd1 = rand() % 2;
			if (rnd1 == 0) //Панмиксия
			{
				int rnd2 = rand() % population.size();
				int rnd3 = rand() % population.size();
				if (rnd2 == rnd3)
					if (rnd2 + 1 < population.size()) rnd2++;
					else rnd2--;
				par1 = population[rnd2];
				par2 = population[rnd3];
			}
			else //Отрицательный ассортативный принцип
			{
				int min = 99999; int max = -1; int kmax = -1; int kmin = -1;
				for (int i = 0; i < population.size(); i++)
				{
					if (Q[i] > max)
					{
						max = Q[i];
						kmax = i;
					}
					if (Q[i] < min)
					{
						max = Q[i];
						kmin = i;
					}
				}
				par1 = population[kmax]; par2 = population[kmin];
			}
			for (int k = 0; k < rand() % 3; k++)
			{
				string tmp = kros(par1, par2);
				child.push_back(tmp);
			}
			cout << par1 << " + " << par2 << endl;
		}
		cout << "Потомки: " << endl;
		for (int j = 0; j < child.size(); j++)
		{
			childQ.push_back(0);
			cout << child[j];
			int weight = getWQ(child[j], w);
			if (weight > maxW) childQ[j] = 0;
			else childQ[j] = getWQ(child[j], c);
			childW.push_back(weight);
			cout << " Критерий: " << childQ[j] << " Вес: " << childW[j] << endl;
		}
		cout << "Мутанты: " << endl;
		for (int j = 0; j < child.size(); j++)
		{
			int rnd = rand() % 100;
			if (rnd < 10)
			{
				child[j] = mutation(child[j]);
				int weight = getWQ(child[j], w);
				if (weight > maxW) childQ[j] = 0;
				else childQ[j] = getWQ(child[j], c);
				childW[j] = weight;
				mutants.push_back(j);
			}
		}
		for (int j = 0; j < mutants.size(); j++)
			cout << child[mutants[j]] << " Критерий: " << childQ[mutants[j]] << " Вес: " << childW[mutants[j]] << endl;
		for (int j = 0; j < child.size(); j++)
			if (childQ[j] > q_max)
			{
				stop = 0;
				q_max = childQ[j];
				str_max = child[j];
				weight_max = childW[j];
			}
		population[0] = str_max;
		Q[0] = q_max;
		W[0] = weight_max;
		vector<int> rang;
		for (int j = 0; j < child.size(); j++)
			rang.push_back(0);
		int sum = 0;
		for (int j = 0; j < child.size(); j++) //Сортируем детей по рангам
		{
			int max = 0, number = 0;
			for (int k = 0; k < child.size(); k++)
				if (childQ[k] >= max && rang[k] == 0)
				{
					max = childQ[k];
					number = k;
				}
			rang[number] = j + 1;
			sum = sum + max;
		}
		vector<float> proc;
		for (int j = 0; j < child.size(); j++)
			proc.push_back((float)childQ[j] / sum);
		for (int j = 1; j < population.size(); j++)
		{
			int rnd = rand() % 2;
			if (rnd == 0)//Селекция, пропорциональная схема
			{
				for (int k = 0; k < child.size(); k++)
				{
					float rnd2 = (float)(rand() % 100) / 100;
					if (proc[k] > rnd2)
					{
						if (childQ[k] == 0) continue;
						population[j] = child[k];
						Q[j] = childQ[k];
						W[j] = childW[k];
						proc[k] = -1;
						break;
					}
				}
			}
			else//Селекция, ранговая схема
			{
				for (int k = 0; k < child.size(); k++)
				{
					int n1 = 0, n2 = 1;
					float rnd2 = (float)(rand() % 100) / 100;
					if (rang[k] == 0) continue;
					if ((float)(n1 + (n2 - n1) * (rang[k] - 1) / ((population.size() - 1)) / population.size()) > rnd2)
					{
						if (childQ[k] == 0) continue;
						population[j] = child[k];
						Q[j] = childQ[k];
						W[j] = childW[k];
						proc[k] = -1;
						break;
					}
				}
			}
		}
		if (stop > 7) break;
	}
	cout << "\n=============================================================\n";
	cout << "Итоговое решение: " << str_max << " Значение критерия: " << q_max << " Вес: " << weight_max << endl;
}
