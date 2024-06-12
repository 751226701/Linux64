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

    static void SetUpTestCase()
    {
        cout << "Start Test" << endl;
    }
    static void TearDownTestCase()
    {
        cout << "Test over" << endl;
    }

    void SetUp() override
    {
        handle = SGP_InitDevice();
        ASSERT_NE(handle, 0) << "SGP_InitDevice failed!" << endl;
        const char* server = "192.168.21.160";
        const char* username = "root";
        const char* password = "guide123";
        int port = 80;
        int retl = SGP_Login(handle, server, username, password, port);
        ASSERT_EQ(retl, SGP_OK) << "SGP_Login failed" << endl;
    }

    void TearDown() override
    {
        int retq = SGP_Logout(handle);
        EXPECT_EQ(retq, SGP_OK) << "SGP_Logout failed" << endl;
        SGP_UnInitDevice(handle);
    }
};

//class SgpTest : public testing::Test
//{
//public:
//    static SGP_HANDLE handle;
//
//    static void SetUpTestCase()
//    {
//        handle = SGP_InitDevice();
//        ASSERT_NE(handle, 0) << "SGP_InitDevice failed!" << endl;
//
//        const char* server = "192.168.21.31";
//        const char* username = "root";
//        const char* password = "guide123";
//        int port = 80;
//        int retl = SGP_Login(handle, server, username, password, port);
//        ASSERT_EQ(retl, SGP_OK) << "SGP_Login failed" << endl;
//    }
//
//    static void TearDownTestCase()
//    {
//        int retq = SGP_Logout(handle);
//        EXPECT_EQ(retq, SGP_OK) << "SGP_Logout failed" << endl;
//
//        SGP_UnInitDevice(handle);
//    }
//
//    void SetUp() override
//    {
//        sleep(1);
//    }
//
//    void TearDown() override
//    {
//        
//    }
//};
//SGP_HANDLE SgpTest::handle;


//01.修改密码
TEST_F(SgpTest, 001_ChangePassword)
{
    const char* username = "admin";
    const char* oldpassword = "admin123";
    const char* newpassword = "admin567";
    int ret = SGP_ChangePassword(handle, username, oldpassword, newpassword);
    EXPECT_EQ(ret, SGP_OK) << "SGP_ChangePassword failed" << endl;
}

//02.重置密码
TEST_F(SgpTest, 002_ResetPassword)
{
    const char* usernames = "admin";
    int ret = SGP_ResetPassword(handle, usernames);
    EXPECT_EQ(ret, SGP_OK) << "SGP_ResetPassword failed" << endl;
}

//03.获取用户信息
TEST_F(SgpTest, 003_GetGeneralInfo)
{
    SGP_GENERAL_INFO info;
    memset(&info, 0, sizeof(info));
    int ret = SGP_GetGeneralInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetGeneralInfo failed" << endl;
    cout << "获取的通用信息为：" << endl;
    cout << "系统时间为：" << info.datetime << endl;
    cout << "红外模组的宽为：" << info.ir_model_w << endl;
    cout << "红外模组的高为：" << info.ir_model_h << endl;
    cout << "红外通道输出宽为：" << info.ir_output_w << endl;
    cout << "红外通道输出高为：" << info.ir_output_h << endl;
    cout << "红外主码流地址为：" << info.ir_rtsp_url << endl;
    cout << "测温范围数量为：" << info.range_num << endl;
}

//04.获取单点温度
TEST_F(SgpTest, 004_GetPointTemp)
{
    int x = 1;
    int y = 1;
    float output = 0.0f;
    int ret = SGP_GetPointTemp(handle, x, y, &output);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetPointTemp failed xy" << endl;
    cout <<  "获取的第一个点温度为：" << output << endl;

    int x1 = 250;
    int y1 = 250;
    float output1 = 0.0f;
    int ret1 = SGP_GetPointTemp(handle, x1, y1, &output);
    ASSERT_EQ(ret1, SGP_OK) << "SGP_GetPointTemp failed x1y1" << endl << endl;
    cout << "获取的最后一个点温度为：" << output << endl;
}

//05.获取分析对象温度
TEST_F(SgpTest, 005_GetAnalyticObjectsTemp)
{
    SGP_ANALYTIC_TEMPS array;
    memset(&array, 0x00, sizeof(array));
    int ret = SGP_GetAnalyticObjectsTemp(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetAnalyticObjectsTemp failed" << endl;
    if (array.analytic_num > 0)
    {
        for (int i = 0; i < array.analytic_num; i++)
        {
            cout << "获取分析对象温度成功,获取分析对象信息如下：" << endl;
            cout << "分析对象个数为：" << array.analytic_num << endl;
            cout << "分析对象的规则ID为：" << array.analytic[i].rule_id << endl;
            cout << "分析对象的规则名称为：" << array.analytic[i].rule_name << endl;
            cout << "分析对象的类型为：" << array.analytic[i].type << endl;
            cout << "分析对象的最高温度为：" << array.analytic[i].max_temp << endl;
            cout << "分析对象的平均温度为：" << array.analytic[i].avg_temp << endl;
            cout << "分析对象的最低温度为：" << array.analytic[i].min_temp << endl;
            cout << endl;
        }
    }
    else { cout << "分析对象个数为：0" << endl; };
}

//06.获取温度矩阵
TEST_F(SgpTest, 006_GetImageTemps)
{
    SGP_GENERAL_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetGeneralInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetGeneralInfo failed" << endl;
    
    int height = info.ir_model_h;
    int width = info.ir_model_w;
    int length = height * width;
    int type = 1;
    float* output = (float*)calloc(length, sizeof(float));
    if (output != NULL)
    {
        ret = SGP_GetImageTemps(handle, output, length * 4, type);
    }
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetImageTemps failed" << endl;
    float max = 0, min = 100, sum = 0;
    for (int i = 0; i < length; i++)
    {
        if (max < output[i])
        {
            max = output[i];
        }
        if (min > output[i])
        {
            min = output[i];
        }
        sum += output[i];
    }

    cout << "获取的最高温为：" <<  max << endl;
    cout << "获取的最低温为：" <<  min << endl;
    cout << "获取的平均温为：" <<  sum / length << endl;
    free(output);
    output = NULL;
}

//07.获取屏幕截图
TEST_F(SgpTest, 007_GetScreenCapture)
{
    SGP_IMAGE_TYPE type = SGP_IR_IMAGE;
    char path[] = "./screencap.jpg";
    int ret = SGP_GetScreenCapture(handle, type, path);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetScreenCapture failed" << endl;
}

//08.获取热图
TEST_F(SgpTest, 008_GetHeatMap)
{
    const char* path = "./screenpic.jpg";
    int ret = SGP_GetHeatMap(handle, path);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetHeatMap failed" << endl;
}

//09.获取高压热图
TEST_F(SgpTest, 009_GetFirHeatMap)
{
    const char* path = "./screenpic.fir";
    int ret = SGP_GetFirHeatMap(handle, path);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetFirHeatMap failed" << endl;
}

//10.获取屏幕截图缓存
TEST_F(SgpTest, 010_GetScreenCaptureCache)
{
    int input_length = 1024 * 1024 * 10;
    int output_length = 0;
    SGP_IMAGE_TYPE type = SGP_IR_IMAGE;
    char* input = (char*)calloc(input_length, sizeof(char));
    if (input != NULL)
    {
        int ret = SGP_GetScreenCaptureCache(handle, type, input, input_length,&output_length);
        ASSERT_EQ(ret, SGP_OK) << "SGP_GetScreenCaptureCache failed" << endl;
        ofstream outputFile("screenPicCache.bin", std::ios::binary);
        if (outputFile.is_open()) 
        {
            outputFile.write(input, output_length);
            outputFile.close();
            cout << "获取屏幕截缓存图成功，并保存为 screenshot.bin" << endl;
        }
        else {cout << "无法打开文件以保存截图数据" << endl;}
    }
    free(input);
    input = NULL;
}

//11.获取热图缓存
TEST_F(SgpTest, 011_GetHeatMapCache)
{
    int input_length = 1024 * 1024 * 10;
    int output_length = 0;
    char* input = (char*)calloc(input_length, sizeof(char));
    if (input != NULL)
    {
        int ret = SGP_GetHeatMapCache(handle, input, input_length, &output_length);
        ASSERT_EQ(ret, SGP_OK) << "SGP_GetHeatMapCache failed" << endl;
        ofstream outputFile("heatMapCache.bin", std::ios::binary);
        if (outputFile.is_open()) 
        {
            outputFile.write(input, output_length);
            outputFile.close();
            cout << "获取热图缓存成功，并保存为 heatMapCache.bin" << endl;
        }
        else {cout << "无法打开文件以保存热图数据" << endl;}
    }
    free(input);
    input = NULL;
}

//12.获取高压热图缓存
TEST_F(SgpTest, 012_GetFirHeatMapCache)
{
    int input_length = 1024 * 1024 * 10;
    int output_length = 0;
    char* input = (char*)calloc(input_length, sizeof(char));
    if (input != NULL)
    {
        int ret = SGP_GetFirHeatMapCache(handle, input, input_length, &output_length);
        ASSERT_EQ(ret, SGP_OK) << "SGP_GetFirHeatMapCache failed" << endl;
        ofstream outputFile("firHeatMapCache.bin", std::ios::binary);
        if (outputFile.is_open()) 
        {
            outputFile.write(input, output_length);
            outputFile.close();
            cout << "获取高压热图缓存成功，并保存为 firHeatMapCache.bin" << endl;
        }
        else {cout << "无法打开文件以保存高压热图数据" << endl;}
    }
    free(input);
    input = NULL;
}

//13.开始录制
TEST_F(SgpTest, 013_Record)
{
    int record = 1;
    int record_stream = 2;
    int ret = SGP_Record(handle, record, record_stream);
    EXPECT_EQ(ret, SGP_OK) << "SGP_Record failed" << endl;
}

//14.停止录制
TEST_F(SgpTest, 014_StopRecord)
{
    int record = 2;
    int record_stream = 2;
    int ret = SGP_Record(handle, record, record_stream);
    EXPECT_EQ(ret, SGP_OK) << "SGP_StopRecord failed" << endl;
}

//15.设置全局温度参数
TEST_F(SgpTest, 015_SetThermometryParam)
{
    SGP_THERMOMETRY_PARAM info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetThermometryParam(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryParam failed" << endl;
    
    info.color_bar = 12;  //色带
    info.color_show = 1;  //色带显示
    info.flag = 1;        //测温开关
    info.mod_temp = 0;    //温度修正
    info.ambient = 22;    //环境温度
    info.atmo_trans = 0.2;//大气透过率
    info.dist = 10;       //距离
    info.emiss = 0.5;     //发射率
    info.emiss_mode = 1;  //发射率类型
    info.gear = 0;        //测温范围
    info.humi = 66;       //温度范围
    info.ref_temp = 23;   //反射温度
    strcpy(info.show_desc, "123456"); //显示字符串
    info.show_mode = 7;   //温度显示类型
    info.show_string = 5; //字符串显示位置
    info.opti_trans = 0.2;//光学透过率
    info.isot_flag = 1;   //等温线开关
    info.isot_high = 20;  //等温线高温阈值
    info.isot_low = 20;   //等温线低温阈值
    info.isot_type = 2;   //等温线类型
    strcpy(info.isot_high_color, "#00ff00");   //等温线高温颜色
    strcpy(info.isot_low_color, "#00ff00");    //等温线低温颜色
    ret = SGP_SetThermometryParam(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryParam failed" << endl;

    SGP_THERMOMETRY_PARAM getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetThermometryParam(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryParam failed" << endl;
    EXPECT_EQ(getInfo.color_bar, info.color_bar) << "assert color_bar failed" << endl;
    EXPECT_EQ(getInfo.color_show, info.color_show) << "assert color_show failed" << endl;
    EXPECT_EQ(getInfo.flag, info.flag) << "assert flag failed" << endl;
    EXPECT_EQ(getInfo.mod_temp, info.mod_temp) << "assert mod_temp failed" << endl;
    EXPECT_EQ(getInfo.ambient, info.ambient) << "assert ambient failed" << endl;
    EXPECT_EQ(getInfo.atmo_trans, info.atmo_trans) << "assert atmo_trans failed" << endl;
    EXPECT_EQ(getInfo.dist, info.dist) << "assert dist failed" << endl;
    EXPECT_EQ(getInfo.emiss, info.emiss) << "assert emiss failed" << endl;
    EXPECT_EQ(getInfo.emiss_mode, info.emiss_mode) << "assert emiss_mode failed" << endl;
    EXPECT_EQ(getInfo.gear, info.gear) << "assert gear failed" << endl;
    EXPECT_EQ(getInfo.humi, info.humi) << "assert humi failed" << endl;
    EXPECT_EQ(getInfo.ref_temp, info.ref_temp) << "assert ref_temp failed" << endl;
    EXPECT_STREQ(getInfo.show_desc, info.show_desc) << "assert show_desc failed" << endl;
    EXPECT_EQ(getInfo.show_mode, info.show_mode) << "assert show_mode failed" << endl;
    EXPECT_EQ(getInfo.show_string, info.show_string) << "assert show_string failed" << endl;
    EXPECT_EQ(getInfo.opti_trans, info.opti_trans) << "assert opti_trans failed" << endl;
    EXPECT_EQ(getInfo.isot_flag, info.isot_flag) << "assert isot_flag failed" << endl;
    EXPECT_EQ(getInfo.isot_high, info.isot_high) << "assert isot_high failed" << endl;
    EXPECT_EQ(getInfo.isot_low, info.isot_low) << "assert isot_low failed" << endl;
    EXPECT_EQ(getInfo.isot_type, info.isot_type) << "assert isot_type failed" << endl;
    EXPECT_STREQ(getInfo.isot_high_color, info.isot_high_color) << "assert isot_high_color failed" << endl;
    EXPECT_STREQ(getInfo.isot_high_color, info.isot_high_color) << "assert isot_high_color failed" << endl;
}

//16.获取全局温度参数
TEST_F(SgpTest, 016_GetThermometryParam)
{
    SGP_THERMOMETRY_PARAM info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetThermometryParam(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryParam failed" << endl;
    cout << "返回成功,全局测温参数信息如下：" << endl;
    cout << "环境温度为：  " << info.ambient << endl;
    cout << "大气透过率为：" << info.atmo_trans << endl;
    cout << "色带号为：    " << info.color_bar << endl;
    cout << "距离为：      " << info.dist << endl;
    cout << "发射率为：    " << info.emiss << endl;
    cout << "发射率类型为：" << info.emiss_mode << endl;
    cout << "湿度为：      " << info.humi << endl;
    cout << "光学透过率为：" << info.opti_trans << endl;
    cout << "字符串显示为：" << info.show_desc << endl;
}

//17.设置全局测温开关-关
TEST_F(SgpTest, 017_SetThermometryFlag_OFF)
{
    int flag = 0;
    int ret = SGP_SetThermometryFlag(handle, flag);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryFlag failed" << endl;
}

//18.设置全局测温开关-开
TEST_F(SgpTest, 018_SetThermometryFlag_ON)
{
    int flag = 1;
    int ret = SGP_SetThermometryFlag(handle, flag);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryFlag failed" << endl;
}

//19.设置色带号1
TEST_F(SgpTest, 019_SetColorBar_1)
{
    int colorbar = 1;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar1 failed" << endl;
}

//20.设置色带号2
TEST_F(SgpTest, 020_SetColorBar_2)
{
    int colorbar = 2;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar2 failed" << endl;
}

//21.设置色带号3
TEST_F(SgpTest, 021_SetColorBar_3)
{
    int colorbar = 3;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar3 failed" << endl;
}

//22.设置色带号4
TEST_F(SgpTest, 022_SetColorBar_4)
{
    int colorbar = 4;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar4 failed" << endl;
}

//23.设置色带号5
TEST_F(SgpTest, 023_SetColorBar_5)
{
    int colorbar = 5;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar5 failed" << endl;
}

//24.设置色带号6
TEST_F(SgpTest, 024_SetColorBar_6)
{
    int colorbar = 6;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar6 failed" << endl;
}

//25.设置色带号7
TEST_F(SgpTest, 025_SetColorBar_7)
{
    int colorbar = 7;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar7 failed" << endl;
}

//26.设置色带号8
TEST_F(SgpTest, 026_SetColorBar_8)
{
    int colorbar = 8;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar8 failed" << endl;
}

//27.设置色带号9
TEST_F(SgpTest, 027_SetColorBar_9)
{
    int colorbar = 9;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar9 failed" << endl;
}

//28.设置色带号10
TEST_F(SgpTest, 028_SetColorBar_10)
{
    int colorbar = 10;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar10 failed" << endl;
}

//29.设置色带号11
TEST_F(SgpTest, 029_SetColorBar_11)
{
    int colorbar = 11;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar11 failed" << endl;
}

//30.设置色带号12
TEST_F(SgpTest, 030_SetColorBar_12)
{
    int colorbar = 12;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar12 failed" << endl;
}

//31.设置色带号13
TEST_F(SgpTest, 031_SetColorBar_13)
{
    int colorbar = 13;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar13 failed" << endl;
}

//32.设置色带号14
TEST_F(SgpTest, 032_SetColorBar_14)
{
    int colorbar = 14;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar14 failed" << endl;
}

//33.设置色带号15
TEST_F(SgpTest, 033_SetColorBar_15)
{
    int colorbar = 15;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar15 failed" << endl;
}

//34.设置色带号16
TEST_F(SgpTest, 034_SetColorBar_16)
{
    int colorbar = 16;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar16 failed" << endl;
}

//35.设置色带号17
TEST_F(SgpTest, 035_SetColorBar_17)
{
    int colorbar = 17;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar17 failed" << endl;
}

//36.设置色带号18
TEST_F(SgpTest, 036_SetColorBar_18)
{
    int colorbar = 18;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar18 failed" << endl;
}

//37.设置色带号19
TEST_F(SgpTest, 037_SetColorBar_19)
{
    int colorbar = 19;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar19 failed" << endl;
}

//38.设置色带号20
TEST_F(SgpTest, 038_SetColorBar_20)
{
    int colorbar = 20;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar20 failed" << endl;
}

//39.设置色带号21
TEST_F(SgpTest, 039_SetColorBar_21)
{
    int colorbar = 21;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar21 failed" << endl;
}

//40.设置色带号22
TEST_F(SgpTest, 040_SetColorBar_22)
{
    int colorbar = 22;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar22 failed" << endl;
}

//41.设置色带号23
TEST_F(SgpTest, 041_SetColorBar_23)
{
    int colorbar = 23;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar23 failed" << endl;
}

//42.设置色带号24
TEST_F(SgpTest, 042_SetColorBar_24)
{
    int colorbar = 24;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar24 failed" << endl;
}

//43.设置色带号25
TEST_F(SgpTest, 043_SetColorBar_25)
{
    int colorbar = 25;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar25 failed" << endl;
}

//44.设置色带号26
TEST_F(SgpTest, 044_SetColorBar_26)
{
    int colorbar = 26;
    int ret = SGP_SetColorBar(handle, colorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBar26 failed" << endl;
}

//45.设置色带显示-关
TEST_F(SgpTest, 045_SetColorBarShow_OFF)
{
    int showcolorbar = 0;
    int ret = SGP_SetColorBarShow(handle, showcolorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBarShowON failed" << endl;
}

//46.设置色带显示-开
TEST_F(SgpTest, 046_SetColorBarShow_ON)
{
    int showcolorbar = 1;
    int ret = SGP_SetColorBarShow(handle, showcolorbar);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColorBarShowOFF failed" << endl;
}

//47.设置温度显示类型1
TEST_F(SgpTest, 047_SetTempShowMode_1)
{
    int showtype = 1;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_1 failed" << endl;
}

//48.设置温度显示类型2
TEST_F(SgpTest, 048_SetTempShowMode_2)
{
    int showtype = 2;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_2 failed" << endl;
}

//49.设置温度显示类型3
TEST_F(SgpTest, 049_SetTempShowMode_3)
{
    int showtype = 3;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_3 failed" << endl;
}

//50.设置温度显示类型4
TEST_F(SgpTest, 050_SetTempShowMode_4)
{
    int showtype = 4;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_4 failed" << endl;
}

//51.设置温度显示类型5
TEST_F(SgpTest, 051_SetTempShowMode_5)
{
    int showtype = 5;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_5 failed" << endl;
}

//52.设置温度显示类型6
TEST_F(SgpTest, 052_SetTempShowMode_6)
{
    int showtype = 6;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_6 failed" << endl;
}

//53.设置温度显示类型7
TEST_F(SgpTest, 053_SetTempShowMode_7)
{
    int showtype = 7;
    int ret = SGP_SetTempShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempShowMode_7 failed" << endl;
}

//54.设置字符串叠加
TEST_F(SgpTest, 054_SetStringShow)
{
    int type = 5;
    const char* dateshow = "123";
    int ret = SGP_SetStringShow(handle, type, dateshow);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetStringShow failed" << endl;
}

//55.设置分析对象温度显示类型1
TEST_F(SgpTest, 055_SetThermometryRuleShowMode_1)
{
    int showtype = 1;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_1 failed" << endl;
}

//56.设置分析对象温度显示类型2
TEST_F(SgpTest, 056_SetThermometryRuleShowMode_2)
{
    int showtype = 2;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_2 failed" << endl;
}

//57.设置分析对象温度显示类型3
TEST_F(SgpTest, 057_SetThermometryRuleShowMode_3)
{
    int showtype = 3;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_3 failed" << endl;
}

//58.设置分析对象温度显示类型4
TEST_F(SgpTest, 058_SetThermometryRuleShowMode_4)
{
    int showtype = 4;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_4 failed" << endl;
}

//59.设置分析对象温度显示类型5
TEST_F(SgpTest, 059_SetThermometryRuleShowMode_5)
{
    int showtype = 5;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_5 failed" << endl;
}

//60.设置分析对象温度显示类型6
TEST_F(SgpTest, 060_SetThermometryRuleShowMode_6)
{
    int showtype = 6;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_6 failed" << endl;
}

//61.设置分析对象温度显示类型7
TEST_F(SgpTest, 061_SetThermometryRuleShowMode_7)
{
    int showtype = 7;
    int ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_7 failed" << endl;

    showtype = 8;
    ret = SGP_SetThermometryRuleShowMode(handle, showtype);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetThermometryRuleShowMode_8 failed" << endl;
}

//62.添加分析对象-点
TEST_F(SgpTest, 062_AddThermometryRule_Point)
{
    SGP_RULE rulePoint;
    memset(&rulePoint, 0x00, sizeof(SGP_RULE));
    rulePoint.alarm_condition = 1;
    rulePoint.alarm_flag = 1;
    rulePoint.alarm_time = 10;
    rulePoint.alarm_type = 1;
    rulePoint.alarm_interal = 30;
    rulePoint.avg_temp = 30;
    rulePoint.flag = 1;
    rulePoint.high_temp = 35;
    rulePoint.low_temp = 28;
    rulePoint.show_type = 1;
    rulePoint.points_num = 1; //点个数是1
    rulePoint.points[0].x = 200;
    rulePoint.points[0].y = 200;
    strcpy(rulePoint.rule_name, "S1");
    rulePoint.show_location = 1;
    rulePoint.temp_mod = 1;
    rulePoint.type = 1;  //类型是1
    rulePoint.atmo_trans = 0.9;
    rulePoint.dist = 2;
    rulePoint.emiss = 0.8;
    rulePoint.emiss_mode = 1;
    rulePoint.humi = 80;
    rulePoint.opti_trans = 1;
    rulePoint.ref_temp = 25;
    int ret = SGP_AddThermometryRule(handle, rulePoint);
    EXPECT_EQ(ret, SGP_OK) << "SGP_AddThermometryRule_Point failed" << endl;

    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    EXPECT_EQ(array.rule[0].alarm_condition, rulePoint.alarm_condition) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_flag, rulePoint.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_time, rulePoint.alarm_time) << "assert alarm_time failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_type, rulePoint.alarm_type) << "assert alarm_type failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_interal, rulePoint.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(array.rule[0].avg_temp, rulePoint.avg_temp) << "assert avg_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].flag, rulePoint.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(array.rule[0].high_temp, rulePoint.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].low_temp, rulePoint.low_temp) << "assert low_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].show_type, rulePoint.show_type) << "assert show_type failed!" << endl;
    EXPECT_EQ(array.rule[0].points_num, rulePoint.points_num) << "assert points_num failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].x, rulePoint.points[0].x) << "assert points[0].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].y, rulePoint.points[0].y) << "assert points[0].y failed!" << endl;
    EXPECT_STREQ(array.rule[0].rule_name, rulePoint.rule_name) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].show_location, rulePoint.show_location) << "assert show_location failed!" << endl;
    EXPECT_EQ(array.rule[0].temp_mod, rulePoint.temp_mod) << "assert temp_mod failed!" << endl;
    EXPECT_EQ(array.rule[0].type, rulePoint.type) << "assert type failed!" << endl;
    EXPECT_EQ(array.rule[0].atmo_trans, rulePoint.atmo_trans) << "assert atmo_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].dist, rulePoint.dist) << "assert dist failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss, rulePoint.emiss) << "assert emiss failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss_mode, rulePoint.emiss_mode) << "assert emiss_mode failed!" << endl;
    EXPECT_EQ(array.rule[0].humi, rulePoint.humi) << "assert humi failed!" << endl;
    EXPECT_EQ(array.rule[0].opti_trans, rulePoint.opti_trans) << "assert opti_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].ref_temp, rulePoint.ref_temp) << "assert ref_temp failed!" << endl;
    SGP_DeleteAllThermometryRule(handle);
}

//63.添加分析对象-线
TEST_F(SgpTest, 063_AddThermometryRule_Line)
{
    SGP_RULE rulePoint;
    memset(&rulePoint, 0x00, sizeof(SGP_RULE));
    rulePoint.alarm_condition = 1;
    rulePoint.alarm_flag = 1;
    rulePoint.alarm_time = 10;
    rulePoint.alarm_type = 1;
    rulePoint.alarm_interal = 30;
    rulePoint.avg_temp = 30;
    rulePoint.flag = 1;
    rulePoint.high_temp = 35;
    rulePoint.low_temp = 28;
    rulePoint.show_type = 1;
    rulePoint.points_num = 2; //点个数是2
    rulePoint.points[0].x = 250;
    rulePoint.points[0].y = 250;
    rulePoint.points[1].x = 300;
    rulePoint.points[1].y = 300;
    strcpy(rulePoint.rule_name, "L1");
    rulePoint.show_location = 1;
    rulePoint.temp_mod = 1;
    rulePoint.type = 2;  //类型是2
    rulePoint.atmo_trans = 0.9;
    rulePoint.dist = 2;
    rulePoint.emiss = 0.8;
    rulePoint.emiss_mode = 1;
    rulePoint.humi = 80;
    rulePoint.opti_trans = 1;
    rulePoint.ref_temp = 25;
    int ret = SGP_AddThermometryRule(handle, rulePoint);
    EXPECT_EQ(ret, SGP_OK) << "SGP_AddThermometryRule_Line failed" << endl;

    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    EXPECT_EQ(array.rule[0].alarm_condition, rulePoint.alarm_condition) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_flag, rulePoint.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_time, rulePoint.alarm_time) << "assert alarm_time failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_type, rulePoint.alarm_type) << "assert alarm_type failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_interal, rulePoint.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(array.rule[0].avg_temp, rulePoint.avg_temp) << "assert avg_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].flag, rulePoint.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(array.rule[0].high_temp, rulePoint.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].low_temp, rulePoint.low_temp) << "assert low_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].show_type, rulePoint.show_type) << "assert show_type failed!" << endl;
    EXPECT_EQ(array.rule[0].points_num, rulePoint.points_num) << "assert points_num failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].x, rulePoint.points[0].x) << "assert points[0].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].y, rulePoint.points[0].y) << "assert points[0].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].x, rulePoint.points[1].x) << "assert points[1].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].y, rulePoint.points[1].y) << "assert points[1].y failed!" << endl;
    EXPECT_STREQ(array.rule[0].rule_name, rulePoint.rule_name) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].show_location, rulePoint.show_location) << "assert show_location failed!" << endl;
    EXPECT_EQ(array.rule[0].temp_mod, rulePoint.temp_mod) << "assert temp_mod failed!" << endl;
    EXPECT_EQ(array.rule[0].type, rulePoint.type) << "assert type failed!" << endl;
    EXPECT_EQ(array.rule[0].atmo_trans, rulePoint.atmo_trans) << "assert atmo_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].dist, rulePoint.dist) << "assert dist failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss, rulePoint.emiss) << "assert emiss failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss_mode, rulePoint.emiss_mode) << "assert emiss_mode failed!" << endl;
    EXPECT_EQ(array.rule[0].humi, rulePoint.humi) << "assert humi failed!" << endl;
    EXPECT_EQ(array.rule[0].opti_trans, rulePoint.opti_trans) << "assert opti_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].ref_temp, rulePoint.ref_temp) << "assert ref_temp failed!" << endl;
    SGP_DeleteAllThermometryRule(handle);
}

//64.添加分析对象-圆
TEST_F(SgpTest, 064_AddThermometryRule_Circle)
{
    SGP_RULE rulePoint;
    memset(&rulePoint, 0x00, sizeof(SGP_RULE));
    rulePoint.alarm_condition = 1;
    rulePoint.alarm_flag = 1;
    rulePoint.alarm_time = 10;
    rulePoint.alarm_type = 1;
    rulePoint.alarm_interal = 30;
    rulePoint.avg_temp = 30;
    rulePoint.flag = 1;
    rulePoint.high_temp = 35;
    rulePoint.low_temp = 28;
    rulePoint.show_type = 1;
    rulePoint.points_num = 4; //点个数是1
    rulePoint.points[0].x = 88;
    rulePoint.points[0].y = 100;
    rulePoint.points[1].x = 144;
    rulePoint.points[1].y = 68;
    rulePoint.points[2].x = 200;
    rulePoint.points[2].y = 100;
    rulePoint.points[3].x = 144;
    rulePoint.points[3].y = 133;
    strcpy(rulePoint.rule_name, "C1");
    rulePoint.show_location = 1;
    rulePoint.temp_mod = 1;
    rulePoint.type = 5;  //类型是5
    rulePoint.atmo_trans = 0.9;
    rulePoint.dist = 2;
    rulePoint.emiss = 0.8;
    rulePoint.emiss_mode = 1;
    rulePoint.humi = 80;
    rulePoint.opti_trans = 1;
    rulePoint.ref_temp = 25;
    int ret = SGP_AddThermometryRule(handle, rulePoint);
    EXPECT_EQ(ret, SGP_OK) << "SGP_AddThermometryRule_Rectangle_Circle failed" << endl;

    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    EXPECT_EQ(array.rule[0].alarm_condition, rulePoint.alarm_condition) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_flag, rulePoint.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_time, rulePoint.alarm_time) << "assert alarm_time failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_type, rulePoint.alarm_type) << "assert alarm_type failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_interal, rulePoint.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(array.rule[0].avg_temp, rulePoint.avg_temp) << "assert avg_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].flag, rulePoint.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(array.rule[0].high_temp, rulePoint.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].low_temp, rulePoint.low_temp) << "assert low_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].show_type, rulePoint.show_type) << "assert show_type failed!" << endl;
    EXPECT_EQ(array.rule[0].points_num, rulePoint.points_num) << "assert points_num failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].x, rulePoint.points[0].x) << "assert points[0].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].y, rulePoint.points[0].y) << "assert points[0].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].x, rulePoint.points[1].x) << "assert points[1].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].y, rulePoint.points[1].y) << "assert points[1].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].x, rulePoint.points[2].x) << "assert points[2].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].y, rulePoint.points[2].y) << "assert points[2].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].x, rulePoint.points[3].x) << "assert points[3].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].y, rulePoint.points[3].y) << "assert points[3].y failed!" << endl;
    EXPECT_STREQ(array.rule[0].rule_name, rulePoint.rule_name) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].show_location, rulePoint.show_location) << "assert show_location failed!" << endl;
    EXPECT_EQ(array.rule[0].temp_mod, rulePoint.temp_mod) << "assert temp_mod failed!" << endl;
    EXPECT_EQ(array.rule[0].type, rulePoint.type) << "assert type failed!" << endl;
    EXPECT_EQ(array.rule[0].atmo_trans, rulePoint.atmo_trans) << "assert atmo_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].dist, rulePoint.dist) << "assert dist failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss, rulePoint.emiss) << "assert emiss failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss_mode, rulePoint.emiss_mode) << "assert emiss_mode failed!" << endl;
    EXPECT_EQ(array.rule[0].humi, rulePoint.humi) << "assert humi failed!" << endl;
    EXPECT_EQ(array.rule[0].opti_trans, rulePoint.opti_trans) << "assert opti_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].ref_temp, rulePoint.ref_temp) << "assert ref_temp failed!" << endl;
    SGP_DeleteAllThermometryRule(handle);
}

//65.添加分析对象-矩形
TEST_F(SgpTest, 065_AddThermometryRule_Rectangle)
{
    SGP_RULE rulePoint;
    memset(&rulePoint, 0x00, sizeof(SGP_RULE));
    rulePoint.alarm_condition = 1;
    rulePoint.alarm_flag = 1;
    rulePoint.alarm_time = 10;
    rulePoint.alarm_type = 1;
    rulePoint.alarm_interal = 30;
    rulePoint.avg_temp = 30;
    rulePoint.flag = 1;
    rulePoint.high_temp = 35;
    rulePoint.low_temp = 28;
    rulePoint.show_type = 1;
    rulePoint.points_num = 4; //点个数是1
    rulePoint.points[0].x = 362;
    rulePoint.points[0].y = 90;
    rulePoint.points[1].x = 497;
    rulePoint.points[1].y = 90;
    rulePoint.points[2].x = 497;
    rulePoint.points[2].y = 175;
    rulePoint.points[3].x = 362;
    rulePoint.points[3].y = 175;
    strcpy(rulePoint.rule_name, "R1");
    rulePoint.show_location = 1;
    rulePoint.temp_mod = 1;
    rulePoint.type = 3;  //类型是3
    rulePoint.atmo_trans = 0.9;
    rulePoint.dist = 2;
    rulePoint.emiss = 0.8;
    rulePoint.emiss_mode = 1;
    rulePoint.humi = 80;
    rulePoint.opti_trans = 1;
    rulePoint.ref_temp = 25;
    int ret = SGP_AddThermometryRule(handle, rulePoint);
    EXPECT_EQ(ret, SGP_OK) << "SGP_AddThermometryRule_Rectangle failed" << endl;

    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    EXPECT_EQ(array.rule[0].alarm_condition, rulePoint.alarm_condition) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_flag, rulePoint.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_time, rulePoint.alarm_time) << "assert alarm_time failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_type, rulePoint.alarm_type) << "assert alarm_type failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_interal, rulePoint.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(array.rule[0].avg_temp, rulePoint.avg_temp) << "assert avg_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].flag, rulePoint.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(array.rule[0].high_temp, rulePoint.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].low_temp, rulePoint.low_temp) << "assert low_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].show_type, rulePoint.show_type) << "assert show_type failed!" << endl;
    EXPECT_EQ(array.rule[0].points_num, rulePoint.points_num) << "assert points_num failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].x, rulePoint.points[0].x) << "assert points[0].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].y, rulePoint.points[0].y) << "assert points[0].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].x, rulePoint.points[1].x) << "assert points[1].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].y, rulePoint.points[1].y) << "assert points[1].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].x, rulePoint.points[2].x) << "assert points[2].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].y, rulePoint.points[2].y) << "assert points[2].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].x, rulePoint.points[3].x) << "assert points[3].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].y, rulePoint.points[3].y) << "assert points[3].y failed!" << endl;
    EXPECT_STREQ(array.rule[0].rule_name, rulePoint.rule_name) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].show_location, rulePoint.show_location) << "assert show_location failed!" << endl;
    EXPECT_EQ(array.rule[0].temp_mod, rulePoint.temp_mod) << "assert temp_mod failed!" << endl;
    EXPECT_EQ(array.rule[0].type, rulePoint.type) << "assert type failed!" << endl;
    EXPECT_EQ(array.rule[0].atmo_trans, rulePoint.atmo_trans) << "assert atmo_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].dist, rulePoint.dist) << "assert dist failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss, rulePoint.emiss) << "assert emiss failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss_mode, rulePoint.emiss_mode) << "assert emiss_mode failed!" << endl;
    EXPECT_EQ(array.rule[0].humi, rulePoint.humi) << "assert humi failed!" << endl;
    EXPECT_EQ(array.rule[0].opti_trans, rulePoint.opti_trans) << "assert opti_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].ref_temp, rulePoint.ref_temp) << "assert ref_temp failed!" << endl;
    SGP_DeleteAllThermometryRule(handle);
}

//66.添加分析对象-多边形
TEST_F(SgpTest, 066_AddThermometryRule_Polygon)
{
    SGP_RULE rulePoint;
    memset(&rulePoint, 0x00, sizeof(SGP_RULE));
    rulePoint.alarm_condition = 1;
    rulePoint.alarm_flag = 1;
    rulePoint.alarm_time = 10;
    rulePoint.alarm_type = 1;
    rulePoint.alarm_interal = 30;
    rulePoint.avg_temp = 30;
    rulePoint.flag = 1;
    rulePoint.high_temp = 35;
    rulePoint.low_temp = 28;
    rulePoint.show_type = 1;
    rulePoint.points_num = 7; //点个数是1
    rulePoint.points[0].x = 95;
    rulePoint.points[0].y = 166;
    rulePoint.points[1].x = 153;
    rulePoint.points[1].y = 159;
    rulePoint.points[2].x = 164;
    rulePoint.points[2].y = 180;
    rulePoint.points[3].x = 167;
    rulePoint.points[3].y = 227;
    rulePoint.points[4].x = 145;
    rulePoint.points[4].y = 248;
    rulePoint.points[5].x = 99;
    rulePoint.points[5].y = 244;
    rulePoint.points[6].x = 72;
    rulePoint.points[6].y = 216;
    strcpy(rulePoint.rule_name, "P1");
    rulePoint.show_location = 1;
    rulePoint.temp_mod = 1;
    rulePoint.type = 4;  //类型是1
    rulePoint.atmo_trans = 0.9;
    rulePoint.dist = 2;
    rulePoint.emiss = 0.8;
    rulePoint.emiss_mode = 1;
    rulePoint.humi = 80;
    rulePoint.opti_trans = 1;
    rulePoint.ref_temp = 25;
    int ret = SGP_AddThermometryRule(handle, rulePoint);
    EXPECT_EQ(ret, SGP_OK) << "SGP_AddThermometryRule_Rectangle_Polygon failed" << endl;

    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    EXPECT_EQ(array.rule[0].alarm_condition, rulePoint.alarm_condition) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_flag, rulePoint.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_time, rulePoint.alarm_time) << "assert alarm_time failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_type, rulePoint.alarm_type) << "assert alarm_type failed!" << endl;
    EXPECT_EQ(array.rule[0].alarm_interal, rulePoint.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(array.rule[0].avg_temp, rulePoint.avg_temp) << "assert avg_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].flag, rulePoint.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(array.rule[0].high_temp, rulePoint.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].low_temp, rulePoint.low_temp) << "assert low_temp failed!" << endl;
    EXPECT_EQ(array.rule[0].show_type, rulePoint.show_type) << "assert show_type failed!" << endl;
    EXPECT_EQ(array.rule[0].points_num, rulePoint.points_num) << "assert points_num failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].x, rulePoint.points[0].x) << "assert points[0].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[0].y, rulePoint.points[0].y) << "assert points[0].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].x, rulePoint.points[1].x) << "assert points[1].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[1].y, rulePoint.points[1].y) << "assert points[1].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].x, rulePoint.points[2].x) << "assert points[2].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[2].y, rulePoint.points[2].y) << "assert points[2].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].x, rulePoint.points[3].x) << "assert points[3].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[3].y, rulePoint.points[3].y) << "assert points[3].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[4].x, rulePoint.points[4].x) << "assert points[4].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[4].y, rulePoint.points[4].y) << "assert points[4].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[5].x, rulePoint.points[5].x) << "assert points[5].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[5].y, rulePoint.points[5].y) << "assert points[5].y failed!" << endl;
    EXPECT_EQ(array.rule[0].points[6].x, rulePoint.points[6].x) << "assert points[6].x failed!" << endl;
    EXPECT_EQ(array.rule[0].points[6].y, rulePoint.points[6].y) << "assert points[6].y failed!" << endl;
    EXPECT_STREQ(array.rule[0].rule_name, rulePoint.rule_name) << "assert alarm_condition failed!" << endl;
    EXPECT_EQ(array.rule[0].show_location, rulePoint.show_location) << "assert show_location failed!" << endl;
    EXPECT_EQ(array.rule[0].temp_mod, rulePoint.temp_mod) << "assert temp_mod failed!" << endl;
    EXPECT_EQ(array.rule[0].type, rulePoint.type) << "assert type failed!" << endl;
    EXPECT_EQ(array.rule[0].atmo_trans, rulePoint.atmo_trans) << "assert atmo_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].dist, rulePoint.dist) << "assert dist failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss, rulePoint.emiss) << "assert emiss failed!" << endl;
    EXPECT_EQ(array.rule[0].emiss_mode, rulePoint.emiss_mode) << "assert emiss_mode failed!" << endl;
    EXPECT_EQ(array.rule[0].humi, rulePoint.humi) << "assert humi failed!" << endl;
    EXPECT_EQ(array.rule[0].opti_trans, rulePoint.opti_trans) << "assert opti_trans failed!" << endl;
    EXPECT_EQ(array.rule[0].ref_temp, rulePoint.ref_temp) << "assert ref_temp failed!" << endl;
}

//67.获取分析对象
TEST_F(SgpTest, 067_GetThermometryRule)
{
    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    int ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    cout << "分析对象个数为：" << array.rule_num << endl;
    for (int i = 0; i < array.rule_num; i++)
    {
        cout << "第" << i + 1 << "个分析对象信息：" << endl;
        cout << "大气透过率为：" << array.rule[i].atmo_trans << endl;
        cout << "距离为：" << array.rule[i].dist << endl;
        cout << "ID为：" << array.rule[i].id << endl;
        cout << "报警条件为：" << array.rule[i].alarm_condition << endl;
        cout << "是否需要报警：" << array.rule[i].alarm_flag << endl;
        cout << "报警类型：" << array.rule[i].alarm_type << endl;
        cout << "平均温：" << array.rule[i].avg_temp << endl;
        cout << "是否启用：" << array.rule[i].flag << endl;
        cout << "报警高温阈值：" << array.rule[i].high_temp << endl;
        cout << "报警低温阈值：" << array.rule[i].low_temp << endl;
        cout << "规则名称：" << array.rule[i].rule_name << endl;
        cout << "显示位置：" << array.rule[i].show_location << endl;
        cout << "温度误差：" << array.rule[i].temp_mod << endl;
        cout << "大气透过率：" << array.rule[i].atmo_trans << endl;
        cout << "发射率：" << array.rule[i].emiss << endl;
        cout << "发射率类型：" << array.rule[i].emiss_mode << endl;
        cout << "湿度：" << array.rule[i].humi << endl;
        cout << "光学透过率：" << array.rule[i].opti_trans << endl;
        cout << "反射温度：" << array.rule[i].ref_temp << endl;
        cout << endl;
    }
}

//68.更新分析对象
TEST_F(SgpTest, 068_UpdateThermometryRule)
{
    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    int ret = SGP_GetThermometryRule(handle, &array);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;

    SGP_RULE rule;
    memset(&rule, 0, sizeof(rule));
    memcpy(&rule, &array.rule[0], sizeof(rule));
    rule.alarm_condition = 1;
    rule.alarm_type = 1;
    rule.alarm_flag = 1;
    rule.alarm_time = 3;
    rule.high_temp = 20;
    rule.low_temp = 22;
    rule.show_type = 1;
    rule.alarm_interal = 30;
    ret = SGP_UpdateThermometryRule(handle, rule);
    EXPECT_EQ(ret, SGP_OK) << "SGP_UpdateThermometryRule failed" << endl;
}


//69.删除分析对象
TEST_F(SgpTest, 069_DeleteThermometryRule)
{
    SGP_RULE_ARRAY array;
    memset(&array, 0x00, sizeof(array));
    int ret = SGP_GetThermometryRule(handle, &array);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetThermometryRule failed" << endl;
    if (array.rule_num > 0)
    {
        int ret = SGP_DeleteThermometryRule(handle, array.rule[0].id);
        EXPECT_EQ(ret, SGP_OK) << "SGP_DeleteThermometryRule failed" << endl;
    }
}

//70.删除全部分析对象
TEST_F(SgpTest, 070_DeleteAllThermometryRule)
{
    int ret = SGP_DeleteAllThermometryRule(handle);
    EXPECT_EQ(ret, SGP_OK) << "SGP_DeleteAllThermometryRule failed" << endl;
}

//71.设置红外图像效果参数-快门间隔时间3min
TEST_F(SgpTest, 071_SetIrImageEffectParam_AUTO_SHUTTER_3)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_AUTO_SHUTTER;
    int value = 3;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_AUTO_SHUTTER_3 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.auto_shutter, value) << "assert auto_shutter failed!" << endl;
}

//72.设置红外图像效果参数-亮度1
TEST_F(SgpTest, 072_SetIrImageEffectParam_BRIGHTNESS_1)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_BRIGHTNESS;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_BRIGHTNESS_1 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.brightness, value) << "assert brightness failed!" << endl;
}

//73.设置红外图像效果参数-亮度100
TEST_F(SgpTest, 073_SetIrImageEffectParam_BRIGHTNESS_100)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_BRIGHTNESS;
    int value = 100;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_BRIGHTNESS_100 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.brightness, value) << "assert brightness failed!" << endl;
}

//74.设置红外图像效果参数-对比度1
TEST_F(SgpTest, 074_SetIrImageEffectParam_CONTRAST_1)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_CONTRAST;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_CONTRAST_1 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.contrast, value) << "assert contrast failed!" << endl;
}

//75.设置红外图像效果参数-对比度100
TEST_F(SgpTest, 075_SetIrImageEffectParam_CONTRAST_100)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_CONTRAST;
    int value = 100;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_CONTRAST_100 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.contrast, value) << "assert contrast failed!" << endl;
}

//76.设置红外图像效果参数-翻转开
TEST_F(SgpTest, 076_SetIrImageEffectParam_REVERSE_ON)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_REVERSE;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_REVERSE_ON failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.reverse, value) << "assert reverse failed!" << endl;
}

//77.设置红外图像效果参数-翻转关
TEST_F(SgpTest, 077_SetIrImageEffectParam_REVERSE_OFF)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_REVERSE;
    int value = 0;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_REVERSE_ON failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.reverse, value) << "assert reverse failed!" << endl;
}

//78.设置红外图像效果参数-锐度1
TEST_F(SgpTest, 078_SetIrImageEffectParam_SHARPNESS_1)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_SHARPNESS;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_SHARPNESS_1 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.sharpness, value) << "assert sharpness failed!" << endl;
}

//79.设置红外图像效果参数-锐度1
TEST_F(SgpTest, 079_SetIrImageEffectParam_SHARPNESS_100)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_SHARPNESS;
    int value = 100;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_SHARPNESS_100 failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.sharpness, value) << "assert sharpness failed!" << endl;
}

//80.设置红外图像效果参数-细节增强开
TEST_F(SgpTest, 080_SetIrImageEffectParam_IEE_FLAG_ON)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_IEE_FLAG;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_IEE_FLAG_ON failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.iee_flag, value) << "assert iee_flag failed!" << endl;
}

//81.设置红外图像效果参数-细节增强关
TEST_F(SgpTest, 081_SetIrImageEffectParam_IEE_FLAG_OFF)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_IEE_FLAG;
    int value = 0;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_IEE_FLAG_OFF failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.iee_flag, value) << "assert iee_flag failed!" << endl;
}

//82.设置红外图像效果参数-细节增强值1
TEST_F(SgpTest, 082_SetIrImageEffectParam_IEE_VALUE_1)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_IEE_VALUE;
    int value = 1;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_IEE_FLAG_OFF failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.iee_value, value) << "assert iee_value failed!" << endl;
}

//83.设置红外图像效果参数-细节增强值100
TEST_F(SgpTest, 083_SetIrImageEffectParam_IEE_VALUE_100)
{
    SGP_IR_IMAGE_EFFECT_ENUM type = SGP_IR_IEE_VALUE;
    int value = 100;
    int ret = SGP_SetIrImageEffectParam(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SetIrImageEffectParam_IEE_FLAG_OFF failed" << endl;

    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
    EXPECT_EQ(info.iee_value, value) << "assert iee_value failed!" << endl;
}

//84.获取红外图像效果参数
TEST_F(SgpTest, 084_GetIrImageEffectParam)
{
    SGP_IAMGE_EFFECT_PARAM_IR_CONFIG info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetIrImageEffectParam(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetIrImageEffectParam failed" << endl;
}

//85.设置网络信息
TEST_F(SgpTest, 085_SetNetInfo)
{
    SGP_NET_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetNetInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_SetNetInfo failed" << endl;
    if (ret == SGP_OK)
    {
        info.card = 0;
        strcpy(info.dns1, "192.168.1.168");
        strcpy(info.dns2, "192.168.1.167");
        strcpy(info.gateway, "192.168.21.1");
        strcpy(info.host_name, "qwe123");
        //strcpy(info.ipaddr, "192.168.21.152");
        strcpy(info.netmask, "255.255.252.0");
        info.mode = 0;
        ret = SGP_SetNetInfo(handle, info);
        EXPECT_EQ(ret, SGP_OK) << "SGP_SetNetInfo failed" << endl;

        SGP_NET_INFO getInfo;
        memset(&getInfo, 0x00, sizeof(getInfo));
        ret = SGP_GetNetInfo(handle, &getInfo);
        EXPECT_EQ(ret, SGP_OK) << "SGP_GetNetInfo failed" << endl;
        EXPECT_STREQ(getInfo.dns1, info.dns1) << "assert dns1 failed!" << endl;
        EXPECT_STREQ(getInfo.dns2, info.dns2) << "assert dns2 failed!" << endl;
        EXPECT_STREQ(getInfo.gateway, info.gateway) << "assert gateway failed!" << endl;
        EXPECT_STREQ(getInfo.host_name, info.host_name) << "assert host_name failed!" << endl;
        EXPECT_STREQ(getInfo.netmask, info.netmask) << "assert netmask failed!" << endl;
        EXPECT_EQ(getInfo.mode, info.mode) << "assert mode failed!" << endl;
    }
}

//86.获取网络信息
TEST_F(SgpTest, 086_GetNetInfo)
{
    SGP_NET_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetNetInfo(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetNetInfo failed" << endl;
    cout << "获取网络信息成功，获取信息如下：" << endl;
    cout << "获取的主机名为：" << info.host_name << endl;
    cout << "获取ip地址为：" << info.ipaddr << endl;
}

//87.设置端口信息
TEST_F(SgpTest, 087_SetPortInfo)
{
    SGP_PORT_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetPortInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetPortInfo failed" << endl;
    info.max_connectios = 20;
    info.http_port = 80;
    info.rtsp_port = 8888;
    info.onvif_check = 0;
    //info.tcp_port = 100;
    ret = SGP_SetPortInfo(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetPortInfo failed" << endl;

    SGP_PORT_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetPortInfo(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetPortInfo failed" << endl;
    EXPECT_EQ(getInfo.max_connectios, info.max_connectios) << "assert max_connectios failed!" << endl;
    EXPECT_EQ(getInfo.http_port, info.http_port) << "assert http_port failed!" << endl;
    EXPECT_EQ(getInfo.rtsp_port, info.rtsp_port) << "assert rtsp_port failed!" << endl;
    EXPECT_EQ(getInfo.onvif_check, info.onvif_check) << "assert onvif_check failed!" << endl;
}

//88.获取端口信息
TEST_F(SgpTest, 088_GetPortInfo)
{
    SGP_PORT_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetPortInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetPortInfo failed" << endl;
    cout << "返回成功，获取的端口信息如下：" << endl;
    cout << "http端口号为：\t" << info.http_port << endl;
    cout << "最大连接设备数为:" << info.max_connectios << endl;
    cout << "onvif开关：\t" << info.onvif_check << endl;
    cout << "rtsp端口号为：\t" << info.rtsp_port << endl;
    cout << "tcp端口号为：\t" << info.tcp_port << endl;
}

//89.设置录制信息
TEST_F(SgpTest, 089_SetRecordInfo)
{
    SGP_RECORD_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetRecordInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetRecordInfo failed" << endl;
    info.record_interval = 10;
    info.record_max_size = 999;
    info.record_time = 10;
    ret = SGP_SetRecordInfo(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetRecordInfo failed" << endl;

    SGP_RECORD_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetRecordInfo(handle, &getInfo);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetRecordInfo failed" << endl;
    EXPECT_EQ(getInfo.record_interval, info.record_interval) << "assert record_interval failed!" << endl;
    EXPECT_EQ(getInfo.record_max_size, info.record_max_size) << "assert record_max_size failed!" << endl;
    EXPECT_EQ(getInfo.record_time, info.record_time) << "assert record_time failed!" << endl;
}

//90.获取录制信息
TEST_F(SgpTest, 090_GetRecordInfo)
{
    SGP_RECORD_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetRecordInfo(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetRecordInfo failed" << endl;
    cout << "返回成功，获取的端口信息如下：" << endl;
    cout << "录像延时时间：\t" << info.record_interval << endl;
    cout << "最大录像文件大小：" << info.record_max_size << endl;
    cout << "录像时长： \t" << info.record_time << endl;
}

//91.设置屏蔽区域
TEST_F(SgpTest, 091_SetShieldArea)
{
    SGP_SHIELD_AREA_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetShieldArea(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetShieldArea failed" << endl;
    info.rect_num = 2;
    info.rect[0].x = 250;
    info.rect[0].y = 250;
    info.rect[0].w = 150;
    info.rect[0].h = 150;
    info.rect[1].x = 50;
    info.rect[1].y = 50;
    info.rect[1].w = 150;
    info.rect[1].h = 150;
    ret = SGP_SetShieldArea(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetShieldArea failed" << endl;

    SGP_SHIELD_AREA_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetShieldArea(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetShieldArea failed" << endl;
    EXPECT_EQ(getInfo.rect[0].x, info.rect[0].x) << "assert rect[0].x failed!" << endl;
    EXPECT_EQ(getInfo.rect[0].y, info.rect[0].y) << "assert rect[0].y failed!" << endl;
    EXPECT_EQ(getInfo.rect[0].w, info.rect[0].w) << "assert rect[0].w failed!" << endl;
    EXPECT_EQ(getInfo.rect[0].h, info.rect[0].h) << "assert rect[0].h failed!" << endl;
    EXPECT_EQ(getInfo.rect[1].x, info.rect[1].x) << "assert rect[0].x failed!" << endl;
    EXPECT_EQ(getInfo.rect[1].y, info.rect[1].y) << "assert rect[0].y failed!" << endl;
    EXPECT_EQ(getInfo.rect[1].w, info.rect[1].w) << "assert rect[0].w failed!" << endl;
    EXPECT_EQ(getInfo.rect[1].h, info.rect[1].h) << "assert rect[0].h failed!" << endl;
}

//92.获取屏蔽区域
TEST_F(SgpTest, 092_GetShieldAre)
{
    SGP_SHIELD_AREA_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetShieldArea(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetShieldArea failed" << endl;
    cout << "返回成功，获取屏蔽区域信息如下：" << endl;
    cout << "屏蔽区域个数为：" << info.rect_num << endl;
    cout << "屏蔽区域坐标为：" << endl;
    for (int i = 0; i < info.rect_num; i++)
    {
        printf("第%d个屏蔽区域的坐标为：\n", i + 1);
        cout << "x=" << info.rect[i].x << endl;
        cout << "y=" << info.rect[i].y << endl;
        cout << "w=" << info.rect[i].w << endl;
        cout << "h=" << info.rect[i].h << endl;
        cout << endl;
    }
}

//93.设置全局温度告警
TEST_F(SgpTest, 093_SetColdHotTrace)
{
    SGP_COLD_HOT_TRACE_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetColdHotTrace(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetColdHotTrace failed" << endl;
    info.high_flag = 1;
    info.high_temp = 33;
    info.low_flag = 1;
    info.low_temp = 22;
    info.capture_flag = 1;
    info.capture_stream = 2;
    info.output_flag = 1;
    info.output_hold = 300;
    info.record_flag = 1;
    info.record_stream = 2;
    info.sendmail = 1;
    info.record_delay = 300;
    info.alarm_shake = 10;
    info.alarm_interal = 300;
    info.trace_flag = 1;    //冷热点追踪
    strcpy(info.high_color, "#0000ff");
    strcpy(info.low_color, "#ff0000");
    ret = SGP_SetColdHotTrace(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetColdHotTrace failed" << endl;

    SGP_COLD_HOT_TRACE_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetColdHotTrace(handle, &getInfo);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetColdHotTrace failed" << endl;
    EXPECT_EQ(getInfo.high_flag, info.high_flag) << "assert high_flag failed!" << endl;
    EXPECT_EQ(getInfo.high_temp, info.high_temp) << "assert high_temp failed!" << endl;
    EXPECT_EQ(getInfo.low_flag, info.low_flag) << "assert low_flag failed!" << endl;
    EXPECT_EQ(getInfo.low_temp, info.low_temp) << "assert low_temp failed!" << endl;
    //EXPECT_EQ(getInfo.capture_flag, info.capture_flag) << "assert capture_flag failed!" << endl;
    EXPECT_EQ(getInfo.capture_stream, info.capture_stream) << "assert capture_stream failed!" << endl;
    EXPECT_EQ(getInfo.output_flag, info.output_flag) << "assert output_flag failed!" << endl;
    EXPECT_EQ(getInfo.output_hold, info.output_hold) << "assert output_hold failed!" << endl;
    //EXPECT_EQ(getInfo.record_flag, info.record_flag) << "assert record_flag failed!" << endl;
    EXPECT_EQ(getInfo.record_stream, info.record_stream) << "assert record_stream failed!" << endl;
    EXPECT_EQ(getInfo.sendmail, info.sendmail) << "assert sendmail failed!" << endl;
    EXPECT_EQ(getInfo.record_delay, info.record_delay) << "assert record_delay failed!" << endl;
    EXPECT_EQ(getInfo.alarm_shake, info.alarm_shake) << "assert alarm_shake failed!" << endl;
    EXPECT_EQ(getInfo.alarm_interal, info.alarm_interal) << "assert alarm_interal failed!" << endl;
    EXPECT_EQ(getInfo.trace_flag, info.trace_flag) << "assert trace_flag failed!" << endl;
    EXPECT_STREQ(getInfo.high_color, info.high_color) << "assert high_color failed!" << endl;
    EXPECT_STREQ(getInfo.low_color, info.low_color) << "assert low_color failed!" << endl;
}

//94.获取全局温度告警
TEST_F(SgpTest, 094_GetColdHotTrace)
{
    SGP_COLD_HOT_TRACE_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetColdHotTrace(handle, &info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_GetColdHotTrace failed" << endl;
    cout << "返回成功，获取全局温度告警信息如下：" << endl;
    cout << "capture_flag:" << info.capture_flag << endl;
    cout << "capture_stream:" << info.capture_stream << endl;
    cout << "high_flag:" << info.high_flag << endl;
    cout << "high_temp:" << info.high_temp << endl;
    cout << "low_flag:" << info.low_flag << endl;
    cout << "low_temp:" << info.low_temp << endl;
    cout << "sendmail:" << info.sendmail << endl;
    cout << "alarm_interal" << info.alarm_interal << endl;
}

//95.设置分析对象告警
TEST_F(SgpTest, 095_SetTempAlarm)
{
    SGP_TEMP_ALARM_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetTempAlarm(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetTempAlarm failed" << endl;
    info.alarm_flag = 1;//是否开启报警 1:开启; 0:不开启
    info.capture_flag = 1;//是否截图 0:否; 1:是
    info.capture_stream = 2;//截图类型 0:不截图; 1:只截图可见光; 2:只截图红外; 3:截图红外和可见光
    info.record_flag = 1;//是否录制 0:不录制; 1:录制
    info.record_stream = 2;//录制类型 0:不录制; 1:只录制可见光; 2:只录制红外; 3:录制红外和可见光
    info.record_delay = 150;//录制时间 10-300s
    info.output_flag = 1;//是否外部输出 0:不输出 1:输出
    info.output_hold = 20;
    info.alarm_shake = 8;
    info.sendmail = 1;
    ret = SGP_SetTempAlarm(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempAlarm failed" << endl;

    SGP_TEMP_ALARM_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetTempAlarm(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetTempAlarm failed" << endl;
    EXPECT_EQ(getInfo.alarm_flag, info.alarm_flag) << "assert alarm_flag failed!" << endl;
    EXPECT_EQ(getInfo.capture_flag, info.capture_flag) << "assert capture_flag failed!" << endl;
    EXPECT_EQ(getInfo.capture_stream, info.capture_stream) << "assert capture_stream failed!" << endl;
    EXPECT_EQ(getInfo.record_flag, info.record_flag) << "assert record_flag failed!" << endl;
    EXPECT_EQ(getInfo.record_stream, info.record_stream) << "assert record_stream failed!" << endl;
    EXPECT_EQ(getInfo.record_delay, info.record_delay) << "assert record_delay failed!" << endl;
    EXPECT_EQ(getInfo.output_flag, info.output_flag) << "assert output_flag failed!" << endl;
    EXPECT_EQ(getInfo.output_hold, info.output_hold) << "assert output_hold failed!" << endl;
    EXPECT_EQ(getInfo.alarm_shake, info.alarm_shake) << "assert alarm_shake failed!" << endl;
    EXPECT_EQ(getInfo.sendmail, info.sendmail) << "assert sendmail failed!" << endl;
}

//96.获取分析对象告警
TEST_F(SgpTest, 096_GetTempAlarm)
{
    SGP_TEMP_ALARM_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetTempAlarm(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetTempAlarm failed" << endl;
    cout << "返回成功，获取分析对象告警信息如下：" << endl;
    cout << "alarm_flag:" << info.alarm_flag << endl;
    cout << "capture_flag:" << info.capture_flag << endl;
    cout << "capture_stream:" << info.capture_stream << endl;
    cout << "output_flag:" << info.output_flag << endl;
    cout << "output_hold:" << info.output_hold << endl;
    cout << "sendmail:" << info.sendmail << endl;
}

//97.设置视频参数
TEST_F(SgpTest, 097_SetVideoParam)
{
    SGP_GENERAL_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetGeneralInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetGeneralInfo failed" << endl;
    SGP_VIDEO_PARAM_ENUM type = SGP_IR;
    SGP_VIDEO_PARAM param;
    memset(&param, 0x00, sizeof(param));
    ret = SGP_GetVideoParam(handle, type, &param);
    param.fps = 25;
    param.gop_size = param.fps * 2;
    param.bit_size = info.ir_model_w * info.ir_model_h * param.fps * 8 * 3 / 2 / 1024 / 18;
    param.level = 1;
    ret = SGP_SetVideoParam(handle, type, param);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetVideoParam failed" << endl;

    SGP_VIDEO_PARAM getParam;
    memset(&getParam, 0x00, sizeof(getParam));
    ret = SGP_GetVideoParam(handle, type, &getParam);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetVideoParam failed" << endl;
    EXPECT_EQ(getParam.fps, param.fps) << "assert fps failed!" << endl;
    EXPECT_EQ(getParam.gop_size, param.gop_size) << "assert gop_size failed!" << endl;
    EXPECT_EQ(getParam.bit_size, param.bit_size) << "assert bit_size failed!" << endl;
    EXPECT_EQ(getParam.level, param.level) << "assert level failed!" << endl;
}

//98.获取视频参数
TEST_F(SgpTest, 098_GetVideoParam)
{
    SGP_VIDEO_PARAM_ENUM type = SGP_IR;
    SGP_VIDEO_PARAM param;
    memset(&param, 0x00, sizeof(param));
    int ret = SGP_GetVideoParam(handle, type, &param);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetVideoParam failed" << endl;
    cout << "返回成功，获取的视频参数如下：" << endl;
    cout << "param.bit_size=" << param.bit_size << endl;
    cout << "param.fps=" << param.fps << endl;
    cout << "param.gop_size=" << param.gop_size << endl;
    cout << "param.level=" << param.level << endl;
}

//99.设置网络异常
TEST_F(SgpTest, 099_SetNetException)
{
    SGP_NET_EXCEPTION_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetNetException(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetNetException failed" << endl;

    info.flag = 1;
    info.output_flag = 1;
    info.output_hold = 20;
    info.interval_time = 30;
    ret = SGP_SetNetException(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetNetException failed" << endl;

    SGP_NET_EXCEPTION_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetNetException(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetNetException failed" << endl;
    EXPECT_EQ(getInfo.flag, info.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(getInfo.output_flag, info.output_flag) << "assert output_flag failed!" << endl;
    EXPECT_EQ(getInfo.output_hold, info.output_hold) << "assert output_hold failed!" << endl;
    EXPECT_EQ(getInfo.interval_time, info.interval_time) << "assert interval_time failed!" << endl;
}

//100.获取网络异常
TEST_F(SgpTest, 100_GetNetException)
{
    SGP_NET_EXCEPTION_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetNetException(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetNetException failed" << endl;
    cout << "返回成功，获取网络异常信息如下：" << endl;
    cout << "flag=" << info.flag << endl;
    cout << "output_flag=" << info.output_flag << endl;
    cout << "output_hold=" << info.output_hold << endl;
}

//101.设置非法访问
TEST_F(SgpTest, 101_SetAccessViolation)
{
    SGP_ACCESS_VIOLATION_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetAccessViolation(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetAccessViolation failed" << endl;

    info.allow_count = 10;
    info.flag = 1;
    info.output_flag = 1;
    info.output_hold = 12;
    info.sendmail = 1;
    info.lock_time = 30;
    ret = SGP_SetAccessViolation(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetAccessViolation failed" << endl;

    SGP_ACCESS_VIOLATION_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetAccessViolation(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetAccessViolation failed" << endl;
    EXPECT_EQ(getInfo.allow_count, info.allow_count) << "assert allow_count failed!" << endl;
    EXPECT_EQ(getInfo.flag, info.flag) << "assert flag failed!" << endl;
    EXPECT_EQ(getInfo.output_flag, info.output_flag) << "assert output_flag failed!" << endl;
    EXPECT_EQ(getInfo.output_hold, info.output_hold) << "assert output_hold failed!" << endl;
    EXPECT_EQ(getInfo.sendmail, info.sendmail) << "assert sendmail failed!" << endl;
    EXPECT_EQ(getInfo.lock_time, info.lock_time) << "assert lock_time failed!" << endl;
}

//102.获取非法访问
TEST_F(SgpTest, 102_GetAccessViolation)
{
    SGP_ACCESS_VIOLATION_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetAccessViolation(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetAccessViolation failed" << endl;
    cout << "返回成功，获取的非法访问信息如下：" << endl;
    cout << "allow_count=" << info.allow_count << endl;
    cout << "flag=" << info.flag << endl;
    cout << "output_flag=" << info.output_flag << endl;
    cout << "output_hold=" << info.output_hold << endl;
    cout << "sendmail=" << info.sendmail << endl;
}

//103.设置邮件信息
TEST_F(SgpTest, 103_SetEmilInfo)
{
    SGP_EMAIL_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetEmilInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetEmilInfo failed" << endl;

    info.alarm = 1;
    info.alarm_value = 3600;
    info.enclosure = 1;
    info.encry_type = 2;
    info.health = 1;
    info.health_value = 1;
    info.is_anon = 1;
    info.mailto_num = 5;
    info.smtp_port = 26;
    strcpy(info.smtp_server, "192.168.21.111");
    strcpy(info.username, "123456789");
    strcpy(info.password, "123456");
    strcpy(info.from, "123qq.com");
    strcpy(info.subject, "TEST");
    strcpy(info.mailto[1], "123qq.com");
    ret = SGP_SetEmilInfo(handle, info);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetEmilInfo failed" << endl;

    SGP_EMAIL_INFO getInfo;
    memset(&getInfo, 0x00, sizeof(getInfo));
    ret = SGP_GetEmilInfo(handle, &getInfo);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetEmilInfo failed" << endl;
    EXPECT_EQ(getInfo.alarm, info.alarm) << "assert alarm failed!" << endl;
    EXPECT_EQ(getInfo.alarm_value, info.alarm_value) << "assert alarm_value failed!" << endl;
    EXPECT_EQ(getInfo.enclosure, info.enclosure) << "assert enclosure failed!" << endl;
    EXPECT_EQ(getInfo.encry_type, info.encry_type) << "assert encry_type failed!" << endl;
    EXPECT_EQ(getInfo.health, info.health) << "assert health failed!" << endl;
    EXPECT_EQ(getInfo.health_value, info.health_value) << "assert health_value failed!" << endl;
    EXPECT_EQ(getInfo.is_anon, info.is_anon) << "assert is_anon failed!" << endl;
    EXPECT_EQ(getInfo.mailto_num, info.mailto_num) << "assert mailto_num failed!" << endl;
    EXPECT_EQ(getInfo.smtp_port, info.smtp_port) << "assert smtp_port failed!" << endl;
    EXPECT_STREQ(getInfo.smtp_server, info.smtp_server) << "assert smtp_server failed!" << endl;
    EXPECT_STREQ(getInfo.username, info.username) << "assert username failed!" << endl;
    EXPECT_STREQ(getInfo.password, info.password) << "assert password failed!" << endl;
    EXPECT_STREQ(getInfo.from, info.from) << "assert from failed!" << endl;
    EXPECT_STREQ(getInfo.subject, info.subject) << "assert subject failed!" << endl;
    EXPECT_STREQ(getInfo.mailto[1], info.mailto[1]) << "assert mailto[1] failed!" << endl;
}

//104.获取邮件信息
TEST_F(SgpTest, 104_GetEmilInfo)
{
    SGP_EMAIL_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetEmilInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetEmilInfo failed" << endl;

    cout << "返回成功，获取的邮件信息如下：" << endl;
    cout << "alarm=" << info.alarm << endl;
    cout << "alarm_value=" << info.alarm_value << endl;
    cout << "enclosure=" << info.enclosure << endl;
    cout << "encry_type=" << info.encry_type << endl;
    cout << "health=" << info.health << endl;
    cout << "health_value=" << info.health_value << endl;
}

//105.切换测温范围
TEST_F(SgpTest, 105_SetRange)
{
    SGP_GENERAL_INFO info;
    memset(&info, 0x00, sizeof(SGP_GENERAL_INFO));
    int ret = SGP_GetGeneralInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetGeneralInfo failed" << endl;

    int range = info.range_num;
    ret = SGP_SetRange(handle, range - 1);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetRange failed" << endl;
}

//106.获取系统版本信息
TEST_F(SgpTest, 106_GetVersionInfo)
{
    SGP_VERSION_INFO info;
    memset(&info, 0x00, sizeof(info));
    int ret = SGP_GetVersionInfo(handle, &info);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetVersionInfo failed" << endl;

    cout << "返回成功，获取的系统信息为：" << endl;
    cout << "设备型号：" << info.model << endl;
    cout << "系统版本：" << info.version << endl;
    cout << "序列号：  " << info.serial << endl;
    cout << "FPGA版本：" << info.fpga_version << endl;
    cout << "测温版本：" << info.measure_version << endl;
    cout << "SDK版本： " << info.sdk_version << endl;
}

//107.同步系统时间
//TEST_F(SgpTest, 107_SynchroTime)
//{
//    const char* datetime = "2022-12-11 11:11:11";
//    int ret = SGP_SynchroTime(handle, datetime);
//    EXPECT_EQ(ret, SGP_OK) << "SGP_SynchroTime failed" << endl;
//}

//108.数据清理
TEST_F(SgpTest, 108_ClearData)
{
    int ret = SGP_ClearData(handle);
    EXPECT_EQ(ret, SGP_OK) << "SGP_ClearData failed" << endl;
}

//109.快门操作-打快门
TEST_F(SgpTest, 109_DoShutter)
{
    SGP_SHUTTER_ENUM type = SGP_SHUTTER;
    int ret = SGP_DoShutter(handle, type);
    EXPECT_EQ(ret, SGP_OK) << "SGP_DoShutter failed" << endl;
}

//110.快门操作-快门常关
TEST_F(SgpTest, 110_DoShutter_SGP_SHUTTER_CLOSE)
{
    SGP_SHUTTER_ENUM type = SGP_SHUTTER_CLOSE;
    int ret = SGP_DoShutter(handle, type);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SHUTTER_CLOSE failed" << endl;
}

//111.快门操作-快门常开
TEST_F(SgpTest, 111_DoShutter_SGP_SHUTTER_OPEN)
{
    SGP_SHUTTER_ENUM type = SGP_SHUTTER_OPEN;
    int ret = SGP_DoShutter(handle, type);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SHUTTER_OPEN failed" << endl;
}

//112.快门操作-自动快门
TEST_F(SgpTest, 112_DoShutter_SGP_SHUTTER_AUTO)
{
    SGP_SHUTTER_ENUM type = SGP_SHUTTER_AUTO;
    int ret = SGP_DoShutter(handle, type);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SHUTTER_AUTO failed" << endl;
}

//113.自动调焦
TEST_F(SgpTest, 113_SetFocus_SGP_FOCUS_AUTO)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_AUTO;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_AUTO failed" << endl;
}

//114.远焦微调
TEST_F(SgpTest, 114_SetFocus_SGP_FOCUS_FAR_FINE)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_FAR_FINE;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_FAR_FINE failed" << endl;
}

//115.近焦微调
TEST_F(SgpTest, 115_SetFocus_SGP_FOCUS_NEAR_FINE)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_NEAR_FINE;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_NEAR_FINE failed" << endl;
}

//116.远焦
TEST_F(SgpTest, 116_SetFocus_SGP_FOCUS_FAR)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_FAR;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_FAR failed" << endl;
}

//117.近焦
TEST_F(SgpTest, 118_SetFocus_SGP_FOCUS_NEAR)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_NEAR;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_NEAR failed" << endl;
}

//118.设置电机位置0
TEST_F(SgpTest, 118_SetFocus_SGP_FOCUS_PLACE_0)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_PLACE;
    int value = 0;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_PLACE_0 failed" << endl;

    sleep(5);
    int getValue = 0;
    ret = SGP_GetMotorPosition(handle, &getValue);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetMotorPosition failed" << endl;
    ASSERT_EQ(getValue, value) << "assert SGP_FOCUS_PLACE failed" << endl;
}

//119.设置电机位置750
TEST_F(SgpTest, 119_SetFocus_SGP_FOCUS_PLACE_750)
{
    SGP_FOCUS_TYPE type = SGP_FOCUS_PLACE;
    int value = 750;
    int ret = SGP_SetFocus(handle, type, value);
    EXPECT_EQ(ret, SGP_OK) << "SGP_FOCUS_PLACE_750 failed" << endl;

    sleep(5);
    int getValue = 0;
    ret = SGP_GetMotorPosition(handle, &getValue);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetMotorPosition failed" << endl;
    ASSERT_EQ(getValue, value) << "assert SGP_FOCUS_PLACE failed" << endl;
}

//120.获取电机位置
TEST_F(SgpTest, 120_GetMotorPosition)
{
    int value = 0;
    int ret = SGP_GetMotorPosition(handle, &value);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetMotorPosition failed" << endl;
    cout << "获取的电机位置为：" << value << endl;
}

//121.设置电子变倍3倍
TEST_F(SgpTest, 121_SGP_SetElectronicMagnification_3)
{
    int magnification = 3;
    SGP_VIDEO_PARAM_ENUM type = SGP_IR;
    int ret = SGP_SetElectronicMagnification(handle, type, magnification);
    ASSERT_EQ(ret, SGP_OK) << "SGP_SetElectronicMagnification_3 failed" << endl;
}

//122.设置电子变倍2倍
TEST_F(SgpTest, 122_SGP_SetElectronicMagnification_2)
{
    int magnification = 2;
    SGP_VIDEO_PARAM_ENUM type = SGP_IR;
    int ret = SGP_SetElectronicMagnification(handle, type, magnification);
    ASSERT_EQ(ret, SGP_OK) << "SGP_SetElectronicMagnification_2 failed" << endl;
}

//123.设置电子变倍1倍
TEST_F(SgpTest, 123_SGP_SetElectronicMagnification_1)
{
    int magnification = 1;
    SGP_VIDEO_PARAM_ENUM type = SGP_IR;
    int ret = SGP_SetElectronicMagnification(handle, type, magnification);
    ASSERT_EQ(ret, SGP_OK) << "SGP_SetElectronicMagnification_1 failed" << endl;
}

//124.设置测温点索引数组
TEST_F(SgpTest, 124_SGP_SetTempPoints)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }
    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    EXPECT_EQ(ret, SGP_OK) << "SGP_SetTempPoints failed" << endl;
    free(index);
    index = NULL;
}

//125.获取测温点数组
TEST_F(SgpTest, 125_SGP_GetTempPoints)
{
    const int pointNum = 100;
    int* index = (int*)malloc(pointNum * sizeof(int));
    for (int i = 0; i < pointNum; i++)
    {
        index[i] = i * 100;
    }

    int ret = SGP_SetTempPoints(handle, index, pointNum, 1);
    ASSERT_EQ(ret, SGP_OK) << "SGP_GetTempPoints failed" << endl;
    float* temp = (float*)malloc(pointNum * sizeof(float));
    memset(temp, 0, 10 * sizeof(float));
    if (temp != NULL)
    {
        int ret = SGP_GetTempPoints(handle, temp, pointNum, 1);
        EXPECT_EQ(ret, SGP_OK) << "SGP_GetTempPoints failed" << endl;
    }
    free(temp);
    temp = NULL;
    free(index);
    index = nullptr;
}

//126.系统重启
TEST_F(SgpTest, 126_RebootSystem)
{
    int ret = SGP_RebootSystem(handle);
    EXPECT_EQ(ret, SGP_OK) << "SGP_RebootSystem failed" << endl;
}

//127.恢复出厂设置
//TEST_F(SgpTest, FactoryReset)
//{
//    int ret = SGP_FactoryReset(handle);
//    EXPECT_EQ(ret, SGP_OK) << "SGP_FactoryReset failed";
//}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "SgpTest.093_SetColdHotTrace";
    return RUN_ALL_TESTS();
}








