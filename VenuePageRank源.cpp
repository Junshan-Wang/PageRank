#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <string.h>
#include <math.h>
using namespace std;

double alpha = 0.85;
int N = 0;
float p[1000][1000] = { 0 };
float Pagerank[1000] = { 0 };
int paper2venue[40000000] = { 0 };
char venueName[1000][1000] = { 0 };

/*��ȡpaper��ID������ת��Ϊ���*/
int getPaperID(char *buf)
{
	int paperID = 0;
	for (int i = 0; i < 9; ++i)
	{
		if (i == 0) paperID += (buf[i] - 65) * 1000000;
		else if (i > 0 && i < 3) paperID += (buf[i] - 48)*pow(10, 6 - i);
		else if (i == 3) continue;
		else if (i<8) paperID += (buf[i] - 48)*pow(10, 7 - i);
		else if (buf[i] != '}' && buf[i]!=' ' && buf[i]!='\0')	paperID += 1000000 * ((buf[i] - 97) + 26);
	}
	return paperID;
}

int main()
{
	/*����Ԥ���������ǳ�ʼ�ļ�����Ҫ������еȸ������*/
	/*����ÿƪpaper�������ڵ�venue*/
	ifstream inVenue("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2012/acl-metadata.txt");
	if (!inVenue.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	while (!inVenue.eof())
	{
		int paperID;
		char buf[1000] = { 0 };
		inVenue.getline(buf, 1000);
		if (buf[0] == '}') inVenue.getline(buf, 1000);
		if (buf[0] == '\0')
		{
			inVenue.getline(buf, 1000);
			if (buf[0] == '\0')
			{
				inVenue.getline(buf, 1000);
				if (buf[0] == '\0') break;
			}
		}
		
		paperID = getPaperID(buf + 6);

		inVenue.getline(buf, 1000);
		inVenue.getline(buf, 1000);

		inVenue.getline(buf, 1000);
		int i;
		for (i = 0; buf[i+9] != '}'; ++i)
			buf[i] = buf[i + 9];
		buf[i] = '\0';
		int flag = 0;
		for (int i = 0; i < N; ++i)
			if (strcmp(buf, venueName[i]) == 0)
			{
				paper2venue[paperID] = i;
				flag = 1;
			}
		if (flag==0)
		{
			paper2venue[paperID] = N;
			for (int j = 0; buf[j] != '\0'; ++j)
				venueName[N][j] = buf[j];	
			N++;		
		}
		inVenue.getline(buf, 1000);
	}

	/*����paper֮������ù�ϵ���õ�venue֮������ù�ϵ������¼������p��*/
	ifstream in_relationship("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/release/2012/acl.txt");
	if (!in_relationship.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	while (!in_relationship.eof())
	{
		int paperID, beginID, endID;
		char buf[1000] = { 0 };
		in_relationship.getline(buf, 100);
		if (buf[0] == '\0') break;

		paperID = getPaperID(buf);
		beginID = paper2venue[paperID];

		if (buf[8] == ' ') paperID = getPaperID(buf + 13);
		else paperID = getPaperID(buf + 14);
		endID = paper2venue[paperID];

		p[beginID][endID] ++;
	}
	for (int i = 0; i < N; ++i)
	{
		double sum = 0;
		for (int j = 0; j < N; ++j)
			if (p[i][j] !=0) sum+=p[i][j];
		if (sum != 0) sum = 1 / sum;
		for (int j = 0; j < N; ++j)
			if (p[i][j] !=0) p[i][j] = sum*p[i][j];
	}

	/*��ʼ��Pagerank*/
	for (int i = 0; i < N; ++i)	Pagerank[i] = 1 / (double)N;

	/*��������Pagerankֵ��Pagerank=alpha*Pagerank+(1-alpha)/n */
	/*ֹͣ����Ϊÿ��author��Pagerank��С��0.00001*/
	while (1)
	{
		double tmp[1000] = { 0 };

		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
				tmp[i] += Pagerank[j] * p[j][i];
			tmp[i] = alpha*tmp[i] + (1 - alpha) / (double)N;
		}

		int flag = 0;
		for (int i = 0; i < N; ++i)
		{
			if (abs(Pagerank[i] - tmp[i])>0.00001)
				flag = 1;
			Pagerank[i] = tmp[i];
		}
		if (flag == 0) break;
	}

	/*Pagerank����*/
	int ID[1000] = { 0 };
	for (int i = 0; i < N; ++i) ID[i] = i;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N - i - 1; ++j)
			if (Pagerank[j] < Pagerank[j + 1])
			{
				float temp;
				temp = Pagerank[j];	Pagerank[j] = Pagerank[j + 1];	Pagerank[j + 1] = temp;
				temp = ID[j];	ID[j] = ID[j + 1];	ID[j + 1] = temp;
			}

	/*���������venue����Pagerank*/
	ofstream outFile("C://Users/user/Documents/Visual Studio 2015/Projects/PageRank/venuePagerank.txt");
	if (!outFile.is_open())
	{
		cout << "Error opening file";
		system("pause");
		return 0;
	}
	for (int i = 0; i < N; ++i)
	{
		outFile << venueName[ID[i]] << "\t" << Pagerank[i] << endl;
	}

	/*
	//ͳ��
	int cal[6] = { 0 };
	for (int i = 0; i < N; ++i)
	{
	if (Pagerank[i] > 0.1) cal[0]++;
	else if (Pagerank[i] > 0.01) cal[1]++;
	else if (Pagerank[i] > 0.001) cal[2]++;
	else if (Pagerank[i] > 0.0001) cal[3]++;
	}
	cout << cal[0] << endl << cal[1] << endl << cal[2] << endl << cal[3] << endl;
	*/

	return 0;
}