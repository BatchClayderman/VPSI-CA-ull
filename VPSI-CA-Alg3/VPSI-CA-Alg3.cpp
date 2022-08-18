#include <iostream>
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
#define RESULT "result_Alg3.tmp"
#endif

#ifndef _VPSI_CA_ALG3_H
#define _VPSI_CA_ALG3_H
#define kBit 128
#define m 26
#define n 10// 10, 11, 12
//#define l 14
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
	Element Y[m];
	Element B[alpha] = { NULL };
	Element k = NULL;
	vector<Element> Y_pi{};
	vector<Element> W1{};
	vector<Element> W2{};
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
	void rand_k()
	{
		this->k = getRandom();
		return;
	}
	Element send_k()
	{
		return this->k;
	}
	void shuffle_from_B_to_Y_pi()
	{
		for (int i = 0; i < alpha; ++i)
			this->Y_pi.push_back(this->B[i]);
		random_shuffle(Y_pi.begin(), Y_pi.end());
		return;
	}
	vector<Element> send_Y_pi()
	{
		return this->Y_pi;
	}
	void receive_W1(vector<Element> W1)
	{
		this->W1.assign(W1.begin(), W1.end());
		return;
	}
	void receive_W2(vector<Element> W2)
	{
		this->W2.assign(W2.begin(), W2.end());
		return;
	}
	bool verify()
	{
		return this->W1 == this->W2;
	}
	void rand_V()
	{
		while (this->V.size() < n)
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
			if (find(this->W1.begin(), this->W1.end(), this->V[i]) != this->W1.end() || find(this->W2.begin(), this->W2.end(), this->V[i]) != this->W2.end())
				this->intersection.push_back(this->V[i]);
#ifdef _DEBUG
		if (this->intersection.size())
		{
			cout << "| V ∩ W | = | { " << this->intersection[0];
			for (size_t i = 1; i < this->intersection.size(); ++i)
				cout << ", " << this->intersection[i];
			cout << " } | = " << this->intersection.size() << endl;
		}
		else
			cout << "| V ∩ W | = 0" << endl;
#else
		cout << "| V ∩ W | = " << this->intersection.size() << endl;
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
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k) = " << sizeof(this->k) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".Y_pi) = " << sizeof(this->Y_pi) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".W1) = " << sizeof(this->W1) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".W2) = " << sizeof(this->W2) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".V) = " << sizeof(this->V) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".intersection) = " << sizeof(this->intersection) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".*) = " << (sizeof(this->Y_pi) + sizeof(this->W1) + sizeof(this->W2)) * baseNum << " B" << endl;
		return (sizeof(this->Y_pi) + sizeof(this->W1) + sizeof(this->W2)) * baseNum;
	}
};
Receiver R;

class Sender
{
private:
	Element X[n] = { NULL };
	Element B[alpha] = { NULL };
	Element k = NULL;
	Element V[alpha] = { NULL };
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
	void receive_k(Element k)
	{
		this->k = k;
		return;
	}
	void rand_V()
	{
		for (int i = 0; i < alpha; ++i)
			this->V[i] = getRandom();
		return;
	}
	void shuffle_from_B_to_X_pi()
	{
		for (int i = 0; i < alpha; ++i)
			this->X_pi.push_back(this->B[i]);
		random_shuffle(this->X_pi.begin(), this->X_pi.end());
		return;
	}
	void create_P_b()
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
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".k) = " << sizeof(this->k) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".V) = " << sizeof(this->V) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".X_pi) = " << sizeof(this->X_pi) * baseNum << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".P_b) = " << sizeof(this->P_b) << " B" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".T_b) = " << sizeof(this->T_b) * baseNum << " B (*)" << endl;
		(isToFile ? fp : cout) << "\tsizeof(" << eleName << ".*) = " << (sizeof(this->k) + (sizeof(this->T_b) << 1)) * baseNum << " B (*)" << endl;
		return (sizeof(this->k) + (sizeof(this->T_b) << 1)) * baseNum;
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
Cloud C1, C2;


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
	R.hash_from_Y_to_B();
	S.hash_from_X_to_B();
	R.rand_k();
	S.receive_k(R.send_k());
	S.rand_V();
	S.shuffle_from_B_to_X_pi();
	S.create_P_b();
	S.generate_T_b();
	C1.receive_T_b(S.send_T_b());
	C2.receive_T_b(S.send_T_b());
	R.shuffle_from_B_to_Y_pi();
	C1.receive_Y_pi(R.send_Y_pi());
	C2.receive_Y_pi(R.send_Y_pi());
	C1.compute_omega_b();
	C1.rand_W();//C1.rand_W1();
	R.receive_W1(C1.send_W());//R.receive_W1(C1.send_W1());
	C2.compute_omega_b();
	C2.rand_W();//C1.rand_W2();
	R.receive_W2(C2.send_W());//R.receive_W2(C2.send_W2());
	cout << "verify = " << (R.verify() ? "true" : "false") << endl;
	R.rand_V();
	R.printIntersection();
	return;
}



/* main 函数 */
void test()
{
	time_t t;
	srand((unsigned int)time(&t));
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
		fp << "kBit = " << kBit << "\t\tm = 2 ** " << m << "\t\tn = 2 ** " << n << endl;
		fp << "Time: " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) << " / " << TimeToTest << " = " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) / TimeToTest << "ms" << endl;
		fp << "sizeof(*) = " << ((R.printSize("R", true) + S.printSize("S", true) + C1.printSize("C1", true) + C2.printSize("C2", true)) >> 2) << " KB (*)" << endl << endl;
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
	cout << "/**************************************** VPSI-CA Alg. 3 ****************************************/" << endl;
	cout << "kBit = " << kBit << "\t\tm = 2 ** " << m << "\t\tn = 2 ** " << n << endl;
	cout << "Time: " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) << " / " << TimeToTest << " = " << ((double)end_time - start_time) * kBit / (sizeof(Element) << 3) / TimeToTest << "ms" << endl;
	cout << "sizeof(*) = " << ((R.printSize("R", false) + S.printSize("S", false) + C1.printSize("C1", false) + C2.printSize("C2", false)) >> 2) << " KB (*)" << endl << endl;
	return dump(start_time, end_time) ? EXIT_SUCCESS : EXIT_FAILURE;
}