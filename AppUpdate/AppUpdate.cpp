#include "AppUpdate.h"
AppUpdate::AppUpdate(boost::asio::io_service& IOService, boost::filesystem::path Path)
	: m_AvHttpStream(IOService), 
	  m_bPause(false),
	  m_FilePath(Path),
	  m_FileSink(Path.string(), std::ios::out | std::ios::binary | std::ios::trunc),
	  m_TotalSize(0)
{
}

AppUpdate::~AppUpdate()
{
}

// ��ʼ����
void AppUpdate::StartUpdate(const std::string& Url, avhttp::proxy_settings avHttpProxy)
{
	m_AvHttpStream.proxy(avHttpProxy);
	if (!m_FileSink.is_open()) { assert(false); return; }
	m_AvHttpStream.async_open(Url, boost::bind(&AppUpdate::HandleOpen, shared_from_this(), boost::asio::placeholders::error));
}

// ��ͣ����
void AppUpdate::PauseUpdate()
{
	assert(m_AvHttpStream.is_open());
	if (m_AvHttpStream.is_open())
	{
		m_bPause = true;
	}
}

// ��������
void AppUpdate::ResumeUpdate()
{
	assert(m_AvHttpStream.is_open());
	if (m_AvHttpStream.is_open())
	{
		m_bPause = false;
	}
}

void AppUpdate::HandleOpen(const boost::system::error_code &ec)
{
	if (!ec)
	{
		// �첽�����http��ȡ���ݲ���.
		m_AvHttpStream.async_read_some(boost::asio::buffer(m_RecvBytes),
			boost::bind(&AppUpdate::HandleRead, shared_from_this(),
			boost::asio::placeholders::bytes_transferred,
			boost::asio::placeholders::error
			)
			);
	}
}

void AppUpdate::HandleRead(int BytesTransferred, const boost::system::error_code &ec)
{

	if (!ec)
	{
		if (m_bPause) return;
		// ��ӡ����
		m_TotalSize += BytesTransferred;
		printf("��������: %d\n", m_TotalSize);
		m_FileSink.write(m_RecvBytes.data(), BytesTransferred);
		// ���ļ�д�������У���ֹ��ʧ
		// ������ȡhttp����.
		m_AvHttpStream.async_read_some(boost::asio::buffer(m_RecvBytes),
			boost::bind(&AppUpdate::HandleRead, shared_from_this(), boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)
			);
	}
	else if (ec == boost::asio::error::eof)
	{
		m_FileSink.flush();
		m_UpdateCompleteSig(true, m_FilePath);
		printf("���سɹ�\n");
	}
	else
	{
		m_UpdateCompleteSig(false, m_FilePath);
		printf("����ʧ��\n");
	}
}
