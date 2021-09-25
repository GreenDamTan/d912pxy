﻿#include "stdafx.h"
#include "../d912pxy/build_version.h"

//gw2addon_get_description
//gw2addon_load
//gw2addon_unload

gw2al_addon_dsc gAddonDeps[] = {
	GW2AL_CORE_DEP_ENTRY,
	{
		L"d3d9_wrapper",
		L"whatever",
		2,
		0,
		1,
		0
	},
	{0,0,0,0,0,0}
};

gw2al_addon_dsc gAddonDsc = {
	L"d912pxy",
	L"DirectX9 to DirectX12 API proxy, designed for performance improvements",
	2,
	4,
	BUILD_VERSION_REV,
	gAddonDeps
};

HMODULE custom_d3d9_module;

gw2al_core_vtable* instance::api = NULL;

#include "../thirdparty/cpu_arch_test.inc"

wchar_t* GetD3D9CustomLib()
{
	cpu_arch arch = GetCPUArch();

	//block AVX builds for now, as they don't have any actuall usage of AVX,
	//simply clocking down CPU due to AVX offset
	//if (arch.AVX2)
	//	return (wchar_t*)L"./addons/d912pxy/dll/release_avx2/d3d9.dll";
	//else if (arch.AVX)
	//	return (wchar_t*)L"./addons/d912pxy/dll/release_avx/d3d9.dll";
	//else 
	if (arch.SSE)
		return (wchar_t*)L"./addons/d912pxy/dll/release/d3d9.dll";
	else 
	{
		gAPI->log_text(LL_ERR, (wchar_t*)L"d912pxy", (wchar_t*)L"can't load due to not supported CPU");
		return (wchar_t*)L"d3d9.dll";
	}
}

gw2al_addon_dsc* gw2addon_get_description()
{
	return &gAddonDsc;
}

gw2al_api_ret gw2addon_load(gw2al_core_vtable* core_api)
{
	gAPI = core_api;

	auto nha = gAPI->hash_name((wchar_t*)L"D3D_wrapper_custom_d3d9_lib_query");

	if (gAPI->load_addon((wchar_t*)L"SelectRender") == GW2AL_OK)
	{
		gAPI->log_text(LL_INFO, (wchar_t*)L"d912pxy", (wchar_t*)L"render selector detected, disabling auto load");
		return GW2AL_OK;
	}

	gAPI->register_function(&GetD3D9CustomLib, nha);
		
	return GW2AL_OK;
}

gw2al_api_ret gw2addon_unload(int gameExiting)
{
	//TODO cleanup
	return GW2AL_OK;
}