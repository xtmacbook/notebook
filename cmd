echo off & color 0A
 
::遍历指定文件夹下的文件
::for [参数] %%变量名 in （匹配符） do （执行的命令） 
::无参数
::for %%i in (*) do ( echo %%i ) 

::[参数]：参数取值一共有四种： /d, /r, /l, /f，加上无参数，所以一共五种场景
::无参：遍历当前路径的文件夹下的文件，但也可在(匹配符)中指定路径
::/d：遍历当前路径的文件夹下的文件夹，但也可在(匹配符)中指定路径
::/r [路径]：深度遍历指定路径下的所有文件，子目录中的文件也会被遍历到，如果没指定路径，默认当前路径
::/l ：当使用参数 /l 时，需结合(匹配符)一起使用，此时 () 括号内部的用法规则为：(start, step, end)，此时的 for 命令作用等同于 java 语言中的 for 语句
::/f ：用于解析文件中的内容，本节不做介绍

%%~fi：表示获取该文件的绝对路径信息
%%~di：表示获取该文件所在的盘符
%%~pi：表示获取该文件的路径，不包含盘符的信息
%%~ni：表示获取该文件的文件名，不包含扩展名信息
%%~xi：表示获取该文件的扩展名
%%~ti：表示获取该文件的上次修改时间
%%~zi：表示获取该文件的大小


::有参数
for  /d %%i  in (*pro) do (
echo %%i 
::echo %%~fi
echo %%~fi
)
https://github.com/TheRealMJP/MSAAFilter
https://blog.selfshadow.com/
