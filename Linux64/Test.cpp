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

static void GetIrRtsp(unsigned char* outdata, int w, int h, void* pUser)
{
    //printf("outdata is %s\n", outdata);
    printf("w is %d, h is %d\n", w, h);
    sleep(2);
}
static void GetRecordStatus(int state, void* pUser)
{
    printf("Current record status is %d\n", state);
    sleep(2);
}
static void GetTempNotify(SGP_TEMPALARMNOTIFY notify, void* pUser)
{
    cout << "时间：" << notify.time << "  最高温：" << notify.high_temp <<
        "  平均温：" << notify.avg_temp << "  最低温：" << notify.low_temp << endl;
    cout << "报警截图地址：" << notify.ir_image_url << endl;
    cout << "报警视频地址：" << notify.ir_video_url << endl;
    cout << "名称：" << notify.name << endl;
    if (notify.temp_flag == 0) { cout << "报警类型：平均温报警！" << endl; }
    if (notify.temp_flag == 1) { cout << "报警类型：高温报警！" << endl; }
    if (notify.temp_flag == 2) { cout << "报警类型：低温报警！" << endl; }
    if (notify.temp_flag == 3) { cout << "报警类型：高低温报警！" << endl; }
    if (notify.type == 1) { cout << "温度报警！" << endl; }
    if (notify.type == 2) { cout << "热点报警！" << endl; }
    if (notify.type == 3) { cout << "冷点报警！" << endl; }
    cout << notify.name << endl;
    cout << endl;
}
static void GetAccessNotify(SGP_ACCESSVIOLATIONNOTIFY notify, void* pUser)
{
    printf("异常登录用户名是%s\n", notify.user);
    printf("异常登录IP是%s\n", notify.ip);
    printf("异常登录时间是%s\n", notify.time);
}
static void GetFocusResult(int result, void* pUser)
{
    printf("Focus result is %d\n", result);
}

int main()
{
    SGP_HANDLE handle = 0;
    handle = SGP_InitDevice();
    /*if (handle != 0)
    {
        cout << "初始化成功" << endl;
    }
    else
    {
        cout << "初始化失败" << endl;
    }*/
    //cout << handle << endl;

    const char* server = "192.168.21.4";
    const char* username = "root";
    const char* password = "guide123";
    int port = 80;
    int ret1 = SGP_Login(handle, server, username, password, port);

    if (ret1 == SGP_OK)
    {
        cout << "登陆成功！" << endl;

        SGP_VERSION_INFO info;
        memset(&info, 0x00, sizeof(info));
        SGP_GetVersionInfo(handle, &info);
        cout << "SDK版本： " << info.sdk_version << endl;
        

        

        //视频回调
        //for (int i = 1; i <= 1; i++)
        //{
        //    int result = SGP_OpenIrVideo(handle, GetIrRtsp, 0);
        //    printf("第%d次SGP_OpenIrVideo接口返回值是%d\n", i, result);
        //    sleep(5);
        //    std::string IRFile = "1234.mp4";
        //    int startResult = SGP_StartRecord(handle, SGP_IR_VIDEO, IRFile.c_str(), GetRecordStatus, 0);
        //    printf("SGP_StartRecord接口红外返回值是%d\n", startResult);
        //    sleep(30);
        //    SGP_StopRecord(handle, SGP_IR_VIDEO);
        //    //关闭视频流
        //    SGP_CloseIrVideo(handle);
        //}

        //聚焦回调
       /* SGP_RegisterAutoFocusCallback(handle, GetFocusResult, 0);
        sleep(3000);*/

        //告警回调
        /*SGP_RegisterTempAlarmCallback(handle, GetTempNotify, 0);
        sleep(30000);*/

        //非法访问回调
       /* SGP_RegisterAccessViolationCallback(handle, GetAccessNotify, 0);
        sleep(3000);*/


        int rets = SGP_Logout(handle);
        if (rets == SGP_OK)
        {
            cout << "用户登出成功！" << endl;
        }
        else
        {
            cout << "用户登出失败！" << endl;
        }
        SGP_UnInitDevice(handle);
    }

    else
    {
        printf("登陆失败");
        SGP_UnInitDevice(handle);
    }


}









