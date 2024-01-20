#include"common.h"

class GetTempMatrixInfo : public testing::Test
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

//01.从ITA_DLT热图获取温度矩阵
TEST_F(GetTempMatrixInfo, 01_DLT)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "从DLT热图获取温度矩阵失败！" << endl;
	cout << "从DLT热图获取温度矩阵！" << endl;
	saveTempMatrix(&MatrixInfo);
	getTempMatrixMaxMinTemp(&MatrixInfo);
	EXPECT_EQ(MatrixInfo.w, WIDTH) << "获取的宽错误！" << endl;
	EXPECT_EQ(MatrixInfo.h, HEIGHT) << "获取的高错误！" << endl;
}

//02.从ITA_FIR热图获取温度矩阵
TEST_F(GetTempMatrixInfo, 02_FIR)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "从FIR热图获取温度矩阵失败！" << endl;
	cout << "从FIR热图获取温度矩阵成功！" << endl;
	saveTempMatrix(&MatrixInfo);
	getTempMatrixMaxMinTemp(&MatrixInfo);
	EXPECT_EQ(MatrixInfo.w, WIDTH) << "获取的宽错误！" << endl;
	EXPECT_EQ(MatrixInfo.h, HEIGHT) << "获取的高错误！" << endl;
}

//03.传入文件与热图类型不匹配1
TEST_F(GetTempMatrixInfo, 03_TYPE_ERR1)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	EXPECT_EQ(result, ITA_ARG_OUT_OF_RANGE) << "传入文件与热图类型不匹配1失败！" << endl;
}

//04.传入文件与热图类型不匹配2
TEST_F(GetTempMatrixInfo, 04_TYPE_ERR2)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	EXPECT_EQ(result, ITA_ARG_OUT_OF_RANGE) << "传入文件与热图类型不匹配2失败！" << endl;
}

//05.获取DLT热图温度矩阵时传入文件路径错误
TEST_F(GetTempMatrixInfo, 05_PATHE_NONE)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = "D:/APP/VS2022/project/Gtest/Gtest/testPictures/test.jpg";
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	EXPECT_EQ(result, ITA_FILE_NOT_EXIST) << "获取DLT热图温度矩阵时传入文件路径错误失败！" << endl;
}

//06.获取DLT热图温度矩阵时handle传入空指针
TEST_F(GetTempMatrixInfo, 06_HANDLE_NULL)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(NULL, file, heatMapType, &MatrixInfo);
	EXPECT_EQ(result, ITA_NULL_PTR_ERR) << "获取DLT热图温度矩阵时handle传入空指针失败！" << endl;
}

//07.获取DLT热图温度矩阵时输出参数传入空指针
TEST_F(GetTempMatrixInfo, 07_OUTPUT_NULL)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, NULL);
	EXPECT_EQ(result, ITA_NULL_PTR_ERR) << "获取DLT热图温度矩阵时输出参数传入空指针失败！" << endl;
}


