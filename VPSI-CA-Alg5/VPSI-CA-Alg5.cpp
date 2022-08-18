﻿#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EOF
#define EOF (-1)
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef MAX_BUFFER
#define MAX_BUFFER 5000
#endif
#ifndef MAX_PATH
#ifdef _MAX_PATH
#define MAX_PATH _MAX_PATH
#else
#define MAX_PATH 260
#endif
#endif
#ifndef RESULT
#define RESULT "result_Alg5.tmp"
#endif
#ifndef _VPSI_CA_ALG4_H
#define _VPSI_CA_ALG4_H

#define kBit 128
#define m 26
#define n 10// 10, 11, 12
#define l 14
#define alpha int(1.5 * n)
#define beta 10
//#define lambda 40
#define TimeToTest 50
#endif//_VPSI_CA_ALG2_H
using namespace std;
typedef unsigned long long int Element;
ofstream fp;//文件流


/* 子函数 */
void getInput(Element array[], int size)//获得输入
{
	char buffer[MAX_BUFFER] = { 0 }, cTmp[MAX_PATH] = { 0 };
	rewind(stdin);
	fflush(stdin);
	fgets(buffer, MAX_BUFFER, stdin);
	int cIndex = 0, eIndex = 0;
	for (int i = 0; i < MAX_BUFFER; ++i)
		if (buffer[i] >= '0' && buffer[i] <= '9')
			cTmp[cIndex++] = buffer[i];
		else if (cIndex)
		{
			char* endPtr;
			array[eIndex++] = strtoull(cTmp, &endPtr, 0);
			if (eIndex >= size)
				return;
			cIndex = 0;// Rewind cIndex
			memset(cTmp, 0, strlen(cTmp));// Rewind cTmp
		}
	return;
}

Element getRandom()//获取随机数
{
	Element random = rand();
	random <<= 32;
	random += rand();
	return random;
}

int h(Element x, int cnt)
{
	return (int)((x << cnt) % alpha);
}

Element H(Element x)
{
	return x;
}

Element encode(Element a, Element b)
{
	return a + b;
}

Element decode(Element a, Element b)
{
	return a - b;
}


/* 类 */
class Receiver
{
private:
	Element Y[m] = { NULL };
	Element B[alpha] = { NULL };
	Element k1 = NULL;
	Element k2 = NULL;
	vector<Element> E{};
	vector<Element> Y_pi{};
	vector<Element> W{};
	vector<Element> V{};
	vector<Element> intersection{};

public:
	void input_Y()
	{
		getInput(this->Y, m);
		return;
	}
	void auto_input_Y()
	{
		for (int i = 0; i < m; ++i)
			this->Y[i] = getRandom();
		return;
	}
	void hash_from_Y_to_B()
	{
		for (int i = 0; i < m; ++i)
		{
			int index = h(this->Y[i], 1);
			if (NULL != this->B[index])//如果已经有
			{
				int new_index = h(this->B[index], 2);//再次哈希
				if (NULL == this->B[new_index])//如果没有冲突了
					this->B[new_index] = this->B[index];
			}
			this->B[index] = this->Y[i];
		}
		return;
	}
	void rand_k1()
	{
		this->k1 = getRandom();
	}
	void rand_k2()
	{
		this->k2 = getRandom();
	}
	Element send_k1()
	{
		return this->k1;
	}
	Element send_k2()
	{
		return this->k2;
	}
	void generate_E()
	{
		for (int i = 0; i < l; ++i)
			this->E.push_back(this->Y[i]);
		random_shuffle(this->E.begin(), this->E.end());
		random_shuffle(this->E.begin(), this->E.end());
		return;
	}
	void shuffle_from_Y_to_Y_pi()
	{
		for (int i = 0; i < m; ++i)
			this->Y_pi.push_back(this->Y[i]);
		for (int i = 0; i < l; ++i)
			this->Y_pi.push_back(this->E[i]);
		random_shuffle(this->Y_pi.begin(), this->Y_pi.end());
		return;
	}
	vector<Element> send_Y_pi()
	{
		return this->Y_pi;
	}
	void receive_W(vector<Element> W)
	{
		this->W.assign(W.begin(), W.end());
		return;
	}
	bool verify()
	{
		for (int i = 0; i < l; ++i)
			if (find(this->W.begin(), this->W.end(), this->E[i]) == this->W.end())
				return false;
		return true;
	}
	void rand_V()
	{
		while (this->V.size() < alpha)
		{
			Element tmp = getRandom();
			if (find(this->V.begin(), this->V.end(), tmp) == this->V.end())
				this->V.push_back(tmp);
		}
		return;
	}
	void printIntersection()
	{
		for (size_t i = 0; i < this->V.size(); ++i)
			if (find(this->W.begin(), this->W.end(), this->V[i]) != this->W.end())
				this->intersection.push_back(this->V[i]);
#ifdef _DEBUG
		if (this->intersection.size())
		{
			cout << "| V ∩ W | - l = | { " << this->intersection[0];
			for (size_t i = 1; i < this->intersection.size(); ++i)
				cout << ", " << this->intersection[i];
			cout << " } | - " << l << " = " << this->intersection.size() - l << endl;
		}
		else
			cout << "| V ∩ W | - l < 0" << endl;
#else
		cout << "| V ∩ W | - l = " << this->intersection.size() - l << endl;
#endif
		return;
	}
	size_t printSize(string eleName, bool isToFile)
	{
		size_t baseNum = kBit / (sizeof(Element) << 3);
		(isToFile ? fp : cout) << "sizeof(Receiver) = " << sizeof(Receiver) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "sizeof(" << eleName << ") = " << sizeof(this) * baseNum << " KB" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".Y) = " << sizeof(this->Y) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".B) = " << sizeof(this->B) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k1) = " << sizeof(this->k1) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k2) = " << sizeof(this->k2) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".E) = " << sizeof(this->E) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".Y_pi) = " << sizeof(this->Y_pi) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".W) = " << sizeof(this->W) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".V) = " << sizeof(this->V) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".intersection) = " << sizeof(this->intersection) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".*) = " << (sizeof(this->k1) + sizeof(this->k2) + sizeof(this->Y_pi) + sizeof(this->W)) * baseNum << " B (*)" << endl;
		return (sizeof(this->k1) + sizeof(this->k2) + sizeof(this->Y_pi) + sizeof(this->W)) * baseNum;
	}
};
Receiver R;

class Sender
{
private:
	Element X[n] = { NULL };
	Element B[alpha] = { NULL };
	Element k1 = NULL;
	Element k2 = NULL;
	vector<Element> E{};
	Element V[n + l] = { NULL };
	vector<Element> V_pi{};
	vector<Element> X_pi{};
	vector<vector<Element>> P_b{};
	vector<Element> T_b{};

public:
	void input_X()
	{
		getInput(this->X, n);
		return;
	}
	void auto_input_X()
	{
		for (int i = 0; i < n; ++i)
			this->X[i] = getRandom();
		return;
	}
	void hash_from_X_to_B()
	{
		for (int i = 0; i < n; ++i)
		{
			int index = h(this->X[i], 1);
			if (NULL != this->B[index])//如果已经有
			{
				int new_index = h(this->B[index], 2);//再次哈希
				if (NULL == this->B[new_index])//如果没有冲突了
					this->B[new_index] = this->B[index];
			}
			this->B[index] = this->X[i];
		}
		return;
	}
	void receive_k1(Element k1)
	{
		this->k1 = k1;
		return;
	}
	void receive_k2(Element k2)
	{
		this->k2 = k2;
		return;
	}
	void generate_E()
	{
		for (int i = 0; i < l; ++i)
			this->E.push_back(getRandom());
		random_shuffle(this->E.begin(), this->E.end());
		random_shuffle(this->E.begin(), this->E.end());
		return;
	}
	void rand_V()
	{
		for (int i = 0; i < alpha; ++i)
			this->V[i] = getRandom();
		return;
	}
	void shuffle_from_V_to_V_pi()
	{
		for (int i = 0; i < n + l; ++i)
			this->V_pi.push_back(this->V[i]);
		random_shuffle(this->V_pi.begin(), this->V_pi.end());
		return;
	}
	void shuffle_from_X_to_X_pi()
	{
		for (int i = 0; i < n; ++i)
			this->X_pi.push_back(this->X[i]);
		for (int i = 0; i < l; ++i)
			this->X_pi.push_back(this->E[i]);
		random_shuffle(this->X_pi.begin(), this->X_pi.end());
		return;
	}
	void generate_P_b()
	{
		for (int i = 0; i < alpha && this->P_b.size() < beta; ++i)
		{
			vector<Element> tmp{ this->X_pi[i], H(this->X_pi[i]) ^ this->V[i] };
			if (find(this->P_b.begin(), this->P_b.end(), tmp) == this->P_b.end())
				this->P_b.push_back(tmp);
		}
		return;
	}
	void generate_T_b()
	{
		for (size_t i = 0; i < this->P_b.size(); ++i)
			this->T_b.push_back(encode(this->P_b[i][0], this->P_b[i][1]));
		return;
	}
	vector<Element> send_T_b()
	{
		return this->T_b;
	}
	size_t printSize(string eleName, bool isToFile)
	{
		size_t baseNum = kBit / (sizeof(Element) << 3);
		(isToFile ? fp : cout) << "sizeof(Sender) = " << sizeof(Sender) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "sizeof(" << eleName << ") = " << sizeof(this) * baseNum << " KB" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".X) = " << sizeof(this->X) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".B) = " << sizeof(this->B) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k1) = " << sizeof(this->k1) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k2) = " << sizeof(this->k2) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".E) = " << sizeof(this->E) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".V) = " << sizeof(this->V) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".V_pi) = " << sizeof(this->V_pi) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".X_pi) = " << sizeof(this->X_pi) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".P_b) = " << sizeof(this->P_b) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".T_b) = " << sizeof(this->T_b) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".*) = " << (sizeof(this->k1) + sizeof(this->k2) + sizeof(this->T_b)) * baseNum << " B (*)" << endl;
		return (sizeof(this->k1) + sizeof(this->k2) + sizeof(this->T_b)) * baseNum;
	}
};
Sender S;

class Cloud
{
private:
	vector<Element> T_b{};
	vector<Element> Y_pi{};
	Element omega_b[alpha] = { NULL };
	vector<Element> W = { NULL };

public:
	void receive_T_b(vector<Element> T_b)
	{
		this->T_b.assign(T_b.begin(), T_b.end());
		return;
	}
	void receive_Y_pi(vector<Element> Y_pi)
	{
		this->Y_pi.assign(Y_pi.begin(), Y_pi.end());
		return;
	}
	void compute_omega_b()
	{
		for (int i = 0; i < alpha; ++i)
			this->omega_b[i] = decode(this->T_b[i % this->T_b.size()], this->Y_pi[i]);
		return;
	}
	void rand_W()
	{
		for (int i = 0; i < alpha; ++i)
			this->W.push_back(this->omega_b[i]);
		random_shuffle(this->W.begin(), this->W.end());
		return;
	}
	vector<Element> send_W()
	{
		return this->W;
	}
	size_t printSize(string eleName, bool isToFile)
	{
		size_t baseNum = kBit / (sizeof(Element) << 3);
		(isToFile ? fp : cout) << "sizeof(Cloud) = " << sizeof(Cloud) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "sizeof(" << eleName << ") = " << sizeof(this) * baseNum << " KB" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".T_b) = " << sizeof(this->T_b) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".Y_pi) = " << sizeof(this->Y_pi) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".*) = " << (sizeof(this->T_b) + sizeof(this->Y_pi)) * baseNum << " B (*)" << endl;
		return (sizeof(this->T_b) + sizeof(this->Y_pi)) * baseNum;
	}
};
Cloud C;


/* 子函数 */
void input(bool isAuto)
{
	if (isAuto)
	{
		S.auto_input_X();
		R.auto_input_Y();
	}
	else
	{
		S.input_X();
		R.input_Y();
	}
	return;
}

void protocol()
{
	R.rand_k1();
	R.rand_k2();
	R.hash_from_Y_to_B();
	S.hash_from_X_to_B();
	S.receive_k1(R.send_k1());
	S.receive_k2(R.send_k2());
	S.generate_E();
	S.rand_V();
	S.shuffle_from_V_to_V_pi();
	S.shuffle_from_X_to_X_pi();
	S.generate_P_b();
	S.generate_T_b();
	C.receive_T_b(S.send_T_b());
	R.generate_E();
	R.shuffle_from_Y_to_Y_pi();
	C.receive_Y_pi(R.send_Y_pi());
	C.compute_omega_b();
	C.rand_W();
	R.receive_W(C.send_W());
	cout << "verify = " << (R.verify() ? "true" : "false") << endl;
	R.rand_V();
	R.printIntersection();
	return;
}



/* main 函数 */
void test()
{
	input(true);
	protocol();
	return;
}

bool dump(clock_t start_time, clock_t end_time)
{
	fp.open(RESULT);
	if (fp)
	{
		fp << "/**************************************** VPSI-CA Alg. 5 ****************************************/" << endl;
		fp << "kBit = " << kBit << "\t\tm = 2 ** " << m << "\t\tn = 2 ** " << n << "\t\tl = 2 ** " << l << endl;
		fp << "Time: " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) << " / " << TimeToTest << " = " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) / TimeToTest << "ms" << endl;
		fp << "sizeof(*) = " << ((R.printSize("R", true) + S.printSize("S", true) + C.printSize("C", true)) >> 2) << " KB (*)" << endl << endl;
		fp.close();
		return true;
	}
	else
		return false;
}

int main()
{
	clock_t start_time = clock();
	for (int i = 0; i < TimeToTest; ++i)
	{
		cout << "/**************************************** Time: " << i + 1 << " ****************************************/" << endl;
		test();
		cout << endl << endl;
	}
	clock_t end_time = clock();
	cout << endl;
	cout << "/**************************************** VPSI-CA Alg. 5 ****************************************/" << endl;
	cout << "kBit = " << kBit << "\t\tm = 2 ** " << m << "\t\tn = 2 ** " << n << "\t\tl = 2 ** " << l << endl;
	cout << "Time: " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) << " / " << TimeToTest << " = " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) / TimeToTest << "ms" << endl;
	cout << "sizeof(*) = " << ((R.printSize("R", false) + S.printSize("S", false) + C.printSize("C", false)) >> 2) << " KB (*)" << endl << endl;
	return dump(start_time, end_time) ? EXIT_SUCCESS : EXIT_FAILURE;
}