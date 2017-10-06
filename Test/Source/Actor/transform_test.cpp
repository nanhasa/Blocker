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
	class TransformPositionParamTest : public TransformTest, public ::testing::WithParamInterface<glm::dvec3> {};
	class TransformAxisRotationParam1Test : public TransformTest, public ::testing::WithParamInterface<double> {};
	class TransformAxisRotationParam2Test : public TransformTest, public ::testing::WithParamInterface<double> {};

	// Test constructors
	TEST_F(TransformTest, defaultBuilder)
	{
		EXPECT_EQ(transform.position, glm::dvec3(0.0f, 0.0f, 0.0f));
		EXPECT_EQ(transform.rotation, glm::dvec3(0.0f, 0.0f, 0.0f));
	}

	TEST_F(TransformTest, doubleConstructor)
	{
		transform = Transform(10, 15, 20, 5, 10, 15);
		EXPECT_EQ(transform.position, glm::dvec3(10, 15, 20));
		EXPECT_EQ(transform.rotation, glm::dvec3(5, 10, 15));
	}

	TEST_P(TransformPositionParamTest, vectorConstructor)
	{
		transform = Transform(GetParam(), GetParam());
		EXPECT_EQ(transform.position, GetParam());
		EXPECT_EQ(transform.rotation, GetParam());
	}

	// Test direction functions
	TEST_P(TransformPositionParamTest, defaultDirections)
	{
		transform = Transform(GetParam(), glm::dvec3(0, 0, 0));
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(),      glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(),   glm::dvec3(1, 0, 0));
	}

	INSTANTIATE_TEST_CASE_P(NegativePositionValues, TransformPositionParamTest,
		::testing::Values(
			glm::dvec3(-10, -15, -20),
			glm::dvec3(-20000, 10000, -30000),
			glm::dvec3(-300000, -100000, -3005000)));

	INSTANTIATE_TEST_CASE_P(DecimalPositionValues, TransformPositionParamTest,
		::testing::Values(
			glm::dvec3(0.00004, 0.002, 0.1),
			glm::dvec3(10.000, 15.99999, 20.9),
			glm::dvec3(20000.999, 10000.111, 30000.000)));

	INSTANTIATE_TEST_CASE_P(PositivePositionValues, TransformPositionParamTest,
		::testing::Values(
			glm::dvec3(0, 0, 0),
			glm::dvec3(10, 15, 20),
			glm::dvec3(200000, 100000, 3000000)));

	INSTANTIATE_TEST_CASE_P(PositiveAndNegativePositionValues, TransformPositionParamTest,
		::testing::Values(
			glm::dvec3(10, -15, 20),
			glm::dvec3(-204400, -100000, 3002500)));

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
		transform.rotation.x = glm::mod(GetParam(), 360.0);
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
		transform.rotation.y = glm::mod(GetParam(), 360.0);
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
		transform.rotation.z = glm::mod(GetParam(), 360.0);
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
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByXDown)
	{
		transform.rotation.x -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
		transform.rotation.x -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByYLeft)
	{
		transform.rotation.y += 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 0, -1));
		transform.rotation.y += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(-1, 0, 0));
		transform.rotation.y += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 0, 1));
		transform.rotation.y += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByYRight)
	{
		transform.rotation.y -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 0, 1));
		transform.rotation.y -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, 1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(-1, 0, 0));
		transform.rotation.y -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 0, -1));
		transform.rotation.y -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByZLeft)
	{
		transform.rotation.z += 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 1, 0));
		transform.rotation.z += 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(-1, 0, 0));
		transform.rotation.z += 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, -1, 0));
		transform.rotation.z += 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}

	TEST_F(TransformTest, DirectionsWhenRotateByZRight)
	{
		transform.rotation.z -= 90; // 270
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, -1, 0));
		transform.rotation.z -= 90; // 180
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, -1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(-1, 0, 0));
		transform.rotation.z -= 90; // 90
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(-1, 0, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(0, 1, 0));
		transform.rotation.z -= 90; // 0
		EXPECT_EQ(transform.getDirectionForward(), glm::dvec3(0, 0, -1));
		EXPECT_EQ(transform.getDirectionUp(), glm::dvec3(0, 1, 0));
		EXPECT_EQ(transform.getDirectionRight(), glm::dvec3(1, 0, 0));
	}
}