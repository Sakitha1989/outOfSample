/*
 * utilities.cpp
 *
 *  Created on: Feb 23, 2019
 *      Author: Harsha Gangammanavar
 * Institution: Southern Methodist University
 *
 * Please send your comments or bug report to harsha (at) smu (dot) edu
 *
 */

#include "utilities.hpp"

bool openFile(ifstream &fptr, string filename) {

	fptr.open(filename.c_str());
	if (fptr.fail()) {
		cout << "Error opening file: " << filename << endl;
		return false;
	}
	return true;
}

bool openFile(ofstream &fptr, string filename) {

	fptr.open(filename.c_str());
	if (fptr.fail()) {
		cout << "Error opening file: " << filename << endl;
		return false;
	}
	return true;
}

void printVector(vector<int> &x) {
	for (unsigned int i = 0; i < x.size(); i++)
		cout << fixed << setprecision(0) << x[i] << " ";
	cout << endl;
}

void printVector(vector<double> &x) {
	if (x.size() == 0) {
		cout << "Empty\n";
		return;
	}

	for (unsigned int i = 0; i < x.size(); i++)
		cout << fixed << setprecision(2) << x[i] << " ";
	cout << endl;
}

void printVector(vector<bool> &x) {
	if (x.size() == 0) {
		cout << "Empty\n";
		return;
	}

	for (unsigned int i = 0; i < x.size(); i++)
		cout << x[i] << " ";
	cout << endl;
}

void printVector(vector <int> &x, ofstream &output) {
	for (unsigned int i = 0; i < x.size(); i++)
		output << x[i] << " ";
	output << endl;
}

void printVector(vector <bool> &x, ofstream &output) {
	for (unsigned int i = 0; i < x.size(); i++)
		output << x[i] << " ";
	output << endl;
}

void printVector(vector <double> &x, ofstream &output) {
	for (unsigned int i = 0; i < x.size(); i++)
		output << x[i] << " ";
	output << endl;
}

void printMatrix(vector< vector<double> > &x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		for (unsigned int j = 0; j < x[0].size(); j++)
			cout << fixed << setprecision(3) << setw(6) << x[i][j] << " ";
		cout << endl;
	}
	cout << endl;
}

void printMatrix(vector< vector<bool> > &x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		for (unsigned int j = 0; j < x[0].size(); j++)
			cout << fixed << setw(1) << x[i][j] << ", ";
		cout << endl;
	}
	cout << endl;
}

void printMatrix(vector< vector<int> > &x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		for (unsigned int j = 0; j < x[0].size(); j++)
			cout << fixed << setprecision(0) << setw(3) << x[i][j] << ", ";
		cout << endl;
	}
	cout << endl;
}

void printMatrix(vector < vector<double> > &x, ofstream &output) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		for (unsigned int j = 0; j < x[0].size(); j++)
			output << fixed << setprecision(20) << setw(23) << x[i][j] << "\t";
		output << endl;
	}
}

void printMatrix(vector < vector<bool> > &x, ofstream &output) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		for (unsigned int j = 0; j < x[0].size(); j++)
			output << fixed << setprecision(0) << setw(3) << x[i][j] << ",";
		output << endl;
	}
}

void printLine() {
	cout << "------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void printDoubleLine() {
	cout << "======================================================================================================================================================" << endl;
}

vector<double> vecString2Double(vector<string> stringVec) {
	vector<double> doubleVec;

	for (int i = 0; i < (int)stringVec.size(); i++) {
		double val = stof(stringVec.at(i));
		doubleVec.push_back(val);
	}
	return doubleVec;
}

vector<int> vecString2Int(vector<string> stringVec) {
	vector<int> intVec;

	for (int i = 0; i < (int)stringVec.size(); i++) {
		int val = stoi(stringVec.at(i));
		intVec.push_back(val);
	}
	return intVec;
}

void resizeMatrix(vector< vector<bool> > &mat, int rows, int cols) {
	mat.resize(rows);
	for (unsigned int j = 0; j < mat.size(); j++)
		mat[j].resize(cols);
}

void resizeMatrix(vector< vector<int> > &mat, int rows, int cols) {
	mat.resize(rows);
	for (unsigned int j = 0; j < mat.size(); j++)
		mat[j].resize(cols);
}

void resizeMatrix(vector< vector<double> > &mat, int rows, int cols) {
	mat.resize(rows);
	for (unsigned int j = 0; j < mat.size(); j++)
		mat[j].resize(cols);
}

vector< vector<int> > createIntMatrix(int rows, int cols) {
	vector< vector<int> > new_matrix(rows, vector<int>(cols));
	return new_matrix;
}

vector< vector<double> > createDoubleMatrix(int rows, int cols) {
	vector< vector<double> > new_matrix(rows, vector<double>(cols));
	return new_matrix;
}

vector< vector< vector<int> > > createIntQuad(int size1, int size2, int size3) {
	vector< vector< vector<int> > > new_matrix(size1, vector< vector<int> >(size2, vector<int>(size3)));
	return new_matrix;
}


vector< vector< vector<bool> > > createBooleanQuad(int size1, int size2, int size3) {
	vector< vector< vector<bool> > > new_matrix(size1, vector< vector<bool> >(size2, vector<bool>(size3)));
	return new_matrix;
}

vector< vector< vector<double> > > createDoubleQuad(int size1, int size2, int size3, double default_val) {
	vector< vector< vector<double> > > new_matrix(size1, vector< vector<double> >(size2, vector<double>(size3, default_val)));
	return new_matrix;
}

CSVcontent::CSVcontent(const string fileName, char delimiter, bool hasHeader) {
	ifstream fptr;
	bool isHeader = true;

	/* If the file does not have header, then skip reading header */
	if (!hasHeader) isHeader = false;

	/* Return empty content and header if the file is missing. */
	fptr.open(fileName.c_str());
	if (!fptr.is_open()) {
		printf("Warning: File %s is missing, returning NULL content.\n", fileName.c_str());
		return;
	}

	while (fptr) {
		string s;
		if (!getline(fptr, s)) break;

		istringstream ss(s);
		vector <string> record;
		while (ss) {
			string s;
			if (!getline(ss, s, delimiter)) break;
			record.push_back(s);
		}

		if (isHeader)
			header = record;
		else
			content.push_back(record);
		isHeader = false; 				// Switch off isHeader after reading the first line
	}
	if (!fptr.eof()) {
		cerr << "Could not locate the file: " << fileName << endl;
	}
}

vector<string> CSVcontent::getColumn(int colIdx) {
	vector<string> colData;

	for (unsigned i = 0; i < content.size(); ++i) {
		colData.push_back(content[i][colIdx]);
	}

	return colData;
}

vector<string> CSVcontent::getColumn(string colName) {
	vector<string> colData;

	/* Find position of column */
	int pos = find(header.begin(), header.end(), colName) - header.begin();
	if (pos == (int)header.size()) {
		cout << "Warning: Column name " << colName << " could not be found." << endl;
		return vector<string>();
	}

	for (unsigned i = 0; i < content.size(); ++i) {
		colData.push_back(content[i][pos]);
	}

	return colData;
}

vector<double> CSVcontent::getColumn(int colIdx, bool isNumeric) {
	vector<double> colData;

	for (unsigned i = 0; i < content.size(); ++i) {
		double num = stof(content[i][colIdx]);
		colData.push_back(num);
	}

	return colData;
}

vector<double> CSVcontent::getColumn(string colName, bool isNumeric) {
	vector<double> colData;

	/* Find position of column */
	int pos = find(header.begin(), header.end(), colName) - header.begin();
	if (pos == (int)header.size()) {
		cout << "Warning: Column name " << colName << " could not be found." << endl;
		return vector<double>();
	}

	for (unsigned i = 0; i < content.size(); ++i) {
		double num = stof(content[i][pos]);
		colData.push_back(num);
	}

	return colData;
}
