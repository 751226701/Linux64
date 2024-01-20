#include"common.h"

class GenerateHeapMapJpg : public testing::Test
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
class GenerateHeapMapJpgParam : public ::testing::TestWithParam<pair<ITA_PALETTE_TYPE, string>>
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

INSTANTIATE_TEST_CASE_P(Colordata, GenerateHeapMapJpgParam,
	::testing::ValuesIn(GenerateColorData()));

//DLT热图温度矩阵生成26种伪彩照片
TEST_P(GenerateHeapMapJpgParam, DLTMaxtrixToPicture)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	ITAHeatMapInputInfo inputinfo;
	inputinfo.w = width;
	inputinfo.h = height;
	inputinfo.tempMatrix = new float[width * height];
	inputinfo.tempMatrixLen = width * height * sizeof(float);
	inputinfo.rgb = new unsigned char[width * height * 3];
	inputinfo.rgbLen = width * height * 3;

	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = MatrixInfo.w;
	int h = MatrixInfo.h;
	int matrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = GetParam().first;
	string colorname = GetParam().second;
	string filename = "DLT" + colorname + ".jpg";
	unsigned char* rgbData = new unsigned char[matrix_len];
	result = ITA_TempMatrix2Rgb(instance, &MatrixInfo, palette_index, rgbData, matrix_len);
	ASSERT_EQ(result, ITA_OK) << "ITA_TempMatrix2Rgb fail" << endl;
	
	cout << "转RGB数据成功" << endl;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = rgbData;
	ITAHeatMapOutput outPutInfo;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outPutInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture(filename.c_str(), outPutInfo.jpgBuf, outPutInfo.jpgBufLen);
}

//FIR热图温度矩阵生成26种伪彩照片
TEST_P(GenerateHeapMapJpgParam, FIRMaxtrixToPicture)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	ITAHeatMapInputInfo inputinfo;
	inputinfo.w = width;
	inputinfo.h = height;
	inputinfo.tempMatrix = new float[width * height];
	inputinfo.tempMatrixLen = width * height * sizeof(float);
	inputinfo.rgb = new unsigned char[width * height * 3];
	inputinfo.rgbLen = width * height * 3;

	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = MatrixInfo.w;
	int h = MatrixInfo.h;
	int matrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = GetParam().first;
	string colorname = GetParam().second;
	string filename = "FIR" + colorname + ".jpg";
	unsigned char* rgbData = new unsigned char[matrix_len];
	result = ITA_TempMatrix2Rgb(instance, &MatrixInfo, palette_index, rgbData, matrix_len);
	ASSERT_EQ(result, ITA_OK) << "ITA_TempMatrix2Rgb fail" << endl;

	cout << "转RGB数据成功" << endl;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = rgbData;
	ITAHeatMapOutput outPutInfo;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outPutInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture(filename.c_str(), outPutInfo.jpgBuf, outPutInfo.jpgBufLen);
}

//NULL1
TEST_F(GenerateHeapMapJpg, NULL1)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	ITAHeatMapInputInfo inputinfo;
	inputinfo.w = width;
	inputinfo.h = height;
	inputinfo.tempMatrix = new float[width * height];
	inputinfo.tempMatrixLen = width * height * sizeof(float);
	inputinfo.rgb = new unsigned char[width * height * 3];
	inputinfo.rgbLen = width * height * 3;

	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = MatrixInfo.w;
	int h = MatrixInfo.h;
	int matrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_WHITE_HEAT;
	unsigned char* rgbData = new unsigned char[matrix_len];
	result = ITA_TempMatrix2Rgb(instance, &MatrixInfo, palette_index, rgbData, matrix_len);
	ASSERT_EQ(result, ITA_OK) << "ITA_TempMatrix2Rgb fail" << endl;

	cout << "转RGB数据成功" << endl;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = rgbData;
	ITAHeatMapOutput outPutInfo;
	result = ITA_GenerateHeapMapJpg(NULL, &inputinfo, &outPutInfo);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL1 fail" << endl;
}

//NULL2
TEST_F(GenerateHeapMapJpg, NULL2)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	ITAHeatMapInputInfo inputinfo;
	inputinfo.w = width;
	inputinfo.h = height;
	inputinfo.tempMatrix = new float[width * height];
	inputinfo.tempMatrixLen = width * height * sizeof(float);
	inputinfo.rgb = new unsigned char[width * height * 3];
	inputinfo.rgbLen = width * height * 3;

	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = MatrixInfo.w;
	int h = MatrixInfo.h;
	int matrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_WHITE_HEAT;
	unsigned char* rgbData = new unsigned char[matrix_len];
	result = ITA_TempMatrix2Rgb(instance, &MatrixInfo, palette_index, rgbData, matrix_len);
	ASSERT_EQ(result, ITA_OK) << "ITA_TempMatrix2Rgb fail" << endl;

	cout << "转RGB数据成功" << endl;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = rgbData;
	ITAHeatMapOutput outPutInfo;
	result = ITA_GenerateHeapMapJpg(instance, NULL, &outPutInfo);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL2 fail" << endl;
}

//NULL3
TEST_F(GenerateHeapMapJpg, FIRMaxtrixToPicture)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	ITAHeatMapInputInfo inputinfo;
	inputinfo.w = width;
	inputinfo.h = height;
	inputinfo.tempMatrix = new float[width * height];
	inputinfo.tempMatrixLen = width * height * sizeof(float);
	inputinfo.rgb = new unsigned char[width * height * 3];
	inputinfo.rgbLen = width * height * 3;

	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO MatrixInfo;
	MatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &MatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = MatrixInfo.w;
	int h = MatrixInfo.h;
	int matrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_WHITE_HEAT;
	unsigned char* rgbData = new unsigned char[matrix_len];
	result = ITA_TempMatrix2Rgb(instance, &MatrixInfo, palette_index, rgbData, matrix_len);
	ASSERT_EQ(result, ITA_OK) << "ITA_TempMatrix2Rgb fail" << endl;

	cout << "转RGB数据成功" << endl;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = rgbData;
	ITAHeatMapOutput outPutInfo;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, NULL);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL3 fail" << endl;
}
