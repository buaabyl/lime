# PIME

RIME的Windows版（小狼毫）基本停更了，而PIME貌似还在更新。
PIME有个libIME库，并且基于这个库有个PIMETextService.dll的前端框架，
以下来看看这个框架的代码和设计。

```
+--------------------+          +------------------+                  +------------+
|  TSF frontend      |          |  Dispatcher      |                  |  Server    |
|                    |          |                  |                  |            |
|PIMETextService.dll |<--pipe-->| PIMELauncher.exe |<--stdin/stdout-->| python.exe |
+--------------------+          +------------------+                  +------------+
```


## 初始化
在 PIMETextService.dll 的DllMain的时候会加载 backends.json 这个脚本。
```cpp
ImeModule::ImeModule(HMODULE module)
{
    ......
	std::ifstream fp(programDir_ + L"\\backends.json", std::ifstream::binary);
    ......
    for (const auto& backend: backendsInfo) {
        std::wstring name = utf8ToUtf16(backend["name"].asCString());
        backendDirs_.push_back(name);
    }
    ......
}

```

典型的 backends.json 如下：
```json
[
    {
        "name": "python",
        "command": "python\\python3\\python.exe",
        "workingDir": "python",
        "params": "server.py"
    },
    {
        "name": "node",
        "command": "node\\node.exe",
        "workingDir": "node",
        "params": "server.js"
    }
]
```

在 DllRegisterServer 的时候会搜索配置文件，例如上面的文件有 python 的后端，
会搜索 `python/input_methods/{folder}/ime.json` 得到所有的候选输入法信息，
并在 windows 的输入法栏注册。注册的接口在 libIME 的 ImeModule.cpp 里：
```cpp
HRESULT ImeModule::registerServer(wchar_t* imeName, LangProfileInfo* langs, int count);
```

## 输入法服务
具体的输入法需要用户自己创建，一般是派生自 libIME 的 TextService 类:
```cpp
class TextService: public Ime::TextService
```

然后再调用 PIMEClient 通过命名管道分发到后端。
管道名称为 `\\\\.\\pipe\\{username}\\PIME\\Launcher`，
具体的通信数据是以 `\0` 结束的 json 字符串。


## JSON数据
onActivate
```json
{
    "seqNum": {digit},
	"method": "onActivate",
	"isKeyboardOpen": 1
}
```

onDeactivate
```json
{
    "seqNum": {digit},
	"method": "onDeactivate"
}
```

filterKeyDown，onKeyDown，filterKeyUp，onKeyUp，会获取当前所有的按键状态，然后发送到后端
```json
{
    "seqNum"      : {digit},
    "method"      : "filterKeyDown",
    "method"      : "onKeyDown",
    "method"      : "filterKeyUp",
    "method"      : "onKeyUp",


	"charCode"    : keyEvent.charCode(),
	"keyCode"     : keyEvent.keyCode(),
	"repeatCount" : keyEvent.repeatCount(),
	"scanCode"    : keyEvent.scanCode(),
	"isExtended"  : keyEvent.isExtended(),
	"keyStates"   : [
        0,
        1,
        2,
        3,
        ...
        255
    ]
}
```

返回的json共同的段如下，基本上都由 handleReply 来处理，这个函数也会更新 UI 的一些信息：
```json
{
    "seqNum"      : {digit},
    "success"     : 1,


    "showCandidates": 1,
    "candidateCursor": {digit},
    "showCandidates": [
        ......
    ],
    "candidateList": [
        ......
    ],
    "commitString": {string},
    "compositionString": {string},
}

```

更新 UI 的入口
```cpp
bool Client::handleReply(Json::Value& msg, Ime::EditSession* session) {
	bool success = msg.get("success", false).asBool();
	if (success) {
		updateStatus(msg, session);
	}
	return success;
}
```

## PIMELauncher/PIMEServer
这个 PIMELauncher 给IME服务（例如Python）是通过stdin和stdout来做的，
和CGI和类似。

从前端接收到消息后，会发送给IME服务这样的格式(例如python)。
```
// message format: <client_id>|<json string>\n
```

实际调用的ime会给Launcher发送消息，格式如下：
```
// Format of each line:
// PIMG_MSG|<client_id>|<json reply>\n

```


