#include <iostream>
#include <string>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/asio.hpp>
#include "AppVersion.h"
#include "AppUpdate.h"
#include "AppFileHandle.h"

/**
*　　　　　　　 ┏┓　　┏┓+ +
*　　　　　　　┏┛┻━━━┛ ┻┓ + +
*　　　　　　　┃　　　　　┃ 　
*　　　　　　　┃　　　━　 ┃ ++ + + +
*　　　　　　 ████━████   ┃+
*　　　　　　　┃　　　　　 ┃ +
*　　　　　　　┃　　　┻　　┃
*　　　　　　　┃　　　　　 ┃ + +
*　　　　　　　┗━┓　　　┏━┛
*　　　　　　　　　┃　　　┃　　　　　　　　　　　
*　　　　　　　　　┃　　　┃ + + + +
*　　　　　　　　　┃　　　┃　Code is far away from bug with the protecting　　　　　　　
*　　　　　　　　　┃　　　┃ + 　　　　神兽保佑,代码无bug　　
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃　　+　　　　　　　　　
*　　　　　　　　　┃　 　　┗━━━┓ + +
*　　　　　　　　　┃ 　　　　　　　┣┓
*　　　　　　　　　┃ 　　　　　　　┏┛
*　　　　　　　　　┗┓┓┏━┳┓┏┛ + + + +
*　　　　　　　　　　┃┫┫　┃┫┫
*　　　　　　　　　　┗┻┛　┗┻┛+ + + +
*/



void OnFileDownLoadComplete(bool bResult, const boost::filesystem::path& FilePath, boost::shared_ptr<AppFileHandle> pAppFileHandle)
{
	printf("文件 : %s 下载完毕\n", FilePath.string().c_str());
	if (pAppFileHandle->Copy(FilePath, boost::filesystem::current_path() / FilePath.filename()))
	{
		std::cout << FilePath.filename() << "文件处理完毕" << std::endl;
	}
}

int main(int argc,char* argv[])
{
	boost::asio::io_service io;
	boost::shared_ptr<AppFileHandle> pAppFileHandle(new AppFileHandle(argv[0], "backup"));
	boost::shared_ptr<AppVersion> pAppVersion(new AppVersion(io, "http://127.0.0.1/test/test.xml"));
	// 设定一个初始化的版本号
	const int version = 2;
	// 下载配置文件并且进行下载
	pAppVersion->m_VersionCompleteSig.connect([pAppVersion, pAppFileHandle, &io, &version](bool bResult)
	{
		// 校验结果
		if (!bResult) return;
		// 校验版本
		if (pAppVersion->GetServeVersion() == boost::lexical_cast<std::string>(version)) return;

		// 获取下载所有文件
		std::vector<DownLoadFile> vecDownLoadFile = pAppVersion->GetDownLoadFileList();
		std::for_each(vecDownLoadFile.begin(), vecDownLoadFile.end(), [pAppFileHandle, &io](const DownLoadFile& File){
			boost::filesystem::path FilePath = pAppFileHandle->BackUpDirPath() / File.m_FileName;
			boost::shared_ptr<AppUpdate> pAppUpdate(new AppUpdate(io, FilePath));
			pAppUpdate->m_UpdateCompleteSig.connect(boost::bind(OnFileDownLoadComplete, _1, _2, pAppFileHandle));
			pAppUpdate->StartUpdate(File.m_Url);
		});
	});

	io.run();
	return 0;
}