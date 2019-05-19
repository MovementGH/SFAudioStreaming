//
//  Outbound Streamer.hpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/18/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//

#ifndef Outbound_Streamer_hpp
#define Outbound_Streamer_hpp
#include <SFML/Network.hpp>
#include <thread>
#include "Filters.hpp"
class OutboundStreamer {
public:
    OutboundStreamer();
    ~OutboundStreamer();
    void Connect(sf::IpAddress IP,unsigned short Port);
    void Disconnect();
    template <typename Filter>
    void SetFilter() {
        delete m_Filter;
        m_Filter=new Filter();
    }
    void InjectSamples(std::vector<sf::Int16>& Samples);
private:
    void SendSamples(std::vector<sf::Int16>& Samples);
    void ProtocolListener();
protected:
    sf::TcpSocket m_ProtocolSocket;
    sf::UdpSocket m_DataSocket;
    std::thread* m_ProcotolListener;
    ConnectionStatus m_Status;
    StreamerFilter* m_Filter;
    bool m_ProtocolListen;
};
#endif /* Outbound_Streamer_hpp */