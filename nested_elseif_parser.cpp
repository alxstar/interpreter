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
};

class D1: public Base
{
public:
	explicit D1(const std::string& name): name_(name){} 
	virtual ~D1(){}
	virtual void Print(){std::cout << "\t\tD1::PRINT, " << name_ << '\n';}
	virtual Base* GetCommand(){std::cout << "D1::GETCOMMAND\n"; return this;}

private:
	std::string name_;
};

class D2: public Base
{
public:
	explicit D2(const std::string& name): name_(name){}
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
		//std::cout << "D2 " << name_ << "::PUSH\n";
		v_.push_back(std::move(u));
	}
	
	virtual Base* GetCommand()
	{
		std::cout << "D2::GETCOMMAND\n";
		return v_.front()->GetCommand();
	}

private:
	std::list<std::unique_ptr<Base>> v_;
	std::string name_;
};

class D2_: public Base
{
public:
	explicit D2_(const std::string& name): name_(name)
	{
		//std::cout << "D2_(" << name_ << ")\n";
	}
	virtual ~D2_(){}
	virtual void Print()
	{
		std::cout << "D2_::PRINT, " << name_ << ", SIZE: " << l_.size() << '\n';
		for(auto& u: l_)
		{
			u->Print();
		}
	}
	virtual void Push(std::unique_ptr<Base>&& u)
	{
		//std::cout << "D2_ " << name_ << "::PUSH\n";
		l_.push_back(std::move(u));
	}

private:
	std::list<std::unique_ptr<Base>> l_;
	std::string name_;
};


class C
{
public:	
	explicit C(const std::vector<std::string>& v)
	{
		Parse(v);
	}
	void Print()
	{
		std::cout << "C::PRINT, SIZE: " << v_.size() << '\n';
		for(auto& elem: v_)
		{
			elem->Print();
		}
	}

	D1* GetCommand()
	{
		auto* command = dynamic_cast<D1*>(v_.front()->GetCommand());
		return command ? command : nullptr;
	}	

	void Next()
	{
		
	}


private:	
	void HandleIf(const std::string& line)
	{
		auto if_pos = line.find("if ");
		std::string name = line.substr(if_pos + 3);
		std::unique_ptr<Base> new_from_if_to_endif_struct = std::make_unique<D2>(name);
		branches_.push(std::move(new_from_if_to_endif_struct));
				
		std::unique_ptr<Base> new_if_branch = std::make_unique<D2>(name);	
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
		branches_.push(std::move(new_elseif_branch));
	}
	
	void HandleEndif()
	{
		auto ended_if_or_elseif_branch = std::move(branches_.top());	
		branches_.pop();
		if(branches_.empty())
		{
			v_.push_back(std::move(ended_if_or_elseif_branch));
		}
		else
		{	
			auto from_if_to_endif_struct = std::move(branches_.top());
			branches_.pop();
			from_if_to_endif_struct->Push(std::move(ended_if_or_elseif_branch));
			if(branches_.empty())
			{
				v_.push_back(std::move(from_if_to_endif_struct));
			}
			else
				branches_.top()->Push(std::move(from_if_to_endif_struct));
		}
	}
	
	void HandleInstruction(const std::string& line)
	{
		std::unique_ptr<Base> instruction = std::make_unique<D1>(line);
		if(!branches_.empty())
		{
			branches_.top()->Push(std::move(instruction));
		}
		else
		{
			v_.push_back(std::move(instruction));
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
	std::list<std::unique_ptr<Base>> v_;
	std::stack<std::unique_ptr<Base>> branches_;
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
		{
			D1* d1 = c.GetCommand(); 
			d1->Print();
		}
}

