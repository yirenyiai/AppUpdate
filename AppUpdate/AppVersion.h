#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/signals2/signal.hpp> 
#include <include/avhttp.hpp>
#include <assert.h>
#include <string>
#include <vector>
/*
name : 文件名字
md5 :  文件MD5
url : 下载网址
description ： 文件描述文本

<?xml version = "1.0" encoding = "UTF-8" ? >
<FileDownLoad version="1" >
	<FileList>
		<File name="file1.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file2.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file3.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file4.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file5.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file6.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file7.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
		<File name="file8.htm" md5="0000000000000000000000000000000"  url="http://www.baidu.com" description="test"/>
	</FileList>
</FileDownLoad>

注意： 保存该测试文档，必须要以UTF-8字符编码进行保存
*/

struct DownLoadFile
{
	std::string m_FileName;
	std::string m_Url;
	std::string m_MD5;
	std::string m_Description;
};

class AppVersion
{
public:
	boost::signals2::signal<void(bool)> m_VersionCompleteSig;
public:
	AppVersion(boost::asio::io_service& IOService, const std::string& xmlVersionUrl, avhttp::proxy_settings avHttpProxy = avhttp::proxy_settings());
	~AppVersion();
	std::string	GetServeVersion();
	std::vector<DownLoadFile> GetDownLoadFileList();
private:
	const AppVersion& operator()(const boost::system::error_code &ec);
	const AppVersion& operator()(int BytesTransferred, const boost::system::error_code &ec, boost::shared_array<char> pbuf);
	void XMLFileParse();
private:
	std::string m_AppVersion;
	std::vector<DownLoadFile>   m_vecDownLoadFile;
	std::string m_DownLoadCTX;
	boost::shared_ptr<avhttp::http_stream>			m_AvHttpStream;
};