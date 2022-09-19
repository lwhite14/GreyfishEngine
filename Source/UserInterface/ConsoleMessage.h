#ifndef CONSOLEMESSAGE_H
#define CONSOLEMESSAGE_H

#include <string>

class ConsoleMessage 
{
private:
	std::string m_message;
	int m_messageType; //0 for normal, 1 for warning, and 2 for error

public:
	ConsoleMessage(std::string message, int messageType) : 
		m_message{ message },
		m_messageType{ messageType }
	{

	}

	std::string GetMessage() { return m_message; }
	int GetMessageType() { return m_messageType; }
};


#endif //CONSOLEMESSAGE_H