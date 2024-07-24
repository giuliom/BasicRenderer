#include <gtest/gtest.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Matrix4.h>

#define SUITE_NAME TestMath

using namespace BasicRenderer;

// TODO add addtional tests

TEST(SUITE_NAME, Dot_V2)
{
	float control = 17.f;
	Vector2 a = { 1.f, 2.f };
	Vector2 b = { 5.f, 6.f };

    float result = Vector2::Dot(a, b);
	
	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Dot_V3)
{
	float control = 38.f;
	Vector3 a = { 1.f, 2.f, 3.f };
	Vector3 b = { 5.f, 6.f, 7.f };

    float result = Vector3::Dot(a, b);
	
	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Cross_V3)
{
	Vector3 control = {-4.f, 8.f, -4.f};
	Vector3 a = { 1.f, 2.f, 3.f };
	Vector3 b = { 5.f, 6.f, 7.f };

    Vector3 result = Vector3::CrossProduct(a, b);
	
	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Dot_V4)
{
	float control = 70.f;
	Vector4 a = { 1.f, 2.f, 3.f, 4.f };
	Vector4 b = { 5.f, 6.f, 7.f, 8.f };

    float result = Vector4::Dot(a, b);
	
	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Mul_M4)
{
	Matrix4 control =  {64.f, 54.f, 44.f, 34.f,
						160.f, 134.f, 108.f, 82.f,
						64.f, 54.f, 44.f, 34.f,
						160.f, 134.f, 108.f, 82.f};

	Matrix4 a = {1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f,
				 1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f};

	Matrix4 b = {4.f, 3.f, 2.f, 1.f,
				 8.f, 7.f, 6.f, 5.f,
				 4.f, 3.f, 2.f, 1.f,
				 8.f, 7.f, 6.f, 5.f,};

	Matrix4 result = a * b;

	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Mul_M4_V4)
{
	Vector4 control =  {30.f, 70.f, 30.f, 70.f};

	Matrix4 a = {1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f,
				 1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f};

	Vector4 b = { 1.f, 2.f, 3.f, 4.f };

	Vector4 result = a * b;

	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Det_M4)
{
	float control = 0.f;

		Matrix4 a = {1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f,
				 1.f, 2.f, 3.f, 4.f,
				 5.f, 6.f, 7.f, 8.f};
	
	float result = a.Det();

	EXPECT_EQ(result, control);
}

TEST(SUITE_NAME, Inverse_M4)
{
	Matrix4 control =  {1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						0.f, 0.f, 0.f, 1.f};

	Matrix4 a = {1.f, 0.f, 0.f, 0.f,
				 0.f, 1.f, 0.f, 0.f,
				 0.f, 0.f, 1.f, 0.f,
				 0.f, 0.f, 0.f, 1.f};

	Matrix4 result = a.Inverse();
	
	EXPECT_EQ(result, control);
}