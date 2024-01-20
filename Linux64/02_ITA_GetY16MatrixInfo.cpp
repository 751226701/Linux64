#include"common.h"

class GetY16MatrixInfo : public testing::Test
{
public:
	HANDLE_T instance;

	void SetUp() override
	{
		instance = ITA_HeatmapInit();
	}

	void TearDown() override
	{
		ITA_HeatmapUninit(instance);
	}
};

//01.从ITA_DLT热图获取Y16矩阵
TEST_F(GetY16MatrixInfo, 01_DLT)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[width * height];

	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "从ITA_DLT热图获取Y16矩阵失败！" << endl;
	cout << "从ITA_DLT热图获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);
	EXPECT_EQ(y16MaxtrixInfo.w, WIDTH) << "获取的宽错误！" << endl;
	EXPECT_EQ(y16MaxtrixInfo.h, HEIGHT) << "获取的高错误！" << endl;
}

//02.传入文件与热图类型不匹配
TEST_F(GetY16MatrixInfo, 02_TYPE_ERR)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[width * height];

	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	EXPECT_EQ(result, ITA_ERROR) << "传入文件与热图类型不匹配失败！" << endl;
}

//03.获取DLT热图Y16矩阵时传入文件路径错误
TEST_F(GetY16MatrixInfo, 03_PATH_ERR)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = "D:/APP/VS2022/project/Gtest/Gtest/testPictures/test.jpg";
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[width * height];

	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	EXPECT_EQ(result, ITA_ERROR) << "获取DLT热图Y16矩阵时传入文件路径错误失败！" << endl;
}

//04.获取DLT热图Y16矩阵时handle传入空指针
TEST_F(GetY16MatrixInfo, 04_HANDLE_NULL)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[width * height];

	result = ITA_GetY16MatrixInfo(NULL, file, heatMapType, &y16MaxtrixInfo);
	EXPECT_EQ(result, ITA_NULL_PTR_ERR) << "获取DLT热图Y16矩阵时handle传入空指针失败！" << endl;
}

// 05.获取DLT热图Y16矩阵时输出参数传入空指针
TEST_F(GetY16MatrixInfo, 05_OUTPUT_NULL)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[width * height];

	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, NULL);
	EXPECT_EQ(result, ITA_NULL_PTR_ERR) << "获取DLT热图Y16矩阵时输出参数传入空指针失败！" << endl;
}
