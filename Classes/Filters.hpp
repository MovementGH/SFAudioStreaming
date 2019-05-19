//
//  Filters.hpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/18/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//

#ifndef Filters_hpp
#define Filters_hpp
#include <SFML/System.hpp>
#include <opus.h>
#include <vector>

enum ConnectionStatus {
    Disconnected,
    Connecting,
    Connected
};

class StreamerFilter {
public:
    virtual bool FilterSamples(std::vector<sf::Int16>& Samples);
};

class BandwidthFilter : public StreamerFilter {
public:
    virtual bool FilterSamples(std::vector<sf::Int16>& Samples);
};

class OpusEncoderFilter : public StreamerFilter {
public:
    OpusEncoderFilter();
    ~OpusEncoderFilter();
    virtual bool FilterSamples(std::vector<sf::Int16>& Samples);
    OpusEncoder* m_Encoder;
    std::vector<unsigned char> m_Samples;
    std::vector<sf::Int16> m_Buffer;
};

class OpusDecoderFilter: public StreamerFilter {
public:
    OpusDecoderFilter();
    ~OpusDecoderFilter();
    virtual bool FilterSamples(std::vector<sf::Int16>& Samples);
    OpusDecoder* m_Decoder;
    std::vector<unsigned char> m_Samples;
};
#endif /* Filters_hpp */
