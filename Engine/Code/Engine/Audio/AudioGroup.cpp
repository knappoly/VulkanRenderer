#include "Engine/Audio/AudioGroup.hpp"
//-----------------------------------------------------------------------------------------------
// Engine Includes
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Math/MathUtils.hpp"
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
// Constructor
//
AudioGroup::AudioGroup(const XMLElement& element)
{
	m_name = ParseXmlAttribute(element, "name", m_name);

	const XMLElement* clipElement = element.FirstChildElement("clip");
	GUARANTEE_OR_DIE(clipElement, "Audio group doesn't contain clips");

	for(; clipElement; clipElement = clipElement->NextSiblingElement("clip"))
	{
		std::string clipPath = "INVALID";
		clipPath = ParseXmlAttribute(*clipElement, "src", clipPath);
		GUARANTEE_OR_DIE(clipPath != "INVALID", "Audio clip path not specified");

		SoundID audioClip = AudioSystem::GetInstance()->CreateOrGetSound(clipPath);
		m_soundIDs.push_back(audioClip);
	}
}

//-----------------------------------------------------------------------------------------------
// Plays one sound from the group by choosing a random sound from the list
//
size_t AudioGroup::PlayOneOffFromGroup()
{
	int random = GetRandomIntInRange(0, (int) m_soundIDs.size() - 1);
	return AudioSystem::GetInstance()->PlaySound(m_soundIDs[random]);
}
