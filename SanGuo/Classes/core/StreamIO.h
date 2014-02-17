#ifndef __STREAM_IO__
#define __STREAM_IO__

#include <string>
#include "QCommon.h"
#include "Encoder.h"
#include "cocos2d.h"
#include "CCObject.h"

USING_NS_CC;

class CC_DLL StreamIO : public Object
{
public:

	static void free_bytes(char* bytes);
	
	virtual char* getBody();
	
	virtual char* getCopyBody();

	virtual void setBody(char* body);
	
	virtual void enter(char* bytes, int length);

	/*
	  扩充byte数组
	*/
	virtual void extend();
	
	virtual int getLength();

	virtual int getPointer();

	void skip(int skipLength);
	
	virtual q_byte decodeByte();
	
	virtual bool decodeBoolean();

	virtual short decodeShort();

	virtual int decodeShort2Int();

	virtual int decodeInt();

	virtual string decodeString();
	
	virtual void enter(string str);

	virtual void enter(q_byte onebyte);
	
	virtual void enter(bool bln);
	
	virtual void enter(int i);
	
	virtual void enter(short s);
	
	virtual char* decodeBytes(int count);
    
    virtual vector<std::string> decodeStrings(unsigned char count);
    
    virtual vector<int> decodeInts(int count);
    
    virtual vector<short> decodeShorts(int count);
	
	virtual vector<q_byte> decodeBytes(char count);
	
	virtual void setPointer(int _pointer);

	virtual bool checkRead(int len);

	StreamIO(void);

	~StreamIO(void);

protected:

	int length;
	int pointer;
	char* body;
};
	
#endif