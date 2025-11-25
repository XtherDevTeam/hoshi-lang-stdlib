# Known issues

[ ] 发送文件时,文件的this指针typeid为OSFile，虚函数表未被正常填充

```
    ❯ LLDB ./build-release/net-http-server
    (lldb) target create "./build-release/net-http-server"
    Current executable set to '/Users/chou/Desktop/project/hoshi-lang-stdlib/build-release/net-http-server' (arm64).
    (lldb) b YoiTypeInfo^C
    (lldb) b net-http-server.hoshi:57
    Breakpoint 1: where = net-http-server`0_file_test#struct#19#1 + 2704 at net-http-server.hoshi:57:6, address = 0x0000000100005500
    (lldb) r
    Process 42892 launched: '/Users/chou/Desktop/project/hoshi-lang-stdlib/build-release/net-http-server' (arm64)
    net-http-server: start
    Process 42892 stopped
    * thread #2, stop reason = breakpoint 1.1
        frame #0: 0x0000000100005500 net-http-server`0_file_test#struct#19#1 at net-http-server.hoshi:57:6
    54       if (f.is_err()) {
    55           return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 404, "Not Found", hashMap.HashMap<str.Str, str.Str>()), str.Str("File not found"))
    56       }
    -> 57       return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 200, "OK").add_header("Content-Type", "application/octet-stream").add_header("Content-Disposition", "attachment; filename=\"The Graduation Trip.7z\""), f.unwrap() as file.AbstractFile)
    58   }
    59  
    60   func main() : int {
    (lldb) p f
    (yoi.struct#3782547968#55 *) 0x0000000100706a00
    (lldb) p *f
    (yoi.struct#3782547968#55) {
    refcount = 1
    typeid = 69
    value = 0x0000000100706320
    error = NULL
    }
    (lldb) p *f.value
    (yoi.struct#3782547968#28) {
    refcount = 1
    typeid = 42
    fp = 0x0000000100706cc0
    isText = 0x0000000100706ce0
    path = 0x0000000101104b50
    }
    Evaluated this expression after applying Fix-It(s):
        *f->value
    (lldb) ^D
    ❯ LLDB ./build-release/net-http-server
    (lldb) target create "./build-release/net-http-server"
    Current executable set to '/Users/chou/Desktop/project/hoshi-lang-stdlib/build-release/net-http-server' (arm64).
    (lldb) ^D
    ❯ ../hoshi-lang/cmake-build-release/hoshi_lang examples/net-http-server.hoshi -o build-release/net-http-server ./cmake-build-debug/libstd.a ./cmake-build-debug/libcrypto.a ./cmake-build-debug/libssl.a -I src --build-mode debug --preserve-intermediate
    ^[[A^[[ALinking Yoi IR modules...
    Generating target object code...
    Target triple arm64-apple-darwin22.6.0, using CPU apple-m1 with features N/A

    Linking final darwin executable...
    Compilation successful!
    Intermediate files preserved.
    ❯ LLDB ./build-release/net-http-server
    (lldb) target create "./build-release/net-http-server"
    Current executable set to '/Users/chou/Desktop/project/hoshi-lang-stdlib/build-release/net-http-server' (arm64).
    (lldb) b net-http-server.hoshi:57
    Breakpoint 1: where = net-http-server`0_file_test#struct#19#1 + 2708 at net-http-server.hoshi:57:6, address = 0x0000000100005184
    (lldb) r
    Process 43015 launched: '/Users/chou/Desktop/project/hoshi-lang-stdlib/build-release/net-http-server' (arm64)
    net-http-server: start
    Process 43015 stopped
    * thread #2, stop reason = breakpoint 1.1
        frame #0: 0x0000000100005184 net-http-server`0_file_test#struct#19#1 at net-http-server.hoshi:57:6
    54       if (f.is_err()) {
    55           return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 404, "Not Found", hashMap.HashMap<str.Str, str.Str>()), str.Str("File not found"))
    56       }
    -> 57       let fi = f.unwrap() as file.AbstractFile
    58       return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 200, "OK").add_header("Content-Type", "application/octet-stream").add_header("Content-Disposition", "attachment; filename=\"The Graduation Trip.7z\""), fi)
    59   }
    60  
    (lldb) n
    Process 43015 stopped
    * thread #2, stop reason = step over
        frame #0: 0x000000010000533c net-http-server`0_file_test#struct#19#1 at net-http-server.hoshi:58:6
    55           return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 404, "Not Found", hashMap.HashMap<str.Str, str.Str>()), str.Str("File not found"))
    56       }
    57       let fi = f.unwrap() as file.AbstractFile
    -> 58       return kiana.response.Response(http.HTTPResponseHeader("HTTP/1.1", 200, "OK").add_header("Content-Type", "application/octet-stream").add_header("Content-Disposition", "attachment; filename=\"The Graduation Trip.7z\""), fi)
    59   }
    60  
    61   func main() : int {
    (lldb) p fi
    (yoi.interface#3782547968#0 *) 0x0000000101204b50
    (lldb) c
    Process 43015 resuming
    Process 43015 stopped
    * thread #2, stop reason = EXC_BAD_ACCESS (code=1, address=0x10)
        frame #0: 0x0000000000000010
    error: memory read failed for 0x10
    (lldb) bt
    * thread #2, stop reason = EXC_BAD_ACCESS (code=1, address=0x10)
    * frame #0: 0x0000000000000010
        frame #1: 0x000000010000d554 net-http-server`4_AbstractFile::content_length#interfaceImpl#interfaceImpl#17#0#interface#4#0 at response.hoshi:32:10
        frame #2: 0x000000010004d530 net-http-server`11_Response::patch_headers# at response.hoshi:101:14
        frame #3: 0x000000010004f678 net-http-server`11_Response::write_to#interfaceObject#10#0 at response.hoshi:117:10
        frame #4: 0x000000010005bb64 net-http-server`16_lambda21_26::operator()# at thread.hoshi:23:14
        frame #5: 0x00000001002d13a8 net-http-server`thread_starter_wrapper(void*) + 32
        frame #6: 0x00000001959affa8 libsystem_pthread.dylib`_pthread_start + 148
    (lldb) f 1
    frame #1: 0x000000010000d554 net-http-server`4_AbstractFile::content_length#interfaceImpl#interfaceImpl#17#0#interface#4#0 at response.hoshi:32:10
    29  
    30   impl file.AbstractFile : HTTPResponseBody {
    31       content_length() : int {
    -> 32           return this.size() // -1 if unknown, then fallback to chunked transfer encoding
    33       },
    34       content_type() : str.Str {
    35           return "application/octet-stream"
    (lldb) p this
    (yoi.interface#3782547968#0 *) 0x0000000101204b50
    (lldb) p *this
    (yoi.interface#3782547968#0) {
    refcount = 1
    typeid = 42
    value = 0x0000000101204b30
    }
    (lldb) ^D
```