//
//  Devices.hpp
//  SFAudioStreaming
//
//  Created by Mayo Furgerson on 5/18/19.
//  Copyright © 2019 WimMa Games. All rights reserved.
//

#ifndef Devices_hpp
#define Devices_hpp
#include <SFML/Audio.hpp>
#include "Inbound Streamer.hpp"
#include "Outbound Streamer.hpp"

class InputDeviceStreamer : public OutboundStreamer, public sf::SoundBufferRecorder {
public:
    InputDeviceStreamer();
    void Play();
    void Stop();
    void SetQuality(sf::Int16 Quality);
    sf::Int16 GetQuality();
protected:
    virtual bool onProcessSamples(const sf::Int16* Samples,std::size_t SampleCount);
    sf::Int16 m_Quality;
};

class OutputDeviceStreamer : public InboundStreamer, public sf::SoundStream {
public:
    OutputDeviceStreamer();
    void Play();
    void Stop();
protected:
    virtual bool onGetData(Chunk& Data);
    virtual void onSeek(sf::Time timeOffset);
    std::vector<sf::Int16> m_Samples;
    
};

#endif /* Devices_hpp */
