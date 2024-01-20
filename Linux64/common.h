#pragma once
#include "SDK_TEST.h"

#define WIDTH 640
#define HEIGHT 512

#define DLT_PICTUERE_PATH "/Test/testPicture/test.jpg"
#define FIR_PICTUERE_PATH "/Test/testPicture/test.fir"


//保存温度矩阵
void saveTempMatrix(ITATempMatrixInfo* tempMatrixInfo);

//获取温度矩阵最高温、最低温、平均温
void getTempMatrixMaxMinTemp(ITATempMatrixInfo* tempMatrixInfo);

//保存Y16矩阵
void saveY16Matrix(ITAY16MatrixInfo* y16Info);

//保存RGB数据
void saveRGBasRaw(const char* filename, unsigned char* rgbData, int dataSize);

//保存色带条数据
void saveColorStripData(const char* filename, unsigned char* colorStripData, int len);

//保存JPG照片
void saveJpgPicture(const char* filename, unsigned char* jpgBuf, int jpgBufLen);

// 将 short 类型数据复制到 unsigned char 类型的内存中
void ConvertShortToUnsignedChar2(const short* srcData, unsigned char* dstData, int height, int width);
void ConvertShortToUnsignedChar1(const short* srcData, unsigned char* dstData, int length);

vector<pair<ITA_PALETTE_TYPE, string>> GenerateColorData();