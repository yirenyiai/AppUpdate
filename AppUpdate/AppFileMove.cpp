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
		printf("�ļ��ƶ�ʧ��, �ļ�������\n");
		return;
	}

	boost::filesystem::copy_file(Src, Dst);
}