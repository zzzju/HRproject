// Dll1.cpp: 定义 DLL 应用程序的导出函数。
//
#define _CRT_SECURE_NO_WARNINGS
#define AFX_CLASS
#include "clover.h"
#include "stdlib.h"
#include <ctime>
#include <cmath>
#include <chrono>
#include <iostream>
#include<algorithm>

//#include <algorithm>
#include <string>

using namespace std;

size_t rest_hr;
size_t day_hr;
size_t night_hr;
int s_time = 7;


void CacHP(double hr,int age,bool isage)
{
	double SBP, DBP;
	if (isage)
	{
		if (age<=65)
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
		SBP = 0.5*(0.3*hr + 95)+0.5*(1.5*hr + 32);
		DBP = 0.5*(0.65*hr + 36)+0.5*(0.85*hr + 18);
	}
}

//心率均值
double average(vector<double> a, int n)
{
	double sum = 0;
	int i;

	for (i = 0; i < n; i++) {
		sum = sum + a[i];
	}
	sum = sum / n;

	return sum;
}

//心率标准差
double variance(vector<double> a, int n)
{
	double sum = 0;
	double ave;
	int i;

	ave = average(a, n);
	for (i = 0; i < n; i++) {
		sum = (ave - a[i]) * (ave - a[i]) + sum;
	}
	sum = sum / n;
	sum = sqrt(sum);

	return sum;
}


//求最大值
double Max(vector<double> a, int n)
{
	int len = a.size();
	int i;

	double max = a[0];

	for (i = 0; i < len; i++) {
		if (a[i] > max) max = a[i];
	}

	return max;
}

//求最小值
double Min(vector<double> a, int n)
{
	int len = a.size();
	int i;

	double min = a[0];

	for (i = 0; i < len; i++) {
		if (a[i] < min) min = a[i];
	}

	return min;
}

//偏高心率时间序列占比，均值，标准差
//定义函数返回值结构指针
struct high_result
{
	double high_ratio;
	double high_avg;
	double high_var;
};
//计算
high_result *High_Heart(vector<double> a, int n)
{
	high_result *high_test = new high_result;

	double high_ratio;
	double high_avg;
	double high_var;

	double max;
	double value;

	int num = 0;
	int i;

	vector<double> high_temp;

	max = Max(a, n);
	value = 2 * max / 3;
	//占比、均值、标准差
	for (i = 0; i < n; i++) {
		if (a[i] > value) {
			high_temp.push_back(a[i]);
			num = num + 1;
		}
	}
	high_test->high_ratio = 1.0*num / n;
	high_test->high_avg = average(high_temp, num);
	high_test->high_var = variance(high_temp, num);

	return high_test;
}

//偏低心率时间序列占比，均值，标准差
struct low_result
{
	double low_ratio;
	double low_avg;
	double low_var;
};
low_result *Low_Heart(vector<double> a, int n)
{
	low_result *low_test = new low_result;

	double low_ratio;
	double low_avg;
	double low_var;

	double max;
	double value;

	int num = 0;
	int i;

	vector<double> low_temp;

	max = Max(a, n);
	value = max / 3;
	for (i = 0; i < n; i++) {
		if (a[i] < value) {
			low_temp.push_back(a[i]);
			num = num + 1;
		}
	}
	low_test->low_ratio = 1.0*num / n;
	low_test->low_avg = average(low_temp, num);
	low_test->low_var = variance(low_temp, num);

	return low_test;
}

//中间心率时间序列占比，均值，标准差
struct mid_result
{
	double mid_ratio;
	double mid_avg;
	double mid_var;
};
mid_result *Mid_Heart(vector<double> a, int n)
{
	mid_result *mid_test = new mid_result;

	double mid_ratio;
	double mid_avg;
	double mid_var;

	double max;
	double value1;
	double value2;

	int num = 0;
	int i;

	vector<double> mid_temp;

	max = Max(a, n);
	value1 = max / 3;
	value2 = 2 * max / 3;
	for (i = 0; i < n; i++) {
		if ((a[i] > value1) && (a[i] < value2)) {
			mid_temp.push_back(a[i]);
			num = num + 1;
		}
	}
	mid_test->mid_ratio = 1.0*num / n;
	mid_test->mid_avg = average(mid_temp, num);
	mid_test->mid_var = variance(mid_temp, num);

	return mid_test;
}
void test() {
	cout << "test" << endl;
}

vector<int> getTime()
{

	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = localtime(&tt);
	int *time = new int[10];
	time[0] = (int)ptm->tm_year + 1900;
	time[1] = (int)ptm->tm_mon + 1;
	time[2] = (int)ptm->tm_mday;
	time[3] = (int)ptm->tm_hour;
	time[4] = (int)ptm->tm_min;
	/*for (size_t i = 0; i < 5; i++)
	{
		cout << time[i] << "-" ;
	}*/
	//cout << endl;
	vector<int> tmp_t(time, time + 5);
	return tmp_t;
}

void addTime(vector<int>& time, int interval)
{
	int month_day = 30;
	bool y_flag = false;
	int a[] = { 1,3,5,7,8,10,12 };
	int b[] = { 4,6,9,11 };
	set<int> aset(a, a + 7);
	set<int> bset(b, b + 4);
	set<int>::iterator it;
	int year, month, day, hour, minute;
	year = time[0];

	if (year % 100 == 0)
	{
		if (year % 400 == 0)
		{
			y_flag = true;
		}
	}
	else
	{
		if (year % 4 == 0)
		{
			y_flag = true;
		}
	}

	//cout << "y_flag:" << y_flag << endl;

	month = time[1];

	it = aset.find(month);
	if (it == aset.end())
	{
		it = bset.find(month);
		if (it == bset.end())
		{
			if (y_flag)
			{
				month_day = 29;
			}
			else
			{
				month_day = 28;
			}
		}
		else
		{
			month_day = 30;
		}
	}
	else
	{
		month_day = 31;
	}
	day = time[2];
	hour = time[3];
	minute = time[4];
	minute += interval;
	hour += minute / 60;
	minute = minute % 60;
	day += hour / 24;
	hour = hour % 24;
	month += day / (month_day + 1);
	day = day % (month_day + 1);
	year += month / 13;
	month = month % 13;


	time[0] = year;
	time[1] = month;
	time[2] = day;
	time[3] = hour;
	time[4] = minute;
	//cout << year << "-" << month << "-" << day << "-" << hour << "-" << minute << endl;
}

void sample::downsample(vector<int>start_time, int interval, bool flag, s_type st)
{
	tmp_hp.clear();
	tmp_out.clear();
	vector<int> tmp_vec;
	vector<int> cur_time = getTime();
	switch (st) {
	case hr:
		tmp_out.push_back(make_pair(cur_time, 0));
	case hr_diff:
		for (size_t i = 0; i <2; i++)
		{
			tmp_out.push_back(make_pair(cur_time, 0));
			addTime(cur_time, interval);
		}
		break;
	case hr_f:
		for (size_t i = 0; i < ((15 / interval)); i++)
		{
			tmp_out.push_back(make_pair(cur_time, 0));
			addTime(cur_time, interval);
		}
		break;
	case hr_p:
		for (size_t i = 0; i < ((15 / interval)); i++)
		{
			tmp_out.push_back(make_pair(cur_time, 0));
			addTime(cur_time, interval);
		}
		break;
	case hr_s:
		if (cur_time[3] < s_time)
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			for (size_t i = 0; i < ((24*60 / interval) - 1); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		else
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			addTime(cur_time, 24 * 60);//change to the next day
			for (size_t i = 0; i < ((24*60 / interval) - 1); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		break;
	case hr_day://7-23
		if (cur_time[3] < s_time)
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			for (size_t i = 0; i < ((16 * 60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		else
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			addTime(cur_time, 24 * 60);//change to the next day
			for (size_t i = 0; i < ((16*60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		break;
	case hr_night://23-7
		if (cur_time[3] < 23)
		{
			cur_time[3] = 23;
			cur_time[4] = 0;
			for (size_t i = 0; i < ((8 * 60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		else
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			addTime(cur_time, 24 * 60);//change to the next day
			for (size_t i = 0; i < ((8 * 60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		break;
	case hr_ad://data from 7 o'clock
		if (cur_time[3] < s_time)
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			for (size_t i = 0; i < ((1440 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		else
		{
			cur_time[3] = s_time;
			cur_time[4] = 0;
			addTime(cur_time, 24 * 60);//change to the next day
			for (size_t i = 0; i < ((1440 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		break;
	case hr_rest:
		if (cur_time[3] < 5)
		{
			cur_time[3] = 5;
			cur_time[4] = 0;
			for (size_t i = 0; i < ((2 * 60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		else
		{
			cur_time[3] = 5;
			cur_time[4] = 0;
			addTime(cur_time, 24 * 60);//change to the next day
			for (size_t i = 0; i < ((2 * 60 / interval) ); i++)
			{
				tmp_out.push_back(make_pair(cur_time, 0));
				addTime(cur_time, interval);
			}
		}
		break;
	case hp:
		vector<int> hp_(2);
		tmp_hp.push_back(make_pair(cur_time, hp_));
		break;
	}
	
};


sample::sample()
{

}

sample::~sample()
{

}


HRindex::HRindex()
{
	result_inx = new index;
}

HRindex::~HRindex()
{
	delete result_inx;
	
}


string HRindex::HR_risk(LISTHR d)
{
	LISTHR::iterator it = d.end();
	it--;

	int heart_rate = (*it).second;

	string tmp_a = "心动过缓";
	string tmp_b = "心动过速";
	string tmp_c;

	if (heart_rate > 150)
	{
		return tmp_b;
	}
	if (heart_rate < 60)
	{
		return tmp_a;
	}
	if ((heart_rate >= 60) && (heart_rate <= 150)) {
		tmp_c = to_string(heart_rate);
		return tmp_c;
	}
}

string HRindex::HR_diff(LISTHR d)
{
	LISTHR::iterator it = d.end();
	it--;
	int tmp_a = (*it).second;
	it--;
	int tmp_b = (*it).second;
	if (abs(tmp_a - tmp_b) > 40)
	{
		return "变动过快";
	}
	else {
		return "变动正常";
	}
	/*
	else
	return "变动过缓";
	*/

}
double HRindex::Rest_HR(LISTHR d)
{
	LISTHR::iterator iter;
	iter = d.begin();

	vector<double> temp_rest;
	int num_rest = 0;
	
	while (iter != d.end()) {
		vector<int> time = (*iter).first;
		if (time[3] >= 5 && time[3] <= 7) {
			num_rest++;
			temp_rest.push_back((*iter).second);
		}
	}

	double avg_rest;
	avg_rest = average(temp_rest, num_rest);

	return avg_rest;

}
string HRindex::HR_FI(LISTHR d)
{
	size_t tmp_mean = 0;
	LISTHR::iterator itor;
	itor = d.begin();
	while (itor != d.end())
	{
		tmp_mean += (*itor).second;
		itor++;
	};
	int tmp = rest_hr - (tmp_mean / d.size());
	if (abs(tmp) > 0.2*rest_hr)
	{
		return "疲劳";
	}
	else
		return "正常";
}
string HRindex::HR_FI_B(LISTHR d)
{
	size_t tmp_mean = 0;
	LISTHR::iterator itor;
	itor = d.begin();
	while (itor != d.end())
	{
		tmp_mean += (*itor).second;
		itor++;
	};
	int tmp = rest_hr - (tmp_mean / d.size());
	if (abs(tmp) > 0.2*rest_hr)
	{
		return "疲劳";
	}
	else
		return "正常";
}
string HRindex::HR_PI(LISTHR d)
{
	size_t tmp_mean = 0;
	size_t tmp_var = 0;
	LISTHR::iterator itor;
	itor = d.begin();
	while (itor != d.end())
	{
		tmp_mean += (*itor).second;
		itor++;
	};
	tmp_mean = tmp_mean / d.size();
	while (itor != d.end())
	{
		tmp_var = (tmp_mean - (*itor).second)*(tmp_mean - (*itor).second) + tmp_var;
	}
	tmp_var = tmp_var / d.size();
	if (tmp_var>0.1*tmp_mean)
	{
		return "有压力";
	}
	else
	{
		return "正常";
	}
}
string HRindex::HR_PI_B(LISTHR d)
{
	size_t tmp_mean = 0;
	size_t tmp_var = 0;
	LISTHR::iterator itor;
	itor = d.begin();
	while (itor != d.end())
	{
		tmp_mean += (*itor).second;
		itor++;
	};
	tmp_mean = tmp_mean / d.size();
	while (itor != d.end())
	{
		tmp_var = (tmp_mean - (*itor).second)*(tmp_mean - (*itor).second) + tmp_var;
	}
	tmp_var = tmp_var / d.size();
	if (tmp_var>0.1*tmp_mean)
	{
		return "有压力";
	}
	else
	{
		return "正常";
	}
}
string HRindex::HR_SI(LISTHR d)
{
	LISTHR::iterator iter;
	iter = d.begin();

	vector<double> temp_day;
	vector<double> temp_night;

	int num_day = 0;
	int num_night = 0;

	while (iter != d.end()) {
		vector<int> time = (*iter).first;
		if ((time[3] >= s_time) && ((time[3]) < 23) && ((time[3] > 7))) {
			temp_day.push_back((*iter).second);
			num_day++;
		}
		else if (((time[3]) >= 23) && (time[3] < 7)) {
			temp_night.push_back((*iter).second);
			num_night++;
		}
		iter++;
	}

	day_hr = average(temp_day, num_day);
	night_hr = average(temp_night, num_night);


	if (night_hr<day_hr)
	{
		int diff = day_hr - night_hr;
		if (diff>0.1*day_hr)
		{
			return "勺型心率";
		}
		else
		{
			return "非勺型心率";
		}
	}
	else
	{
		cout << "Non-spoonful heart rate" << endl;
		return  "非勺型心率";
	}
}
string HRindex::HP(LISTHP d)
{
	int sbp = d.back().second[0];
	int dbp = d.back().second[1];
	if ((sbp>90 && sbp<140) || (dbp>60 && dbp<90))
	{
		return"正常血压";
	}
	else
	{
		if (sbp>140 || dbp>90)
		{
			return "高血压";
		}
		else if (sbp<90 || dbp<60)
		{
			return "低血压";
		}
	}
}

void HRindex::HRMonitor(LISTHR d, period p)
{
	LISTHR::iterator iter;
	iter = d.begin();

	vector<double> HR_data;

	int num = 0;

	if (p == day) {

		while (iter != d.end()) {
			vector<int> time = (*iter).first;
			
			if (time[3] >= 7 && time[3] <= 23) {
				HR_data.push_back((*iter).second);
				num = num + 1;
			}
			iter++;
		
		}

	}
	else if (p == night) {
		
		while (iter != d.end()) {
			vector<int> time = (*iter).first;

			if (time[3] >= 23 && time[3] <= 7) {
				HR_data.push_back((*iter).second);
				num = num + 1;
			}
			iter++;

		}
	}
	else if (p == allday) {

		while (iter != d.end()) {
			//vector<int> time = (*iter).first;
			HR_data.push_back((*iter).second);
			num = num + 1;
			iter++;
		}

	}
	else if (p == rest) {

		while (iter != d.end()) {
			vector<int> time = (*iter).first;

			if (time[3] >= 5 && time[3] <= 7) {
				HR_data.push_back((*iter).second);
				num = num + 1;
			}
			iter++;

		}
	}

	/*
	while (iter != d.end()) {
		//vector<int> time = (*iter).first;
		HR_data.push_back((*iter).second);
		num = num + 1;
		iter++;
	}

	*/
	
	//调用结构体指针为指标赋值
	//struct index *temp;
	
	Histogram h(minval_, gap_, bins_);
	h.record(d);
	result_inx->his=h.vbins;
	//method是需要用户自己选择的
	//int method;
	//p = (enum period)method;

	result_inx->HRMean = average(HR_data, num);
	result_inx->HRStd = variance(HR_data, num);
	result_inx->maxHR = Max(HR_data, num);
	result_inx->minHR = Min(HR_data, num);

	high_result *high_heart;
	high_heart = High_Heart(HR_data, num);
	result_inx->HP = (high_heart->high_ratio);
	result_inx->HMean = (high_heart->high_avg);
	result_inx->HStd = (high_heart->high_var);
	delete high_heart;

	low_result *low_heart;
	low_heart = Low_Heart(HR_data, num);
	result_inx->LP = (low_heart->low_ratio);
	result_inx->LMean = (low_heart->low_avg);
	result_inx->LStd = (low_heart->low_var);
	delete low_heart;

	mid_result *mid_heart;
	mid_heart = Mid_Heart(HR_data, num);
	result_inx->NP = (mid_heart->mid_ratio);
	result_inx->NMean = (mid_heart->mid_avg);
	result_inx->NStd = (mid_heart->mid_var);
	delete mid_heart;
}






Histogram::Histogram(size_t m, size_t g, size_t n) :minval_(m), maxval_(m + n * g - 1), gap_(g), bins(new size_t[n])
{
	for (size_t i = 0; i < n; i++)
	{
		bins[i] =0 ;
	}
	vector<int> t(bins, bins + n);
	vbins = t;
	assert(g != 0 && n != 0);
}

void Histogram::record(LISTHR data)
{

	LISTHR::iterator it;
	it = data.begin();
	//std::for_each(data.begin(), data.end(), [](int i) {});
	while (it!=data.end())
	{
		int tmp = (*it++).second;
		//++bins[(tmp - minval_) / gap_];
		++vbins[(tmp - minval_) / gap_];


	}
	
	for (size_t i = 0; i < 8; i++)
	{
		//cout << vbins[i] << endl;
	}
}

Histogram::~Histogram()
{
	delete[] bins;
}
