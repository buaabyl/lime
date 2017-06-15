# gplime

## Warning
1. main dialog for IME __must__ not using MFC based dialog or windows!!!
2. `RegisterClassEx` on `DllMain`.
3. An empty framework still need correct parameters for `ImeInquire` function.

## gplime-0.1.0-2012
Learning some `chinese word split method` and `full-text search`,
So this simplest input method is a summary of study, as my handout:)

Feature:
1. Using dict based wordseg and frequency order.
2. Simplest input interface.
3. Embedded lua as control engine.

Step:
1. 先创建一个空的win32 dll项目（在新建项目那选择win32项目，然后在随后打开的向导中把应用程序类型设置为Dll，再把[空项目]一项选中；
2. 往建好的项目里新建一个资源文件（.rc），然后往这个资源文件中添加一个Version资源；
3. 打开这个Version资源，里面有三处信息是要注意的，其中两处要把值改对：
- FILETYPE 默认的是`VFT DLL`，但这里必须改选成`VFT DRV`，否则调用ImmInstallIME时会返回调用失败；
- FILESUBTYPE 默认的是`VFT2 UNKNOWN`，但这里必须改选成`VFT2 DRV INPUTMETHOD`，否则同样会导致调用ImmInstallIME函数失败；
- FileDescription 当输入法在语言栏中显示时，这项里的值将作为该输入法在语言栏上显示的文本信息，此信息可与输入法名称不相同。
错误的时候一般都是找不到文件，而不会提示文件有错误。
资源里`FileDescription` 这一项会在输入法任务栏里显示的，不要乱写。
第一次加载调用会`ImeInquire`函数的。

## gplime-0.1.0-2014
Modify dialog create.

## gplime-0.2.0-2017
Learning `Machine Learning` and `Deep Learning`, 
Try to rewrite the old input method for fun.

Stage1:
1. Seperate algorithm and UI.
2. Seperate to server and client.
3. Rewrite to HMM or CRF algorithm.
4. Embedded luajit and duktape(javascript) extension interface.
5. Support windows7 only, drop windows xp test.
6. Push main program to GitHub as MIT license.
7. Push model as GPLv3 license, because the license of most corpus data for research only.

Stage2:
1. Using deep learning algorithm.
2. Digging Google's cosine relation.




