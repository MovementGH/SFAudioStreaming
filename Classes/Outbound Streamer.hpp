

#ifndef Outbound_Streamer_hpp
#define Outbound_Streamer_hpp
#include <SFML/Network.hpp>
#include <thread>
#include "Filters.hpp"
#include "Codecs.hpp"
class OutboundStreamer {
public:
    OutboundStreamer();
    ~OutboundStreamer();
    void Connect(sf::IpAddress IP,unsigned short Port);
    void Disconnect();
    template<typename Filter> void SetFilter(Filter* FilterPtr=nullptr) { delete m_Filter; if(!FilterPtr) m_Filter=new Filter(); else m_Filter=FilterPtr; }
    template<typename Codec> void SetCodec(Codec* CodecPtr=nullptr) { delete m_Codec; if(!CodecPtr) m_Codec=new Codec(); else m_Codec=CodecPtr; }
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
    StreamerCodec* m_Codec;
    bool m_ProtocolListen;
};
#endif /* Outbound_Streamer_hpp */
