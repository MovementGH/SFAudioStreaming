//
//  Filters.cpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/18/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//

#include "Filters.hpp"


bool StreamerFilter::FilterSamples(std::vector<sf::Int16>& Samples) {
    return true;
}


bool BandwidthFilter::FilterSamples(std::vector<sf::Int16>& Samples) {
    for(int i=0;i<Samples.size();i++)
        if(Samples[i]>100||Samples[i]<-100)
            return true;
    return false;
}


OpusEncoderFilter::OpusEncoderFilter() {
    int Error=0;
    m_Encoder=opus_encoder_create(48000,2,OPUS_APPLICATION_AUDIO,&Error);
    opus_encoder_ctl(m_Encoder,OPUS_SET_BITRATE(64000));
    m_Buffer.resize(10,0);
    m_Samples.resize(1000);
    
}
OpusEncoderFilter::~OpusEncoderFilter() {
    opus_encoder_destroy(m_Encoder);
}
bool OpusEncoderFilter::FilterSamples(std::vector<sf::Int16>& Samples) {
    Samples.insert(Samples.begin(),m_Buffer.begin(),m_Buffer.end());
    int FrameSize=(Samples.size()>=5760)?2880:((Samples.size()>=3840)?1920:((Samples.size()>=1920?960:((Samples.size()>=960?480:0)))));
    if(FrameSize==0)
        return false;
    m_Buffer.resize(0);
    m_Buffer.insert(m_Buffer.begin(),Samples.begin()+FrameSize*2,Samples.end());
    Samples.resize(FrameSize*2);
    sf::Uint32 EncodeSize=opus_encode(m_Encoder,Samples.data(),FrameSize,m_Samples.data(),(int)m_Samples.size());
    Samples.resize((EncodeSize/2)+2);
    Samples[0]=FrameSize;
    Samples[1]=EncodeSize;
    for(int i=0;i<Samples.size()-2;i++)
        Samples[i+2]=(m_Samples[i*2]+m_Samples[i*2+1]*256)-32768;
    return true;
}


OpusDecoderFilter::OpusDecoderFilter() {
    int Error;
    m_Decoder=opus_decoder_create(48000,2,&Error);
    m_Samples.resize(1000);
}
OpusDecoderFilter::~OpusDecoderFilter() {
    opus_decoder_destroy(m_Decoder);
}
bool OpusDecoderFilter::FilterSamples(std::vector<sf::Int16>& Samples) {
    for(int i=0;i<Samples.size()-2;i++)
        m_Samples[i*2]=(Samples[i+2]+32768)%256,
        m_Samples[i*2+1]=((Samples[i+2]+32768)-m_Samples[i*2])/256;
    Samples.resize(Samples[0]*2);
    opus_decode(m_Decoder,m_Samples.data(),Samples[1],Samples.data(),Samples[0],0);
    return true;
}
