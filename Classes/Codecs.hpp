//
//  Codecs.hpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/19/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//
#ifndef Codecs_hpp
#define Codecs_hpp
#include <SFML/System.hpp>
#include <opus.h>
#include <vector>
class StreamerCodec {
public:
    virtual void Encode(std::vector<sf::Int16>& Samples);
    virtual void Decode(std::vector<sf::Int16>& Samples);
};

class OpusCodec : public StreamerCodec {
public:
    OpusCodec();
    ~OpusCodec();
    virtual void Encode(std::vector<sf::Int16>& Samples);
    virtual void Decode(std::vector<sf::Int16>& Samples);
    OpusEncoder* m_Encoder;
    OpusDecoder* m_Decoder;
    std::vector<unsigned char> m_Samples;
    std::vector<sf::Int16> m_Buffer;
};
#endif /* Codecs_hpp */
