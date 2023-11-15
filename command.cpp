#include "command.h"

Branch::Branch(const std::string& name): 
		name_(name)
{
}

void Branch::Push(std::unique_ptr<CommandBase>&& u)
{
	u->SetParent(this);
	v_.push_back(std::move(u));
}

CommandBase* Branch::GetCommand()
{
	return v_.front()->GetCommand();
}

CommandBase* Branch::Front()
{
	return v_.empty() ? nullptr : v_.front().get();
}

void Branch::PopFront()
{
	v_.pop_front();
	if(v_.empty())
	{
		if(parent_)
		{
			parent_->PopFront();
		}
	}
}

