#include"common.h"

class TempMatrix2Rgb : public testing::Test
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
class TempMatrix2RgbParam : public ::testing::TestWithParam<pair<ITA_PALETTE_TYPE, string>>
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

INSTANTIATE_TEST_CASE_P(Colordata, TempMatrix2RgbParam,
	::testing::ValuesIn(GenerateColorData()));

// 将DLT热图中的温度矩阵转为26种伪彩RGB数据
TEST_P(TempMatrix2RgbParam, DLTMaxtrixToRGB)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	char file[] = DLT_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_DLT;
	ITA_TEMPMATRIX_INFO dltMatrixInfo;
	dltMatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &dltMatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = dltMatrixInfo.w;
	int h = dltMatrixInfo.h;
	int dltmatrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = GetParam().first;
	string colorname = GetParam().second;
	string filename = "DLT"+colorname + ".raw";
	unsigned char* rgbData = new unsigned char[dltmatrix_len];
	result = ITA_TempMatrix2Rgb(instance, &dltMatrixInfo, palette_index, rgbData, dltmatrix_len);
	ASSERT_EQ(result, ITA_OK) << "MaxtrixToRGB fail" << filename << endl;
	cout << "转RGB数据成功" << endl;
	saveRGBasRaw(filename.c_str(), rgbData, dltmatrix_len);
}

// 将FIR热图中的温度矩阵转为26种伪彩RGB数据
TEST_P(TempMatrix2RgbParam, FIRMaxtrixToRGB)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO dltMatrixInfo;
	dltMatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &dltMatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = dltMatrixInfo.w;
	int h = dltMatrixInfo.h;
	int dltmatrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = GetParam().first;
	string colorname = GetParam().second;
	string filename = "FIR" + colorname + ".raw";
	unsigned char* rgbData = new unsigned char[dltmatrix_len];
	result = ITA_TempMatrix2Rgb(instance, &dltMatrixInfo, palette_index, rgbData, dltmatrix_len);
	ASSERT_EQ(result, ITA_OK) << "MaxtrixToRGB fail" << filename << endl;
	cout << "转RGB数据成功" << endl;
	saveRGBasRaw(filename.c_str(), rgbData, dltmatrix_len);
}

// handle传入空指针
TEST_F(TempMatrix2Rgb, NULL1)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO dltMatrixInfo;
	dltMatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &dltMatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = dltMatrixInfo.w;
	int h = dltMatrixInfo.h;
	int dltmatrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_AMBER;
	unsigned char* rgbData = new unsigned char[dltmatrix_len];
	result = ITA_TempMatrix2Rgb(NULL, &dltMatrixInfo, palette_index, rgbData, dltmatrix_len);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL1 fail" << endl;
}

// 输入温度矩阵传入空指针
TEST_F(TempMatrix2Rgb, NULL2)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO dltMatrixInfo;
	dltMatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &dltMatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = dltMatrixInfo.w;
	int h = dltMatrixInfo.h;
	int dltmatrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_AMBER;
	unsigned char* rgbData = new unsigned char[dltmatrix_len];
	result = ITA_TempMatrix2Rgb(instance, NULL, palette_index, rgbData, dltmatrix_len);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL2 fail" << endl;
}

// 输出RGB数据传入空指针
TEST_F(TempMatrix2Rgb, NULL3)
{
	int width = WIDTH;
	int height = HEIGHT;
	ITA_RESULT result;
	HANDLE_T instance = ITA_HeatmapInit();
	char file[] = FIR_PICTUERE_PATH;
	ITA_HEATMAP_TYPE heatMapType = ITA_FIR;
	ITA_TEMPMATRIX_INFO dltMatrixInfo;
	dltMatrixInfo.tempMatrix = new float[width * height];

	result = ITA_GetTempMatrixInfo(instance, file, heatMapType, &dltMatrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetTempMatrixInfo fail" << endl;
	cout << "获取温度矩阵成功！" << endl;

	int w = dltMatrixInfo.w;
	int h = dltMatrixInfo.h;
	int dltmatrix_len = w * h * 3;
	ITA_PALETTE_TYPE palette_index = ITA_AMBER;
	unsigned char* rgbData = new unsigned char[dltmatrix_len];
	result = ITA_TempMatrix2Rgb(instance, &dltMatrixInfo, palette_index, NULL, dltmatrix_len);
	ASSERT_EQ(result, ITA_NULL_PTR_ERR) << "NULL3 fail" << endl;
}