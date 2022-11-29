#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	string input;
	bool validInput = false;
	bool validFile = false;
	string selectedFile;
	
	while(!validInput || !validFile)
	{
		cout << "Welcome to the Feature Selection Algorithm." << endl;
		cout << "Type in the name of the file to test: ";
		
		cin >> selectedFile;
		
		ifstream fileInput(selectedFile);
		if(!fileInput.is_open())
		{
			cout << "File: " << selectedFile << " could not be opened." << endl;
		}
		else
		{
			validFile = true;
			
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
