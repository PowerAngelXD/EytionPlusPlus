# Eytion++
![](eppp.png)
### Introduce
Eytion++ is developed by PowerAgnelXD based on the original project Eytion, \
which can be regarded as an enhanced version of Eytion, \
and has some more practical functions than Eytion

### License
This project uses the MIT open source license

### Install
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
> Special keywords
```go
# typeof
var a=typeof("hello");
out a; # output: __STRING__

# input
var b=input();
var c=input("input: ");
out b;
out c;
```