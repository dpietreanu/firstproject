#include <fstream>
#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>
#include <boost/algorithm/string.hpp>

/*
 * A class to read data from a csv file.
 */
class CSVReader
{
	std::string fileName;
	std::string delimeter;

public:
	CSVReader(std::string filename, std::string delm = ",") :
			fileName(filename), delimeter(delm)
	{ }

	// Function to fetch data from a CSV File
	std::vector<std::vector<std::string> > getData();
};

/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
	std::ifstream file(fileName);

	std::vector<std::vector<std::string> > dataList;

	std::string line = "";
	// Iterate through each line and split the content using delimeter
	while (getline(file, line))
	{
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}
	// Close the File
	file.close();

	return dataList;
}


	int main(int argc, char **argv){

	char* filetext=new char[400];
	char* numefisier=new char[200];
	char* currentdir=new char[400];
	getcwd(currentdir,100);
	if (argc > 1)
      strcpy(numefisier, argv[1]);
	else {
      printf("Usage: read_binary <filename>\n");
      return 0;
   }

	sprintf(filetext,"%s/%s.csv",currentdir,numefisier);
    printf("Processing file: %s \n",filetext);

	// Creating an object of CSVWriter
	CSVReader reader(filetext);

	// Get the data from CSV File
	std::vector<std::vector<std::string> > dataList = reader.getData();
int firstline=0;
	// Print the content of row by row on screen
	for(std::vector<std::string> vec : dataList)
	{
        if (firstline==0)
        std::cout<<vec.size()<<std::endl;
		for(std::string data : vec)
		{
            if (firstline==0)
			std::cout<<data << " , ";
		}
		if (firstline==0)
		std::cout<<std::endl;
		firstline++;
	}

	delete[] filetext;
	delete[] numefisier;
	delete[] currentdir;

	return 0;

}
