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
	/*读入author名字和id，注意，authorID并不是连续的，共17551个author，但是最大的id为18648*/
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
		inFile.get();						//读入id
		inFile.getline(name, 100);			//读入author名字
		id2num[id] = i;						//给不连续的id重新安排连续的ID号（1-17551）
		strcpy_s(authorName[i], name);
	}

	/*读入author之间的引用关系，记录到矩阵p中*/
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
	
	/*初始化Pagerank值，为1/n*/
	for (int i = 0; i < n; ++i)	Pagerank[i] = 1 / (double)n;

	/*迭代计算Pagerank值，Pagerank=alpha*Pagerank+(1-alpha)/n */
	/*停止条件为每个author的Pagerank都小于0.00001*/
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

	/*Pagerank排序*/
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

	/*输出排序后的author及其Pagerank值*/
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
	//统计
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