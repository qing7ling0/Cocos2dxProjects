#include "StreamIO.h"

inline char* StreamIO::getBody()
{
	return body;
}
	
char* StreamIO::getCopyBody()
{
	char* chs = (char*)malloc(length*CHAR_LENGTH);
    memcpy(chs, body, length*CHAR_LENGTH);

	return chs;
}

void StreamIO::setBody(char* body)
{
	pointer = 0;
	length = 0;
	free((void *)this->body);
	this->body = nullptr;
		
	enter(body, sizeof(body));
}
	
inline void StreamIO::enter(char* bytes, int length)
{
	if (nullptr == body || this->length < pointer + length)
	{
		extend();
	}
	else if (length < pointer + length)
	{
		extend();
	}

	memcpy(this->body+pointer*CHAR_LENGTH, bytes, length * CHAR_LENGTH);
	pointer = pointer + length;
}

/*
	扩充byte数组
*/
inline void StreamIO::extend()
{
	int oldLength = length;
	length = length + 64;
	char* newBody = (char *) malloc(length * CHAR_LENGTH);
    memcpy(newBody, body, oldLength * CHAR_LENGTH);
	free((void*)body);
	body = newBody;
	newBody = nullptr;
}
	
int StreamIO::getLength()
{
	return length;
}

int StreamIO::getPointer()
{
	return pointer;
}

void StreamIO::skip(int skipLength)
{
	pointer += skipLength;
}
	
q_byte StreamIO::decodeByte() 
{
	if (checkRead(1))
	{
		q_byte result = body[getPointer()];
		skip(1);
		return result;
	}
	else
	{
		QLog("StreamIO decodeByte error read length > body length");
	}
	return -1;
}
	
bool StreamIO::decodeBoolean()
{
	if (checkRead(1))
	{
		bool ret = body[getPointer()]!=0;
		skip(1);
		return ret;
	}
	else
	{
		QLog("StreamIO decodeBoolean error read length > body length");
	}
	return false;
}

short StreamIO::decodeShort()
{
	if (checkRead(2))
	{
		short result = readShort(body, getPointer());
		skip(2);
		return result;
	}
	else
	{
		QLog("StreamIO decodeShort error read length > body length");
	}
	return -1;
}

int StreamIO::decodeShort2Int()
{
	return decodeShort() & 0xFFFF;
}

int StreamIO::decodeInt()
{
	if (checkRead(4))
	{
		int result = readInt(body, getPointer());
		skip(4);
		return result;
	}
	else
	{
		QLog("StreamIO decodeInt error read length > body length");
	}
	return -1;
	
}

string StreamIO::decodeString()
{
	int str_len = decodeShort();
	if (checkRead(str_len))
	{
		string result = readString(body, getPointer()-2);
		skip(str_len-2);
		return result;
	}
	else
	{
		QLog("StreamIO decodeString error read length > body length");
	}
	return nullptr;
}
	
void StreamIO::enter(string str)
{
	char* bytes = string2bytes(str);
	enter(bytes, str.length()+3);
}

void StreamIO::enter(q_byte onebyte)
{
	enter((char *)&onebyte, 1);
}
	
void StreamIO::enter(bool bln)
{
	q_byte bl = (q_byte) (bln?1:0);
	enter((char *)&bl, 1);
}
	
void StreamIO::enter(int i)
{
	enter(int2Bytes(i), 4);
}
	
void StreamIO::enter(short s)
{
	enter(short2Bytes(s), 2);
}
	
char* StreamIO::decodeBytes(int count)
{
	return readBytes(body, getPointer(), count);
}

bool StreamIO::checkRead(int len)
{
	return getPointer() + len <= length;
}
    
vector<std::string> StreamIO::decodeStrings(unsigned char count)
{
    vector<string> ints(count);
    for(int i=0; i<count; i++)
    {
        ints[i] = decodeString();
    }
    return ints;
}
    
vector<int> StreamIO::decodeInts(int count)
{
    vector<int > ret(count);
    for (int i = 0; i < count; i++)
    {
        ret[i] = decodeInt();
    }
        
    return ret;
}
    
vector<short> StreamIO::decodeShorts(int count)
{
    vector<short > ret(count);
    for (int i = 0; i < count; i++)
    {
		ret[i] = decodeShort();
    }
        
    return ret;
}
	
vector<q_byte> StreamIO::decodeBytes(char count)
{
    vector< char> bytes(count);
    for(int i=0;  i<count; i++)
    {
        bytes[i] = decodeByte(); 
    }
        
    return bytes;
}
	
void StreamIO::free_bytes(char* bytes)
{
	free(bytes);
}

void StreamIO::setPointer(int _pointer)
{
	pointer = _pointer;
}
	
StreamIO::StreamIO(void)
{
	length = 0;
	pointer = 0;
	body = nullptr;
}

StreamIO::~StreamIO(void)
{
	if (nullptr == body)
	{
		free(body);
		body = nullptr;
	}
};