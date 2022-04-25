/*
 * utilities.hpp
 *
 *  Created on: Feb 23, 2019
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 */

#ifndef UTILITIES_HPP_
#define UTILITIES_HPP_

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <string>
#include <iomanip>
#include <numeric>

using namespace std;

enum dataType {
	deterministic,
	stochastic,
};

class CSVcontent {

public:
	CSVcontent() = default;
	CSVcontent(const string fileName, char delimiter, bool hasHeader);

	vector<string> getColumn(int colIdx);
	vector<string> getColumn(string colName);
	vector<double> getColumn(int colIdx, bool isNumeric);
	vector<double> getColumn(string colName, bool isNumeric);

	vector <vector <string> > content;
	vector <string> header;
	vector <string> rowname;

};

/* File operations */
bool openFile(ifstream &fptr, string filename);
bool openFile(ofstream &fptr, string filename);

/* Print options */
void printVector(vector<int> &x);
void printVector(vector<double> &x);
void printVector(vector<bool> &x);
void printVector(vector <int> &x, ofstream &output);
void printVector(vector <double> &x, ofstream &output);
void printVector(vector <bool> &x, ofstream &output);

void printMatrix(vector< vector<double> > &x);
void printMatrix(vector< vector<bool> > &x);
void printMatrix(vector< vector<int> > &x);
void printMatrix(vector < vector<double> > &x, ofstream &output);
void printMatrix(vector < vector<bool> > &x, ofstream &output);
void printMatrix(vector < vector<bool> > &x, ofstream &output);

void printLine();
void printDoubleLine();

vector<double> vecString2Double(vector<string> stringVec);
vector<int> vecString2Int(vector<string> stringVec);

/* Resize and create new three and four dimensional matrices */
void resizeMatrix(vector< vector<bool> > &mat, int rows, int cols);
void resizeMatrix(vector< vector<int> > &mat, int rows, int cols);
void resizeMatrix(vector< vector<double> > &mat, int rows, int cols);

vector< vector<int> > createIntMatrix(int rows, int cols);
vector< vector<double> > createDoubleMatrix(int rows, int cols);
vector< vector< vector<int> > > createIntQuad(int size1, int size2, int size3);
vector< vector< vector<bool> > > createDoubleQuad(int size1, int size2, int size3, bool default_val);
vector< vector< vector<double> > > createBooleanQuad(int size1, int size2, int size3, bool default_val);

/* Time series utilities */
vector<vector<double>> readTS(string fileName);

template<typename T>
double getAverage(std::vector<T> const& v);
template<typename T>
double getMinimum(std::vector<T> const& v);
template<typename T>
double getMaximum(std::vector<T> const& v);

#endif /* UTILITIES_HPP_ */