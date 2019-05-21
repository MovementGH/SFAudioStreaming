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
