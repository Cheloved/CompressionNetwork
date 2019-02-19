#include <vector>
#include <string>
#include <fstream>
#ifndef NET_H
#define NET_H

class Neuron
{
public:
	class Akson
	{
	public:
		float getSignal();
		Neuron *ownerNeuron;
	};

	class Dendrit
	{
	public:
		float weight;
		Akson *inputAkson;
		float dw = 0;
	};

	Neuron(float startValue, bool input = false);
	~Neuron();

	std::vector<Dendrit> dendrits;
	Akson akson;
	char value;
	bool isInput;

	float d;

	virtual float kernelFunction();
};

class Network
{
public:
	Network(std::vector<int> layers, char* data);

	void ShowStructure();

	void CreateWeights();
	void SaveWeights(std::string path);
	void LoadWeights(std::string path);
	void ShowWeights();

	bool Check(char* ans);
	void Run(bool silent = false);
	void Study(char *data, char *answeres);

	std::string WeightPath = "Weights.txt";
	int layers;
	float N;
	char* data;
	std::vector<std::vector<Neuron>> neurons;
};
#endif
