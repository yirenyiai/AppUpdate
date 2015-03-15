#include "AppFileHandle.h"

AppFileHandle::AppFileHandle(const std::string& FilePath, const std::string& BackUpDirName)
	:m_BackUpDirPath(FilePath)
{
	m_CurrentPath = boost::filesystem::system_complete(m_BackUpDirPath.remove_filename());
	m_CurrentPath += ".\\";
	m_BackUpDirPath = boost::filesystem::system_complete(m_BackUpDirPath.remove_filename()) / BackUpDirName;
    // 创建这个备份文件夹
    boost::filesystem::create_directory(m_BackUpDirPath);
}

AppFileHandle::~AppFileHandle()
{
}

// 获取下载文件列表的文件夹
const boost::filesystem::path& AppFileHandle::BackUpDirPath()
{
    return m_BackUpDirPath;
}

const boost::filesystem::path& AppFileHandle::GetCurrentPath()
{
	return m_CurrentPath;
}

// 当文件下载完毕，对文件进行处理
bool AppFileHandle::Copy(const boost::filesystem::path& from, const boost::filesystem::path& to)
{
    bool bHandled = false;
    boost::system::error_code ec;
    if (!boost::filesystem::exists(from)) return bHandled;

	if (boost::filesystem::is_directory(from))
	{
		boost::filesystem::remove(to);
		boost::filesystem::copy_directory(from, to, ec);
	}
	else
	{
		boost::filesystem::copy_file(from, to, boost::filesystem::copy_option::overwrite_if_exists, ec);

	}
    if (0==ec)
    {
        bHandled = true;
    }
	else
	{
		std::cout << ec.message() << std::endl;
	}
    return bHandled;
}
