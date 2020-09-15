/**
 * @file udpserver.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "udpserver.h"

WiFiUDP UDPServer::udpServer;
IPAddress UDPServer::ip_local;
IPAddress UDPServer::ip_remote;
IPAddress UDPServer::sub_local;
uint32_t UDPServer::udp_local_port;
uint32_t UDPServer::udp_remote_port;
bool UDPServer::broadcast;

/**
 * @brief Construct a new UDPServer::UDPServer object
 * 
 * @param udp_local_port 
 * @param broadcast 
 * @param udp_remote_port 
 * @param ip_remote 
 */
UDPServer::UDPServer(uint32_t udp_local_port, bool broadcast, uint32_t udp_remote_port, IPAddress ip_remote)
{
    UDPServer::udp_local_port = udp_local_port;
    UDPServer::broadcast = broadcast;
    UDPServer::udp_remote_port = udp_remote_port;
    UDPServer::ip_remote = ip_remote;
}

/**
 * @brief Initialize the UDPServer:: Object as singleton
 * 
 * @param udp_local_port 
 * @param broadcast 
 * @param udp_remote_port 
 * @param ip_remote 
 * @return UDPServer* 
 */
UDPServer *UDPServer::init(uint32_t udp_local_port, bool broadcast, uint32_t udp_remote_port, IPAddress ip_remote)
{
    static UDPServer udp(udp_local_port, broadcast, udp_remote_port, ip_remote);
    return &udp;
}

/**
 * @brief Start the udp server
 * 
 * @param ip_local 
 * @param sub_local 
 */
void UDPServer::start(IPAddress ip_local, IPAddress sub_local)
{
    UDPServer::ip_local = ip_local;
    UDPServer::sub_local = sub_local;

    if (UDPServer::broadcast)
    {
        UDPServer::ip_remote = ip_local | (~sub_local);
        UDPServer::udp_remote_port = udp_remote_port;
    }

    udpServer.begin(ip_local, udp_local_port);
}

/**
 * @brief Receive data via udp and return as string in receiveData
 * 
 * @param receiveData 
 */
void UDPServer::receiveUDP(String *receiveData)
{

    int packetSize = udpServer.parsePacket();
    if (packetSize)
    {
        constexpr int BUFFER_LEN = 512;
        char buffer[BUFFER_LEN];
        if (packetSize >= BUFFER_LEN)
        {
            Serial.printf("udp rx ERROR packetSize (%d) >= BUFFER_LEN (%d)",
                          packetSize, BUFFER_LEN);
        }

        int len = udpServer.read(buffer, BUFFER_LEN - 1);
        buffer[len + 1] = '\0';
        *receiveData = buffer;

        if (!broadcast)
            ip_remote = udpServer.remoteIP(); // Get IP Address from Sender and use unicast communication 

        udp_remote_port = udpServer.remotePort();
        // Serial.printf("udp rx %s:%d %d/%d %s \n", udpServer.remoteIP().toString().c_str(), udpServer.remotePort(), packetSize, len, buffer);
        // Serial.printf("Receive UDP IP:Port %s:%d %s \n", udpServer.remoteIP().toString().c_str(), udpServer.remotePort(), buffer);
    }
}

/**
 * @brief Send data via udp
 * 
 * @param sendData 
 */
void UDPServer::sendUDP(String sendData)
{
    
    if (ip_remote && udp_remote_port)
    {
        // Serial.printf("Send UDP: %s\n", sendData.c_str());
        udpServer.beginPacket(ip_remote, udp_remote_port);
        udpServer.printf("%s", sendData.c_str());
        udpServer.endPacket();
        
    }
}