

#include "Devices.hpp"


InputDeviceStreamer::InputDeviceStreamer() : m_Quality(100) {}
void InputDeviceStreamer::Play() {
    setChannelCount(2);
    setDevice(getDefaultDevice());
    start();
}
void InputDeviceStreamer::Stop() {
    stop();
}
void InputDeviceStreamer::SetQuality(sf::Int16 Quality) {
    m_Quality=Quality;
    setProcessingInterval(sf::milliseconds(Quality));
}
sf::Int16 InputDeviceStreamer::GetQuality() {
    return m_Quality;
}
bool InputDeviceStreamer::onProcessSamples(const sf::Int16* Samples,std::size_t SampleCount)  {
    std::vector<sf::Int16> SampleVec(SampleCount);
    for(int i=0;i<SampleVec.size();i++)
        SampleVec[i]=Samples[i];
    InjectSamples(SampleVec);
    return true;
}


OutputDeviceStreamer::OutputDeviceStreamer() {
    initialize(2,44100);
}
void OutputDeviceStreamer::Play() {
    play();
}
void OutputDeviceStreamer::Stop() {
    stop();
}
bool OutputDeviceStreamer::onGetData(Chunk& Data) {
    GetSampleBuffer(m_Samples,4800);
    if(m_Samples.size()==0)
        m_Samples.resize(4800,0);
    Data.samples=m_Samples.data();
    Data.sampleCount=m_Samples.size();
    return true;
}
void OutputDeviceStreamer::onSeek(sf::Time timeOffset) {
    
}
