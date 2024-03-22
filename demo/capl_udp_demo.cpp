/**
 * @Author: Xia Yunkai
 * @Date:   2024-03-13 19:35:15
 * @Last Modified by:   Xia Yunkai
 * @Last Modified time: 2024-03-22 18:49:29
 */
#include <iostream>
#include "capl_udp_client.h"
#include <memory>
using namespace std;

int main(int argc, char const *argv[])
{
    std::unique_ptr<CaplUdpClient> udp_server(new CaplUdpClient);
    if (!udp_server->Init())
    {
        std::cout << "failed to init " << std::endl;
        return 1;
    }
    ControlCmd cmd;
    
    cmd.data.cmd.typeCode[0] = 'X';
    cmd.data.cmd.counter = 10;
    cmd.data.cmd.controlType = 100;
    cmd.data.cmd.tar_torque = -1000;
    cmd.data.cmd.tar_speed = -200;
    cmd.data.cmd.tar_acc = -1000;
    udp_server->UpdateControlCommand(cmd);

    udp_server->Run();
    return 0;
}
