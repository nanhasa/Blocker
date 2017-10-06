#include "stdafx.h"  

#include "3rdParty/gtest/gtest.h"

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke
}