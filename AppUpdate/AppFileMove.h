#pragma once

#include <boost/filesystem.hpp>
class AppFileMove
{
public:
	AppFileMove();
	~AppFileMove();
	void CopyFiles(const boost::filesystem::path& Src, const boost::filesystem::path& Dst);
};