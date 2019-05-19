
//
//  Outbound Streamer.cpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/18/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//
#include "Outbound Streamer.hpp"
OutboundStreamer::OutboundStreamer() {
    m_Filter=new StreamerFilter();
    m_ProtocolListen=true;
    m_ProcotolListener=new std::thread([&]{ProtocolListener();});
    m_ProtocolSocket.setBlocking(false);
    m_DataSocket.setBlocking(false);
    m_Status=ConnectionStatus::Disconnected;
}
OutboundStreamer::~OutboundStreamer() {
    Disconnect();
    m_ProtocolListen=false;
    m_ProcotolListener->join();
    delete m_ProcotolListener;
    delete m_Filter;
}
void OutboundStreamer::Connect(sf::IpAddress IP,unsigned short Port) {
    if(m_Status==ConnectionStatus::Disconnected) {
        m_ProtocolSocket.setBlocking(true);
        sf::Socket::Status Status=m_ProtocolSocket.connect(IP,Port);
        m_ProtocolSocket.setBlocking(false);
        if(Status==sf::Socket::Done)
            m_Status=ConnectionStatus::Connected;
    }
}
void OutboundStreamer::Disconnect() {
    m_Status=ConnectionStatus::Disconnected;
    m_ProtocolSocket.disconnect();
}
void OutboundStreamer::InjectSamples(std::vector<sf::Int16>& Samples) {
    if(m_Filter->FilterSamples(Samples))
        SendSamples(Samples);
}
void OutboundStreamer::ProtocolListener() {
    sf::Packet Packet;
    sf::Socket::Status Status;
    while(m_ProtocolListen) {
        if(m_Status==ConnectionStatus::Connected) {
            Packet.clear();
            Status=m_ProtocolSocket.receive(Packet);
            if(Status==sf::Socket::Done) {
            }
            else if(Status==sf::Socket::Error)
                Disconnect();
            else if(Status==sf::Socket::Disconnected)
                m_Status=ConnectionStatus::Disconnected;
        }
        sf::sleep(sf::seconds(0.01));
    }
}
void OutboundStreamer::SendSamples(std::vector<sf::Int16>& Samples) {
    if(m_Status==ConnectionStatus::Connected) {
        sf::Packet DataPacket;
        DataPacket<<(sf::Uint16)Samples.size();
        for(int i=0;i<Samples.size();i++)
            DataPacket<<Samples[i];
        m_DataSocket.send(DataPacket,m_ProtocolSocket.getRemoteAddress(),40001);
    }
}
