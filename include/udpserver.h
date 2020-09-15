/**
 * @file udpserver.h
 * @author Philipp Köhler
 * @brief Provides an interface to control the udp server
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <ArduinoJson.h>
#include <WiFi.h>


class UDPServer
{
public:
    static UDPServer *init(uint32_t udp_local_port, bool broadcast = false, uint32_t udp_remote_port = 0, IPAddress ip_remote = IPAddress());
    void start(IPAddress ip_local, IPAddress sub_local);
    void receiveUDP(String *receiveData);
    void sendUDP(String sendData);

private:
    UDPServer(uint32_t udp_local_port, bool broadcast, uint32_t udp_remote_port, IPAddress ip_remote);

    static WiFiUDP udpServer;

    static IPAddress ip_local;
    static IPAddress sub_local;
    static uint32_t udp_local_port;

    static IPAddress ip_remote;
    static uint32_t udp_remote_port;

    static bool broadcast;
};

#endif //UDPSERVER_H