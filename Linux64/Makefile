all: target2 target3 target1

target1: SDKNewTest.cpp
	g++ -g SDKNewTest.cpp -o SDKNewTest -I/Test/include -L/Test/linux64/ -lgtest -lpthread -lSgpApi -std=c++14

target2: RunAllTest.cpp
	g++ -g RunAllTest.cpp -o RunAllTest -I/Test/include -L/Test/linux64/ -lgtest -lpthread -lSgpApi -std=c++14

target3: Test.cpp
	g++ -g Test.cpp -o Test -I/Test/include -L/Test/linux64/ -lgtest -lpthread -lSgpApi -std=c++14

target4:01_ITA_GetTempMatrixInfo.cpp 02_ITA_GetY16MatrixInfo.cpp 03_ITA_TempMatrixRotation.cpp 04_ITA_TempMatrix2Rgb.cpp 05_ITA_GetTempMatrixMaxMinT.cpp 06_ITA_GetColorStripData.cpp 07_ITA_GenerateHeapMapJpg.cpp 08_Y16Set.cpp common.cpp ITA_SDK.cpp
	g++ -g 01_ITA_GetTempMatrixInfo.cpp 02_ITA_GetY16MatrixInfo.cpp 03_ITA_TempMatrixRotation.cpp 04_ITA_TempMatrix2Rgb.cpp 05_ITA_GetTempMatrixMaxMinT.cpp 06_ITA_GetColorStripData.cpp 07_ITA_GenerateHeapMapJpg.cpp 08_Y16Set.cpp common.cpp ITA_SDK.cpp -o ITA_Test -I/Test/include -L/Test/linux64/ -lgtest -lpthread -lSgpApi -lita -ljsoncpp -std=c++14

clean:
	rm -rf target1 target2 target3 target4

