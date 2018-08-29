# 模块简单说明

## Q1. 如何增加一个自己的模块
整个librime是模块化的，具体加载哪些模块由 default.yaml 来控制，例如 default.yaml 里有如下的一段:
```yaml
schema_list:
  - schema: luna_pinyin
  - schema: cangjie5
```

librime启动的时候去去找 `luna_pinyin.schema.yaml` 这个文件，主要的内容如下，列出调用的engine:
```yaml
engine:
  processors:
    - ascii_composer
  segmentors:
    - ascii_segmentor
  translators:
    - longwords_translator
  filters:
    - reverse_lookup_filter@cangjie_lookup
```

像 `ascii_composer` 之类的名称是在 `gear/gears_module.cc` 里注册时给出的。
```cpp
r.Register("ascii_composer", new Component<AsciiComposer>);
```

所以新建一个模块的方法如下：
1. 在src/gear里新建一个模块，假设为 test_translator.cc ， 类名为 TestTranslator
2. 在 `src/gear/core_module.cc` 的对应位置增加 `Register("test_translator", new Component<TestTranslator>")`
3. 修改对应的 schema 的配置文件，在 translators engine 里增加对应的注册名称
4. touch `src/CMakeFiles.txt` 触发新的源文件检测


## Q2. 在 `src/gear` 路径下的文件都做什么了
这些的模块的接口貌似都是一样的，用到了工厂模式。具体到类又是各种继承和模板，
然后在运行时通过RTTI判断来处理。为了避免全局变量又用了单例。

举例来说gear模块的对外接口定义如下`composition.h`:
```cpp
class ComponentBase {
public:
    ComponentBase() = default;
    virtual ~ComponentBase() = default;
};

template <class T, class Arg>
struct Class {
    class Component : public ComponentBase {
    public:
        virtual T* Create(Initializer arg) = 0;
    };
};

template <class T>
struct Component : public T::Component {
public:
    T* Create(typename T::Initializer arg) {
        return new T(arg);
    }
};

```

具体到一个字符处理的接口`processors.h`:
```cpp
class Processor : public Class<Processor, const Ticket&> {
public:
    virtual ProcessResult ProcessKeyEvent(const KeyEvent& key_event);
protected:
    Engine* engine_;
    std::string name_space_;
};

```

对于分词功能`segmentor.h`:
```cpp
class Segmentor : public Class<Segmentor, const Ticket&> {
public:
    virtual bool Proceed(Segmentation* segmentation) = 0;
protected:
    Engine* engine_;
    std::string name_space_;
};
```

`translator.h`:
```cpp
class Translator : public Class<Translator, const Ticket&> {
public:
    virtual shared_ptr<Translation> Query(const std::string& input,
            const Segment& segment, std::string* prompt = NULL);
protected:
    Engine* engine_;
    std::string name_space_;
};
```

在registry.cc里会注册这些模块，引用的地方有这么几处


| files                     | memo                  |   
| ------------------------- | --------------------- |   
| `core_module.cc`          | 核心模块加载用        |   
| `dict/dict_module.cc`     | 注册词典相关的        |   
| `gear/gears_module.cc`    | 注册所有的gear模块    |   
| `lever/levers_module.cc`  | 一些工具              |   


## Q3. 一些子模块的功能说明
librime的处理分为几个大的步骤，例如：预处理（可以快捷键之类的），分词和语义检测，翻译，筛选候选词

### step1: processors  处理用户输入的字符，如果返回kAccepted则跳到下一阶段
| files                         | class             | memo                                                      |
| ----------------------------- | ----------------- | --------------------------------------------------------- |
| ascii_composer.cc             | AsciiComposer     | 切换输入法模式和ASCII直接输入模式                         |
| chord_composer.cc             | ChordComposer     | 处理案件映射关系（类似于输入字母转到五笔键码的）          |
| editor.cc                     | ExpressEditor     | 处理space/enter/backspace/del/esc等按键的行为             |
| editor.cc                     | fluency_editor    | (空的)                                                    |
| key_binder.cc                 | KeyBinder         | 处理快捷方式                                              |
| navigator.cc                  | Navigator         | 处理方向键返回之前的候选词                                |
| punctuator.cc                 | Punctuator        | 按理来说应该是和语音相关的，但是还不确定                  |
| recognizer.cc                 | Recognizer        | 识别输入的字符是否匹配某个模式，类似于前向匹配和后向匹配  |
| selector.cc                   | Selector          | 处理翻页和选词的按键信息                                  |
| speller.cc                    | Speller           | 可能是自动纠正声母韵母错误的                              |
| shape.cc                      | ShapeProcessor    | (不知道干嘛的)                                            |

### step2: segmentors 分词和语义检测
| files                         | class             | memo                                                      |
| ----------------------------- | ----------------- | --------------------------------------------------------- |
| abc_segmentor.cc              | AbcSegmentor      |                                                           |
| affix_segmentor.cc            | AffixSegmentor    |                                                           |
| ascii_segmentor.cc            | AsciiSegmentor    |                                                           |
| matcher.cc                    | Matcher           |                                                           |
| punctuator.cc                 | PunctSegmentor    |                                                           |
| fallback_segmentor.cc         | FallbackSegmentor |                                                           |

### step3: translators 一般是从音转字或词，可以通过词典查找
在Translator这个阶段，会调用Query来转换，一般如果整个用户的字符串转换成功，就会跳过后续的Translator。
在有些插件里会调用内部的Translation（private），也可以直接在Query里反馈。

| files                         | class             | memo                                                      |
| ----------------------------- | ----------------- | --------------------------------------------------------- |
| echo_translator.cc            | EchoTranslator    | 貌似就是回显                                              |
| punctuator.cc                 | PunctTranslator   |                                                           |
| table_translator.cc           | TableTranslator   | 包含查表转换和整句转换                                    |
| script_translator.cc          | ScriptTranslator  |                                                           |
| reverse_lookup_translator.cc  | ReverseLookupTranslator   | 反向查表                                          |
| schema_list_translator.cc     | SchemaListTranslator      | 简单的映射                                        |
| switch_translator.cc          | SwitchTranslator  |                                                           |

table_translator 首先查找用户的词库，然后用的是最大匹配，得到一个词，然后在同样的处理后续的候选词。



### step4: filters 过滤候选词
| files                         | class               | memo                                                    |
| ----------------------------- | ------------------- | ------------------------------------------------------- |
| simplifier.cc                 | Simplifier          |                                                         |
| uniquifier.cc                 | Uniquifier          |                                                         |
| cjk_minifier.cc               | CJKMinifier         |                                                         |
| reverse_lookup_filter.cc      | ReverseLookupFilter |                                                         |
| single_char_filter.cc         | SingleCharFilter    |                                                         |

### step5: formatters
| files                         | class             | memo                                                      |
| ----------------------------- | ----------------- | --------------------------------------------------------- |
| shape.cc                      | ShapeFormatter    |                                                           |


## Q4. 如何得到候选词
外部程序调用 `RimeGetContext` 接口的时候会筛选候选词。

```cpp
RIME_API Bool RimeGetContext(RimeSessionId session_id, RimeContext* context) {

  if (ctx->HasMenu()) {
    rime::Segment &seg(ctx->composition()->back());
    //......
    rime::unique_ptr<rime::Page> page(seg.menu->CreatePage(page_size, page_no));
    if (page) {
      context->menu.page_size = page_size;
      context->menu.page_no = page_no;
      //......
      context->menu.num_candidates = page->candidates.size();
      context->menu.candidates = new RimeCandidate[page->candidates.size()];
      for (const rime::shared_ptr<rime::Candidate> &cand : page->candidates) {
        RimeCandidate* dest = &context->menu.candidates[i++];
        dest->text = new char[cand->text().length() + 1];
        std::strcpy(dest->text, cand->text().c_str());
      }
      //......
    }
  }
}
```

在调用 `rime::Segment &seg(ctx->composition()->back())` 获取最后一个Segmentation


当调用 `seg.menu->CreatePage(page_size, page_no)` 的时候会调用候选词过滤的接口：
```cpp
// src\menu.cc
size_t Menu::Prepare(size_t candidate_count)
{
    //......
    CandidateList next_candidates;
    next_candidates.push_back(translations_[k]->Peek());
    if (filter_) {
      filter_(&candidates_, &next_candidates);
    }
    //......
}
```

然后调用这个：
```cpp
// src\engine.cc
void ConcreteEngine::FilterCandidates(Segment* segment,
                                      CandidateList* recruited,
                                      CandidateList* candidates)
{
}
```

## Q5. 如何用rime_api_console.exe测试
输入分为普通输入和特殊命令输入，加大括号可以当作命令输入，
例如输入 `{space}` 可以模拟空格，详细的特殊键名参考 `librime/src/key_table.cc`

常见的如下：
```
space
Tab
Return
BackSpace

Home
End

Left
Up
Right
Down

Page_Up
Page_Down

```

## Q6. 如何实现提供一串候选词

```cpp

shared_ptr<Translation> YourTranslator::Query(const std::string& input, const Segment& segment, std::string* prompt)
{
    ......
    auto res = New<FifoTranslation>();
    int freq = 0.7; //0.7 is very higer rate!
    for (lime_candidate_t* p = my_candidates;p != NULL;p = p->next) {
        auto candidate = New<SimpleCandidate>(
            "longwords",            // who translated it
            segment.start,
            segment.end,
            std::string(p->str),    // translated string
            "~"                // comment
        );

        candidate->set_quality(freq);
        freq -= 0.01;
        res->Append(candidate);
    }
    return res;
}

```


