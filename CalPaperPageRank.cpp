#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <math.h>
using namespace std;

double alpha = 0.85;
int id2num[40000000] = { 0 };
int num2id[25000] = { 0 };
int N = 0;

double pp[25000] = { 0 };
double Pagerank[25000] = { 0 };

char paperName[25000][1000] = { 0 };

class Inlink
{
public:
	int n;
	int *father;
	double *p;
	Inlink() { n = 0; father = new int[500]; p = new double[500]; }
};
Inlink inlink[25000];

int readPaperID(char *buf)
{
	int ID = 0;
	for (int i = 0; i < 9; ++i)
	{
		if (i == 0) ID += (buf[i] - 65) * 1000000;
		else if (i > 0 && i < 3) ID += (buf[i] - 48)*pow(10, 6 - i);
		else if (i == 3) continue;
		else if (i<8) ID += (buf[i] - 48)*pow(10, 7 - i);
		else if (buf[i] != ' ' && buf[i]!='\0' && buf[i]!='\t')
			ID += 1000000 * ((buf[i] - 97) + 26);
	}
	return ID;
}

int main()
{
	char buf[1000] = { 0 };

	/*读入每个paper的出度，计算p值*/
	ifstream in_outcite("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2013/paper_outcites.txt");
	if (!in_outcite.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	while (!in_outcite.eof())
	{
		in_outcite.get(buf, 10);
		if (buf[0] == '\0') break;
		int cha = 0;
		int beginID = 0, beginNUM = 0;
		beginID = readPaperID(buf);
		id2num[beginID] = N;
		num2id[N] = beginID;

		double s;
		in_outcite >> s;
		if (s!=0) pp[N] = 1 / s;
		in_outcite.get();
		N++;		
	}

	/*读入paper之间的引用关系，记录到p矩阵中*/
	ifstream in_relationship("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2013/acl.txt");
	if (!in_relationship.is_open())
	{
		cout << "Error opening file"; 
		system("pause");
		return 0;
	}
	while (!in_relationship.eof())
	{
		in_relationship.getline(buf, 100);
		if (buf[0] == '\0') break;

		int cha = 0;
		int beginID = 0, beginNUM = 0;
		beginID = readPaperID(buf);
		beginNUM = id2num[beginID];


		int endID = 0, endNUM = 0;
		if (buf[8]==' ') endID = readPaperID(buf + 13);
		else endID = readPaperID(buf + 14);
		endNUM = id2num[endID];
			
		inlink[endNUM].father[inlink[endNUM].n] = beginNUM;
		inlink[endNUM].p[inlink[endNUM].n] = pp[beginNUM];
		inlink[endNUM].n++;
	}

	/*初始化Pagerank*/
	for (int i = 0; i < N; ++i)	Pagerank[i] = 1 / (double)N;

	/*迭代计算Pagerank值，Pagerank=alpha*Pagerank+(1-alpha)/n */
	/*停止条件为每个author的Pagerank都小于0.0000001*/
	while (1)
	{
		double tmp[25000] = { 0 };
		
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < inlink[i].n; ++j)
				tmp[i] += Pagerank[inlink[i].father[j]] * inlink[i].p[j];
			tmp[i] = alpha*tmp[i] + (1 - alpha) / (double)N;
		}
		
		int flag = 0;
		for (int i = 0; i < N; ++i)
		{
			if (abs(Pagerank[i] - tmp[i])>0.0000001)
				flag = 1;
			Pagerank[i] = tmp[i];
		}
		if (flag == 0) break;
					
	}

	/*Pagerank排序*/
	int ID[19000] = { 0 };
	for (int i = 0; i < N; ++i) ID[i] = i;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N - i - 1; ++j)
			if (Pagerank[j] < Pagerank[j + 1])
			{
				float temp;
				temp = Pagerank[j];	Pagerank[j] = Pagerank[j + 1];	Pagerank[j + 1] = temp;
				temp = ID[j];	ID[j] = ID[j + 1];	ID[j + 1] = temp;
			}

	/*读入paper名字*/
	ifstream inFile("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2013/paper_ids.txt");
	if (!inFile.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	while (!inFile.eof())
	{
		int id;
		inFile.getline(buf, 1000);
		if (buf[0] == '\0') break;
		if (buf[1] == 'u') 	inFile.getline(buf, 1000);
		id = readPaperID(buf);
		int delta;
		if (buf[8] == '\t') delta = 9;
		else delta = 10;
		for (int j=0; buf[j+delta] != '\0'; ++j)
			paperName[id2num[id]][j] = buf[j+delta];
	}
	
	/*输出排序后的paper名字及其Pagerank值*/
	ofstream outFile("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/paperPagerank.txt");
	for (int i = 0; i < N; ++i)
		outFile << paperName[ID[i]] << "\t" << Pagerank[i] << endl;

	/*
	//统计
	int cal[6] = { 0 };
	for (int i = 0; i < N; ++i)
	{
		if (Pagerank[i] > 0.01) cal[0]++;
		else if (Pagerank[i] > 0.001) cal[1]++;
		else if (Pagerank[i] > 0.0001) cal[2]++;
		else if (Pagerank[i] > 0.00001) cal[3]++;
		else if (Pagerank[i] > 0.000001) cal[4]++;
		else cal[5]++;
	}
	cout << cal[0] << endl << cal[1] << endl << cal[2] << endl << cal[3] << endl << cal[4] << endl << cal[5] << endl;
	*/

	return 0;
}
