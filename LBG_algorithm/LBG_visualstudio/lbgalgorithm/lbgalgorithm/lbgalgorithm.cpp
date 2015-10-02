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

int maxcodebook_size;	//max_code_book_size

double tokhura(vector<double> f1, vector<double> f2, vector<int> w)	//calculates tokhura's distance using the weights given.
{
	double distance = 0;
	for (int i = 0; i < f1.size(); ++i)
	{
		distance += w[i] * (f1[i] - f2[i])*(f1[i] - f2[i]);
	}
	return distance;
}

double euclidean(vector<double> f1, vector<double> f2)		// calculates the euclidean distance	
{
	double dist = 0;
	for (int i = 0; i < f1.size(); ++i)
	{
		dist += (f1[i] - f2[i])*(f1[i] - f2[i]);
	}
	return dist;
}

int main(int argc, char* argv)
{

	ifstream inputfile("Universe.txt");
	vector<vector<double> > funiverse;
	string line;
	int k = 1;					//takes input the universe of feature vectors
	while (getline(inputfile, line))
	{
		stringstream  ss(line);
		double a;
		vector<double> v;
		while (ss >> a)
		{
			v.push_back(a);
			if (ss.peek() == ',' || ss.peek() == '\t' || ss.peek() == ' ')
				ss.ignore();
		}
		funiverse.push_back(v);
	}
	cout << funiverse.size() << endl;

	vector<int> w;
	int a;
	ifstream weightfile("Configuration.txt");		//taking the inputs from file - max codebook size, tokhura's weights
	weightfile >> maxcodebook_size;
	while (weightfile >> a)
	{
		w.push_back(a);
	}


	vector<vector<double> > codebook;				//codebook to hold all the codebook vectors
	srand(time(NULL));
	int codebook_size = 1;
	vector<double> mean(12, 0);
	vector<double> er(12, 0);

	srand(time(NULL));								// to generate a random vector to be used later - epsilon
	int num = rand() % 5;
	//double num2 = num / 100 + 0.01;				//uncomment this to generate it randomly
	double num2 = 0.03;								//hardcoded it to be 0.03
	cout << " epsilon : " << num2 << endl;
	for (int j = 0; j<12; ++j)
	{
		er[j] = num2;
	}

	int t = 1;

	while (codebook_size <= maxcodebook_size)
	{
		cout << "codebook_size " << codebook_size << " maxcodebook_size "<< maxcodebook_size<<endl;
		if (codebook_size == 1)
		{							// if the codebook size=1, then mean is used as the codebook vector.
			for (int i = 0; i<funiverse.size(); ++i)
			{
				for (int j = 0; j<12; ++j)
				{
					mean[j] += funiverse[i][j];
				}
			}

			for (int j = 0; j<12; ++j)			//calculating the mean vector as the first codebook vector
			{
				mean[j] = mean[j] / funiverse.size();
			}
			ofstream outfile("Logfile1.txt");
			ofstream outfile2("Distortion1.txt");
			ofstream outfile3("Codebook1.txt");
			double distortion = 0;
			for (int i = 0; i<funiverse.size(); ++i)		//calculating distortion
			{
				distortion += tokhura(funiverse[i], mean, w);
				//distortion += euclidean(universe[i], codebook[clusters[i]]);
			}

			distortion /= funiverse.size();

			outfile << "Iteration : 1 " << endl;

			outfile << "cluster 1 : " << funiverse.size() << endl;
			outfile << "Distortion : " << distortion<< endl;
			outfile2 << "Final Distortion : " << distortion << endl;
			cout <<"distortion : "<< distortion << endl;
			outfile3 << "Final Codebook " << endl;
			
			for (int i = 0; i<codebook_size; ++i)
			{
				for (int j = 0; j < 12; ++j)
				{
					outfile3 << mean[j] << " ";
				}
				outfile3 << endl;
			}
			codebook.push_back(mean);			//pushing mean as the initial codebook vector
			
			outfile.close();
			outfile2.close();
			outfile3.close();
		}
		else
		{
			t++;							//generating respective files at each stage of the LBG algorithm
			char achar = '0' + t;
			char n[2]; 
			n[0] = achar;
			n[1] = '\0';
			string filename = "Logfile" + string(n) + ".txt";
			ofstream outfile(filename);
			filename = "Distortion" + string(n) + ".txt";
			ofstream outfile2(filename);
			filename = "Codebook" + string(n) + ".txt";
			ofstream outfile3(filename);

			for (int i = 0; i < codebook_size / 2; i++)	//changing the codebook vectors as in the LBG algorithm
			{
				vector<double> temp(12, 0);
				for (int j = 0; j<12; ++j)
				{
					temp[j] = codebook[i][j] + er[j];
					codebook[i][j] = codebook[i][j] - er[j];
				}
				codebook.push_back(temp);
			}

			vector<int> clusters(funiverse.size());
			vector<int> count(codebook_size, 0);
			double diff = 1, prev_distortion;
			for (int c = 0; diff>0.001; ++c)		//applying kmeans until there is almost no change in the distortion
			{
				outfile << "Iteration : " << c + 1 << endl;
				for (int i = 0; i < codebook_size; ++i)
					count[i] = 0;
				//cout << "classifying vectors into clusters\n";
				for (int i = 0; i<funiverse.size(); ++i)
				{
					//calculate which cluster each vector belongs to.
					double dist = INT_MAX;
					int k = 0;
					for (int j = 0; j<codebook_size; ++j)
					{
						double temp = tokhura(funiverse[i], codebook[j], w);
						if (temp<dist)
						{
							dist = temp;
							k = j;
						}
					}
					clusters[i] = k;
					count[k]++;
				}

				for (int i = 0; i<codebook_size; ++i)
					outfile << "cluster " << i + 1 << " : " << count[i] << endl;
				//Updating the means in the codebook.
				//cout << "Updating code vector\n";
				vector<double> temp(12, 0);
				for (int i = 0; i<codebook_size; ++i)
				{
					if (count[i] != 0)
						codebook[i] = temp;
				}

				for (int i = 0; i<clusters.size(); ++i)
				{
					for (int j = 0; j<12; ++j)
					{
						codebook[clusters[i]][j] += funiverse[i][j];
					}
				}
				for (int i = 0; i<codebook_size; ++i)
				{
					if (count[i] != 0)
					for (int j = 0; j < 12; ++j)
					{
						codebook[i][j] /= count[i];
					}
				}

				//Calculating  distortion using Tokhura's distance.
				double distortion = 0;
				for (int i = 0; i<funiverse.size(); ++i)
				{
					distortion += tokhura(funiverse[i], codebook[clusters[i]], w);
					//distortion += euclidean(universe[i], codebook[clusters[i]]);
				}

				distortion /= funiverse.size();
				if (c != 0)
					diff = prev_distortion - distortion;
				prev_distortion = distortion;
				cout << "distortion : " << distortion << endl;
				outfile << "distortion : " << distortion << endl;
				outfile2 << "Iteration " << c + 1 << " : " << distortion << endl;
			}

			outfile2 << "Final Distortion : " << prev_distortion << endl;

			outfile3 << "Final Codebook " << endl;
			for (int i = 0; i<codebook_size; ++i)
			{
				for (int j = 0; j < 12; ++j)
				{
					outfile3 << codebook[i][j] << " ";
				}
				outfile3 << endl;
			}
			outfile.close();
			outfile2.close();
			outfile3.close();

		}
		codebook_size = 2 * codebook_size;		//doubling the codebook vector at every stage in the LBG algorithm

	}

	getchar();
	return 0;

}

