# Eytion++
# (说明)作者因学业问题，本项目暂时停更至6.20日，中途会有小更新，敬请期待
![](eppp.png)
[![https://img.shields.io/badge/EppVersion-v0.2.6|Dev-red](https://img.shields.io/badge/EppVersion-v0.2.6|Dev-red)]()
# Introduce
Eytion++ is developed by PowerAgnelXD based on the original project Eytion, \
which can be regarded as an enhanced version of Eytion, \
and has some more practical functions than Eytion

# License
This project uses the MIT open source license

# Install
You have two ways to get E++:
1. Download the latest compilation from the "action" column above (the versions here are still under debugging and unstable)
2. Get the latest release version through the "release" column on the right. The application packages released here are generally complete and stable (for pure use, it is recommended to download this way)

# Build
After you download the source code package of this project, go to the inside of the downloaded folder\
after setting the working directory of the console to that directory, \
You can get the compilation by the following command:
```
cmake --build build
```

# Example
> HelloWorld
```go
out "hello world!";
```
> Variable
```go
var a="hello ";
var b="world";
out a+b;
a="helloworld!";
out a;
var c:string = "yes"; #You can also write that
out c;
```
> Constant
```go
const c="constant";
out c;
c="hello" # will error
```
> List
```go
var a=[1,2,3,4];
out a[0];
a[1] = 45;
out a[1];
```
> Delete
```go
var a=[1,2,3,4];
delete a;
var b="string";
delete b;
```
> If-elif-else
```go
var a=input();
if (a=="hello")
    out "hello! my friend!";
elif (a=="good bye"){
    out "good bye!!!";
}
else{
    out "well done!";
}
```
> Repeat
```go
var a=["a","cb","fgh","err"];
var index = 0;
repeat(len(a)){
    out a[index];
    index=index+1;
}
```
> While
```go
var a=[1,6,2,5];
var index = 0;
while(index < len(a)){
    out str(a[index]) + "\n";
    index = ++index;
}
```
> For_each
```go
var str_list = ["hello!", "Eytion", "plusplus"];
for_each(var s: str_list){
    out "content: " + s + "\n";
}
```
> For
```go
var a=0-20;
for(a=0-20; a < 0-10; a=a+2;){
    println(a);
}
println(a);
```
> Break
```go
var a=[1,6,2,5];
var index = 0;
var in = input();
while(index < len(a)){
    if(index == 2 && in == "yes") break;
    out str(a[index]) + "\n";
    index=index+1;
}
```
> Special keywords/built-in functions/features
```go
# typeof
var a=typeof("hello");
out a; # output: __STRING__

# input
var b=input();
var c=input("input: ");
out b;
out c;

# len
var list = [1,2,3,4,5];
out len(list);

# type conversion
var i=input() # type: __STRING__
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

# Config File
Typically, the format of Epp's configuration file is: ". ecfg"\
Now, this section will introduce you to the configuration system in EppCli Generally speaking\
the default name of EPP's configuration file is ***common.ecfg*** it should be in the folder: \
`resource\config\` \
Here are the names of several configuration items and their purposes:
|name|purpose|value|
|:--:|:--:|:--:|
|cli-exittip|Used to prompt you whether to exit when exiting eppcli|true/false|
|cli-nameshow|Display the word eppcli under the cursor of eppcli: "EppCli"|true/false|
|cli-workspace|Displays the current working path|true/false|
|cli-errorhighlight|Whether to highlight errors in EPP (not for errors in eppcli)|true/false|
|debug-echoast|Output AST of running program|true/false|
|debug-echotokeng|Output TokenGroup of running program|true/false|

When you modify the configuration file and need to re apply and load it, you should enter the following code in eppcli:
```bash
redcfg
```

# New Feature
1. - [x] null value
2. - [ ] More convenient function declaration
3. - [ ] More humanized variable declaration statement