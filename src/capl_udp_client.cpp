/**
 * @Author: Xia Yunkai
 * @Date:   2024-03-13 19:35:02
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-03-22 18:56:16
 */
#include <iostream>

#include "capl_udp_client.h"
#include "timer_thread.h"
CaplUdpClient::CaplUdpClient() {}
CaplUdpClient::~CaplUdpClient()
{
    closesocket(m_chassisSubServSock);
    closesocket(m_commandPubcliSock);
    // 清理网络环境
    WSACleanup();
}
bool CaplUdpClient::Init(int command_port, int chassis_port)
{

    // 初始化网络环境,windows udp特有的代码段
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return false;
    }
    // 建立套接字
    m_chassisSubServSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_chassisSubServSock == INVALID_SOCKET)
    {
        printf("create m_chassisSubServSock failed\n");
        return false;
    }

    m_commandPubcliSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_commandPubcliSock == INVALID_SOCKET)
    {
        printf("create m_commandPubcliSock failed\n");
        return false;
    }

    memset(&m_cAddr, 0, sizeof(m_cAddr)); // 远端地址初始化
    m_cAddr.sin_family = AF_INET;
    m_cAddr.sin_port = htons(command_port);
    m_cAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 回环地址

    //
    memset(&m_sAddr, 0, sizeof(m_sAddr)); // 本地地址初始化
    m_sAddr.sin_family = AF_INET;
    m_sAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 回环地址
    m_sAddr.sin_port = htons(chassis_port);           // 接受底盘数据的端口号
    // 绑定socket

    if (bind(m_chassisSubServSock, (struct sockaddr *)&m_sAddr, sizeof(m_sAddr)) < 0)
    {

        return false;
    }

    return true;
}

void CaplUdpClient::Run()
{

    // 创建发送数据的定时器 20cm发送一次
    std::unique_ptr<TimerThread> sendTimerThread; // 发送数据的定时器
    sendTimerThread = std::make_unique<TimerThread>();
    sendTimerThread->RegisterCallback(20, std::bind(&CaplUdpClient::SendCallback, this));
    sendTimerThread->Start();

    m_recvThread = std::make_unique<std::thread>(std::bind(&CaplUdpClient::RecvLoop, this));

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

bool CaplUdpClient::GetGetChassisStatus(StatusResponse &statusResponse) const
{
    if (!m_connected)
    {
        return false;
    }
    statusResponse = m_statusResponse;
    return true;
}

void CaplUdpClient::UpdateControlCommand(const ControlCmd &cmd)
{
    m_controlCmd = cmd;
}

void CaplUdpClient::RecvLoop()
{
    std::cout << "RecvLoop" << std::endl;
    int len = sizeof(m_sAddr);
    int32_t n, ret;
    while (true)
    {
        n = recvfrom(m_chassisSubServSock, m_statusResponse.data.raw, sizeof(StatusResponse), 0, (struct sockaddr *)&m_sAddr, &len);

        if (n < 0)
        {
            std::cout << "recvfrom failed" << std::endl;
            m_connected = false;
            continue;
        }
        else if (n != sizeof(StatusResponse))
        {
            std::cout << "recvfrom status error!" << std::endl;
            continue;
        }
        else
        {
            m_connected = true;
        }
    }
}

void CaplUdpClient::SendCallback()
{

    m_controlCmd.data.cmd.comState = m_connected;
    m_controlCmd.data.cmd.typeCode[0] = 'H';
    m_controlCmd.data.cmd.typeCode[1] = 'C';

    // 发送给远程端
    sendto(m_commandPubcliSock, m_controlCmd.data.raw, sizeof(ControlCmd), 0, (struct sockaddr *)&m_cAddr, sizeof(m_cAddr));
}