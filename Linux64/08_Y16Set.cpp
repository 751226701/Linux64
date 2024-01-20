#include"common.h"

class Y16Set : public testing::Test
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

//01 DLT热图Y16数据转RGB，成像
TEST_F(Y16Set,1)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);
	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;
	
	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16转RGB自动调光.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//02 对DLT热图Y16成像、手动调光、设置温宽（最高温、最低温）
TEST_F(Y16Set, 2)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	ITA_DRT_TYPE type = ITA_DRT_MANUAL;
	ITADRTParam Param;
	Param.manltone_minY16 = 2000;
	Param.manltone_maxY16 = 5000;
	result = ITA_DimmingType(handle, type, &Param);
	ASSERT_EQ(result, ITA_OK) << "ITA_DimmingType fail" << endl;
	cout << "设置温宽成功！" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置温宽.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//03 DLT热图Y16数据转RGB，成像，设置亮度1
TEST_F(Y16Set, 3_bright_1)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	int bright = 1;
	result = ITA_ImageControl(handle, ITA_SET_BRIGHTNESS, &bright); //设置亮度
	ASSERT_EQ(result, ITA_OK) << "Set_bright_1 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置亮度1.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//04 DLT热图Y16数据转RGB，成像，设置亮度255
TEST_F(Y16Set, 4_bright_255)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	int bright = 255;
	result = ITA_ImageControl(handle, ITA_SET_BRIGHTNESS, &bright); //设置亮度
	ASSERT_EQ(result, ITA_OK) << "Set_bright_255 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置亮度255.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//05 DLT热图Y16数据转RGB，成像，设置对比度1
TEST_F(Y16Set, 5_contrast_1)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	int contrast = 1;
	result = ITA_ImageControl(handle, ITA_SET_CONTRAST, &contrast); //设置对比度
	ASSERT_EQ(result, ITA_OK) << "Set_contrast_1 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置对比度1.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//06 DLT热图Y16数据转RGB，成像，设置对比度255
TEST_F(Y16Set, 6_contrast_255)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	int contrast = 255;
	result = ITA_ImageControl(handle, ITA_SET_CONTRAST, &contrast); //设置对比度
	ASSERT_EQ(result, ITA_OK) << "Set_contrast_255 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置对比度255.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//07 DLT热图Y16数据转RGB，成像，设置锐度1
TEST_F(Y16Set, 7_sharp_1)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	bool sharpening = true;
	float  sharpNum = 1;

	result = ITA_ImageControl(handle, ITA_SHARPENING, &sharpening); //设置锐度开
	ASSERT_EQ(result, ITA_OK) << "Set_sharpening_on fail" << endl;

	result = ITA_ImageControl(handle, ITA_SET_LAPLACE_WEIGHT, &sharpNum); //设置锐值1
	ASSERT_EQ(result, ITA_OK) << "Set__sharp_1 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置锐度1.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}

//08 DLT热图Y16数据转RGB，成像，设置锐度10
TEST_F(Y16Set, 8_sharp_10)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	bool sharpening = true;
	float  sharpNum = 10;

	result = ITA_ImageControl(handle, ITA_SHARPENING, &sharpening); //设置锐度开
	ASSERT_EQ(result, ITA_OK) << "Set_sharpening_on fail" << endl;

	result = ITA_ImageControl(handle, ITA_SET_LAPLACE_WEIGHT, &sharpNum); //设置锐值10
	ASSERT_EQ(result, ITA_OK) << "Set__sharp_10 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置锐度10.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}


//09 DLT热图Y16数据转RGB，成像，设置细节增强1
TEST_F(Y16Set, 9_enhance_1)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	bool enhance = true;
	int enhanceNum = 1;

	result = ITA_ImageControl(handle, ITA_DETAIL_ENHANCE, &enhance); //设置细节增强开
	ASSERT_EQ(result, ITA_OK) << "Set_enhance_ON fail" << endl;

	result = ITA_ImageControl(handle, ITA_IIE_ENHANCE_COEF, &enhanceNum); //设置细节增强值
	ASSERT_EQ(result, ITA_OK) << "Set_enhance_1 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置细节增强1.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}


//10 DLT热图Y16数据转RGB，成像，设置细节增强128
TEST_F(Y16Set, 3)
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

	ITA_Y16MATRIX_INFO y16MaxtrixInfo;
	y16MaxtrixInfo.y16Matrix = new short[inputinfo.w * inputinfo.h];
	result = ITA_GetY16MatrixInfo(instance, file, heatMapType, &y16MaxtrixInfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GetY16MatrixInfo fail" << endl;
	cout << "获取Y16矩阵成功！" << endl;
	saveY16Matrix(&y16MaxtrixInfo);

	ITA_MODE mode = ITA_Y16;
	ITA_RANGE range = ITA_INDUSTRY_LOW;
	int cameraID = 1;
	HANDLE_T handle = ITA_Init(mode, range, width, height, cameraID);

	bool enhance = true;
	int enhanceNum = 128;

	result = ITA_ImageControl(handle, ITA_DETAIL_ENHANCE, &enhance); //设置细节增强开
	ASSERT_EQ(result, ITA_OK) << "Set_enhance_ON fail" << endl;

	result = ITA_ImageControl(handle, ITA_IIE_ENHANCE_COEF, &enhanceNum); //设置细节增强值
	ASSERT_EQ(result, ITA_OK) << "Set_enhance_128 fail" << endl;

	ITAISPResult resultinfo;
	int srcSize = width * height * sizeof(short);
	unsigned char* srcData = new unsigned char[srcSize];
	ConvertShortToUnsignedChar1(y16MaxtrixInfo.y16Matrix, srcData, height * width);
	result = ITA_ISP(handle, srcData, srcSize, &resultinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_ISP fail" << endl;

	cout << "图像处理成功！" << endl;
	ITAHeatMapOutput outputinfo;
	inputinfo.tempMatrix = MatrixInfo.tempMatrix;
	inputinfo.rgb = resultinfo.imgDst;
	result = ITA_GenerateHeapMapJpg(instance, &inputinfo, &outputinfo);
	ASSERT_EQ(result, ITA_OK) << "ITA_GenerateHeapMapJpg fail" << endl;
	saveJpgPicture("Y16调光设置细节增强128.jpg", outputinfo.jpgBuf, outputinfo.jpgBufLen);
}