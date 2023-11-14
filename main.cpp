#include "scenario.h"
#include "command.h"
#include "testing.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

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
	test(nested_1);
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
	nested_test();
}

