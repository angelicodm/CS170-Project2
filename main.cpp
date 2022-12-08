#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector <vector <long double>> dataSet; //separates the data set into individual rows
vector<long double> featureSet; //the columns in the data set

void loadData(string selectedFile)
{
	ifstream fileInput(selectedFile);
	string currLine, feature;
	getline(fileInput, currLine);
	stringstream fileStream(currLine);
			
	while(fileStream >> feature) //loop creates vectors for every column as opposed to a vector for every row
	{
		featureSet.push_back(stold(feature)); //converts input into long double and adds it to the first vector
		dataSet.push_back(featureSet);
		featureSet.clear();
	}
	
	while(getline(fileInput, currLine)) //goes through each line's input
	{
		stringstream fileStream(currLine);
		
		for(int i = 0; i < featureSet.size(); i++) //goes through each column feature and adds it to its respective feature
		{
			if(fileStream >> feature)
			{
				dataSet[i].push_back(stold(feature));
			}
		}
	}
}

int nearestNeighbor()
{
	
}

double accuracy()
{
	
}

void forwardSelection()
{
	
}

void backwardElimination()
{
	
}


int main()
{
	string input;
	bool validInput = false;
	bool validFile = false;
	string selectedFile, currLine, feature;
	
	//datasets are in a 2D format which means 2D vectors
	vector <vector <long double>> dataSet; //separates the data set into individual rows
	vector<long double> featureSet; //the columns in the data set
	
	
	
	while(!validInput || !validFile)
	{
		cout << "Welcome to the Feature Selection Algorithm." << endl;
		cout << "Type in the name of the file to test: ";
		
		cin >> selectedFile;
		
		ifstream fileInput(selectedFile);
		if(!fileInput.is_open())
		{
			cout << "Could not open selected file." << endl;
		}
		else
		{
			validFile = true;
			
			loadData(selectedFile);
			
			cout << "Enter the corresponding number for the algorithm you want to run." << endl;
			cout << "1) Forward Selection \n" << "2) Backward Elimination" << endl;
			
			cin >> input;
			
			if(input == "1")
			{
				// calls Forward Selection function
				validInput = true;
			}
			else if(input == "2")
			{
				// calls Backward Elimination function
				validInput = true;
			}
			else
			{
				cout << "Invalid input" << endl;
			}
		}
	}
	
	return 0;
}
