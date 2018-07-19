#include "Engine/Core/StringTokenizer.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "EngineCommon.hpp"
#include "Engine/Console/DevConsole.hpp"

//-----------------------------------------------------------------------------------------------
// Tokenize the string with the delimiter specified
//
void StringTokenizer::Tokenize( const std::string& delimiter /*= ""*/ )
{
	if(!delimiter.empty())
		delimiterString = delimiter; // Override if available
	
	tokens.clear();

	size_t prevDemiliterIndex = 0;
	size_t currentDelimiterIndex = 0;

	currentDelimiterIndex = inputString.find_first_of(delimiterString, currentDelimiterIndex);
	while(prevDemiliterIndex != std::string::npos && prevDemiliterIndex < inputString.size()) // Loops through the string as long as currentDelim index is valid
	{
		
		std::string token = inputString.substr(prevDemiliterIndex, currentDelimiterIndex - prevDemiliterIndex );
		if(prevDemiliterIndex != 0)
		{
			prevDemiliterIndex += delimiterString.size(); // Accounts for multi-character delimiters
		}

		size_t quoteIndex = token.find_first_of("\"");
		
		if(quoteCheck && quoteIndex != inputString.npos && token[quoteIndex - 1] != '\\' ) // There is a quote that's not escaped
		{
			prevDemiliterIndex = HandleQuotation(prevDemiliterIndex);

			if(prevDemiliterIndex == -1)
			{
				// Failed parse due to Quote mark imbalance
				ConsolePrintf(Rgba::RED, "\" imbalanced in string. Please check input");
				return;
			}

			if(inputString[prevDemiliterIndex] != delimiterString[0])
			{
				// Appends the remaining part of the token if delimiter is not found
				currentDelimiterIndex = inputString.find_first_of(delimiterString, prevDemiliterIndex);
				tokens[tokens.size() - 1].append(inputString.substr(prevDemiliterIndex, currentDelimiterIndex - prevDemiliterIndex));
				prevDemiliterIndex = currentDelimiterIndex;
			}
		}

		else
		{
			token = std::string(inputString, prevDemiliterIndex, currentDelimiterIndex - prevDemiliterIndex);
			prevDemiliterIndex = currentDelimiterIndex;
			tokens.push_back(token); //Pushes the token into the list of tokens
		}
		currentDelimiterIndex = inputString.find_first_of(delimiterString, currentDelimiterIndex + delimiterString.size());
	}

	if(tokens.empty())
		tokens.push_back(inputString);
}

size_t StringTokenizer::HandleQuotation(size_t prevDelimiterIndex)
{
	std::string token;
	bool flag = false;
	while(prevDelimiterIndex != inputString.npos && prevDelimiterIndex < inputString.size())
	{
		// Escape sequence check within string
		if( flag && inputString[prevDelimiterIndex] == '"' && inputString[prevDelimiterIndex - 1] != '\\')
		{
			tokens.push_back(token);
			return prevDelimiterIndex + 1;
		}
		else 
		{
			if(inputString[prevDelimiterIndex] == '"' && inputString[prevDelimiterIndex - 1] != '\\')
			{
				flag = true;
				prevDelimiterIndex++; // Move to the next char
			}
			token.append(Stringf("%c",inputString[prevDelimiterIndex])); 
		}
		
		++prevDelimiterIndex;
	}
		
	return (size_t) -1; // No errors. Just no quote marks as expected
}

//-----------------------------------------------------------------------------------------------
// Trims empty tokens in the token list
//
void StringTokenizer::TrimEmpty()
{
	for(size_t index = 0; index < tokens.size(); ++index)
	{
		if(tokens[index].empty())
		{
			tokens.erase(tokens.begin() + index);
			index--;
		}
	}
}
