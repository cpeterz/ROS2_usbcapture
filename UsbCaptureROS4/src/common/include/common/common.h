#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <functional>
#include <fstream>

#include <unistd.h>
#include <sys/time.h>
#include <sys/statfs.h>
#include <termio.h>

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#define PI 3.14159265358
#define HINT                                                        \
    std::cout << _warning("<<<<<\t" + std::string(__FILE__) + ":" + \
                          std::to_string(__LINE__) + "\t>>>>>") +   \
                     "\n";
#ifndef CV_Assert
#define CV_Assert(expr)                                                          \
    do                                                                           \
    {                                                                            \
        if (!!(expr))                                                            \
            ;                                                                    \
        else                                                                     \
            cv::error(cv::Error::StsAssert, #expr, CV_Func, __FILE__, __LINE__); \
    } while (0)
#endif
namespace wmj
{

    unsigned int getTimeUSecNow();

    double now();

    //颜色定义： 红色 _RED = 0, 蓝色 _BLUE = 1, 白色 _WHITE = 2
    enum _COLOR
    {
        _RED = 0,
        _BLUE = 1,
        _WHITE = 2
    };

    /*装甲板大小定义： 未识别 ARMOR_NONE = 0, 小装甲板 ARMOR_SMALL = 1, 大装甲板
     * ARMOR_LARGE = 2*/
    enum ARMORTYPE
    {
        ARMOR_NONE = 0,
        ARMOR_SMALL = 1,
        ARMOR_LARGE = 2
    };

    /*识别方式定义： 左相机识别到的 DETECT_LEFT = 0, 右相机识别到的 DETECT_RIGHT =
     * 1, 双目识别到的（即匹配成功） DETECT_DOUBLE = 2*/
    enum DETECTEDTYPE
    {
        DETECT_LEFT = 0,
        DETECT_RIGHT = 1,
        DETECT_DOUBLE = 2
    };

    //识别类型定义： 未识别到 NOTFIND = 0, 第一次识别到 FIRST = 1, 射击 SHOOT = 2
    enum FINDTYPE
    {
        NOTFIND = 0,
        FIRST = 1,
        SHOOT = 2
    };

    //状态定义： 寻找 SEARCHING = 0, 追踪 TRACKING = 2,
    //缓冲，当有一帧未识别的时候，使用上一次识别到的帧作为ROI BUFFER = 1
    enum STATE
    {
        SEARCHING = 0,
        TRACKING = 2,
        BUFFER = 1
    };

    /*识别状态定义： 单目调试 Debug = 0, 单目 Single = 1, 双目 Double = 2, 双目调试
     * DoubleDebug = 3, 打哨兵 SentryAttack = 4, 哨兵专用 SentryUse = 5*/
    enum DETECTSTATE
    {
        Debug = 0,
        Single = 1,
        Double = 2,
        DoubleDebug = 3,
        SentryAttack = 4,
        SentryUse = 5
    };

    class Rate
    {
    public:
        Rate();
        Rate(unsigned int);
        double m_start_time_with_usec;

        void sleep();

    private:
        int m_LoopRate;
    };

    class MatWithTime
    {
    public:
        MatWithTime();
        MatWithTime(cv::Mat, double, std::string);
        ~MatWithTime();

        double m_time_stamp;
        std::string m_orientation;
        cv::Mat m_img;
    };

    struct GimbalPose
    {
        float pitch;
        float yaw;
        float roll;
        double timestamp;

        GimbalPose(float pitch = 0.0, float yaw = 0.0, float roll = 0.0)
        {
            this->pitch = pitch;
            this->yaw = yaw;
            this->roll = roll;
        }
        constexpr GimbalPose(const GimbalPose& other)
        :   pitch(other.pitch),
            yaw(other.yaw),
            roll(other.roll),
            timestamp(other.timestamp)
        {}

        GimbalPose operator=(const GimbalPose &gm)
        {
            this->pitch = gm.pitch;
            this->yaw = gm.yaw;
            this->roll = gm.roll;
            this->timestamp = gm.timestamp;
            return *this;
        }

        GimbalPose operator=(const float init_value)
        {
            this->pitch = init_value;
            this->yaw = init_value;
            this->roll = init_value;
            this->timestamp = wmj::now();
            return *this;
        }

        friend GimbalPose operator-(const GimbalPose &gm1, const GimbalPose gm2)
        {
            GimbalPose temp{};
            temp.pitch = gm1.pitch - gm2.pitch;
            temp.yaw = gm1.yaw - gm2.yaw;
            temp.roll = gm1.roll - gm2.roll;
            temp.timestamp = wmj::now();
            return temp;
        }

        friend GimbalPose operator+(const GimbalPose &gm1, const GimbalPose gm2)
        {
            GimbalPose temp{};
            temp.pitch = gm1.pitch + gm2.pitch;
            temp.yaw = gm1.yaw + gm2.yaw;
            temp.roll = gm1.roll + gm2.roll;
            temp.timestamp = wmj::now();
            return temp;
        }

        friend GimbalPose operator*(const GimbalPose &gm, const float k)
        {
            GimbalPose temp{};
            temp.pitch = gm.pitch * k;
            temp.yaw = gm.yaw * k;
            temp.roll = gm.roll * k;
            temp.timestamp = wmj::now();
            return temp;
        }

        friend GimbalPose operator*(const float k, const GimbalPose &gm)
        {

            GimbalPose temp{};
            temp.pitch = gm.pitch * k;
            temp.yaw = gm.yaw * k;
            temp.roll = gm.roll * k;
            temp.timestamp = wmj::now();
            return temp;
        }

        friend GimbalPose operator/(const GimbalPose &gm, const float k)
        {
            GimbalPose temp{};
            temp.pitch = gm.pitch / k;
            temp.yaw = gm.yaw / k;
            temp.roll = gm.roll / k;
            temp.timestamp = wmj::now();
            return temp;
        }

        friend std::ostream &operator<<(std::ostream &out, const GimbalPose &gm)
        {
            out << "[pitch : " << gm.pitch << ", yaw : " << gm.yaw << "]";
            return out;
        }

        float norm()
        {
            return sqrt(pow(this->pitch, 2) + pow(this->yaw, 2));
        }
    };

    /**
     * @brief 主要是为了解决雷达测量各部分运行时间做的，
     * 能分别记录并统一显示各个模块中设置的过程所用时间，适用于运行时间长且线程多导致输出时间时混乱的问题
     * 各个函数的运行效率比较差，延迟在0.18~0.5ms不等，且有点不稳定
     * 不过总体上还是挺好用的
     * eg:
     * module a{
     *  function{
     *      timetester.start("a process_1");
     *
     *          code......
     *      timetester.end("a process_1");
     *      code....  //运行结束
     *      timetester.printInfo();
     *  }
     * }
     */
    class TimeTester
    {
    private:
        struct Time
        {
            long start; //开始时间ms
            long end;   //结束时间ms
        };
        std::map<std::string, std::map<std::string, struct Time>> m_debug_time_info;
        std::vector<std::string> m_color_info;
        std::mutex m_timetester_mutex;
        bool split(std::string str, std::vector<std::string> &str_vec);
        double now(); //获得ms时间
    public:
        TimeTester();
        ~TimeTester() = default;
        /**
         * @brief 记录label代表的过程开始的时间
         *
         * @param label 过程名
         * @return true 录入时间成功
         * @return false 时间录入失败
         */
        bool start(std::string label);
        /**
         * @brief 记录label代表的过程结束的时间
         *
         * @param label 过程名，必须和start的相同
         * @return true 录入时间成功
         * @return false 时间录入失败
         */
        bool end(std::string label);
        /**
         * @brief 记录label_end过程的结束时间和label_start开始时间
         *
         * @param label_end 结束的过程名
         * @param label_start 开始的过程名
         * @return true 记录成功
         * @return false 记录失败
         */
        bool both(std::string label_end, std::string label_start);
        /**
         * @brief both(std::string label_end, std::string label_start)的重载函数，当label部分一样时可以少写一点
         *
         * @param label label_start 和 label_end的公共部分
         * @param _end _end部分
         * @param _start _start部分
         * @return true 记录成功
         * @return false 记录失败
         */
        bool both(std::string label, std::string _end, std::string _start);
        /**
         * @brief 打印所有时间记录
         *
         */
        void printInfo();
    };

    class MemoryMonitor
    {
    private:
        uint64_t m_mem = 0;
        uint64_t m_free_mem = 0;

    public:
        long get_available_memory();
    };

} // namespace wmj

void monitorKeyboard(char *);

std::string _red(std::string content);
std::string _lightred(std::string content);
std::string _green(std::string content);
std::string _lightgreen(std::string content);
std::string _yellow(std::string content);
std::string _brown(std::string content);
std::string _blue(std::string content);
std::string _lightblue(std::string content);
std::string _purple(std::string content);
std::string _lightpurple(std::string content);
std::string _cyan(std::string content);
std::string _lightcyan(std::string content);
std::string _white(std::string content);
std::string _warning(std::string content);
std::string _underline(std::string content);
std::string _reverse(std::string content);

void _clear();

// rad to degree
inline double R2D(double rad)
{
    return rad * 180 / PI;
}

// degree to rad
inline double D2R(double degree)
{
    return degree * PI / 180;
}

inline double getDistance(cv::Point2d p)
{
    return sqrt(pow(p.x, 2) + pow(p.y, 2));
}

inline double getDistance(cv::Point3d p, bool flag = true)
{
    return sqrt(pow(p.x, 2) + pow(p.y, 2) + pow(p.z, 2) * (int)flag);
}
