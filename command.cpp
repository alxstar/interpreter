#include "command.h"

D2::D2(const std::string& name): 
		name_(name)
{
}

void D2::Push(std::unique_ptr<Base>&& u)
{
	u->SetParent(this);
	v_.push_back(std::move(u));
}

Base* D2::GetCommand()
{
	return v_.front()->GetCommand();
}

Base* D2::Front()
{
	return v_.empty() ? nullptr : v_.front().get();
}

void D2::PopFront()
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

