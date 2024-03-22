/**
 * @Author: Xia Yunkai
 * @Date:   2024-03-13 19:32:51
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-03-22 18:51:13
 */
#include <stdint.h>

#ifndef __CAPL_UDP_H__
#define __CAPL_UDP_H__

#include <winsock2.h>
#include <windows.h>

#include <mutex>
#include <memory>

// 连接静态库
#pragma comment(lib, "ws2_32.lib")

#include "chassis_protocol.h"

// 使用udp，与capl建立通信，实现数据的接受与发送
// 发布控制数据，接受底盘数据

class CaplUdpClient
{
public:
    CaplUdpClient();
    ~CaplUdpClient();
    bool Init(int command_port = 5001, int chassis_port = 5002);

    void Run();

    /**
     * @brief 更新控制的数据
     * @param cmd
     */
    void UpdateControlCommand(const ControlCmd &cmd);
    /**
     * @brief 获取底盘数据
     * @param statusResponse
     * @return true接收到信息
     */
    bool GetGetChassisStatus(StatusResponse &statusResponse) const;

private:
    /**
     * @brief 接受数据的线程
     *
     */
    void RecvLoop();

    /**
     * @brief 发送控制量数据的线程
     */
    void SendCallback();

private:
    SOCKET m_chassisSubServSock;     // 服务器套接字，用于接受底盘数据
    SOCKET m_commandPubcliSock;      // 客户端套接字，用于发送控制数据
    struct sockaddr_in m_sAddr;      // 本地地址
    struct sockaddr_in m_cAddr;      // 远程地址
    ControlCmd m_controlCmd;         // 发送的数据
    std::mutex m_sendMutex;          // 数据发送的锁
    StatusResponse m_statusResponse; // 保存的底盘数据
    std::unique_ptr<std::thread> m_recvThread;
    bool m_connected = false; // 连接客户端
};
#endif /* __CAPL_UDP_H__ */
