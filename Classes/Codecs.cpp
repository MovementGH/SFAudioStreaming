//
//  Codecs.cpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/19/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//
#include "Codecs.hpp"
void StreamerCodec::Encode(std::vector<sf::Int16>& Samples) {
    
}
void StreamerCodec::Decode(std::vector<sf::Int16>& Samples) {
    
}


OpusCodec::OpusCodec(int m_Bitrate) {
    int Error=0;
    m_Encoder=opus_encoder_create(48000,2,OPUS_APPLICATION_AUDIO,&Error);
    opus_encoder_ctl(m_Encoder,OPUS_SET_BITRATE(m_Bitrate));
    m_Decoder=opus_decoder_create(48000,2,&Error);
    m_Buffer.resize(0);
    m_Samples.resize(1000);
}
OpusCodec::~OpusCodec() {
    opus_encoder_destroy(m_Encoder);
    opus_decoder_destroy(m_Decoder);
}
void OpusCodec::Encode(std::vector<sf::Int16>& Samples) {
    Samples.insert(Samples.begin(),m_Buffer.begin(),m_Buffer.end());
    int FrameSize=(Samples.size()>=5760)?2880:((Samples.size()>=3840)?1920:((Samples.size()>=1920?960:((Samples.size()>=960?480:0)))));
    if(FrameSize==0) return;
    m_Buffer.clear();
    m_Buffer.insert(m_Buffer.begin(),Samples.begin()+FrameSize*2,Samples.end());
    Samples.erase(Samples.begin()+FrameSize*2,Samples.end());
    sf::Uint32 DataSize=opus_encode(m_Encoder,Samples.data(),FrameSize,m_Samples.data(),(int)m_Samples.size());
    Samples.erase(Samples.begin()+DataSize/2+2,Samples.end());
    Samples[0]=FrameSize,Samples[1]=DataSize;
    for(int i=0;i<Samples.size()-2;i++)
        Samples[i+2]=(m_Samples[i*2]+m_Samples[i*2+1]*256)-32768;
}
void OpusCodec::Decode(std::vector<sf::Int16>& Samples) {
    for(int i=0;i<Samples.size()-2;i++)
        m_Samples[i*2]=(Samples[i+2]+32768)%256,
        m_Samples[i*2+1]=(Samples[i+2]+32768-m_Samples[i*2])/256;
    Samples.resize(Samples[0]*2);
    opus_decode(m_Decoder,m_Samples.data(),Samples[1],Samples.data(),Samples[0],0);
}
