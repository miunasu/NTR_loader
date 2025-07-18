# NTR loader
Loader Pre-Technology, Main thread hijacking without using API, get ntdll and kernel32 handle without peb.  
加载器前置技术，不使用API进行主线程劫持，不使用PEB获取ntdll和kernel32的地址。  

This method is an improvement and extension of [NativeThreadRobber](https://github.com/miunasu/NativeThreadRobber).  
该技术是[NativeThreadRobber](https://github.com/miunasu/NativeThreadRobber)的改进和扩展。

# What this project can do
Are you tired of the cloud sandbox?  
你是否厌烦了云沙箱?  
Are you afraid that the method to get the handle through the PEB is already marked?    
你是否害怕通过PEB获取句柄的方法已被标记?  

Cloud sandbox does not have white exe file. For the dll who used main thread hijack，cloud sandbox can't run dynamically to get the behavior results.  
云沙箱没有exe白文件，对于使用主线程劫持的dll，云沙箱无法动态跑出行为结果。  
Get the handles of ntdll and kernel32 through the process of main thread hijacking.  
通过主线程劫持的过程获取ntdll和kernel32的句柄。 

No API will be used at all to achieve the above purpose!  
不使用任何API达到以上效果！

![](./pic/demo.png)

# Other
This Pre-Technology can achieve good anti anti-virus effect in combination with other technologies。After writing a simple loader for testing, it can avoid killing by most anti-virus software.  

该前置技术配合其他免杀技术可以达成良好的免杀效果，经过编写简易加载器测试，免杀大部分国外杀软，通杀国内杀软，包括XXX天擎edr、xx智甲edr等。

This project is just to share new and interesting simple techniques I found, so I won’t write about the matching technology in detail.   
Recommended matching technology:
>Gate Series  
>Stack Spoof  
>RDI  
>Unhook  
>Anti local Sandbox  
>SigFlip  
>Obfuscate  
>Encode  
>......  

该项目仅分享我新发现的有趣的简单技术，所以就不详细编写其他配合的技术。  
推荐配合使用的技术：
>门系列  
>栈欺骗  
>反射式注入  
>反hook  
>反杀软本地沙箱  
>SigFlip  
>混淆  
>加密  
>......



# Support
It's support x64 and x86 both.  
If you have any question, please open an issue on GitHub.

