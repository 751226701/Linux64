#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "SgpApi.h"
#include "SgpParam.h"
#include "gtest/gtest.h"
using namespace std;

class SgpTest : public testing::Test
{
public:
    SGP_HANDLE handle;

    void SetUp() override
    {
        handle = SGP_InitDevice();
        ASSERT_NE(handle, 0) << "SGP_InitDevice failed!";

        const char* server = "192.168.21.4";
        const char* username = "root";
        const char* password = "guide123";
        int port = 80;
        int retl = SGP_Login(handle, server, username, password, port);
        ASSERT_EQ(retl, SGP_OK) << "SGP_Login failed";
    }

    void TearDown() override
    {
        int retq = SGP_Logout(handle);
        EXPECT_EQ(retq, SGP_OK) << "SGP_Logout failed";

        SGP_UnInitDevice(handle);
    }
};

//一、SGP_SetTempPoints
//01 设置索引，type=0  
TEST_F(SgpTest, 01_type_0)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 0);
    EXPECT_EQ(ret, SGP_OK) << "01_type_0 fail" << endl;
    free(index);
    index = NULL;
}

//02 设置索引，type=1
TEST_F(SgpTest, 02_type_1)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "02_type_1 fail" << endl;
    free(index);
    index = NULL;
}

//03 设置第一行索引
TEST_F(SgpTest, 03_first_line)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "03_first_line fail" << endl;
    free(index);
    index = NULL;
}

//04 设置最后一行索引
TEST_F(SgpTest, 04_last_line)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i+327040;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "04_last_line fail" << endl;
    free(index);
    index = NULL;
}

// 05 设置第一列索引
TEST_F(SgpTest, 05_first_column)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i*640;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "05_first_column fail" << endl;
    free(index);
    index = NULL;
}

//06 设置最后一列索引
TEST_F(SgpTest, 06_last_column)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = 639+i*640;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "06_last_column fail" << endl;
    free(index);
    index = NULL;
}

//07 设置所有点索引
TEST_F(SgpTest, 07_all_point)
{
    const int pointNum = 640*512;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "07_all_point fail" << endl;
    free(index);
    index = NULL;
}

//08 设置的索引不存在(反)
//TEST_F(SgpTest, 08_err_index)
//{
//    const int pointNum = 1;
//    int* index = (int*)malloc(pointNum * sizeof(int));
//    for (int i = 0; i < pointNum; i++)
//    {
//        index[i] = -1;
//    }
//    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
//    EXPECT_EQ(ret, SGP_ERR) << "08 设置的索引不存在失败" << endl;
//    free(index);
//    index = NULL;
//}

//09 输入的索引长度与实际不一致(反)
//TEST_F(SgpTest, 09输入的索引长度与实际不一致)
//{
//    const int pointNum = 20;
//    int* index = (int*)malloc(pointNum * sizeof(int));
//    for (int i = 0; i < 21; i++)
//    {
//        index[i] = i;
//    }
//    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
//    EXPECT_EQ(ret, SGP_ERR) << "09 输入的索引长度与实际不一致失败" << endl;
//    free(index);
//    index = NULL;
//}

//10 设置不存在的type=3(反)
//TEST_F(SgpTest, 10设置不存在的type_3)
//{
//    const int pointNum = 20;
//    int* index = (int*)malloc(pointNum * sizeof(int));
//    for (int i = 0; i < pointNum; i++)
//    {
//        index[i] = i;
//    }
//    int ret = SGP_SetTempPoints(handle, index, pointNum, 3);
//    EXPECT_EQ(ret, SGP_ERR) << "10 设置不存在的type=3失败" << endl;
//    free(index);
//    index = NULL;
//}

//11 索引范围传入空指针（反）
TEST_F(SgpTest, 11_index_NULL)
{
    const int pointNum = 20;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(handle, NULL, pointNum, 1);
    EXPECT_EQ(ret, SGP_ERR) << "11_index_NULL fail" << endl;
    free(index);
    index = NULL;
}

//12 传入错误的handle(反)
TEST_F(SgpTest, 12_handle_NULL)
{
    const int pointNum = 20;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(NULL, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_ERR) << "12_handle_NULL fail" << endl;
    free(index);
    index = NULL;
}

//二、SGP_GetTempPoints
//13 设置索引，type=0，获取索引位置温度  
TEST_F(SgpTest, 13_type_0_get_temp)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 0);
    ASSERT_EQ(ret, SGP_OK) << "13_type_0 fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 0);
        ASSERT_EQ(ret, SGP_OK) << "13_type_0_get_temp fail" << endl;
        cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//14 设置索引，type=1，获取索引位置温度
TEST_F(SgpTest, 14_type_1_get_temp)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "14_type_1 fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "14_type_1_get_temp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//15 设置第一行索引，获取索引位置温度
TEST_F(SgpTest, 15_first_line_getTemp)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "15_first_line fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "15_first_line_getTemp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//16 设置最后一行索引，获取索引位置温度
TEST_F(SgpTest, 16_last_line_getTemp)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i + 327040;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "16_last_line fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "16_last_line_getTemp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

// 17 设置第一列索引，获取索引温位置度
TEST_F(SgpTest, 17_first_column_getTemp)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 640;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "17_first_column fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "17_first_column_getTemp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//18 设置最后一列索引，获取索引位置温度
TEST_F(SgpTest, 18_last_column_getTemp)
{
    const int pointNum = 640;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = 639 + i * 640;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "18_last_column fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "18_last_column_getTemp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//19 设置所有点索引，获取温度
TEST_F(SgpTest, 19_all_points_getTemp)
{
    const int pointNum = 640 * 512;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "19_all_points fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_OK) << "19_all_points_getTemp fail" << endl;
        /*cout << "获取温度成功！" << endl;
        for (int i = 0; i < pointNum; i++)
        {
            cout << temp[i] << endl;
        }*/
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//20 设置输出参数为空指针output=NULL(反)
TEST_F(SgpTest, 20_output_NULL)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "20_type_1 fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, NULL, pointNum, 1);
        ASSERT_EQ(ret, SGP_ERR) << "20_output_NULL fail" << endl;
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//21 索引长度与分配的不一致
TEST_F(SgpTest, 21)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "21 设置索引，type=1失败" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, 101, 1);
        ASSERT_EQ(ret, SGP_ERR) << "21 索引长度与分配的不一致失败" << endl;
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//22 温度矩阵类型与分配不一致
//TEST_F(SgpTest, 22温度矩阵类型与分配不一致)
//{
//    const int pointNum = 100;
//    int* index = (int*)malloc(pointNum * sizeof(int));
//    for (int i = 0; i < pointNum; i++)
//    {
//        index[i] = i * 100;
//    }
//    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
//    ASSERT_EQ(ret, SGP_OK) << "22 设置索引，type=1失败" << endl;
//
//    float* temp = (float*)malloc(pointNum * sizeof(float));
//    memset(temp, 0, pointNum * sizeof(float));
//    if (temp != NULL)
//    {
//        int ret = SGP_GetTempPoints(handle, temp, pointNum, 0);
//        ASSERT_EQ(ret, SGP_ERR) << "22 温度矩阵类型与分配不一致" << endl;
//    }
//    free(temp);
//    temp = NULL;
//    free(index);
//    index = NULL;
//}

//23 传入错误的handle
TEST_F(SgpTest, 23_handle_NULL)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "23_type_1 fail" << endl;

    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, pointNum * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(NULL, temp, pointNum, 1);
        ASSERT_EQ(ret, SGP_ERR) << "23_handle_NULL fail" << endl;
    }
    free(temp);
    temp = NULL;
    free(index);
    index = NULL;
}

//三、SGP_GetMatrixTempPoints
//24 获取全图范围内温度矩阵
TEST_F(SgpTest, 24_getMaxtrix_all)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 640;
    rect.h = 512;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(handle, output, length, rect);
    EXPECT_EQ(ret, SGP_OK) << "24_getMaxtrix_all fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//25 获取指定范围内温度矩阵
TEST_F(SgpTest, 25_getMaxtrix_all_region)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 100;
    rect.h = 100;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(handle, output, length, rect);
    EXPECT_EQ(ret, SGP_OK) << "25_getMaxtrix_all_region fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//26 传入错误的handle
//TEST_F(SgpTest, 26传入错误的handle)
//{
//    float* output = new float[640 * 512];
//    SGP_RECT rect;
//    rect.x = 0;
//    rect.y = 0;
//    rect.w = 100;
//    rect.h = 100;
//    const int  length = rect.w * rect.h;
//    int ret = SGP_GetMatrixTempPoints(123, output, length, rect);
//    EXPECT_EQ(ret, SGP_OK) << "26 传入错误的handle失败" << endl;
//    /*cout << "获取成功" << endl;
//    for (int i = 0; i < length; i++)
//    {
//        cout << output[i] << endl;
//    }*/
//    delete[] output;
//    output = nullptr;
//}

//27 handle传入NULL
TEST_F(SgpTest, 27_handle_NULL)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 100;
    rect.h = 100;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(NULL, output, length, rect);
    EXPECT_EQ(ret, SGP_ERR) << "27_handle_NULL fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//28 输出参数output=NULL
TEST_F(SgpTest, 28_output_NULL)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 100;
    rect.h = 100;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(handle, NULL, length, rect);
    EXPECT_EQ(ret, SGP_ERR) << "28_output_NULL fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//29 X+W超范围
TEST_F(SgpTest, 29_XW_overRange)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 541;
    rect.y = 0;
    rect.w = 100;
    rect.h = 100;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(handle, output, length, rect);
    EXPECT_EQ(ret, SGP_ERR_10002) << "29_XW_overRange fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//30 Y+H超范围
TEST_F(SgpTest, 30_YH_overRange)
{
    float* output = new float[640 * 512];
    SGP_RECT rect;
    rect.x = 0;
    rect.y = 413;
    rect.w = 100;
    rect.h = 100;
    const int  length = rect.w * rect.h;
    int ret = SGP_GetMatrixTempPoints(handle, output, length, rect);
    EXPECT_EQ(ret, SGP_ERR_10002) << "30_YH_overRange fail" << endl;
    /*cout << "获取成功" << endl;
    for (int i = 0; i < length; i++)
    {
        cout << output[i] << endl;
    }*/
    delete[] output;
    output = nullptr;
}

//31 获取全局、分析对象实时温度
TEST_F(SgpTest, 31_getAllTemp)
{
    SGP_ANALYTIC_TEMPS array;
    memset(&array, 0x00, sizeof(array));
    int ret = SGP_GetAnalyticObjectsTemp(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "31_getAllTemp fail" << endl;
    cout << "获取成功！" << endl;
    cout << "全局最高温:" << array.global_max_temp << endl;
    cout << "全局最低温:" << array.global_min_temp << endl;
    cout << "全局平均温:" << array.global_avg_temp << endl;
    cout << "分析对象个数:" << array.analytic_num << endl;
    cout << endl;
    if (array.analytic_num > 0)
    {
        for (int i = 0; i < array.analytic_num; i++)
        {
            cout << "第" << i + 1 << "个分析对象：" << endl;
            cout << "分析对象ID：" << array.analytic[i].rule_id << endl;
            cout << "分析对象名称：" << array.analytic[i].rule_name << endl;
            cout << "分析对象类型：" << array.analytic[i].type << endl;
            cout << "最高温:" << array.analytic[i].max_temp << endl;
            cout << "最低温:" << array.analytic[i].min_temp << endl;
            cout << "平均温:" << array.analytic[i].avg_temp << endl;
            cout << endl;
        }
    }
    else { cout << "无分析对象" << endl; }
}

//32 获取校温信息
TEST_F(SgpTest, 32_getMeasureTemp)
{
    SGP_MEASURE_TEMP_INFO output;
    int ret = SGP_GetMeasureTempInfo(handle, output);
    ASSERT_EQ(ret, SGP_OK) << "32_getMeasureTemp fail" << endl;
    cout << "获取成功！" << endl;
    cout << "实时快门温：" << output.realshuttertemp << endl;
    cout << "上次快门温：" << output.lastshuttertemp << endl;
    cout << "实时快门温：" << output.realmirrortemp << endl;
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "SgpTest.19_all_points_getTemp:SgpTest.21";
    return RUN_ALL_TESTS();
}