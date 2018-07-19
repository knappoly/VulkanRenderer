#pragma once
#include <string>
#include <vector>

//-----------------------------------------------------------------------------------------------
class StringTokenizer
{
public:
	//-----------------------------------------------------------------------------------------------
	// Constructors/Destructor
	StringTokenizer( const std::string& input, const std::string& delimiter = "" ): inputString(input), delimiterString(delimiter) {}
	~StringTokenizer(){}

	//-----------------------------------------------------------------------------------------------
	// Accessors
	std::vector<std::string> GetTokens() const { return tokens; } // Returns the tokens after tokenizing
	size_t					 GetTokenCount() const { return tokens.size(); }
	bool					 IsQuoteCheck() const { return quoteCheck; }
	
	//-----------------------------------------------------------------------------------------------
	// Mutators
	void					 SetString(const std::string& newString) { inputString = newString; } // Changes the input string
	void					 Tokenize(const std::string& delimiter = ""); // Tokenizes the string with the new delimiter on demand
	size_t					 HandleQuotation(size_t prevDelimiterIndex);
	void					 TrimEmpty(); // Trims empty tokens in the token list and maintains order
	void					 SetQuoteCheck(bool flag) { quoteCheck = flag; }

private:
	//-----------------------------------------------------------------------------------------------
	// Members
	std::string					inputString;
	std::string					delimiterString;
	std::vector<std::string>	tokens; 
	bool						quoteCheck = false;
};