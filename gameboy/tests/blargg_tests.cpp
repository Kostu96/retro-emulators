#include <gtest/gtest.h>

struct BlarggTestParam
{

};

struct BlarggTests :
	public testing::TestWithParam<BlarggTestParam>
{

};
