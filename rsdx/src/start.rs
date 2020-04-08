extern crate kernel32;
use crate::*;

pub fn ProcAttach() {
    unsafe {
        kernel32::AllocConsole();
        kernel32::SetConsoleTitleA(cs!("[FFXIV] Output"));
    }; println!("hello world?");
}
