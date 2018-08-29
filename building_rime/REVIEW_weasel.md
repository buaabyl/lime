# weasel

## Sources
Fellow this build order,

| directory         | target                    | memo                      |
| ----------------  | ------------------------- | ------------------------- |
| WeaselIPC         | WeaselICP.lib             | boost的IPC库封装          |
| WeaselTSF         | weasel.dll, weaselx64.dll | Windows的TSF接口适配层    |
| WeaselIME         | weasel.ime, weaselx64.ime | Windows的IME接口适配层    |
| RimeWithWeasel    | RimeWithWeasel.lib        | 封装librime的接口         |
| WeaselUI          | WeaselUI.lib              | 输入法界面(候选词等)      |
| WeaselIPCServer   | WeaselIPCServer.lib       | boost的IPC库封装          |
| WeaselServer      | WeaselServer.exe          | 输入法的                  |
| WeaselDeployer    | WeaselDeployer.exe        | 输入法的配置工具          |
| WeaselSetup       | WeaselSetup.exe           | 输入法的安装工具          |


```
    +>>>>>>>>> windows message >>>>>>>>>>>>>+
    |                                       |
+---------+                             +---------------------------------------+
|WeaselIPC| <<< boost-shared-memory <<< |WeaselIPCServer --- WeaselUI           |
|WeaselIME|                             |    |                                  |
+---------+                             |    +--- RimeWithWeasel --- librime    |
                                        +---------------------------------------+
```

using `WeaselSetup.exe` to install

default configure file will copy to `C:\Users\{username}\AppData\Roaming\Rime` on first startup.
and rime will generate some binary file, it is really slow, just wait.
if `weasel.yaml` not copy to that path, you may copy it yourself.

__Tips__

If you change yaml, must kill `WeaselServer.exe` process by yourself.

## boost IPC message format
The user defined message is look like this:
```
#comment\n
action=A,B,C,D\n
commit=stringaaaa\n
status.ascii_mode=\d+\n
status.composing=\d+\n
status.disabled=\d+\n
ctx.preedit=\s+\n
ctx.preedit.cursor=\d+,\d+\n
.\n
```

`.\n` indicate message end.


## WeaselIME
The IME dll send Windows Message to WeaselIPCServer, expect boost message for reading.

```cpp
bool accepted = m_client.ProcessKeyEvent(ke);

// get commit string from server
wstring commit;
weasel::Status status;
weasel::ResponseParser parser(&commit, NULL, &status);
bool ok = m_client.GetResponseData(boost::ref(parser));
```

this `ProcessKeyEvent` send message, and `GetResponseData` wait for response.
the real `GetResponseData` implement in `WeaselIPC/WeaselIPCImpl.cpp`.


## WeaselUI
the source code is `WeaselUI/WeaselUI.cpp` and `WeaselUI/WeaselPanel.cpp`.

`void WeaselPanel::DoPaint(CDCHandle dc)` is main paint function.
and the fellow function draw pre-edit word, most of time is act as inline editor:
```cpp
bool WeaselPanel::_DrawPreedit(Text const& text, CDCHandle dc, CRect const& rc)
```
the word is store in `m_ctx.preedit`

the candidate words is draw by this function:
```cpp
bool WeaselPanel::_DrawCandidates(CDCHandle dc)
```

The weasel `context` (`include/WeaselCommon.h`) is like this:
```cpp
struct Context
{
    //......
	Text preedit;
    CandidateInfo cinfo;
    //......
}

struct CandidateInfo
{
    //......
    int currentPage;
    int totalPages;
    int highlighted;
    std::vector<Text> candies;
    std::vector<Text> comments;
    std::string labels;
    //......
}
```


## RimeWithWeasel
This is backend of weasel, and wrapped of `librime`, the fellow interface is the main event entry of every inputs.
WeaselUI show some candidate words.

```cpp
BOOL RimeWithWeaselHandler::ProcessKeyEvent(weasel::KeyEvent keyEvent, UINT session_id, LPWSTR buffer)
```

`ProcessKeyEvent` call `RimeGetCommit` everytime it called.
And commit string is store inside librime, then the WeaselIPCServer will send message to Client.
the message buffer is `boost::interprocess::wbufferstream`.

This function update candidate words.
```cpp
void RimeWithWeaselHandler::_UpdateUI(UINT session_id)
{
    //......
    if (ctx.menu.num_candidates)
    {
        weasel::CandidateInfo &cinfo(weasel_context.cinfo);
        cinfo.candies.resize(ctx.menu.num_candidates);
        cinfo.comments.resize(ctx.menu.num_candidates);
        for (int i = 0; i < ctx.menu.num_candidates; ++i)
        {
            cinfo.candies[i].str = utf8towcs(ctx.menu.candidates[i].text);
            if (ctx.menu.candidates[i].comment)
            {
                cinfo.comments[i].str = utf8towcs(ctx.menu.candidates[i].comment);
            }
        }
    }
    //......
}
```

Most of time weasel store `session_id`, it call `RimeGetContext` to get rime context when need.

```cpp
RIME_API Bool RimeGetContext(RimeSessionId session_id, RimeContext* context);
```

The `RimeContext` call `FilterCandidates` to get candidates (`src\engine.cc`)


so let's take a look at `ctx` struct (`librime/include/rime_api.h`):
```cpp
typedef struct rime_context_t {
  int data_size;
  // v0.9
  RimeComposition composition;
  RimeMenu menu;
  // v0.9.2
  char* commit_text_preview;
} RimeContext;

typedef struct {
  int page_size;
  int page_no;
  Bool is_last_page;
  int highlighted_candidate_index;
  int num_candidates;
  RimeCandidate* candidates;
  char* select_keys;
} RimeMenu;

typedef struct rime_candidate_t {
  char* text;
  char* comment;
  void* reserved;
} RimeCandidate;
```







