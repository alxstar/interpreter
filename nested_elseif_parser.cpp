#include "testing.h"

#include <stack>
#include <list>
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
	virtual void SetParent(Base* parent)=0;
	virtual std::string Name() const=0;


	virtual void PopLast(){}
};

class D1: public Base
{
public:
	explicit D1(const std::string& name, bool is_condition=false): 
		name_(name),
		is_condition_(is_condition)
	{} 
	
	virtual ~D1(){}
	virtual void Print(){std::cout << "\t\tD1::PRINT, " << name_ << '\n';}
	virtual Base* GetCommand(){return this;}
	virtual Base* Parent(){return parent_;}
	bool IsCondition() const {return is_condition_;}
	virtual std::string Name() const {return name_;}
	virtual void SetParent(Base* parent){parent_ = parent;}

private:	
	std::string name_;
	Base* parent_ = nullptr;
	bool is_condition_;
};

class D2: public Base
{
public:
	explicit D2(const std::string& name): 
		name_(name)
	{
	}
	
	virtual ~D2(){}
	virtual std::string Name() const {return name_;}
	virtual void PopLast(){v_.erase(std::next(std::begin(v_)), std::end(v_));}
	virtual void Print(){
		//std::cout << "D2::PRINT BEGIN, " << name_ << "\n";
		for(auto& u: v_){
			u->Print();
		}
		//std::cout << "D2::PRINT END, " << name_ << "\n";
	}
	
	virtual void Push(std::unique_ptr<Base>&& u)
	{
		u->SetParent(this);
		v_.push_back(std::move(u));
	}
	
	virtual Base* GetCommand()
	{
		return v_.front()->GetCommand();
	}

	virtual Base* Front()
	{
		return v_.empty() ? nullptr : v_.front().get();
	}

	virtual void PopFront()
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

	virtual Base* Parent(){return parent_;}
	bool IsEmpty() const { return v_.empty();}
	virtual std::size_t Size() const {return v_.size();}

	virtual void SetParent(Base* parent){parent_ = parent;}

private:
	std::list<std::unique_ptr<Base>> v_;
	std::string name_;
	Base* parent_ = nullptr;
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
			return result;
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
	
	void HandleIf(const std::string& line)
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
		std::unique_ptr<Base> elseif_condition = std::make_unique<D1>(name, true);
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
			std::unique_ptr<Base> instruction = std::make_unique<D1>(line);
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


void test(const std::vector<std::string>& v)
{
	std::unordered_map<std::string, bool> m_2(m);
	//m_2["a"]=false;
	//m_2["a1"]=false;
	//m_2["a2"]=false;
	//m_2["a3"]=false;
	
	//m_2["b"]=false;
	//m_2["b1"]=false;
	//m_2["b2"]=false;
	//m_2["b3"]=false;
	
	//m_2["c"]=false;
	//m_2["c1"]=false;
	//m_2["c2"]=false;
	//m_2["c3"]=false;
	
	//m_2["d"]=false;
	//m_2["d1"]=false;
	//m_2["d2"]=false;
	//m_2["d3"]=false;
	
	//m_2["aa"]=false;
	//m_2["aa1"]=false;
	//m_2["aa2"]=false;
	//m_2["aa3"]=false;
	
	//m_2["bb"]=false;
	//m_2["bb1"]=false;
	//m_2["bb2"]=false;
	//m_2["bb3"]=false;
	
	//m_2["cc"]=false;
	//m_2["cc1"]=false;
	//m_2["cc2"]=false;
	//m_2["cc3"]=false;
	
	//m_2["dd"]=false;
	//m_2["dd1"]=false;
	//m_2["dd2"]=false;
	//m_2["dd3"]=false;
	
	C c(v);
	while(!c.AtEnd())
	{
		D1* command = c.GetCommand();
		std::cout << "NAME: " << command->Name() << '\n'; 
		bool result = m_2[command->Name()]; 
		if(!c.Next(result))
			break;
	}	
}

void simple_test()
{
	//test(v1);
	//test(v2);
	//test(v3);
	//test(v4);
	//test(v5);
	//test(v6);
	//test(v7);
	//test(v8);
	//test(v9);
	//test(v10);
	//test(v11);
	//test(v12);
	//test(v13);
	//test(v14);
	//test(v15);
}

void nested_test()
{
	//test(nested_1);
	//test(nested_2);
	//test(nested_2_1);
	//test(nested_3);
	//test(nested_4);
	//test(nested_5);
	//test(nested_6);
	//test(nested_7);
	//test(nested_7_1);
	//test(nested_7_2);
	//test(nested_8);
	//test(nested_8_1);
	//test(nested_9);
	//test(nested_10);
	//test(nested_11);
	//test(nested_12);
	//test(nested_13);
	//test(nested_14);
}

int main()
{
	//simple_test();
	nested_test();	
	//C c(v2);
	//c.Print();
	//C c(v3);
}

