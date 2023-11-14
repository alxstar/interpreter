#ifndef SCENARIO_H
#define SCENARIO_H

#include "command.h"

#include <stack>
#include <list>
#include <memory>
#include <string>
#include <vector>

class C
{
public:	
	explicit C(const std::vector<std::string>& v);
	std::size_t Size() const {return root_->Size();}
	D1* GetCommand();
	bool Next(bool result);
	bool AtEnd() const {return root_->IsEmpty();}

private:	
	bool MoveNext(bool is_condition, bool result);
	void HandleIf(const std::string& line);
	void HandleElseif(const std::string& line);
	void HandleEndif();
	void HandleInstruction(const std::string& line);
	void Parse(const std::vector<std::string>& v);

private:				
	std::unique_ptr<Base> root_ = nullptr;
	std::stack<std::unique_ptr<Base>> branches_;
	D1* last_command_ = nullptr;
};

#endif

