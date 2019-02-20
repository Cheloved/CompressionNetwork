#include "net.h"
#include <cmath>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>

/*float Sygma(float x)
{
	return ( 1 / (1 + exp(-x)) );
}

float D_Sygma(float x)
{
	return Sygma(x) * (1 - Sygma(x));
}*/
float Sygma(float x)
{
	if ( x >= 1 )
		return x;
	else
		return 0;
}

float D_Sygma(float x)
{
	if ( x >= 1 )
		return 1;
	else
		return 0;
}

float GetRandomFloat()
{
	return ( static_cast <float> (rand()) / static_cast <float> (RAND_MAX) );
}

float Neuron::Akson::getSignal()
{
	if ( this->ownerNeuron->isInput == true )
		return Sygma(this->ownerNeuron->value);
	else
	{
		float x = ownerNeuron->kernelFunction();
		return Sygma(x);
	}
}

Neuron::Neuron(float startValue, bool input)
{
	this->value = startValue;
	this->akson.ownerNeuron = this;
    this->isInput = input;
}

Neuron::~Neuron()
{
	
}

float Neuron::kernelFunction()
{
	if ( this->isInput == true )
	{
		return (this->value);
	}
	else
	{
		float result(0.);
		for(Neuron::Dendrit d : this->dendrits){
			result += d.inputAkson->getSignal() * d.weight;
		}
		return result;
	}
}

void Network::SaveWeights(std::string path)
{
	std::ofstream file;
	file.open(path);

	for ( int layer = 1; layer < this->layers; layer++ )
	{
		for ( int i = 0; i < neurons[layer].size(); i++ )
		{
			for ( int a = 0; a < neurons[layer-1].size(); a++ )
			{
				file << neurons[layer][i].dendrits[a].weight << std::endl;
			}
		}
	}

	file.close();
	//std::cout << "[OK] Weights saved" << std::endl;
}

void Network::LoadWeights(std::string path)
{
	std::ifstream file;
	std::string line;
	file.open(path);

	if(file.is_open())
	{
		for ( int layer = 1; layer < this->layers; layer++ )
		{
			for ( int i = 0; i < neurons[layer].size(); i++ )
			{
				for ( int a = 0; a < neurons[layer-1].size(); a++ )
				{
					std::getline(file,line);
					float w = std::stof(line);
					Neuron::Dendrit d;
					d.weight = w;
					d.inputAkson = &neurons[layer-1][a].akson;
					this->neurons[layer][i].dendrits.push_back(d);
				}
			}
		}
	}
	file.close();
	//std::cout << "[OK] Weights loaded" << std::endl;
}

void Network::CreateWeights()
{
	for ( int layer = 1; layer < this->layers; layer++ )
	{
		for ( int i = 0; i < neurons[layer].size(); i++ )
		{
			for ( int a = 0; a < neurons[layer-1].size(); a++ )
			{
				Neuron::Dendrit d;
				d.weight = GetRandomFloat();
				d.inputAkson = &neurons[layer-1][a].akson;
				this->neurons[layer][i].dendrits.push_back(d);
			}
		}
	}
	std::cout << "[OK] Weights created" << std::endl;
}

void Network::ShowStructure()
{
	for ( int layer = 0; layer < this->layers; layer++ )
	{
		std::cout << std::endl << "Layer[" << layer << "] : " << std::endl;
		for ( int i = 0; i < this->neurons[layer].size(); i++ ) { std::cout << this->neurons[layer][i].value << " "; }
		std::cout << std::endl;
	}
}

void Network::ShowWeights()
{
	for ( int layer = 1; layer <  this->layers; layer++)
	{
		std::cout << std::endl << "Layer[" << layer << "] : " << std::endl;
		for ( int i = 0; i < this->neurons[layer].size(); i++ )
		{
			for ( int a = 0; a < this->neurons[layer][i].dendrits.size(); a++ )
			{
				std::cout << this->neurons[layer][i].dendrits[a].weight << " ";
			}
		}
		std::cout << std::endl;
	}
}

bool Network::Check(char *ans)
{
	bool equal = true;
	for ( int i = 0; i < this->neurons.back().size(); i++ ) 
		if ( this->neurons.back()[i].value != ans[i] )
			equal = false;

	return equal;
}

void Network::ShowAverageError()
{
	float sum = 0;
	for ( int i = 0; i < this->neurons.back().size(); i++ )
		sum += this->neurons.back()[i].d;
	std::cout << "Error average: " << sum / this->neurons.back().size() << std::endl; 
}

void Network::Run(bool silent)
{
	for ( int layer = 0; layer < this->layers; layer++ )
	{
		if ( layer == 0 )
			for ( int i = 0; i < this->neurons[layer].size(); i++ ) { this->neurons[layer][i].value = this->data[i]; }
		else		
			for ( int i = 0; i < this->neurons[layer].size(); i++ ) { this->neurons[layer][i].value = this->neurons[layer][i].kernelFunction(); }
	}

	if ( silent == false )
	{
		std::cout  << "Result: " << std::endl;
		for ( int x = 0; x < this->neurons.back().size(); x++ )
		{
			std::cout << "Out Neuron[" << x << "] = " << this->neurons.back()[x].value << std::endl;
		}
	}
}

int Network::Study(char *data)
{
	this->data = data;
	char *answeres = this->data;
	Run(true);
	if ( Check(answeres) == true )
	{
		std::cout << "Network has been learned!" << std::endl;
		this->SaveWeights(this->WeightPath);
		return 1;
	}
	for ( int i = 0; i < this->neurons.back().size(); i++ )
	{
		this->neurons.back()[i].d = answeres[i] - neurons.back()[i].value;
	}
	for ( int layer = this->layers-2; layer >= 0; layer-- )
	{
		for ( int i = 0; i < this->neurons[layer].size(); i++ )
		{
			float _d = 0;
			for ( int a = 0; a < this->neurons[layer+1].size(); a++ )
			{
				_d += this->neurons[layer+1][a].d * this->neurons[layer+1][a].dendrits[i].weight;
			}
			this->neurons[layer][i].d = _d;
		}
	}

	for ( int layer = 1; layer < this->layers; layer++ )
	{
		for ( int i = 0; i < this->neurons[layer].size(); i++ )
		{
			for ( int a = 0; a < this->neurons[layer-1].size(); a++ )
			{
				float _dw = this->N * this->neurons[layer][i].d * D_Sygma(this->neurons[layer][i].value) * this->neurons[layer-1][a].value;
				this->neurons[layer][i].dendrits[a].weight += _dw;
			}
		}
	}
	return 0;
	//std::cout << "[OK] Study example ready" << std::endl;
}

Network::Network(std::vector<int> _layers, char* data)
{
	for ( int layer = 0; layer < _layers.size(); layer++ )
	{
		std::vector<Neuron> vec;

		for ( int i = 0; i < _layers[layer]; i++ )
		{
			if ( layer == 0 )
			{
				Neuron n(data[i], true);
				vec.push_back(n);
			} else {
				Neuron n(0, false);
				vec.push_back(n);
			}
		}
		this->neurons.push_back(vec);
	}
	this->N = 0.2;
	this->layers = _layers.size();
	this->data = data;
	srand (static_cast <unsigned> (time(0)));
}

