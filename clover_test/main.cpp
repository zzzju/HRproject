#include<iostream>
#include <vector>
#include <chrono>
#include <set>
#include <string>
#include "../HRproject/clover.h"
#pragma comment(lib,"clover.lib")
using namespace std;

void CacHP(double hr, int age, bool isage,double& SBP,double& DBP)
{
	
	if (isage)
	{
		if (age <= 65)
		{
			SBP = 0.3*hr + 95;
			DBP = 0.65*hr + 36;
		}
		else
		{
			SBP = 1.5*hr + 32;
			DBP = 0.85*hr + 18;
		}
	}
	else
	{
		SBP = 0.5*(0.3*hr + 95) + 0.5*(1.5*hr + 32);
		DBP = 0.5*(0.65*hr + 36) + 0.5*(0.85*hr + 18);
	}
}





void main()
{
	HRindex p;
	getchar();

}
/*
int main()
{

	//data generation
	vector<int> a,b;
	for (int i = 0; i < 5; i++) {
		a.push_back(i);
	}
	for (int i = 90; i <=95; i+=5) {
		b.push_back(i);
	}
	LISTHR lst;
	LISTHP pst;
	int i = 0;
	for (size_t i = 0; i <= 100; i=i+1)
	{
		lst.push_back(make_pair(a,i));
		pst.push_back(make_pair(a,b));
	}

	HRindex p;
	index* iHR;

	p.HRMonitor(lst,day);
	iHR = p.result_inx;//获取分析型指标中的子指标
	
	//for (size_t i = 0; i < 8; i++)
	//{
	//	//cout << p.result_inx->his[i] << endl;
	//	cout << iHR->his[i] << endl;
	//}
	
	//获取分析型指标
	cout << p.HR_diff(lst) << endl;
	cout << p.HR_FI(lst) << endl;
	cout << p.HR_FI_B(lst) << endl;
	cout << p.HR_PI(lst) << endl;
	cout << p.HR_PI_B(lst) << endl;
	cout << p.HR_risk(lst) << endl;
	cout << p.HR_SI(lst) << endl;
	cout << p.HP(pst) << endl;

	getchar();

	return 0;
}
*/