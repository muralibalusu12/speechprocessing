// Problem: Calculating the codebook vectors for the input speech feature fectors using a distance metric
// Created by Murali Raghu Babu B
// kmeans.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <ctime>
#include <functional>
#include <algorithm>

using namespace std;

//to define the codebook size;
int codebook_size;

//function to calculate the distance between the two speech feature vectors- here the distance used is tokhura's distance.
double tokhura(vector<double> f1, vector<double> f2, vector<int> w)
{
	double distance = 0;
	for (int i = 0; i < f1.size(); ++i)
	{
		distance += w[i] * (f1[i] - f2[i])*(f1[i] - f2[i]);
	}
	return distance;
}

//function to calculate euclidean distance between the two speech feature vectors
double euclidean(vector<double> f1, vector<double> f2)
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

	ifstream inputfile("Universe.txt"); //input file for the feature vectors
	ofstream outfile("Logfile.txt");    //outputs the cluster details in every iteration.
	ofstream outfile2("Distortion.txt");//outputs the distortion in every iteration
	ofstream outfile3("Codebook.txt");  //final outputs of the codebook vectors of the particular size.
	vector<vector<double> > funiverse;
	string line;
	int k = 1;
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
	ifstream weightfile("Configuration.txt");	//input of the codebook size to be taken as input from the file.
	weightfile>>codebook_size;
	while (weightfile>>a)
	{
		w.push_back(a);
	}

	//Randomly selecting initial vectors a codebook of size 32.
	vector<vector<double> > codebook;
	srand(time(NULL));
	for (int i = 0; i<codebook_size; i++)
	{
		int num = rand() % funiverse.size();
		cout<<num<<endl;
		codebook.push_back(funiverse[num]);
	}

	//classify the set of training vectors  into clusters.
	vector<int> clusters(funiverse.size());
	vector<int> count(codebook_size, 0);
	double diff = 1, prev_distortion;
	for (int c = 0; diff>0.001; ++c)
	{
		outfile<<"Iteration : "<<c+1<<endl;
		for (int i = 0; i < codebook_size; ++i)
			count[i] = 0;
		cout << "classifying vectors into clusters\n";
		for (int i = 0; i<funiverse.size(); ++i)
		{
			//calculate which cluster each vector belongs to.
			double dist = INT_MAX;
			int k = 0;
			for (int j = 0; j<codebook_size; ++j)
			{
				double temp = tokhura(funiverse[i], codebook[j], w);	//Calculation of Tokhura Distance 
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
			outfile<<"cluster "<<i+1<<" : "<<count[i]<<endl;
		//Updating the means in the codebook.
		cout << "Updating code vector\n";
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
		cout<<"distortion : "<<distortion<<endl;
		outfile<<"distortion : "<<distortion<<endl;
		outfile2<<"Iteration "<<c+1<<" : "<<distortion<<endl;
	}
	outfile3 << "Codebook " << endl;
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
	getchar();
	return 0;


}
