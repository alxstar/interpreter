#include "scenario.h"
#include "command.h"

#include <stack>
#include <list>
#include <memory>
#include <string>
#include <iostream>

C::C(const std::vector<std::string>& v)
{
	root_ = std::make_unique<D2>("root");
	Parse(v);
}

D1* C::GetCommand()
{
	auto* command = dynamic_cast<D1*>(root_->Front()->GetCommand());
	
	if(!command)
		return nullptr;

	last_command_ = command;
	return command;
}	

bool C::Next(bool result)
{
	if(root_->IsEmpty()) 
		return false;

	if(dynamic_cast<D1*>(root_->Front()))
	{
		root_->PopFront();
		last_command_ = nullptr;
		return result;
	}

	if(!last_command_)
		return false;

	bool is_condition = last_command_->IsCondition();
	return MoveNext(is_condition, result);
}

bool C::MoveNext(bool is_condition, bool result)
{
	if(is_condition)
	{
		if(result)
		{
			last_command_->Parent()->Parent()->PopLast();
			last_command_->Parent()->PopFront();
		}
		else
		{
			last_command_->Parent()->Parent()->PopFront();
		}
	}
	else
	{
		if(result)
		{
			if(last_command_->Parent()->Size()==1)
			{
				last_command_->Parent()->Parent()->Parent()->PopFront();
			}
			else
				last_command_->Parent()->PopFront();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void C::HandleIf(const std::string& line)
{
	auto if_pos = line.find("if ");
	std::string name = line.substr(if_pos + 3);
	
	std::unique_ptr<Base> new_from_if_to_endif_struct = std::make_unique<D2>(std::string("starting with: ") + name);
	branches_.push(std::move(new_from_if_to_endif_struct));
			
	std::unique_ptr<Base> new_if_branch = std::make_unique<D2>(name);	
	std::unique_ptr<Base> if_condition = std::make_unique<D1>(name, true);
	new_if_branch->Push(std::move(if_condition));
	branches_.push(std::move(new_if_branch));
}

void C::HandleElseif(const std::string& line)
{
	auto elseif_pos = line.find("elseif ");
	std::string name = line.substr(elseif_pos + 7);
	auto prev_if_or_elseif_branch = std::move(branches_.top());		
	branches_.pop();
	if(!branches_.empty())
	{
		branches_.top()->Push(std::move(prev_if_or_elseif_branch));
	}	
			
	std::unique_ptr<Base> new_elseif_branch = std::make_unique<D2>(name);		  
	std::unique_ptr<Base> elseif_condition = std::make_unique<D1>(name, true);
	new_elseif_branch->Push(std::move(elseif_condition));
	branches_.push(std::move(new_elseif_branch));
}

void C::HandleEndif()
{
	auto ended_if_or_elseif_branch = std::move(branches_.top());	
	branches_.pop();
	if(branches_.empty())
	{
		root_->Push(std::move(ended_if_or_elseif_branch));
	}
	else
	{	
		auto from_if_to_endif_struct = std::move(branches_.top());
		branches_.pop();
		from_if_to_endif_struct->Push(std::move(ended_if_or_elseif_branch));
		if(branches_.empty())
		{
			root_->Push(std::move(from_if_to_endif_struct));
		}
		else
			branches_.top()->Push(std::move(from_if_to_endif_struct));
	}
}

void C::HandleInstruction(const std::string& line)
{
	if(!branches_.empty())
	{
		std::unique_ptr<Base> instruction = std::make_unique<D1>(line);
		branches_.top()->Push(std::move(instruction));
	}
	else
	{
		std::unique_ptr<Base> instruction = std::make_unique<D1>(line);
		root_->Push(std::move(instruction));
	}
}

void C::Parse(const std::vector<std::string>& v)
{
	for(const auto& line: v)
	{
		if(line.empty())
			continue;

		bool if_found = (line.find("if ") != std::string::npos);
		bool elseif_found = (line.find("elseif ") != std::string::npos);
		auto endif_found = (line.find("endif") != std::string::npos);
		
		if(if_found && !elseif_found)
		{
			HandleIf(line);
		}
		else if(elseif_found)
		{
			HandleElseif(line);
		}
		else if(endif_found)
		{
			HandleEndif();
		}
		else if(!if_found && !elseif_found && !endif_found)
		{
			HandleInstruction(line);
		}
	}
}


