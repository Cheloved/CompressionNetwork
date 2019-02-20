#include <iostream>
#include "net.h"
#include <fstream>
#include <string.h>

class Program
{
	public:

	int len = 50;
	std::vector<char> dataVector;
	char *data;

	std::vector<int> layers;
	Network *net;

	std::vector<char> GenerateData(int length = 0)
	{
		if ( length == 0 )
			length = this->len;
		std::vector<char> _data;
		for ( int i = 0; i < length; i++ )
		{
			_data.push_back('a' + rand()%26);
		}
		return _data;
	}

	void TerminalWrite(std::string text)
	{
		char type[1024];
		std::string temp = "echo Current example: " + text;
		strcpy(type, temp.c_str());
		system(type);
	}

	void ShowData()
	{
		for ( int i = 0; i < this->len; i++ )
		{
			std::cout << this->dataVector[i];
		}
		std::cout << std::endl;
	}

	void Initialize(bool createWeights)
	{
		srand (static_cast <unsigned> (time(0)));

		this->dataVector = GenerateData(50);
		this->data = &dataVector[0];
		ShowData();
		this->layers = {this->len, 20, 10, 20, this->len};
		this->net = new Network(this->layers, this->data);
		if ( createWeights == true )
		{
			this->net->CreateWeights();
			this->net->SaveWeights(this->net->WeightPath);
		}
		else this->net->LoadWeights(this->net->WeightPath);
	}

	void Learn(int count = 10)
	{
		for ( int i = 0; i < count; i++ )
		{
			TerminalWrite( std::to_string(i) );
			this->dataVector = GenerateData();
			this->data = &dataVector[0];

			int result = this->net->Study(this->data);
			this->net->SaveWeights(this->net->WeightPath);			
			if ( result == 1 )
				break;
			system("clear");
			this->net->ShowAverageError();
			//delete net;
			//net = new Network(layers, data);
		}
		this->net->ShowStructure();
	}

	Program() {
		this->Initialize(false);
		Learn(10000);
	}
};

int main(int argc, char *argv[])
{
	Program *p = new Program();
	delete p;
}

