#include "3rdParty/gtest/gtest.h"

#include "Actor/transform.h"

namespace {

	class TransformTest : public ::testing::Test {
	protected:
		Transform transform;

		void SetUp() override
		{
			transform = Transform();
		}
	};
	// Derived classes are used to divide test cases with different parameter lists
	class TransformVectorParamTest : public TransformTest, public ::testing::WithParamInterface<glm::vec3> {};
	class TransformAxisRotationParam1Test : public TransformTest, public ::testing::WithParamInterface<float> {};
	class TransformAxisRotationParam2Test : public TransformTest, public ::testing::WithParamInterface<float> {};

	// Test constructors
	TEST_F(TransformTest, defaultBuilder)
	{
		EXPECT_EQ(transform.position, glm::vec3(0, 0, 0));
		EXPECT_EQ(transform.rotation, glm::vec3(0, 0, 0));
	}

	TEST_P(TransformVectorParamTest, doubleConstructor)
	{
		auto vec = GetParam();
		float x = vec.x;
		float y = vec.y;
		float z = vec.z;
		transform = Transform(x, y, z, 0, 0, 0);
		EXPECT_EQ(transform.position, vec);
		EXPECT_EQ(transform.rotation, glm::vec3(0, 0, 0));
		transform = Transform(0, 0, 0, x, y, z);
		EXPECT_EQ(transform.position, glm::vec3(0, 0, 0));
		EXPECT_EQ(transform.rotation, vec);
		transform = Transform(x, y, z, x, y, z);
		EXPECT_EQ(transform.position, vec);
		EXPECT_EQ(transform.rotation, vec);
	}

	TEST_P(TransformVectorParamTest, vectorConstructor)
	{
		transform = Transform(GetParam(), GetParam());
		EXPECT_EQ(transform.position, GetParam());
		EXPECT_EQ(transform.rotation, GetParam());
	}

	// Test direction functions
	TEST_P(TransformVectorParamTest, defaultDirections)
	{
		transform = Transform(GetParam(), glm::vec3(0, 0, 0));
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(),      glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(),   glm::vec3(1, 0, 0));
	}

	INSTANTIATE_TEST_CASE_P(NegativeValues, TransformVectorParamTest,
		::testing::Values(
			glm::vec3(-10, -15, -20),
			glm::vec3(-20000, 10000, -30000),
			glm::vec3(-300000, -100000, -3005000)));

	INSTANTIATE_TEST_CASE_P(FloatValues, TransformVectorParamTest,
		::testing::Values(
			glm::vec3(0.00004f, 0.002f, 0.1f),
			glm::vec3(10.000f, 15.99999f, 20.9f),
			glm::vec3(20000.999f, 10000.111f, 30000.000f),
			glm::vec3(20000123.999f, 10000125.111f, 30000120.000f)));

	INSTANTIATE_TEST_CASE_P(PositiveValues, TransformVectorParamTest,
		::testing::Values(
			glm::vec3(0, 0, 0),
			glm::vec3(10, 15, 20),
			glm::vec3(200000, 100000, 3000000)));

	INSTANTIATE_TEST_CASE_P(MixedValues, TransformVectorParamTest,
		::testing::Values(
			glm::vec3(10, -15, 20),
			glm::vec3(-204400, -100000, 3002500),
			glm::vec3(-20000.999f, 10000.111f, 30000.000f)));

	TEST_P(TransformAxisRotationParam1Test, NegativeXRotations)
	{
		transform.rotation.x = GetParam();
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.x = GetParam() - 360;
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	TEST_P(TransformAxisRotationParam1Test, NegativeYRotations)
	{
		transform.rotation.y = GetParam();
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.y = GetParam() - 360;
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	TEST_P(TransformAxisRotationParam1Test, NegativeZRotations)
	{
		transform.rotation.z = GetParam();
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.z = GetParam() - 360;
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	INSTANTIATE_TEST_CASE_P(AxisRotationValues, TransformAxisRotationParam1Test,
		::testing::Values(0, 90, 180, 270, 360));

	TEST_P(TransformAxisRotationParam2Test, FullCirclesXRotations)
	{
		transform.rotation.x = glm::mod(GetParam(), 360.0f);
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.x = GetParam();
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	TEST_P(TransformAxisRotationParam2Test, FullCirclesYRotations)
	{
		transform.rotation.y = glm::mod(GetParam(), 360.0f);
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.y = GetParam();
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	TEST_P(TransformAxisRotationParam2Test, FullCirclesZRotations)
	{
		transform.rotation.z = glm::mod(GetParam(), 360.0f);
		auto fwd = transform.getDirectionForward();
		auto up = transform.getDirectionUp();
		auto right = transform.getDirectionRight();
		transform.rotation.z = GetParam();
		EXPECT_EQ(transform.getDirectionForward(), fwd);
		EXPECT_EQ(transform.getDirectionUp(), up);
		EXPECT_EQ(transform.getDirectionRight(), right);
	}

	INSTANTIATE_TEST_CASE_P(AxisPositiveRotationValues, TransformAxisRotationParam2Test,
		::testing::Values(0, 90, 180, 270, 360, 540, 720, 810, 900, 990, 1080));

	INSTANTIATE_TEST_CASE_P(AxisNegativeRotationValues, TransformAxisRotationParam2Test,
		::testing::Values(-90, -180, -270, -360, -540, -720, -810, -900, -990, -1080));

	TEST_F(TransformTest, DirectionsWhenRotateByXUp) 
	{
		transform.rotation.x += 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByXDown)
	{
		transform.rotation.x -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
		transform.rotation.x -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByYLeft)
	{
		transform.rotation.y += 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 0, -1));
		transform.rotation.y += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(-1, 0, 0));
		transform.rotation.y += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 0, 1));
		transform.rotation.y += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByYRight)
	{
		transform.rotation.y -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 0, 1));
		transform.rotation.y -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(-1, 0, 0));
		transform.rotation.y -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 0, -1));
		transform.rotation.y -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByZLeft)
	{
		transform.rotation.z += 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 1, 0));
		transform.rotation.z += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(-1, 0, 0));
		transform.rotation.z += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, -1, 0));
		transform.rotation.z += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByZRight)
	{
		transform.rotation.z -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, -1, 0));
		transform.rotation.z -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(-1, 0, 0));
		transform.rotation.z -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(0, 1, 0));
		transform.rotation.z -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::vec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::vec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::vec3(1, 0, 0));
	}
}