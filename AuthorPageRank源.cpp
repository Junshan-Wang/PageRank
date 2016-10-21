#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <math.h>
using namespace std;

double alpha = 0.85;
int n = 17551;
float p[19000][19000] = { 0 };
float Pagerank[19000] = { 0 };
char authorName[19000][100] = { 0 };
int id2num[19000] = { 0 };

int main()
{
	/*����author���ֺ�id��ע�⣬authorID�����������ģ���17551��author����������idΪ18648*/
	ifstream inFile("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2013/author_ids.txt");
	if (!inFile.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	for (int i = 0; i < n; ++i)
	{
		int id;
		char name[100] = { 0 };
		inFile >> id;
		inFile.get();						//����id
		inFile.getline(name, 100);			//����author����
		id2num[id] = i;						//����������id���°���������ID�ţ�1-17551��
		strcpy_s(authorName[i], name);
	}

	/*����author֮������ù�ϵ����¼������p��*/
	ifstream in_relationship("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2013/author_citation_network.txt");
	if (!in_relationship.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	while (!in_relationship.eof())
	{
		char buf[100] = { 0 };
		int beginID, endID;
		in_relationship >> beginID;
		in_relationship.get(buf, 6);
		in_relationship >> endID;

		p[id2num[beginID]][id2num[endID]] = 1;
	}
	for (int i = 0; i < n; ++i)
	{
		double sum = 0;
		for (int j = 0; j < n; ++j)
			if (p[i][j] == 1) sum++;
		if (sum != 0) sum = 1 / sum;
		for (int j = 0; j < n; ++j)
			if (p[i][j] == 1) p[i][j] = sum;
	}
	
	/*��ʼ��Pagerankֵ��Ϊ1/n*/
	for (int i = 0; i < n; ++i)	Pagerank[i] = 1 / (double)n;

	/*��������Pagerankֵ��Pagerank=alpha*Pagerank+(1-alpha)/n */
	/*ֹͣ����Ϊÿ��author��Pagerank��С��0.00001*/
	while (1)
	{
		double tmp[19000] = { 0 };
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
				tmp[i] += Pagerank[j] * p[j][i];
			tmp[i] = alpha*tmp[i] + (1 - alpha) / (double)n;
		}
		int flag = 0;
		for (int i = 0; i < n; ++i)
		{
			if (abs(Pagerank[i] - tmp[i])>0.00001)
				flag = 1;
			Pagerank[i] = tmp[i];
		}
		if (flag == 0) break;
	}

	/*Pagerank����*/
	int ID[19000] = { 0 };
	for (int i = 0; i < n; ++i) ID[i] = i;
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n-i-1;++j)
			if (Pagerank[j] < Pagerank[j + 1])
			{
				float temp;
				temp = Pagerank[j];	Pagerank[j] = Pagerank[j + 1];	Pagerank[j + 1] = temp;
				temp = ID[j];	ID[j] = ID[j + 1];	ID[j + 1] = temp;
			}

	/*���������author����Pagerankֵ*/
	ofstream outFile("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/authorPagerank.txt");	
	if (!outFile.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	for (int i = 0; i < n; ++i)
		outFile << authorName[ID[i]] << "\t" << Pagerank[i] << endl;

	/*
	//ͳ��
	int cal[6] = { 0 };
	for (int i = 0; i < n; ++i)
	{
	if (Pagerank[i] > 0.001) cal[0]++;
	else if (Pagerank[i] > 0.0001) cal[1]++;
	else if (Pagerank[i] > 0.00001) cal[2]++;
	else if (Pagerank[i] > 0.000001) cal[3]++;
	}
	cout << cal[0] << endl << cal[1] << endl << cal[2] << endl << cal[3] << endl;
	*/

	return 0;
}