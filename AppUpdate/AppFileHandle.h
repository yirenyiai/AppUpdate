#pragma once
#include <boost\filesystem.hpp>

class AppFileHandle
{
public:
	AppFileHandle(const std::string& FilePath, const std::string& BackUpDirName);
    ~AppFileHandle();
    // ��ȡ�����ļ��б���ļ���
    const boost::filesystem::path& BackUpDirPath();
    // ���ļ�������ϣ����ļ����д���
	bool Copy(const boost::filesystem::path& from, const boost::filesystem::path& to);
private:
    boost::filesystem::path m_BackUpDirPath;
};