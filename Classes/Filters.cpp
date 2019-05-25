
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
