#include "dynamic_pointers.h"
#include "context.h"
#include "hooks.h"

#define $(Function) __pragma(comment(linker, "/export:"#Function"=C:\\Windows\\System32\\dxgi.dll."#Function));
#define exportFN comment(linker, "/export:" __FUNCTION__ "=" __FUNCDNAME__)

const Dynamic::Library _DXGI{ L"C:\\Windows\\System32\\dxgi.dll" };
using namespace vMethods;

EXTERN_C HRESULT WINAPI CreateDXGIFactory(
	REFIID riid, _COM_Outptr_ void** ppFactory
) {
#pragma exportFN
// https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-createdxgifactory
	static decltype(&CreateDXGIFactory) original = _DXGI[__FUNCTION__];
	local& _vmt = Context::Current()._vmt.vDXGI_Factory;

	auto value = original(riid, ppFactory);
	printf("CreateDXGIFactory() -> ptr: %llx, v: %llx\n", ppFactory, *ppFactory);

	if (_vmt != nullptr)
		delete _vmt;

	_vmt = new VMT(*ppFactory);
	_vmt->ReplaceMethod<DXGI_Factory::CreateSwapChain>();
	_vmt->ApplyVMT();

	return value;
}

EXTERN_C HRESULT WINAPI CreateDXGIFactory1(
	REFIID riid, _COM_Outptr_ void** ppFactory
) {
#pragma exportFN
// https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-createdxgifactory1

	static decltype(&CreateDXGIFactory1) original = _DXGI[__FUNCTION__];
	local& _vmt = Context::Current()._vmt.vDXGI_Factory;

	auto value = original(riid, ppFactory);
	printf("CreateDXGIFactory1() -> ptr: %llx, v: %llx\n", ppFactory, *ppFactory);

	if (_vmt != nullptr) 
		delete _vmt;

	_vmt = new VMT(*ppFactory);
	_vmt->ReplaceMethod<DXGI_Factory::CreateSwapChain>();
	_vmt->ApplyVMT();

	return value;
}

// https://windows10dll.nirsoft.net/dxgi_dll.html
// https://learn.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-reference-dxgi-functions
$(ApplyCompatResolutionQuirking)
$(CompatString)
$(CompatValue)
// $(CreateDXGIFactory)
// $(CreateDXGIFactory1)
$(CreateDXGIFactory2)
$(DXGID3D10CreateDevice)
$(DXGID3D10CreateLayeredDevice)
$(DXGID3D10ETWRundown)
$(DXGID3D10GetLayeredDeviceSize)
$(DXGID3D10RegisterLayers)
$(DXGIDumpJournal)
$(DXGIGetDebugInterface1)
$(DXGIReportAdapterConfiguration)
$(DXGIRevertToSxS)
$(PIXBeginCapture)
$(PIXEndCapture)
$(PIXGetCaptureState)
$(SetAppCompatStringPointer)

// https://www.nirsoft.net/dll_information/windows8/dxgi_dll.html
$(CheckETWTLS)
$(CompatString)
$(CompatValue)
// $(CreateDXGIFactory)
// $(CreateDXGIFactory1)
$(D3DKMTCloseAdapter)
$(D3DKMTCreateAllocation)
$(D3DKMTCreateContext)
$(D3DKMTCreateDevice)
$(D3DKMTCreateSynchronizationObject)
$(D3DKMTDestroyAllocation)
$(D3DKMTDestroyContext)
$(D3DKMTDestroyDevice)
$(D3DKMTDestroySynchronizationObject)
$(D3DKMTEscape)
$(D3DKMTGetContextSchedulingPriority)
$(D3DKMTGetDeviceState)
$(D3DKMTGetDisplayModeList)
$(D3DKMTGetMultisampleMethodList)
$(D3DKMTGetRuntimeData)
$(D3DKMTGetSharedPrimaryHandle)
$(D3DKMTLock)
$(D3DKMTOpenAdapterFromHdc)
$(D3DKMTOpenResource)
$(D3DKMTPresent)
$(D3DKMTQueryAdapterInfo)
$(D3DKMTQueryAllocationResidency)
$(D3DKMTQueryResourceInfo)
$(D3DKMTRender)
$(D3DKMTSetAllocationPriority)
$(D3DKMTSetContextSchedulingPriority)
$(D3DKMTSetDisplayMode)
$(D3DKMTSetDisplayPrivateDriverFormat)
$(D3DKMTSetGammaRamp)
$(D3DKMTSetVidPnSourceOwner)
$(D3DKMTSignalSynchronizationObject)
$(D3DKMTUnlock)
$(D3DKMTWaitForSynchronizationObject)
$(D3DKMTWaitForVerticalBlankEvent)
$(DXGID3D10CreateDevice)
$(DXGID3D10CreateLayeredDevice)
$(DXGID3D10ETWRundown)
$(DXGID3D10GetLayeredDeviceSize)
$(DXGID3D10RegisterLayers)
$(DXGIDumpJournal)
$(DXGIReportAdapterConfiguration)
$(DXGIRevertToSxS)
$(OpenAdapter10)
$(OpenAdapter10_2)
$(SetAppCompatStringPointer)

// https://www.win7dll.info/dxgi_dll.html
// $(CreateDXGIFactory)
// $(CreateDXGIFactory1)
$(D3DKMTCloseAdapter)
$(D3DKMTCreateAllocation)
$(D3DKMTCreateContext)
$(D3DKMTCreateDevice)
$(D3DKMTCreateSynchronizationObject)
$(D3DKMTDestroyAllocation)
$(D3DKMTDestroyContext)
$(D3DKMTDestroyDevice)
$(D3DKMTDestroySynchronizationObject)
$(D3DKMTEscape)
$(D3DKMTGetContextSchedulingPriority)
$(D3DKMTGetDeviceState)
$(D3DKMTGetDisplayModeList)
$(D3DKMTGetMultisampleMethodList)
$(D3DKMTGetRuntimeData)
$(D3DKMTGetSharedPrimaryHandle)
$(D3DKMTLock)
$(D3DKMTOpenAdapterFromHdc)
$(D3DKMTOpenResource)
$(D3DKMTPresent)
$(D3DKMTQueryAdapterInfo)
$(D3DKMTQueryAllocationResidency)
$(D3DKMTQueryResourceInfo)
$(D3DKMTRender)
$(D3DKMTSetAllocationPriority)
$(D3DKMTSetContextSchedulingPriority)
$(D3DKMTSetDisplayMode)
$(D3DKMTSetDisplayPrivateDriverFormat)
$(D3DKMTSetGammaRamp)
$(D3DKMTSetVidPnSourceOwner)
$(D3DKMTSignalSynchronizationObject)
$(D3DKMTUnlock)
$(D3DKMTWaitForSynchronizationObject)
$(D3DKMTWaitForVerticalBlankEvent)
$(DXGID3D10CreateDevice)
$(DXGID3D10CreateLayeredDevice)
$(DXGID3D10GetLayeredDeviceSize)
$(DXGID3D10RegisterLayers)
$(DXGIDumpJournal)
$(DXGIReportAdapterConfiguration)
$(OpenAdapter10)
$(OpenAdapter10_2)