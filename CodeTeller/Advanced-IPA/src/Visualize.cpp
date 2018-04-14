
#include <iostream>
#include <fstream>
#include "Visualize.hpp"

using namespace IPA;

Visualize::Visualize(VisualizationPolicy* policy, const char* src, const char* dot)
	: p_policy(policy), p_src(src), p_dot(dot) 
{

}


bool Visualize::ChangeVisualizePolicy(VisualizationPolicy* new_policy)
{
	if(p_policy == nullptr)
	{
		p_policy = new_policy;
	}
	else
	{
		delete p_policy;
		p_policy = new_policy;
	}

	return true;
}

bool Visualize::run()
{
	p_policy->activate(p_dot);

	return true;
}

bool Visualize::Setup()
{
	
	p_policy->apply_input_value(p_src);

	return true;
}

bool Visualize::debug()
{

	p_policy->debug_show_input_value();
	return true;
}

bool Graphviz::apply_input_value(const char* dat)
{
	
	std::ifstream fin;
	fin.open(dat, std::ifstream::in);

	#define STRING_SIZE 128
	//char inputString[STRING_SIZE];
	std::string inputString;
	std::string* p_inputString;

	while(!fin.eof())
	{	
		fin >> inputString;
		//fin.getline(inputString, STRING_SIZE);
		p_inputString = new std::string(inputString);
		input_value_list.push_back(p_inputString);
	}

	input_value_list.pop_back();

	#undef STRING_SIZE
	
	fin.close();
	return true;
}

bool Graphviz::debug_show_input_value()
{
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{
		std::string currentstring(**iter);
		std::cout << currentstring << std::endl;
	}

	return true;
}



bool Graphviz::activate(const char* dat)
{
	
	std::ofstream fout;
	fout.open(dat, std::ofstream::out);

	fout << "digraph Resource {" << std::endl;	

	fout << "node [shape=circle, width=1.7]; ";
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{
		bool redundant_flag = false;
		for(auto iter1 = iter;
				iter1 != input_value_list.end(); )
		{
			iter1++;

			if(*iter == *iter1)
			{
				redundant_flag = true;
			}
		}
		if(redundant_flag == true)
		{
			continue;
		}
		
		std::string currentString(**iter);
		if(currentString.at(0) == '#')
		{
			fout << " " << "\"" << currentString.substr(1) << "\"" << ";";
		
		}

	}
	fout << std::endl;

	fout << "node [shape=box]; ";
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{	

		bool redundant_flag = false;
		for(auto iter1 = iter;
				iter1 != input_value_list.end(); )
		{
			iter1++;
			if(*iter == *iter1)
			{
				redundant_flag = true;
			}
		}
		if(redundant_flag == true)
		{
			continue;
		}

		std::string currentString(**iter);
		if(currentString.at(0) != '#')
		{	
			fout << " " << "\"" << currentString << "\"" << ";";
		}	
	}
	fout << std::endl;

	std::string processString;
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{	

		std::string currentString(**iter);
		if(currentString.at(0) == '#')
		{	
			processString = currentString.substr(1);
		
		}
		else
		{	
			fout << "\"" << processString << "\"" << "->";
			fout << "\"" << currentString << "\"" << ";";
		}
		fout << std::endl;

	}
	fout << std::endl;
	fout << "}";
	fout.close();
	return true;
}
