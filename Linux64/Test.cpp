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
    //cout << pUser << endl;
    sleep(2);
}
static void GetRecordStatus(int state, void* pUser)
{
    //printf("Current record status is %d\n", state);
    sleep(2);
}
static void GetAccessNotify(SGP_ACCESSVIOLATIONNOTIFY notify, void* pUser)
{
    printf("异常登录用户名是%s\n", notify.user);
    printf("异常登录IP是%s\n", notify.ip);
    printf("异常登录时间是%s\n", notify.time);
}
static void MemoryFull(SGP_MEMORYFULLNOTIFY notify, void* pUser)

{
    printf("总存储是%dG\n", notify.total);
    printf("可用大小%dM\n", notify.free);
    printf("报警阈值%dM\n", notify.limit);
}
static void NetworkError(SGP_NETWORKERRORNOTIFY notify, void* pUser)

{
    //std::string ip(notify.ip);
    cout << "类型：" << notify.type << endl;
    cout << "ip:" << notify.ip << endl;
}
static void GetTempNotify(SGP_TEMPALARMNOTIFY notify, void* pUser)
{
    cout << "时间：" << notify.time << "  最高温：" << notify.high_temp <<
        "  最低温：" << notify.low_temp << "  平均温：" << notify.avg_temp <<
        "  温升：" << notify.rate_temp << "  温差：" << notify.diff_temp << endl;
    cout << "报警截图地址：" << notify.ir_image_url << endl;
    cout << "报警视频地址：" << notify.ir_video_url << endl;
    cout << "名称：" << notify.name << endl;
    if (notify.temp_flag == 0) { cout << "报警类型：平均温报警！" << endl; }
    if (notify.temp_flag == 1) { cout << "报警类型：高温报警！" << endl; }
    if (notify.temp_flag == 2) { cout << "报警类型：低温报警！" << endl; }
    if (notify.temp_flag == 3) { cout << "报警类型：高低温报警！" << endl; }
    if (notify.temp_flag == 5) { cout << "报警类型：温升报警！" << endl; }
    if (notify.temp_flag == 6) { cout << "报警类型：温差报警！" << endl; }
    if (notify.type == 1) { cout << "温度报警！" << endl; }
    if (notify.type == 2) { cout << "热点报警！" << endl; }
    if (notify.type == 3) { cout << "冷点报警！" << endl; }
    cout << endl;

    if (notify.config.type == 1) { cout << "高温报警" << endl; }
    if (notify.config.type == 2) { cout << "低温报警" << endl; }
    if (notify.config.type == 3) { cout << "平均温报警" << endl; }
    if (notify.config.type == 4) { cout << "高低温报警" << endl; }
    if (notify.config.condition == 1) { cout << "报警条件：高于" << endl; }
    if (notify.config.condition == 2) { cout << "报警条件：低于" << endl; }
    if (notify.config.condition == 3) { cout << "报警条件：匹配" << endl; }
    cout << "高温阈值：" << notify.config.high_temp << endl;
    cout << "低温阈值：" << notify.config.low_temp << endl;
    cout << "平均温阈值：" << notify.config.avg_temp << endl;
    if (notify.config.objtype == 1) { cout << "点" << endl; }
    if (notify.config.objtype == 2) { cout << "线" << endl; }
    if (notify.config.objtype == 3) { cout << "矩形" << endl; }
    if (notify.config.objtype == 4) { cout << "多边形" << endl; }
    if (notify.config.objtype == 5) { cout << "圆" << endl; }

    for (int i = 0; i < sizeof(notify.config.points) / sizeof(notify.config.points[0]); i++) {
        cout << "第" << i + 1 << "个点的横坐标：" << notify.config.points[i].x << endl;
        cout << "第" << i + 1 << "个点的纵坐标：" << notify.config.points[i].y << endl;
    }
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


    const char* server = "192.168.21.32";
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
        for (int i = 1; i <= 1; i++)
        {
            int result = SGP_OpenIrVideo(handle, GetIrRtsp, 0);
            printf("第%d次SGP_OpenIrVideo接口返回值是%d\n", i, result);
            sleep(5);
            std::string IRFile = "1234.mp4";
            int startResult = SGP_StartRecord(handle, SGP_IR_VIDEO, IRFile.c_str(), GetRecordStatus, 0);
            printf("SGP_StartRecord接口红外返回值是%d\n", startResult);
            sleep(30);
            SGP_StopRecord(handle, SGP_IR_VIDEO);
            //关闭视频流
            SGP_CloseIrVideo(handle);
        }

        //非法访问回调
        SGP_RegisterAccessViolationCallback(handle, GetAccessNotify, 0);

        //内存已满回调
        SGP_RegisterMemoryFullCallback(handle, MemoryFull, 0);

        //网络异常回调
        SGP_RegisterNetworkErrorCallback(handle, NetworkError, 0);

        //温度告警回调
        SGP_RegisterTempAlarmCallback(handle, GetTempNotify, 0);

        //自动调焦回调
        SGP_RegisterAutoFocusCallback(handle, GetFocusResult, 0);

        sleep(99999);

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

