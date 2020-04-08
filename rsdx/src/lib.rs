#![allow(non_snake_case, unused_variables, dead_code)]
extern crate kernel32; extern crate user32;
extern crate winapi; extern crate detour;
#[macro_use]
extern crate lazy_static;
use detour::GenericDetour;
// Internal Modules
mod macros; mod start;
// Shorthands
use winapi::shared::*;
use std::mem::{transmute};
use winapi::shared::minwindef::*;
use winapi::um::libloaderapi::{LoadLibraryA, GetProcAddress, GetModuleHandleA};
// DllMain
static mut RSDX: HMODULE = 0 as HMODULE; static mut DXGI: HMODULE = 0 as HMODULE;
#[no_mangle] extern "system" fn DllMain
    (hInstance: HMODULE, fdwReason: DWORD, lpvReserved: LPVOID) -> BOOL
{
    use winapi::um::winnt::*; match fdwReason {
        DLL_PROCESS_ATTACH => unsafe {
            kernel32::AllocConsole();
            kernel32::SetConsoleTitleA(cs!("[FFXIV] Rust Edition"));
            DXGI = LoadLibraryA(cs!("C:\\Windows\\System32\\dxgi.dll"));
            extern "fastcall" fn tmeth() {
                println!("hello");
            }
            RSDX = hInstance; //std::thread::spawn(|| {start::ProcAttach()});
            // fuck().is_ok() as BOOL;
        }, DLL_PROCESS_DETACH => (),
        _ => ()
    }; return TRUE;
}

// use std::error::Error;
// unsafe fn fuck() -> Result<(), Box<dyn Error>> {
//     let k32 = GetModuleHandleA(cs!("Kernel32.dll"));
//     let target = GetProcAddress(k32, cs!("IsDebuggerPresent"));
//     use IsDebuggerPresent::*; Detour.initialize
//     (transmute(target), Function)?.enable()?; Ok(())
// }

use winapi::um::d3d11::ID3D11Device;
hook!(CreateSwapChain -> ntdef::HRESULT, "fastcall", [pFactory: *mut dxgi::IDXGIFactory, pDevice: *mut ID3D11Device,
    pDesc: *mut dxgi::DXGI_SWAP_CHAIN_DESC, ppSwapChain: *mut *mut dxgi::IDXGISwapChain]
    //unsafe {CreateSwapChain::Detour.call(pFactory, pDevice, pDesc, ppSwapChain)}
    return 1;
);
// extern "fastcall" fn CreateSwapChain(pFactory: *mut dxgi::IDXGIFactory, pDevice: *mut ID3D11Device,
    // pDesc: *mut dxgi::DXGI_SWAP_CHAIN_DESC, ppSwapChain: *mut *mut dxgi::IDXGISwapChain) -> ntdef::HRESULT
// {
    // unsafe {CreateSwapChain::Detour.call(pFactory, pDevice, pDesc, ppSwapChain)}
// }

// hook!(IsDebuggerPresent -> BOOL, "system", []
//     println!("debugger check..", );
//     return FALSE;
// );

sysfn!(CreateDXGIFactory -> ntdef::HRESULT, "system", [riid: guiddef::REFIID, ppFactory: *mut LPVOID]
    let fProc = GetProcAddress(DXGI, cs!("CreateDXGIFactory"));
    let realFn = transmute::<FARPROC, CreateDXGIFactory>(fProc);
    return realFn(riid, ppFactory);
);

sysfn!(CreateDXGIFactory1 -> ntdef::HRESULT, "system", [riid: guiddef::REFIID, ppFactory: *mut LPVOID]
    let fProc = GetProcAddress(DXGI, cs!("CreateDXGIFactory1"));
    let realFn = transmute::<FARPROC, CreateDXGIFactory1>(fProc);
    return realFn(riid, ppFactory);
);
