#include <gtest/gtest.h>
#include <Vector2.h>

#define SUITE_NAME TestMath

using namespace BasicRenderer;

TEST(SUITE_NAME, Vector2)
{
	Vector2 control = { 0, 1 };
	Vector2 result = { 0, 1 };

    // TODO
	
	EXPECT_EQ(result, control);
}