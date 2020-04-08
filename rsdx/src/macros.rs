#[macro_export]
macro_rules! hook {
    [$name:ident -> $ret:ty, $conv:tt, [$($params:tt)*] $($body:tt)*] => {
        type Type = unsafe extern $conv fn ($($params)*) -> $ret;
        unsafe extern $conv fn Function ($($params)*) -> $ret {$($body)*}
    }
}


#[macro_export]
macro_rules! sysfn {
    [$name:ident -> $ret:ty, $conv:tt, [$($params:tt)*] $($body:tt)*] => {
        type $name = unsafe extern $conv fn ($($params)*) -> $ret;
        #[no_mangle] unsafe extern $conv fn $name ($($params)*) -> $ret {$($body)*}
    }
}

#[macro_export]
macro_rules! cs {
    ($thing:expr) => [std::ffi::CString::new($thing).unwrap().as_ptr()]
}
