// lbgalgorithm.cpp : Defines the entry point for the console application.
//
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <functional>
#include <algorithm>
#include <string.h>

using namespace std;


int main(int argc, char* argv)
{
	int iteration = 1;

	int states = 5; int i = 0; int j = 0; double a; int obs = 32;  int cbsize = 32; 
	double cur_ans = 100000, prev_ans = 10;

	ifstream inputfile1("a.txt");				//initialising matrix A
	vector<vector<double> > matrixa;
	i = 0; j = 0;
	while (i < states)
	{
		j = 0; vector <double> v;
		while (j < states)
		{
			inputfile1 >> a;
			v.push_back(a);
			j++;
		}
		matrixa.push_back(v);
		i++;
	}
	//cout << matrixa.size() < endl;
	cout << "Input A taken\n";
	//getchar();

	ifstream inputfile2("b.txt");						//initialising matrix B
	vector<vector<double> > matrixb;
	i = 0; j = 0;
	while (i < states)
	{
		j = 0; vector <double> v;
		while (j < obs)
		{
			inputfile2 >> a;
			v.push_back(a);
			j++;
		}
		matrixb.push_back(v);
		i++;
	}
	//cout << matrixb.size() < endl;
	cout << "Input B taken\n";
	//getchar();

	ifstream inputfile3("pi.txt");						//initialising vector pi
	j = 0; vector<double> pi;
	while (j < states)
	{
		inputfile3 >> a;
		pi.push_back(a);
		j++;
	}
	cout << "Input pi taken\n";
	//getchar();

	ifstream inputfile4("1.txt");					//initialising observations vector
	vector<int> ok;
	while (inputfile4 >> a)
	{
		ok.push_back(a);
	}
	cout << "Input observations taken\n";
	//getchar();


	//output files
	ofstream fout1;				//output files for alpha, probabilities, q, si matrices, state sequences, restimated values etc..
	fout1.open("alpha.txt");
	ofstream fout2;
	fout2.open("ans1.txt");
	ofstream fout3; 
	fout3.open("q.txt");
	ofstream fout4;
	fout4.open("si.txt");
	ofstream fout5;
	fout5.open("ans2.txt");
	ofstream fout6;
	fout6.open("states.txt");
	ofstream fout7;
	fout7.open("newpi.txt");
	ofstream fout8;
	fout8.open("newa.txt");
	ofstream fout9;
	fout9.open("newb.txt");
	ofstream fout10;
	fout10.open("newans1.txt");
	ofstream fout11;
	fout11.open("statesequence.csv");
	ofstream fout12;
	fout12.open("beta.txt");



	while (iteration<21)		//termination condition to be specified in the loop
	{
		fout1 << " Iteration no: " <<iteration<< endl;
		fout2 << " Iteration no: " <<iteration<< endl;
		fout3 << " Iteration no: " << iteration << endl;
		fout4 << " Iteration no: " << iteration << endl;
		fout5 << " Iteration no: " << iteration << endl;
		fout6 << " Iteration no: " << iteration << endl;
		fout7 << " Iteration no: " << iteration << endl;
		fout8 << " Iteration no: " << iteration << endl;
		fout9 << " Iteration no: " << iteration << endl;
		fout10 << " Iteration no: " << iteration << endl;
		fout12 << " Iteration no: " << iteration << endl;

		cout << "\n  =========================================  " << endl;
		cout << "      Iteration no: " << iteration << endl;
		cout << "\n  =========================================  " << endl;
		cout << endl;
		iteration++;

		i = 0, j = 0; int N = states, T = ok.size();
		int p = 0; int t = 0; double sum = 0; double max = 0;

		cout << " starting problem 1 " << endl;
		//using forward procedure for the evaluation problem p(o|l);
		vector<double> temp(T);
		vector<vector<double> > solution1(N,temp);			//initialising that matrix

		i = 0; t = 0;						//calculating the matrix using forward procedure
		while (t < T)
		{
			i = 0;
			while (i < N)
			{
				if (t == 0)
				{
					solution1[i][t] = pi[i] * matrixb[i][ok[t] - 1];
				}
				else
				{
					p = 0; sum = 0;
					while (p < N)
					{
						sum += solution1[p][t - 1] * matrixa[p][i];
						p++;
					}

					solution1[i][t] = sum * matrixb[i][ok[t] - 1];
				}

				i++;
			}

			t++;

		}
		cout << " alpha Matrix Calculated\n";
		//getchar();

		double ans1 = 0;			//final probability for the model is calculated
		p = 0;
		while (p < N)
		{
			ans1 += solution1[p][T - 1];
			p++;
		}
		prev_ans = ans1;
		cout << " final probability calculated" << " " << ans1 << endl;;
		//getchar();

		

		i = 0; j = 0;
		while (i < states)			//output into files
		{
			j = 0;
			while (j < T)
			{
				fout1 << solution1[i][j] << " ";
				j++;
			}
			fout1 << endl;
			i++;
		}
		fout1 << endl;

		fout2 << " Probability of problem1 : " << ans1 << endl;
		cout << " Output the values prob one" << " : " << ans1 << endl;
		//getchar();


		cout << " Starting Problem 2 " << endl;

		//using the viterbi algorithm for the solution of problem2
		vector<vector<double> > solution21(N,temp); 
		vector<vector<double> > solution22(N,temp);

		i = 0; t = 0;
		while (t < T)				//calculating q and si matrices
		{
			i = 0;
			while (i < N)
			{
				if (t == 0)
				{
					solution21[i][t] = pi[i] * matrixb[i][ok[t] - 1];

				}
				else
				{
					p = 0; max = 0;
					while (p < N)
					{
						if (max < solution21[p][t - 1] * matrixa[p][i])
						{
							max = solution21[p][t - 1] * matrixa[p][i];
							solution22[i][t - 1] = p;
						}
						p++;
					}

					solution21[i][t] = max * matrixb[i][ok[t] - 1];
				}

				i++;
			}

			t++;
		}			//calculating the matrices for solution2


		cout << " q and si matrices calculated\n"; 
		//getchar();

		vector<int> optstates; int flag;
		p = 0; max = 0;						//calculating the prob of state sequence
		while (p < N)
		{
			if (max < solution21[p][T - 1])
			{
				max = solution21[p][T - 1];
				flag = p;
			}
			p++;
		}
		double ans2 = max;
		cout << " prob of that state sequence calculated : " << ans2 << endl;
		//getchar();

		optstates.push_back(flag); int k = T - 1; i = 0;	//pushing the last optimal state

		while (k > 0)					//pushing the optimal states over the time from backwards i.e. T, T-1, T-2,... 1.
		{
			optstates.push_back(solution22[(optstates[i])][k - 1]);
			i++;  k--;
		}
		cout << " final states calculated\n";			//calc optimal states
		//getchar();

		i = 0; t = 0;			//output the final matrices and the states to the problem2 in solution2.txt
		fout3 << "q matrix" << endl;
		fout3 << endl;
		while (i < N)				//output into files
		{
			t = 0;
			while (t < T)
			{
				fout3 << solution21[i][t] << " ";
				t++;
			}
			i++;
			fout3 << endl;
		}
		fout3 << endl;


		fout4 << "si matrix of optimal states" << endl;
		fout4 << endl;
		i = 0; t = 0;
		while (i < N)			//output into files
		{
			t = 0;
			while (t < T)
			{
				fout4 << solution22[i][t] << " ";
				t++;
			}
			i++;
			fout4 << endl;
		}
		fout4 << endl;

		fout5 << "solution to prob2 : " << ans2 << endl;

		fout6 << " states :" << endl;
		fout11 << "Iteration no:" << (iteration-1) << endl;
		fout6 << endl;
		i = optstates.size();
		while (i > 0)		//outputing optimal states in reverse order
		{
			fout6 << optstates[i - 1] << " ";
			fout11 << optstates[i - 1] << endl;
			i--;
		}
		fout11 << endl;
		fout6 << endl;
		cout << " output q, si matrices and states into files done\n";
		//getchar();

		fout12 << " matrix beta" << endl;
		
		vector<vector<double> > matrixbeta(N, temp);
		i = 0; j = 0;
		while (i < N)			//matrix beta initialised
		{
			j = 0;
			while (j < T)
			{
				matrixbeta[i][j] = 1;
				j++;
			}
			i++;
		}
		
		j = T - 2;
		i = 0;

		while (j >= 0)			//calculating beta matrix
		{
			i = 0;
			while (i < N)
			{
				p = 0; sum = 0;
				while (p < N)
				{
					sum += matrixa[i][p] * matrixb[p][ok[j + 1] - 1] * matrixbeta[p][j + 1];
					p++;
				}
				matrixbeta[i][j] = sum;
				fout12 << matrixbeta[i][j] << " ";
				i++;

			}
			fout12 << endl;
			j--;
		}
		fout12 << endl;
		cout << " Matrix beta calculated" << endl;
		//getchar();

		//problem-3////////////
		cout << " Starting problem 3 " << endl;


		//Finding gamma
		//vector<double> temp(T);
		vector<vector<double> > gamma(N, temp);	//calculating gamma matrix
		for (int t = 0; t < T; ++t)
		{
			double sum = 0;
			for (int j = 0; j < N; ++j)
			{
				sum += solution1[j][t] * matrixbeta[j][t];
			}

			for (int i = 0; i < N; ++i)
			{
				double val = solution1[i][t] * matrixbeta[i][t];
				if(sum!=0) gamma[i][t] = val / sum;
				else gamma[i][t] = 0;
			}
		}
		cout << " Gamma calculated" << endl;
		//getchar();


		// Finding eta
		//vector<double> temp(T);
		vector<vector<double> > temp2(N, temp);		//calculating eta matrix
		vector< vector< vector<double> > > eta(N, temp2);
		for (int t = 0; t < T - 1; ++t)
		{
			double sum = 0;

			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					sum += (solution1[i][t] * matrixa[i][j] * matrixb[j][ok[t + 1] - 1] * matrixbeta[j][t + 1]);
				}
			}
			for (int i = 0; i < N; ++i)
			{
				for (int j = 0; j < N; ++j)
				{
					if(sum!=0) eta[i][j][t] = (solution1[i][t] * matrixa[i][j] * matrixb[j][ok[t + 1] - 1] * matrixbeta[j][t + 1]) / sum;
					else eta[i][j][t] = 0;
				}

			}
		}
		cout << " Matrix eta calculated" << endl;
		//getchar();

			

												//reestimating pi
		fout7 << " new pi: " << endl;
		vector<double> pinew(N);
		for (int i = 0; i < N; i++)
		{
			pinew[i] = gamma[i][0];
			fout7 << pinew[i] << " ";
		}
		fout7 << endl;
		cout << " reesimating pi done" << endl;
		//getchar();

															//reestimating matrix a
		fout8 << " matrix a : " << endl;
		vector<vector<double> >matrixanew(N, pinew); double sum1 = 0, sum2 = 0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				sum1 = 0; sum2 = 0;
				for (int t = 0; t < T - 1; t++)
				{
					sum1 += eta[i][j][t];
					sum2 += gamma[i][t];
				}
				if(sum2!=0) matrixanew[i][j] = sum1 / sum2;
				else matrixanew[i][j] = 0;
				fout8 << matrixanew[i][j] << " ";
			}
			fout8 << endl;
		}
		fout8 << endl;
		cout << " reestimating matrix a done" << endl;
		//getchar();

															//reestimating matrix b
		fout9 << " matrix b : " << endl;
		vector<double> temp3(obs);
		vector<vector<double> >matrixbnew(N, temp3); sum1 = 0, sum2 = 0;
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < obs; j++)
			{
				sum1 = 0; sum2 = 0;
				for (int t = 0; t < T; t++)
				{
					if (ok[t] - 1 == j)
					{
						sum1 += gamma[i][t];
					}
					sum2 += gamma[i][t];
				}
				if (sum2 != 0) matrixbnew[i][j] = sum1 / sum2;
				else matrixbnew[i][j] = 0;
				fout9 << matrixbnew[i][j] << " ";
			}
			fout9 << endl;
		}
		fout9 << endl;
		cout << " reestimating matrix b done" << endl;
		cout << " output the new values of a,b, pi done" << endl;
		//getchar();


		i = 0; t = 0;						//calculating the matrix using forward procedure
		while (t < T)
		{
			i = 0;
			while (i < N)
			{
				if (t == 0)
				{
					solution1[i][t] = pinew[i] * matrixbnew[i][ok[t] - 1];
				}
				else
				{
					p = 0; sum = 0;
					while (p < N)
					{
						sum += solution1[p][t - 1] * matrixanew[p][i];
						p++;
					}

					solution1[i][t] = sum * matrixbnew[i][ok[t] - 1];
				}

				i++;
			}

			t++;

		}
		cout << " alpha matrix again calculated\n";
		//getchar();

		ans1 = 0;			//final probability for the model is calculated
		p = 0;
		while (p < N)
		{
			ans1 += solution1[p][T - 1];
			p++;
		}
		cout << " answer 1 calculated again : " << ans1 << endl;
		fout10 << " reestimated probability prob1 : " << ans1 << endl;
		
		cur_ans = ans1;

		pi = pinew;
		matrixa = matrixanew;
		matrixb = matrixbnew;


	}
												//closing all files
	inputfile1.close();
	inputfile2.close();
	inputfile3.close();
	inputfile4.close();
	fout1.close();
	fout2.close();
	fout3.close();
	fout4.close();
	fout5.close();
	fout6.close();
	fout7.close();
	fout8.close();
	fout9.close();
	fout10.close();
	fout11.close();
	fout12.close();

	getchar();
	return 0;

}

