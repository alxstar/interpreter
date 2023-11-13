#include <stack>
#include <list>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

class Base
{
public:
	virtual ~Base(){}
	virtual void Print()=0;
	virtual void Push(std::unique_ptr<Base>&& u){}
	virtual Base* GetCommand()=0;
	virtual void PopFront(){}
	virtual Base* Parent()=0;
	virtual bool IsEmpty() const {return true;}
	virtual Base* Front(){return nullptr;}
	virtual std::size_t Size() const {return 0;}
};

class D1: public Base
{
public:
	explicit D1(const std::string& name, Base* parent=nullptr, bool is_condition=false): 
		name_(name),
		parent_(parent),
		is_condition_(is_condition)
	{} 
	
	virtual ~D1(){}
	virtual void Print(){std::cout << "\t\tD1::PRINT, " << name_ << '\n';}
	virtual Base* GetCommand(){std::cout << "D1::GETCOMMAND\n"; return this;}
	virtual Base* Parent(){return parent_;}
	bool IsCondition() const {return is_condition_;}

private:
	std::string name_;
	Base* parent_;
	bool is_condition_;
};

class D2: public Base
{
public:
	explicit D2(const std::string& name, Base* parent=nullptr): 
		name_(name),
		parent_(parent)
	{
		std::cout << "D2 CONSTRUCTOR, NAME: " << name_ << ", PARENT: " << parent << '\n';
	}
	
	virtual ~D2(){}
	virtual void Print(){
		std::cout << "D2::PRINT BEGIN, " << name_ << "\n";
		for(auto& u: v_){
			u->Print();
		}
		std::cout << "D2::PRINT END, " << name_ << "\n";
	}
	
	virtual void Push(std::unique_ptr<Base>&& u)
	{
		std::cout << "D2 " << name_ << "::PUSH\n";
		v_.push_back(std::move(u));
	}
	
	virtual Base* GetCommand()
	{
		std::cout << name_ << ", SIZE: " << v_.size() << ", D2::GETCOMMAND\n";
		return v_.front()->GetCommand();
	}

	virtual Base* Front()
	{
		return v_.empty() ? nullptr : v_.front().get();
	}

	virtual void PopFront()
	{
		std::cout << name_ << ", D2::POPFRONT, " << "размер до pop_front: " << v_.size() << "\n";
		v_.pop_front();
		std::cout << name_ << ", D2::POPFRONT, " << "размер после pop_front: " << v_.size() << "\n";
		if(v_.empty())
		{
			std::cout << "if(v_.empty()), parent: " << parent_ << "\n";
			if(parent_)
				parent_->PopFront();
		}
	}

	virtual Base* Parent(){return parent_;}
	bool IsEmpty() const { return v_.empty();}
	virtual std::size_t Size() const {std::cout << name_ << " D2::SIZE, " << v_.size() << '\n';return v_.size();}

private:
	std::list<std::unique_ptr<Base>> v_;
	std::string name_;
	Base* parent_;
};


class C
{
public:	
	explicit C(const std::vector<std::string>& v)
	{
		root_ = std::make_unique<D2>("root");
		Parse(v);
	}
	
	void Print()
	{
		root_->Print();
	}
	
	std::size_t Size() const {return root_->Size();}

	D1* GetCommand()
	{
		auto* command = dynamic_cast<D1*>(root_->Front()->GetCommand());
		
		if(!command)
			return nullptr;

		last_command_ = command;
		return command;
	}	

	bool Next(bool result)
	{
		if(root_->IsEmpty()) 
			return false;

		if(dynamic_cast<D1*>(root_->Front()))
		{
			root_->PopFront();
			last_command_ = nullptr;
			return true;
		}
	
		if(!last_command_)
			return false;

		bool is_condition = last_command_->IsCondition();
		return MoveNext(is_condition, result);
	}
	bool AtEnd() const {return root_->IsEmpty();}

private:	
	
	bool MoveNext(bool is_condition, bool result)
	{
		std::cout << "MOVENEXT(" << std::boolalpha << is_condition << ", " << result << ")\n";
		if(is_condition)
		{
			if(result)
			{
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
				last_command_->Parent()->PopFront();
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	
	void HandleIf(const std::string& line)
	{
		auto if_pos = line.find("if ");
		std::string name = line.substr(if_pos + 3);
		
		std::unique_ptr<Base> new_from_if_to_endif_struct = std::make_unique<D2>(std::string("starting with: ") + name);
		branches_.push(std::move(new_from_if_to_endif_struct));
				
		std::unique_ptr<Base> new_if_branch = std::make_unique<D2>(name);	
		std::unique_ptr<Base> if_condition = std::make_unique<D1>(name, new_if_branch.get(), true);
		new_if_branch->Push(std::move(if_condition));
		branches_.push(std::move(new_if_branch));
	}
	
	void HandleElseif(const std::string& line)
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
		std::unique_ptr<Base> elseif_condition = std::make_unique<D1>(name, new_elseif_branch.get(), true);
		new_elseif_branch->Push(std::move(elseif_condition));
		branches_.push(std::move(new_elseif_branch));
	}
	
	void HandleEndif()
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
	
	void HandleInstruction(const std::string& line)
	{
		if(!branches_.empty())
		{
			std::unique_ptr<Base> instruction = std::make_unique<D1>(line, branches_.top().get());
			branches_.top()->Push(std::move(instruction));
		}
		else
		{
			std::unique_ptr<Base> instruction = std::make_unique<D1>(line);
			root_->Push(std::move(instruction));
		}
	}
	
	void Parse(const std::vector<std::string>& v)
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

private:				
	//std::list<std::unique_ptr<Base>> v_;
	std::unique_ptr<Base> root_ = nullptr;
	std::stack<std::unique_ptr<Base>> branches_;
	D1* last_command_ = nullptr;
};

int main()
{
	std::vector<std::string> v{
		//"keyright",
		"if a", 
			"a1", 
			"a2", 
			"a3", 
			"if b", 
				"b1", 
				"b2", 
				"b3", 
				"if c", 
					"c1", 
					"c2", 
					"c3", 
				"endif", 
			"endif", 
		"endif"
	};	
	
	std::vector<std::string> v2{
		//"keyright",
		"if a", 
			"a1", 
			"a2", 
			"a3", 
			"if b", 
				"b1", 
				"b2", 
				"b3", 
			"elseif bbb",
					 "bbb1",
				 	 "bbb2",
					 "bbb3",
			"endif",	 
				"if c", 
					"c1", 
					"c2", 
					"c3", 
				"endif", 
		"endif"
	};	
	
	
	C c(v2);
	c.Print();
	while(!c.AtEnd())
	{
		D1* command = c.GetCommand();
		command->Print();
		c.Next(true);
	}	
}

