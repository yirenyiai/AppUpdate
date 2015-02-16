#include "AppFileMove.h"


AppFileMove::AppFileMove()
{

}

AppFileMove::~AppFileMove()
{
}

void AppFileMove::CopyFiles(const boost::filesystem::path& Src, const boost::filesystem::path& Dst)
{
	if (!boost::filesystem::exists(Src))
	{
		printf("文件移动失败, 文件不存在\n");
		return;
	}

	boost::filesystem::copy_file(Src, Dst);
}