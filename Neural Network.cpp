#include <iostream>
#include "net.h"
#include <fstream>
#include <string.h>

std::vector<char> GenerateData(int length)
{
	std::vector<char> data;
	for ( int i = 0; i < length; i++ )
	{
		data.push_back('a' + rand()%26);
	}
	return data;
}

int main(int argc, char *argv[]) {
	srand (static_cast <unsigned> (time(0)));

	int len = 60;
	std::vector<char> dataVector = GenerateData(len);
	char* data = &dataVector[0];

	std::vector<int> layers = {len, 50, 20, 10, 20, 50, len};

	Network net(layers, data);

	net.LoadWeights(net.WeightPath);
	net.Run(true);

	for ( int i = 0; i < 100; i++ )
	{
		char type[1024];
		std::string temp = "echo " + std::to_string(i);
		strcpy(type, temp.c_str());
		system(type);
		dataVector = GenerateData(len);
		data = &dataVector[0];
		net.Study(data, data);
		net.SaveWeights(net.WeightPath);
		system("clear");
	}	
	net.Run();
	return 0;
}
