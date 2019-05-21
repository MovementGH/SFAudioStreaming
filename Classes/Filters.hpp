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
#endif /* Filters_hpp */
