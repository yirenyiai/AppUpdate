#include "AppVersion.h"
#include <strstream>


AppVersion::AppVersion(boost::asio::io_service& IOService, const std::string& xmlVersionUrl, avhttp::proxy_settings avHttpProxy)
	: m_AvHttpStream(new avhttp::http_stream(IOService))
{
	m_AvHttpStream->proxy(avHttpProxy);
	m_AvHttpStream->async_open(xmlVersionUrl,
		boost::bind(&AppVersion::operator(), this,
		boost::asio::placeholders::error)
		);
}

AppVersion::~AppVersion()
{
}

std::string	AppVersion::GetServeVersion()
{
	return m_AppVersion;
}

std::vector<DownLoadFile> AppVersion::GetDownLoadFileList()
{
	return m_vecDownLoadFile;
}

const AppVersion& AppVersion::operator()(const boost::system::error_code &ec)
{
	if (!ec)
	{
		// 异步发起从http读取数据操作.
		char* pbuf(new char[1024]);
		memset(pbuf, 0, 1024);

		m_AvHttpStream->async_read_some(boost::asio::buffer(pbuf, 1023),
			boost::bind(&AppVersion::operator(), this,
			boost::asio::placeholders::bytes_transferred,
			boost::asio::placeholders::error,
			boost::shared_array<char>(pbuf)
			)
			);
	}
	else
	{
		printf("下载失败,错误原因： %s\n", ec.message());
	}
	return *this;
}

const AppVersion& AppVersion::operator()(int BytesTransferred, const boost::system::error_code &ec, boost::shared_array<char> pbuf)
{
	if (!ec)
	{
		m_DownLoadCTX += pbuf.get();
		memset(pbuf.get(), 0, BytesTransferred);
		m_AvHttpStream->async_read_some(boost::asio::buffer(pbuf.get(), 1023),
			boost::bind(&AppVersion::operator(), this,
			boost::asio::placeholders::bytes_transferred,
			boost::asio::placeholders::error,
			pbuf
			)
			);
	}
	else if (ec == boost::asio::error::eof)
	{
		printf("下载成功,对XML进行解析\n");
		XMLFileParse();
	}
	else
	{
		printf("下载失败,错误原因： %s\n", ec.message());
	}
	return *this;
}

void AppVersion::XMLFileParse()
{
	boost::property_tree::ptree pt;
	try
	{
		std::istrstream iss(m_DownLoadCTX.c_str(), m_DownLoadCTX.length());
		boost::property_tree::xml_parser::read_xml(iss, pt, boost::property_tree::xml_parser::trim_whitespace);

		auto xmlFileDownLoad = pt.get_child("FileDownLoad");
		for (auto pos(xmlFileDownLoad.begin()); pos != xmlFileDownLoad.end(); ++pos)
		{
			if ("<xmlattr>" == pos->first)
			{
				m_AppVersion = pos->second.get<std::string>("version");
			}
		}

		auto xmlFileList = pt.get_child("FileDownLoad.FileList");
		for (auto pos(xmlFileList.begin()); pos != xmlFileList.end(); ++pos)
		{
			if ("File" == pos->first)
			{
				DownLoadFile File;
				File.m_MD5 = pos->second.get<std::string>("<xmlattr>.md5");
				File.m_Url = pos->second.get<std::string>("<xmlattr>.url");
				File.m_FileName = pos->second.get<std::string>("<xmlattr>.name");
				File.m_Description = pos->second.get<std::string>("<xmlattr>.description");
				m_vecDownLoadFile.push_back(File);
			}
		}

		m_VersionCompleteSig(true);
	}
	catch (boost::property_tree::xml_parser::xml_parser_error ec)
	{
		printf("XML解析错误，错误信息：%s \n", ec.message());
		assert(false);
		m_VersionCompleteSig(false);
	}
}
