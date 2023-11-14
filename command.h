#ifndef COMMAND_H
#define COMMAND_H

#include <memory>
#include <list>
#include <string>

class Base
{
public:
	virtual ~Base(){}
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
	explicit D2(const std::string& name); 
	
	virtual ~D2(){}
	virtual std::string Name() const {return name_;}
	virtual void PopLast(){v_.erase(std::next(std::begin(v_)), std::end(v_));}
	
	virtual void Push(std::unique_ptr<Base>&& u);
	
	virtual Base* GetCommand();

	virtual Base* Front();

	virtual void PopFront();

	virtual Base* Parent(){return parent_;}
	bool IsEmpty() const { return v_.empty();}
	virtual std::size_t Size() const {return v_.size();}

	virtual void SetParent(Base* parent){parent_ = parent;}

private:
	std::list<std::unique_ptr<Base>> v_;
	std::string name_;
	Base* parent_ = nullptr;
};


#endif

