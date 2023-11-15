#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <list>
#include <string>

class CommandBase
{
public:
	virtual ~CommandBase(){}
	virtual void Push(std::unique_ptr<CommandBase>&& u){}
	virtual CommandBase* GetCommand()=0;
	virtual void PopFront(){}
	virtual CommandBase* Parent()=0;
	virtual bool IsEmpty() const {return true;}
	virtual CommandBase* Front(){return nullptr;}
	virtual std::size_t Size() const {return 0;}
	virtual void SetParent(CommandBase* parent)=0;
	virtual std::string Name() const=0;

	virtual void PopLast(){}
};

class TestCommand: public CommandBase
{
public:
	explicit TestCommand(const std::string& name, bool is_condition=false): 
		name_(name),
		is_condition_(is_condition)
	{} 
	
	virtual ~TestCommand(){}
	virtual CommandBase* GetCommand(){return this;}
	virtual CommandBase* Parent(){return parent_;}
	bool IsCondition() const {return is_condition_;}
	virtual std::string Name() const {return name_;}
	virtual void SetParent(CommandBase* parent){parent_ = parent;}

private:	
	std::string name_;
	CommandBase* parent_ = nullptr;
	bool is_condition_;
};

class Branch: public CommandBase
{
public:
	explicit Branch(const std::string& name=""); 
	
	virtual ~Branch(){}
	virtual std::string Name() const {return name_;}
	virtual void PopLast(){v_.erase(std::next(std::begin(v_)), std::end(v_));}
	
	virtual void Push(std::unique_ptr<CommandBase>&& u);
	
	virtual CommandBase* GetCommand();

	virtual CommandBase* Front();

	virtual void PopFront();

	virtual CommandBase* Parent(){return parent_;}
	bool IsEmpty() const { return v_.empty();}
	virtual std::size_t Size() const {return v_.size();}

	virtual void SetParent(CommandBase* parent){parent_ = parent;}

private:
	std::list<std::unique_ptr<CommandBase>> v_;
	std::string name_;
	CommandBase* parent_ = nullptr;
};


#endif

