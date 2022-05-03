# Eytion++
![](eppp.png)
[![https://img.shields.io/badge/EppVersion-v0.2.7|Dev-red](https://img.shields.io/badge/EppVersion-v0.2.9|Dev-red)]()
[![https://img.shields.io/badge/Lincense-MIT-green](https://img.shields.io/badge/Lincense-MIT-green)]()
[![https://img.shields.io/badge/Platform-Windows/Linux-blue](https://img.shields.io/badge/Platform-Windows/Linux-blue)]()
# Introduce 介绍
Eytion++是由PowerAgnelXD在项目Eytion的基础上开发的，因此可以看作是Eytion的增强版，他也有比Eytion有更多的实用功能

# License 许可证
该项目使用MIT开源许可证

# Install 安装/下载
你有两种方式去获得Epp解释器：
 - 你可以通过右侧的“release”栏获得最新的Epp解释器包，它更完整
 - 你还可以通过上方的“Action”获得最新的编译，注意，这些获取的都是应用程序！

# Build 编译
需要说明的是，Epp使用CMake构建，所以当您下载到源码包后，你只需要按照CMake的方式编译就行了\
当然我本人更推荐您用 VisualStudioCode，这是一款十分好用的编辑器，上面有专属于Epp的扩展和便利的CMake扩展

# Example 例子
> 您的第一个程序
```go
# 三种方法：
out "hello world!";
println("hello world!");
print("hello world!");
```
> 变量操作
```go
var a="hello ";
var b="world";
println(a+b);
a="helloworld!"; # 赋值
println(a);
var c:string = "yes"; #你也可以指定变量的类型
println(c);
```
> 常量操作
```go
const c="constant";
println(c);
c="hello" # will error
```
> 列表
```go
var a=[1,2,3,4];
println(a[0]);
a[1] = 45;
println(a[1]);
```
> 删除语句
```go
var a=[1,2,3,4];
delete a;
var b="string";
delete b;
```
> 条件判断语句
```go
var a=input();
if (a=="hello")
    println("hello! my friend!");
elif (a=="good bye"){
    println("good bye!!!");
}
else{
    println("well done!");
}
```
> 重复执行语句
```go
var a=["a","cb","fgh","err"];
var index = 0;
repeat(len(a)){
    println(a[index]);
    index=index+1;
}
```
> While循环语句
```go
var a=[1,6,2,5];
var index = 0;
while(index < len(a)){
    println(str(a[index]) + "\n");
    index = ++index;
}
```
> 适用于遍历的循环语句
```go
var str_list = ["hello!", "Eytion", "plusplus"];
for_each(var s: str_list){
    println("content: " + s + "\n");
}
```
> For循环语句
```go
var a=0-20;
for(a=0-20; a < 0-10; a=a+2;){
    println(a);
}
println(a);
```
> 跳出循环语句
```go
var a=[1,6,2,5];
var index = 0;
var in = input();
while(index < len(a)){
    if(index == 2 && in == "yes") break;
    println(str(a[index]) + "\n");
    index=index+1;
}
```
> 特殊的关键字/BIF/或者特性
```go
# typeof
var a=typeof("hello");
out a; # 输出的是传入的表达式的类型

# input
var b=input();
var c=input("input: ");
out b;
out c;

# len
var list = [1,2,3,4,5];
out len(list);

# type conversion
var i=input() # input的返回值默认为string类型
if(int(i) == 12)
    out "yes!";

# print/println
var a=print(123)
out a;

# system
system("echo hhh");

# null
var a=null;
if(a==null){
    println(123);
}
a=4;
println(a);
```

# Config File 配置文件
通常来说，Epp解释器的配置文件格式为: ". ecfg"\
现在，本节将向您介绍EppCli中的配置系统\
EPP配置文件的默认名称为 ***.ecfg*** 它应该在文件夹: `resource\config\` 中
以下是所有配置项的名称及其用途:
|名称|用途|可用值|
|:--:|:--:|:--:|
|cli-exittip|用于在退出eppcli时提示您是否退出|true/false|
|cli-nameshow|在eppcli的光标下显示单词eppcli：“eppcli”|true/false|
|cli-workspace|显示当前工作路径 *** 只能用于Windows！ ***|true/false|
|cli-errorhighlight|是否突出显示EPP中的错误（不适用于eppcli中的错误）|true/false|
|debug-echoast|输出正在运行的程序的抽象语法树|true/false|
|debug-echotokeng|输出正在运行的程序的Token Group|true/false|

修改配置文件并需要重新应用和加载时，应在eppcli中输入以下代码：
```bash
redcfg
```

# What will happen next 将会更新
1. - [x] null value
2. - [ ] More convenient function declaration
3. - [ ] More humanized variable declaration statement
4. - [ ] Modular system
5. - [ ] Faster speed
6. - [ ] Better support for Linux