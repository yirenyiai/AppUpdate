#pragma once 
#include <include\avhttp.hpp>
#include <boost\asio.hpp>
#include <boost\function.hpp>
#include <boost\enable_shared_from_this.hpp>
#include <boost\array.hpp>
#include <boost\iostreams\device\file.hpp>
#include <boost/signals2/signal.hpp> 

#include <string>
#include <algorithm>
#include <vector>


class AppUpdate
	: public boost::enable_shared_from_this<AppUpdate>
{
public:
	boost::signals2::signal<void(const float&)> m_UpdateProgressSig;
	boost::signals2::signal<void(bool, const boost::filesystem::path&, boost::shared_ptr<AppUpdate>)> m_UpdateCompleteSig;
public:
	AppUpdate(boost::asio::io_service& IOService, boost::filesystem::path Path);
	~AppUpdate();
	// 开始升级
	void StartUpdate(const std::string& Url, const std::string& FileMD5, avhttp::proxy_settings avHttpProxy = avhttp::proxy_settings());
	// 暂停升级
	void PauseUpdate();
	// 继续升级
	void ResumeUpdate();
private:
	void HandleOpen(const boost::system::error_code &ec);
	void HandleRead(int BytesTransferred, const boost::system::error_code &ec);
	bool MD5Check(const std::string& FileMD5);
private:
	boost::array<char, 2048>    m_RecvBytes;
	boost::shared_ptr<boost::iostreams::file_sink> m_FileSink;
	avhttp::http_stream			m_AvHttpStream;
	boost::filesystem::path 	m_FilePath;
	std::string					m_DownLoadUrl;
	bool						m_bPause;
	int							m_TotalSize;
	int							m_ServerFileLen;
};