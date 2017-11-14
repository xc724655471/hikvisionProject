// Basic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "HIK.h"


int main()
{
	HIK hik;
	hik.Init();
	//hik.StartPreview();
	hik.GetCompressionParameters();
	hik.GetBasicParameters();
	system("pause");
	
    return 0;
}

