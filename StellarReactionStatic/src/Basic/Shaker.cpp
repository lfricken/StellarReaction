#include "Shaker.hpp"


Shaker::Shaker() : m_shakeTimer(0.f)
{

}
Shaker::~Shaker()
{

}
Vec2 Shaker::getPosition() const
{
	if(m_samples.size() < 1)//if there are no samples
		return Vec2(0, 0);

	float percentDone = m_shakeTimer.getTimePercentageElapsed();

	if(percentDone >= 1.f || percentDone < 0.f)
		return Vec2(0, 0);



	int sample1 = static_cast<int>(percentDone * m_samples.size());//sample 1 guaranteed to be less than size because it can't be 1 and gets truncated
	//int sample2 = static_cast<int>(sample1 + 1);

	Vec2 sampleStart = m_samples[sample1];//round down
	//Vec2 sampleEnd(0, 0);
	//if(sample2 < m_samples.size())
	//	sampleEnd = m_samples[sample2];//round up

	return sampleStart; //  Vec2((sampleStart.x + sampleEnd.x) / 2.f, (sampleStart.y + sampleEnd.y) / 2.f);
}
void Shaker::generate(float duration, float frequency, float amplitude)
{
	m_samples.clear();
	m_shakeTimer.setCountDown(duration);
	m_shakeTimer.restartCountDown();

	int numSamples = static_cast<int>(duration * frequency);

	for(int i = 0; i < numSamples; ++i)
	{
		float percentRemaining = 1 - (static_cast<float>(i) / static_cast<float>(numSamples));//decrease with time
		m_samples.push_back(generateSample(percentRemaining * amplitude));
	}
}
Vec2 Shaker::generateSample(float maxAmplitude) const
{
	return Vec2(Rand::get(-maxAmplitude, maxAmplitude), Rand::get(-maxAmplitude, maxAmplitude));
}

