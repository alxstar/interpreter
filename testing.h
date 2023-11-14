#ifndef TESTING_H
#define TESTING_H

#include <unordered_map>
#include <vector>
#include <string>

std::unordered_map<std::string, bool> m{
		{"a",  true},
		{"a1", true},
		{"a2", true},
		{"a3", true},
		{"b",  true},
		{"b1", true},
		{"b2", true},
		{"b3", true},
		{"c",  true},
		{"c1", true},
		{"c2", true},
		{"c3", true},
		{"d",  true},
		{"d1", true},
		{"d2", true},
		{"d3", true},
		{"aa",  true},
		{"aa1", true},
		{"aa2", true},
		{"aa3", true},
		{"bb",  true},
		{"bb1", true},
		{"bb2", true},
		{"bb3", true},
		{"cc",  true},
		{"cc1", true},
		{"cc2", true},
		{"cc3", true},
		{"dd",  true},
		{"dd1", true},
		{"dd2", true},
		{"dd3", true}
	};


	std::vector<std::string> v1{
		"a"
	};
	
	std::vector<std::string> v2{
		"a",
		"b"
	};
	
	std::vector<std::string> v3{
		"a",
		"b",
		"c"
	};
	
	std::vector<std::string> v4{
		"if a",
			"a1",
		"endif"
	};

	std::vector<std::string> v5{
		"b",
		"if a",
			"a1",
		"endif"
	};

	std::vector<std::string> v6{
		"if a",
			"a1",
		"endif",
		"b"
	};

	std::vector<std::string> v7{
		"b",
		"if a",
			"a1",
		"endif",
		"c"
	};
	
	std::vector<std::string> v8{
		"if a",
			"a1",
		"elseif b",
			"b1",
		"endif"
	};
	
	std::vector<std::string> v9{
		"c",
		"if a",
			"a1",
		"elseif b",
			"b1",
		"endif"
	};
	
	std::vector<std::string> v10{
		"c",
		"if a",
			"a1",
		"elseif b",
			"b1",
		"endif",
		"d"
	};

	std::vector<std::string> v11{
		"if a",
			"a1",
		"elseif b",
			"b1",
		"elseif c",
			"c1",
		"endif"
	};
		
	std::vector<std::string> v12{
		"if a",
			"a1",
			"a2",
			"a3",
		"elseif b",
			"b1",
			"b2",
			"b3",
		"elseif c",
			"c1",
			"c2",
			"c3",
		"endif"
	};
	
	std::vector<std::string> v13{
		"d",
		"if a",
			"a1",
			"a2",
			"a3",
		"elseif b",
			"b1",
			"b2",
			"b3",
		"elseif c",
			"c1",
			"c2",
			"c3",
		"endif"
	};

	std::vector<std::string> v14{
		"if a",
			"a1",
			"a2",
			"a3",
		"elseif b",
			"b1",
			"b2",
			"b3",
		"elseif c",
			"c1",
			"c2",
			"c3",
		"endif",
		"d",
		"d1"
	};

	std::vector<std::string> v15{
		"d1",
		"if a",
			"a1",
			"a2",
			"a3",
		"elseif b",
			"b1",
			"b2",
			"b3",
		"elseif c",
			"c1",
			"c2",
			"c3",
		"endif",
		"d2"
	};

	std::vector<std::string> nested_1{
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
	
	std::vector<std::string> nested_2{
		"if a", 
			"a1", 
			"a2", 
			"a3", 
			"if b", 
				"b1", 
				"b2", 
				"b3", 
			"elseif bb",
					 "bb1",
				 	 "bb2",
					 "bb3",
			"endif",	 
			"if c", 
				"c1", 
				"c2", 
				"c3", 
			"endif", 
		"endif"
	};	
	
	std::vector<std::string> nested_2_1{
		"if a", 
			"a1", 
			"a2", 
			"a3", 
			"if b", 
				"b1", 
				"b2", 
				"b3", 
			"elseif bb",
					 "bb1",
				 	 "if cc",
					 	"cc1",
						"cc2",
					 "endif",
					 "bb2",
					 "bb3",
			"endif",	 
			"if c", 
				"c1", 
				"c2", 
				"c3", 
			"endif", 
		"endif"
	};	
	
	std::vector<std::string> nested_3{
		"if a",
			"if b",
				"if c",
					"if d",
							"d1",
							"d2",
							"d3",
					
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_4{
		"if a",
			"if b",
				"if c",
					"c1",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_5{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"c1",
				"endif",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_6{
		"if a",
			"if b",
				"if c",
					"c1",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"c2",
				"endif",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_7{
		"if a",
			"if b",
				"if c",
					"if d",
							"d1",
							"d2",
							"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"elseif cc",
					"cc1",
					"cc2",
					"cc3",
				"endif",
			"endif",
		"endif"
	};
	
	std::vector<std::string> nested_7_1{
		"if a",
			"if b",
				"if c",
					"if d",
							"d1",
							"d2",
							"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"elseif cc",
					"cc1",
					"cc2",
					"cc3",
				"endif",
			"elseif bb",
				"bb1",
				"bb2",
			"endif",
		"endif"
	};
	
	std::vector<std::string> nested_7_2{
		"if a",
			"if b",
				"if c",
					"if d",
							"d1",
							"d2",
							"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"c1",
				"c2",
				"c3",
				"elseif cc",
					"cc1",
					"cc2",
					"cc3",
				"endif",
			"elseif bb",
				"bb1",
				"bb2",
			"endif",
		"elseif aa",
			"aa1",
			"aa2",
		"endif"
	};

	std::vector<std::string> nested_8{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"elseif cc",
					"cc1",
					"cc2",
				"endif",
			"elseif bb",
				"bb1",
				"bb2",
			"endif",
		"elseif aa",
			"aa1",
			"aa2",
		"endif"
	};

	std::vector<std::string> nested_8_1{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"c1",
				"c2",
				"elseif cc",
					"cc1",
					"cc2",
				"endif",
			"b1",
			"b2",
			"elseif bb",
				"bb1",
				"bb2",
			"endif",
		"a1",
		"a2",
		"elseif aa",
			"aa1",
			"aa2",
		"endif"
	};

	std::vector<std::string> nested_9{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"elseif bb",
			"bb1",
			"bb2",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_10{
		"a1",
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"endif"
	};

	std::vector<std::string> nested_11{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"endif",
		"a1"
	};
	
	std::vector<std::string> nested_12{
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"a1",
		"endif"
	};

	std::vector<std::string> nested_13{
		"a1",
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"endif",
		"a2"
	};

	std::vector<std::string> nested_14{
		"a1",
		"if a",
			"if b",
				"if c",
					"if d",
						"d1",
						"d2",
						"d3",
					"elseif dd",
						    "dd1",
							"dd2",
							"dd3",
					"endif",
				"endif",
			"endif",
		"a2",
		"endif",
		"a3"
	};

#endif
