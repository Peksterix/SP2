#include "SaveManager.h"
#include <fstream>
#include <sstream>
using std::ofstream;
using std::ifstream;

SaveManager::SaveManager()
{
	//constructor
}

SaveManager::~SaveManager()
{
	//destructor
}

// OLD CODE
//void SaveManager::SaveVehicle(int num, int cha, int whe, int wea, std::string name)
//{
//
//	ifstream outfile;
//	outfile.open("test.txt");
//	std::string line = ""; //didn't initialize it
//	std::string line2 = "";
//	//take everything from file
//	int iRow = 0;
//	
//
//	while (getline(outfile, line2)) {
//		if (iRow == num)
//		{
//			line.append(std::to_string(cha) + " " + std::to_string(whe) + " " + std::to_string(wea) + name);
//		}
//		else
//		{
//			line.append(line);
//		}
//
//		line.append(" ");
//
//		iRow++;
//	}
//
//	std::cout << line << std::endl;
//	outfile.close();
//
//	ofstream infile;
//	infile.open("text.txt");
//	infile << line;

void SaveManager::SaveVehicle(int num, int cha, int whe, int wea, std::string name)
{

	ifstream outfile;
	outfile.open("text.txt");
	std::string Myline = ""; //didn't initialize it
	std::string line2 = "";  
	//take everything from file
	int iRow = 0;

	// make sure less then or == to 8 slots
	if (num < 8) {
		//read the text if got save
		while (getline(outfile, line2)) 
		{ // read  from Outfile (initialize it to line2)

			if (num == iRow)
			{
				// replaces current line with save
				Myline.append(std::to_string(cha) + " " + std::to_string(whe) + " " + std::to_string(wea) + name);
			}
			else
			{
				Myline.append(line2); // if not inputted line add current line to Myline
			}

			Myline.append("\n"); // go to next line

			iRow++; // row count
		}

		std::cout << Myline << std::endl; // debug
		outfile.close(); //close file


		ofstream infile;
		infile.open("text.txt");
		infile << Myline;
		infile.close();


		if (num >= iRow || iRow == 0) // if txt is empty or slot is more than number of rows
		{
			infile.open("text.txt");
			// create a new line and input(comment for the for loop)
			for (int i = iRow; i <= num; i++)  // start from last line find next line
			{
				if (i == num) // if current line == inputted line then write save
				{
					Myline.append(std::to_string(cha) + " " + std::to_string(whe) + " " + std::to_string(wea) + name);
				}
				else
				{
						Myline.append("\n"); // add a new line if line is not == inputted line
				}
			}
			infile << Myline;
			infile.close();
		}
	}
}

void SaveManager::SavePlayer(std::string name, int num) // allows the player's name to be saved in the same slot as the vehicles
{
	ifstream outfile;
	outfile.open("text2.txt");
	std::string line = ""; // didn't initalize it
	std::string line2 = "";
	//take everything from file
	int iRow = 0;

	// make sure less then or == to 4 names
	if (num <= 3)
	{
		//read the text if got save
		while (getline(outfile, line2)) // read  from Outfile (initialize it to MyString2)
		{

			if (num == iRow)
			{
				line.append(std::string(name));
			}
			else
			{
				line.append(line2); // if not inputted line add current line to MyString
			}

			line.append("\n"); // go to next line

			iRow++; // row count

		}

		std::cout << line << std::endl; // debug
		outfile.close(); //close file


		ofstream infile;
		infile.open("text2.txt");
		infile << line;
		infile.close();

		if ((num >= iRow || iRow == 0)) // if txt is empty or slot is more than number of rows
		{
			infile.open("text2.txt");
			// create a new line and input(comment for the for loop)
			for (int i = iRow; i <= num; i++)  // start from last line find next line
			{
				if (i == num) // if current line == inputted line then write save
				{
					line.append(std::string(name));
				}
				else
				{
					line.append("\n"); // add a new line if line is not == inputted line
				}
			}
			infile << line;
			infile.close();
		}
	}
}

void SaveManager::AccessVehicle(int num, int& cha, int& whe, int& wea, std::string& name)
{
	ifstream file;
	file.open("text.txt");
	std::string line;
	std::string myline;
	name = ""; // make name empty;
	bool isName = 0; // since they check up to down left to right
	bool isCha = 0;  // use bool to make sure they check for num,cha,whe,wea,name... only once
	bool isWhe = 0;
	bool isWea = 0;
	int Irow = 0;
	while (getline(file, line))// read thru the entire txt
	{
		myline = line; // make myline the curr line
		for (int i = 0; i < line.size(); i++)
		{
			if (num == Irow) 
			{
				if (line[i] >= '0' && line[i] <= '7') 
				{ // only read numbers from 0 to 7

					if (isCha == 0) // hardcode
					{
						cha = line[i] - 48;
						isCha = 1;
					}
					else if (isWhe == 0)// hardcode
					{
						whe = line[i] - 48;
						isWhe = 1;
					}
					else if (isWea == 0)// hardcode
					{
						wea = line[i] - 48;
						isWea = 1;

					}
				}
				if (isName == 0) {
					if (i >= 5) // hard code // prevents read more then once
					{
						name.push_back(line[i]);//from first letter of word to last letter of name
					}
				}
			}
		}
		Irow++; // keep track of current rows

	}
}