// owlpch.cpp : source file that includes just the standard includes
//	again.pch will be the pre-compiled header
//	owlpch.obj will contain the pre-compiled type information

#include "owlpch.h"
#pragma hdrstop
#include <owl/template.h>
#include <owl/drawitem.h>

void nevercalled()
{
	TIPtrArray<TDrawItem*> item1;
}
