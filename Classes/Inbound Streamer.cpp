
#include "Inbound Streamer.hpp"
InboundStreamer::InboundStreamer() {
    m_Filter=new StreamerFilter();
    m_Codec=new StreamerCodec();
    m_ProtocolListen=true;
    m_DataListen=true;
    m_ProtocolListener=new std::thread([&]{ProtocolListener();});
    m_DataListener=new std::thread([&]{DataListener();});
    m_ConnectionListener=nullptr;
    m_ProtocolSocket.setBlocking(false);
    m_DataSocket.setBlocking(false);
    m_DataSocket.bind(40001);
    m_UsingBuffer=false;
    m_Listening=false;
    m_Status=ConnectionStatus::Disconnected;
}
InboundStreamer::~InboundStreamer() {
    Disconnect();
    m_ProtocolListen=false;
    m_DataListen=false;
    m_ProtocolListener->join();
    m_DataListener->join();
    delete m_ProtocolListener;
    delete m_DataListener;
    delete m_Filter;
    delete m_Codec;
    if(m_ConnectionListener)
        delete m_ConnectionListener;
}
void InboundStreamer::Listen(unsigned short Port) {
    if(!m_Listening) {
        m_Listening=true;
        m_ConnectionListener=new std::thread([&]{
            sf::TcpListener Listener;
            Listener.setBlocking(true);
            sf::Socket::Status Status=Listener.listen(Port);
            if(Status==sf::Socket::Done) {
                Listener.setBlocking(false);
                while(m_ProtocolListen) {
                    if(m_Status==ConnectionStatus::Disconnected) {
                        Status=Listener.accept(m_ProtocolSocket);
                        if(Status==sf::Socket::Done)
                            m_Status=ConnectionStatus::Connected;
                    }
                    sf::sleep(sf::seconds(0.01));
                }
            }
        });
    }
}
void InboundStreamer::Disconnect() {
    m_ProtocolSocket.disconnect();
    m_Status=ConnectionStatus::Disconnected;
}
void InboundStreamer::SetBufferSize(sf::Uint8 Size) {
    m_BufferSize=Size;
}
sf::Uint8 InboundStreamer::GetBufferSize() {
    return m_BufferSize;
}
void InboundStreamer::GetSampleBuffer(std::vector<sf::Int16>& Samples,sf::Uint16 Amount) {
    while(m_UsingBuffer){sf::sleep(sf::seconds(0.001));}
    m_UsingBuffer=true;
    Samples.resize(std::min(Amount,(sf::Uint16)m_SampleBuffer.size()));
    for(int i=0;i<Samples.size();i++)
        Samples[i]=m_SampleBuffer[i];
    m_SampleBuffer.erase(m_SampleBuffer.begin(),m_SampleBuffer.begin()+Samples.size());
    m_UsingBuffer=false;
}
void InboundStreamer::ProtocolListener() {
    sf::Socket::Status Status;
    sf::Packet Packet;
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
void InboundStreamer::DataListener() {
    sf::Socket::Status Status;
    sf::Packet Packet;
    sf::IpAddress IP;
    unsigned short Port;
    while(m_DataListen) {
        if(m_Status==ConnectionStatus::Connected) {
            Packet.clear();
            Status=m_DataSocket.receive(Packet,IP,Port);
            if(Status==sf::Socket::Done) {
                sf::Uint16 SampleCount;
                Packet>>SampleCount;
                std::vector<sf::Int16> Samples;
                Samples.resize(SampleCount);
                for(int i=0;i<Samples.size();i++)
                    Packet>>Samples[i];
                m_Codec->Decode(Samples);
                if(m_Filter->FilterSamples(Samples)) {
                    while(m_UsingBuffer){sf::sleep(sf::seconds(0.001));}
                    m_UsingBuffer=true;
                    m_SampleBuffer.resize(m_SampleBuffer.size()+Samples.size());
                    for(int i=0;i<Samples.size();i++)
                        m_SampleBuffer[i+m_SampleBuffer.size()-Samples.size()]=Samples[i];
                    if(m_SampleBuffer.size()>Samples.size()*m_BufferSize)
                        m_SampleBuffer.erase(m_SampleBuffer.begin(),m_SampleBuffer.begin()+(m_SampleBuffer.size()-(Samples.size()*m_BufferSize)));
                    m_UsingBuffer=false;
                }
            }
        }
        sf::sleep(sf::seconds(0.01));
    }
}
