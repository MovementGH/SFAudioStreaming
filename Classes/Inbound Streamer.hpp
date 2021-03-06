

#ifndef Inbound_Streamer_hpp
#define Inbound_Streamer_hpp
#include <SFML/Network.hpp>
#include <thread>
#include "Filters.hpp"
#include "Codecs.hpp"
class InboundStreamer {
public:
    InboundStreamer();
    ~InboundStreamer();
    void Listen(unsigned short Port);
    void Disconnect();
    template<typename Filter> void SetFilter(Filter* FilterPtr=nullptr) { delete m_Filter; if(!FilterPtr) m_Filter=new Filter(); else m_Filter=FilterPtr; }
    template<typename Codec> void SetCodec(Codec* CodecPtr=nullptr) {
        delete m_Codec;
        if(!CodecPtr)
            m_Codec=new Codec();
        else
            m_Codec=CodecPtr;
        
    }
    void GetSampleBuffer(std::vector<sf::Int16>& Samples,sf::Uint16 Amount);
    void SetBufferSize(sf::Uint8 Size);
    sf::Uint8 GetBufferSize();
private:
    void ProtocolListener();
    void DataListener();
protected:
    sf::TcpSocket m_ProtocolSocket;
    sf::UdpSocket m_DataSocket;
    std::thread* m_ProtocolListener;
    std::thread* m_DataListener;
    std::thread* m_ConnectionListener;
    ConnectionStatus m_Status;
    StreamerFilter* m_Filter;
    StreamerCodec* m_Codec;
    bool m_ProtocolListen;
    bool m_DataListen;
    bool m_UsingBuffer;
    bool m_Listening;
    sf::Uint8 m_BufferSize;
    std::vector<sf::Int16> m_SampleBuffer;
};
#endif /* Inbound_Streamer_hpp */
