#pragma once
#include <iostream>
using std::string;
namespace quotes
{
	enum Quotes
	{
		Q1 = 0,
		Q2,
		Q3,
		Q4,
		Q5,
		Q6,
		Q7,
		Q8,
		Q9,
		Q10,
		Q11,
		Q12,
		Q13,
		Q14,
		Q15,
		Q16,
		Q17,
		Q18,
		Q19,
		Q20,
	};

	static string quoteRequests[]
	{
		"GET 1\r\n",
		"GET 2\r\n",
		"GET 3\r\n",
		"GET 4\r\n",
		"GET 5\r\n",
		"GET 6\r\n",
		"GET 7\r\n",
		"GET 8\r\n",
		"GET 9\r\n",
		"GET 10\r\n",
		"GET 11\r\n",
		"GET 12\r\n",
		"GET 13\r\n",
		"GET 14\r\n",
		"GET 15\r\n",
		"GET 16\r\n",
		"GET 17\r\n",
		"GET 18\r\n",
		"GET 19\r\n",
		"GET 20\r\n",
	};
}