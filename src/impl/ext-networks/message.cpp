#include "../api_dev_mod.h"
#include "../../inc/ext-networks/message.h"

using namespace Cet::Ext_Net;

Message::Message()
	: m_type(DataType::Unknown)
	, m_size(0)
	, m_data(nullptr)
{}

Message::~Message()
{
	SAFE_DELETE_ARR(m_data);
}

Message::Message(const Message& rhs)
	: m_type(rhs.m_type)
	, m_size(rhs.m_size)
{
	m_data = new unsigned char[m_size];
	memcpy(m_data, rhs.m_data, m_size);
}

Message::Message(Message&& rhs)
	: m_type(rhs.m_type)
	, m_size(rhs.m_size)
	, m_data(rhs.m_data)
{
	rhs.m_data = nullptr;
}

Message::Message(int32_t msg)
	: m_type(Int32)
	, m_size(sizeof(int32_t))
{
	m_data = new unsigned char[m_size];
	memcpy(m_data, &msg, m_size);
}

Message::Message(int64_t msg)
	: m_type(Int64)
	, m_size(sizeof(int64_t))
{
	m_data = new unsigned char[m_size];
	memcpy(m_data, &msg, m_size);
}

Message::Message(const std::string& msg)
	: m_type(String)
	, m_size((unsigned short)msg.length())
{
	m_data = new unsigned char[m_size];
	memcpy(m_data, msg.c_str(), m_size);
}

Message::Message(const unsigned char* data, unsigned short len)
	: m_size(len)
{
	m_data = new unsigned char[m_size];
	memcpy(m_data, data, m_size);
}

Message& Message::operator=(const Message& rhs)
{
	m_type = rhs.m_type;
	m_size = rhs.m_size;
	m_data = new unsigned char[m_size];
	memcpy(m_data, rhs.m_data, m_size);
	return *this;
}

Message& Message::operator=(Message&& rhs)
{
	m_type = rhs.m_type;
	m_size = rhs.m_size;
	m_data = rhs.m_data;

	rhs.m_data = nullptr;
	return *this;
}

std::string Message::to_string() const
{
	std::string res;

	if (String == m_type)
	{
		res.insert(0, (char*)m_data, m_size);
	}

	return res;
}

int32_t Message::to_int32() const
{
	int32_t res(0);

	if (Int32 == m_type)
	{
		memcpy(&res, m_data, sizeof(int32_t));
	}

	return res;
}

int64_t Message::to_int64() const
{
	int64_t res(0);

	if (Int64 == m_type)
	{
		memcpy(&res, m_data, sizeof(int64_t));
	}

	return res;
}

unsigned short Message::size() const
{
	return m_size;
}

unsigned char* Message::buffer(unsigned short offset) const
{
	return m_data + offset;
}