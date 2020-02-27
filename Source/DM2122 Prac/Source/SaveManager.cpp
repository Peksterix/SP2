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

void SaveManager::SavePlayer(std::string name, int num)
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

void SaveManager::AccessVehicle(int& num, int& cha, int& whe, int& wea, std::string& name)
{
	//ifstream outfile;
	//outfile.open("text.txt");
	//std::string Myline = ""; //didn't initialize it
	//std::string line2 = "";
	////take everything from file
	//int iRow = 0;

	//if ()
	//{
	//	// make sure less then or == to 8 slots
	//	if (num < 8) {
	//		//read the text if got save
	//		while (getline(outfile, line2)) { // read  from Outfile (initialize it to line2)

	//			if (num == iRow)
	//			{
	//				// replaces current line with save
	//				//Myline.append(std::to_string(cha) + " " + std::to_string(whe) + " " + std::to_string(wea) + name);
	//				Myline.append(name + std::to_string(wea) + " " + std::to_string(whe) + " " + std::to_string(cha));
	//			}
	//			else
	//				Myline.append(line2); // if not inputted line add current line to Myline

	//			Myline.append("\n"); // go to next line

	//			iRow++; // row count
	//		}

	//		std::cout << Myline << std::endl; // debug
	//		outfile.close(); //close file


	//		ofstream infile;
	//		infile.open("text.txt");
	//		infile << Myline;
	//		infile.close();


	//		if (num >= iRow || iRow == 0) // if txt is empty or slot is more than number of rows
	//		{
	//			infile.open("text.txt");
	//			// create a new line and input(comment for the for loop)
	//			for (int i = iRow; i <= num; i++)  // start from last line find next line
	//			{
	//				if (i == num) // if current line == inputted line then write save
	//				{
	//					//Myline.append(std::to_string(cha) + " " + std::to_string(whe) + " " + std::to_string(wea) + name);
	//					Myline.append(name + std::to_string(wea) + " " + std::to_string(whe) + " " + std::to_string(cha));
	//				}
	//				else
	//					Myline.append("\n"); // add a new line if line is not == inputted line
	//			}
	//			infile << Myline;
	//			infile.close();
	//		}
	//	}
	//}
}