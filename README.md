# Eytion++
![](eppp.png)
### Introduce
Eytion++ is developed by PowerAgnelXD based on the original project Eytion, \
which can be regarded as an enhanced version of Eytion, \
and has some more practical functions than Eytion

### License
This project uses the MIT open source license

### Install
If you are a linux system, you can get the latest compilation through action \
If your system is Windows, you can get the compilation through release

### Build
After you download the source code package of this project, go to the inside of the downloaded folder\
after setting the working directory of the console to that directory, \
You can get the compilation by the following command:
```
cmake --build build
```

### Example
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
> If
```go
var a=input();
if (a=="hello")
    out "hello! my friend!";
if (a=="good bye"){
    out "good bye!!!";
}
```
> Repeat
```go
var a=["a","cb",fgh","err"];
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
    index=index+1;
}
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
> Special keywords/built-in functions
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
```