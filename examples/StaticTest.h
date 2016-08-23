#pragma once
class StaticTest
{
public:

	StaticTest()
	{
	}

	virtual ~StaticTest()
	{
	}
	static int SetInt(int n)
	{
		return s_int = n;
	}

	int m_int;
	static int s_int;
};

