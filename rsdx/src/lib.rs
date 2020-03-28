#![allow(non_snake_case, unused_variables)]
extern crate winapi; extern crate user32;
use winapi::um::libloaderapi::{LoadLibraryA, GetProcAddress};
use std::mem::{transmute};
use winapi::shared::minwindef::*;
use winapi::um::winnt::*;
use std::ffi::CString;
use winapi::shared::*;

#[no_mangle] extern "system" fn DllMain
    (hInstance: HMODULE, fdwReason: DWORD, lpvReserved: LPVOID)
-> BOOL
{
    match fdwReason {
        DLL_PROCESS_ATTACH => (),
        DLL_PROCESS_DETACH => (),
        _ => ()
    }; return TRUE;
}

macro_rules! sysfn {
    [$name:ident -> $ret:ty, $conv:tt, [$($params:tt)*] $($body:tt)*] => {
        type $name = unsafe extern $conv fn ($($params)*) -> $ret;
        #[no_mangle] unsafe extern $conv fn $name ($($params)*) -> $ret {$($body)*}
    }
}

macro_rules! cs {
    ($thing:expr) => (CString::new($thing).unwrap().as_ptr());
}

sysfn!(CreateDXGIFactory -> ntdef::HRESULT, "system", [riid: guiddef::REFIID, ppFactory: *mut LPVOID]
    let module = LoadLibraryA(cs!("C:\\Windows\\System32\\dxgi.dll"));
    let fProc = GetProcAddress(module, cs!("CreateDXGIFactory"));
    let realFn = transmute::<FARPROC, CreateDXGIFactory>(fProc);
    return realFn(riid, ppFactory);
);

sysfn!(CreateDXGIFactory1 -> ntdef::HRESULT, "system", [riid: guiddef::REFIID, ppFactory: *mut LPVOID]
    let module = LoadLibraryA(cs!("C:\\Windows\\System32\\dxgi.dll"));
    let fProc = GetProcAddress(module, cs!("CreateDXGIFactory1"));
    let realFn = transmute::<FARPROC, CreateDXGIFactory1>(fProc);
    return realFn(riid, ppFactory);
);
