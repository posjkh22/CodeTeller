
#include <list>
#include <string>

namespace IPA
{

class Visualize;
class VisualizationPolicy;

class Visualize final
{

public:
	Visualize(VisualizationPolicy*, const char*, const char*);

private:
	VisualizationPolicy* p_policy;
	const char* p_src;
	const char* p_dot;

public:
	bool ChangeVisualizePolicy(VisualizationPolicy* );
	bool run();
	bool Setup();
	bool debug();
};


class VisualizationPolicy
{
protected:
	std::list<std::string* > input_value_list;

public:
	virtual bool apply_input_value(const char*){ return true; };
	virtual bool activate(const char*){ return true; };
	virtual bool debug_show_input_value(){ return true; };

	virtual ~VisualizationPolicy(){};
};


class Graphviz final: public VisualizationPolicy
{
	
public:
	bool apply_input_value(const char* ) override;
	bool debug_show_input_value() override;
	bool activate(const char* ) override;

};

};
