#include "AppUpdate.h"
#include "md5.hpp"
AppUpdate::AppUpdate(boost::asio::io_service& IOService, boost::filesystem::path Path)
	: m_AvHttpStream(IOService), 
	  m_bPause(false),
	  m_FilePath(Path),
	  m_TotalSize(0)
{
}

AppUpdate::~AppUpdate()
{
}

// 开始升级
void AppUpdate::StartUpdate(const std::string& Url, const std::string& FileMD5, avhttp::proxy_settings avHttpProxy)
{
	// 先检查MD5，如果本地文件一致，则不再进行下载
	if (!MD5Check(FileMD5))
	{
		m_FileSink.reset(new boost::iostreams::file_sink(m_FilePath.string(), std::ios::out | std::ios::binary | std::ios::trunc));
		if (!m_FileSink->is_open()) { assert(false); return; }
		m_AvHttpStream.proxy(avHttpProxy);
		m_AvHttpStream.async_open(Url, boost::bind(&AppUpdate::HandleOpen, shared_from_this(), boost::asio::placeholders::error));
	}
}

// 暂停升级
void AppUpdate::PauseUpdate()
{
	assert(m_AvHttpStream.is_open());
	if (m_AvHttpStream.is_open())
	{
		m_bPause = true;
	}
}

// 继续升级
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
		// 异步发起从http读取数据操作.
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
		// 打印总数
		m_TotalSize += BytesTransferred;
		printf("下载总数: %d\n", m_TotalSize);
		m_FileSink->write(m_RecvBytes.data(), BytesTransferred);
		// 把文件写到磁盘中，防止丢失
		// 继续读取http数据.
		m_AvHttpStream.async_read_some(boost::asio::buffer(m_RecvBytes),
			boost::bind(&AppUpdate::HandleRead, shared_from_this(), boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error)
			);
	}
	else if (ec == boost::asio::error::eof)
	{
		m_FileSink->flush();
		m_UpdateCompleteSig(true, m_FilePath);
		printf("下载成功\n");
	}
	else
	{
		m_UpdateCompleteSig(false, m_FilePath);
		printf("下载失败\n");
	}
}

bool AppUpdate::MD5Check(const std::string& FileMD5)
{
	bool bCheckMD5 = false;
	char LocalFileMD5[33] = {'\0'};
	std::string path = m_FilePath.string();
	char* LoaclFilePath = (char*)(path.c_str());
	MDFile(LoaclFilePath, LocalFileMD5);

	if (0 == FileMD5.compare(LocalFileMD5))
	{
		printf("MD5校验成功，不进行网络请求\n");
		bCheckMD5 = true;
		m_UpdateCompleteSig(true, m_FilePath);
	}
	return bCheckMD5;
}