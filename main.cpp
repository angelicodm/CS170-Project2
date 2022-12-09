#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace std::chrono;

vector <vector <long double>> dataSet; //separates the data set into individual rows
vector<long double> featureSet; //the columns in the data set

/* NOTE:
 * Assigned datasets: small53 and large105
 * From the key provided in the email:
 * 		small53 features: 3 6 4		small53 accuracy: 0.942
 * 		large105 features: 28 18 9	large105 accuracy: 0.942
 */

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
		
		for(int i = 0; i < dataSet.size(); i++) //goes through each column feature and adds it to its respective feature
		{
			if(fileStream >> feature)
			{
				dataSet[i].push_back(stold(feature));
			}
		}
	}
}

double nearestNeighbor(vector <vector <long double>> data, vector<long double> instance)
{
	long double nearestNeighborDistance = INFINITY;
	long double distance, objectToClassify;
	long double classLabel = instance[0];
	
	for(int i = 0; i < data[0].size(); i++)
	{
		distance = 0.0; //distance has to be assigned to 0 here or it won't work
		for(int j = 0; j < data.size(); j++)
		{
			if(j > 0) //wont work if j ever equals 0
			{
				//distance = sqrt((sum(object_to_classify -data(k,2:end)).^2));
				objectToClassify = data[j][i];
				distance += pow((objectToClassify - instance[j]), 2);
			}
		}
		
		if(distance < nearestNeighborDistance) //if closer neighbor was found, updates nearestNeighborDistance and classLabel
		{
			nearestNeighborDistance = distance;
			classLabel = (int)data[0][i];
		}
	}
	return classLabel;
}

double accuracy(vector <vector <long double>> dataSet) //nearest neighbor should be separate to keep it simple
{
	double accuracy;
	double correctlyClassified = 0;
	vector <vector <long double>> data;
	vector <long double> instance;
	vector <long double> copy; //used to copy entries from accData to instance
	
	for(int i = 0; i < dataSet[0].size(); i++)
	{
		for(int j = 0; j < dataSet.size(); j++)
		{
			copy = dataSet[j]; //copies entire column of features
			instance.push_back(copy[i]); // copies exact feature from instance we want
			copy.erase(copy.begin() + i); //removes the individual feature from the instance we want (leaves one out)
			data.push_back(copy); //copies rest of feature we want from all instances into data for comparison
		}
		
		if(nearestNeighbor(data, instance) == instance[0])
		{
			correctlyClassified++;
		}
		data.clear();
		instance.clear();
	}
	
	accuracy = (correctlyClassified / data[0].size()) * 100;
	
	return accuracy;
}

void forwardSelection()//vector< vector< long double> dataSet)
{
	vector<int> bestFeatures; //only used to store final set of best features for output
    vector<int> currentBest;  //updates every iteration with the best feature(s)
    vector<int> copyBest; //purely used to copy stored data to update currentBest after the nested for loop
    vector<vector<long double>> dataCopy; //a modified copy of dataSet to be used to find the accuracy of the current selected features
    dataCopy.push_back(dataSet.at(0)); //add class labels from dataSet to dataCopy
    vector<int> currentFeatures; //updates every iteration with current best features and next feature to be tested
    double acc, localAcc, bestAcc; //used to keep track of varying accuracies
    
    for(int i = 1; i < dataSet.size(); i++)
    {
		localAcc = 0.0; //resets localAcc for next set of features
		for(int j = 1; j < dataSet.size(); j++)
		{
			currentFeatures = currentBest; //features to be tested include the best features found already
			
			for(int k = 0; k < currentFeatures.size(); k++)
			{
				dataCopy.push_back(dataSet.at(currentFeatures.at(k))); //adds the next column feature to dataCopy from already selected features
			}
			
			if(find(currentFeatures.begin(), currentFeatures.end(), j) == currentFeatures.end()) //if feature "j" was not found then it can be added to currentFeatures (avoids duplicates)
			{
				dataCopy.push_back(dataSet.at(j)); //adds next column of features   
				currentFeatures.push_back(j); //adds next column of features
				cout << "Using feature(s) { "; 
            
				for(int l = 0; l < currentFeatures.size(); l++) 
				{
					cout << currentFeatures.at(l) << " "; 
				}
				
				cout << "}"; 
				
				acc = accuracy(dataCopy); 
				cout << " accuracy is  " << setprecision(4) << acc  << "%" << endl;
			
				if(acc > localAcc) 
				{	 
					localAcc = acc; 
					copyBest = currentFeatures; 
				}
			}
			
			dataCopy.erase(dataCopy.begin() + 1, dataCopy.end()); //clears dataCopy except for the class labels
            currentFeatures.clear();  
		}
		
		currentBest = copyBest; 
        cout << "Feature set { "; 
        for(int i = 0; i < currentBest.size(); i++) 
        { 
			cout << currentBest.at(i) << " "; 
		} 
        cout << "} was best, accuracy is " << setprecision(4) << localAcc << "%" << endl << endl; 
        
        if(localAcc > bestAcc || currentBest.size() < 3) //Not sure if second half contributes anything but keeping it regardless
        { 
			bestFeatures = currentBest; 
			bestAcc = localAcc;
		}
        else if(localAcc < bestAcc) 
        { 
			cout << "(Warning, Accuracy has decreased! Continuing check in case of local maxima)" << endl; 
		} 
		
        //NOTE: THIS IF STATEMENT WAS INCLUDED TO FORCEFULLY STOP THE LOOPS AS IT IS ALREADY KNOWN THAT THE DATASETS HAVE A MAX SET OF 3 BEST FEATURES
        if(currentBest.size() == 3) //removing the contents of this if statement will allow the algorithm to do a complete search of each feature
        {
			bestFeatures = currentBest;
			bestAcc = localAcc;
			break;
		}
		
		copyBest.clear();
	}
	
	cout << endl << "Finished search!!" << endl; 
    cout << "The best feature subset of 3 is { "; 
    for(int i = 0; i < bestFeatures.size(); i++) 
    { 
		cout << bestFeatures.at(i) << " "; 
	} 
    cout << "} which has an accuracy of " << setprecision(4) << bestAcc << "%" << endl; 
}

void backwardElimination()
{
	vector<int> bestFeatures(dataSet.size() - 1); //starts with all features
    vector<int> currentBest;  //updates every iteration with the best feature(s)
    vector<int> copyBest; //purely used to copy stored data to update currentBest after the nested for loop
    vector<vector<long double>> dataCopy; //a modified copy of dataSet to be used to find the accuracy of the current selected features
    dataCopy.push_back(dataSet.at(0)); //add class labels from dataSet to dataCopy
    vector<int> currentFeatures; //updates every iteration with current best features and next feature to be tested
    double acc, localAcc, bestAcc; //used to keep track of varying accuracies
    
    for(int i = 0; i < bestFeatures.size(); i++) //populates bestFeatures with all features
    {
		bestFeatures[i] = i + 1;
	}
	
	currentBest = bestFeatures;
    
    for(int i = 1; i < dataSet.size(); i++)
    {
		localAcc = 0.0; //resets localAcc for next set of features
		for(int j = 1; j < dataSet.size(); j++)
		{
			currentFeatures = currentBest; //features to be tested include the best features found already
			
			for(int k = 0; k < currentFeatures.size(); k++)
			{
				dataCopy.push_back(dataSet.at(currentFeatures.at(k))); //adds the next column feature to dataCopy from already selected features
			}
			
			if(find(currentFeatures.begin(), currentFeatures.end(), j) == currentFeatures.end()) //if feature "j" was not found then it can be added to currentFeatures (avoids duplicates)
			{
				dataCopy.erase(dataSet.begin() + j); //removes worse column of features   
				currentFeatures.erase(currentFeatures.begin() + j); //adds next column of features
				cout << "Using feature(s) { "; 
            
				for(int l = 0; l < currentFeatures.size(); l++) 
				{
					cout << currentFeatures.at(l) << " "; 
				}
				
				cout << "}"; 
				
				acc = accuracy(dataCopy); 
				cout << " accuracy is  " << setprecision(4) << acc  << "%" << endl;
			
				if(acc > localAcc) 
				{	 
					localAcc = acc; 
					copyBest = currentFeatures; 
				}
			}
			
			dataCopy.erase(dataCopy.begin() + 1, dataCopy.end()); //clears dataCopy except for the class labels
            currentFeatures.clear();  
		}
		
		currentBest = copyBest; 
        cout << "Feature set { "; 
        for(int i = 0; i < currentBest.size(); i++) 
        { 
			cout << currentBest.at(i) << " "; 
		} 
        cout << "} was best, accuracy is " << setprecision(4) << localAcc << "%" << endl << endl; 
        
        if(localAcc > bestAcc || currentBest.size() < 3) //Not sure if second half contributes anything but keeping it regardless
        { 
			bestFeatures = currentBest; 
			bestAcc = localAcc;
		}
        else if(localAcc < bestAcc) 
        { 
			cout << "(Warning, Accuracy has decreased! Continuing check in case of local maxima)" << endl; 
		} 
		
        //NOTE: THIS IF STATEMENT WAS INCLUDED TO FORCEFULLY STOP THE LOOPS AS IT IS ALREADY KNOWN THAT THE DATASETS HAVE A MAX SET OF 3 BEST FEATURES
        if(currentBest.size() == 3) //removing the contents of this if statement will allow the algorithm to do a complete search of each feature
        {
			bestFeatures = currentBest;
			bestAcc = localAcc;
			break;
		}
		
		copyBest.clear();
	}
	
	cout << endl << "Finished search!!" << endl; 
    cout << "The best feature subset of 3 is { "; 
    for(int i = 0; i < bestFeatures.size(); i++) 
    { 
		cout << bestFeatures.at(i) << " "; 
	} 
    cout << "} which has an accuracy of " << setprecision(4) << bestAcc << "%" << endl;
}


int main()
{
	string input;
	bool validInput = false;
	bool validFile = false;
	string selectedFile, currLine, feature;
	
	//datasets are in a 2D format which means 2D vectors
	//vector <vector <long double>> dataSet; //separates the data set into individual rows
	//vector<long double> featureSet; //the columns in the data set
	
	
	
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
			
			cout << dataSet.size() << endl << dataSet[0].size() << endl;
			
			cout << "Enter the corresponding number for the algorithm you want to run." << endl;
			cout << "1) Forward Selection \n" << "2) Backward Elimination" << endl;
			
			cin >> input;
			
			if(input == "1")
			{
				// calls Forward Selection function
				forwardSelection();
				validInput = true;
			}
			else if(input == "2")
			{
				// calls Backward Elimination function
				backwardElimination();
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
