#include "Engine/Renderer/IsoSpriteAnimSetDefinition.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/IsoSpriteAnimDefinition.hpp"
#include "Engine/Console/DevConsole.hpp"

typedef tinyxml2::XMLDocument XMLDocument;

//-----------------------------------------------------------------------------------------------
// Static globals
std::map<std::string, IsoSpriteAnimSetDefinition*> IsoSpriteAnimSetDefinition::s_definitions;

//-----------------------------------------------------------------------------------------------
// Constructor
//
IsoSpriteAnimSetDefinition::IsoSpriteAnimSetDefinition(const XMLElement& element)
{
	m_setName = ParseXmlAttribute(element, "id", m_setName);
	GUARANTEE_OR_DIE( m_setName != "Invalid ID", Stringf("AnimSetDefinition name not specified at line:%d", element.GetLineNum()).c_str());

	for(const XMLElement* defElement = element.FirstChildElement("set"); defElement; defElement = defElement->NextSiblingElement("set"))
	{
		std::string animAliasName = "Invalid ID";
		std::string actualAnimName = "Invalid ID";
		const XMLElement* setElement = nullptr;

		animAliasName = ParseXmlAttribute(*defElement, "id", animAliasName);
		GUARANTEE_OR_DIE( animAliasName != "Invalid ID", Stringf("Anim Alias name not specified at line:%d", defElement->GetLineNum()).c_str());

		setElement = defElement->FirstChildElement("anim");
		GUARANTEE_OR_DIE(setElement != nullptr, Stringf("Actual anim name not specified for alias:%s", animAliasName.c_str()).c_str())

		actualAnimName = ParseXmlAttribute(*setElement, "id", actualAnimName);
		GUARANTEE_OR_DIE(  actualAnimName != "Invalid ID", Stringf("Actual anim name not specified for alias:%s", animAliasName.c_str()).c_str());

		std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = m_namedAnimDefs.find(animAliasName);
		GUARANTEE_OR_DIE( found == m_namedAnimDefs.end(), Stringf("Duplicate entry for aliasName:%s", animAliasName.c_str()).c_str());

		IsoSpriteAnimDefinition* animDef = IsoSpriteAnimDefinition::GetDefinition(actualAnimName);
		GUARANTEE_OR_DIE( animDef != nullptr, Stringf("Animation specified in alias:%s not found", animAliasName.c_str()).c_str());
		
		m_namedAnimDefs[animAliasName] = animDef;
	}
}

//-----------------------------------------------------------------------------------------------
// Initializes definitions from an xml file
//
void IsoSpriteAnimSetDefinition::InitializeDefinitions(const std::string& filePath)
{
	DebuggerPrintf("\nLoading iso-sprite animset definitions from %s", filePath.c_str());
	ConsolePrintf("Loading iso-sprite animset definitions from %s", filePath.c_str());
	if(filePath.find(".xml") != std::string::npos)
	{
		XMLDocument definitionsDoc;
		definitionsDoc.LoadFile(filePath.c_str());
		XMLElement* root = definitionsDoc.FirstChildElement();
		for(const XMLElement* definitionElement = root->FirstChildElement(); definitionElement; definitionElement = definitionElement->NextSiblingElement())
		{
			DebuggerPrintf("\n\"%s\" definition loading...", definitionElement->Attribute("id"));
			ConsolePrintf("\"%s\" definition loading...", definitionElement->Attribute("id"));
			std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = IsoSpriteAnimDefinition::s_definitions.find(definitionElement->Attribute("id"));
			if(found != IsoSpriteAnimDefinition::s_definitions.end())
			{
				// Definition already exists
				GUARANTEE_OR_DIE(false, Stringf("IsoSpriteAnimSetDefinition already exists for \"%s\". Check the file for errors.", definitionElement->Attribute("id")).c_str());
			}
			else
			{
				// New Definition
				IsoSpriteAnimSetDefinition* newDefinition = new IsoSpriteAnimSetDefinition(*definitionElement);
				s_definitions[newDefinition->m_setName] = newDefinition;
				DebuggerPrintf("\nDefinition loaded.");
				ConsolePrintf("Definition loaded.");
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Returns the definition
//
IsoSpriteAnimSetDefinition* IsoSpriteAnimSetDefinition::GetDefinition(const std::string& name)
{
	std::map<std::string, IsoSpriteAnimSetDefinition*>::iterator found = IsoSpriteAnimSetDefinition::s_definitions.find(name);
	//GUARANTEE_OR_DIE( found != s_definitions.end(), Stringf("Illegal isosprite animset name:%s", name.c_str()).c_str());
	if(found != s_definitions.end())
	{
		return found->second;
	}
	else
	{
		return nullptr;
	}
	
}

//-----------------------------------------------------------------------------------------------
// Checks for the name in the named anim list and returns true if found
//
bool IsoSpriteAnimSetDefinition::IsValidAnimName(const std::string& name)
{
	std::map<std::string, IsoSpriteAnimDefinition*>::iterator found = m_namedAnimDefs.find(name);
	if(found == m_namedAnimDefs.end())
	{
		return false;
	}
	else
		return true;
}
