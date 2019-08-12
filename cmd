echo off & color 0A
 
::获取"animation"文件夹下文件夹以及文件名,输出到:a.txt文件中
DIR /R "animation" /B >a.txt

::指定起始文件夹
echo DIR=%DIR%

:: 参数 /R 表示需要遍历子文件夹,去掉表示不遍历子文件夹
:: %%f 是一个变量,类似于迭代器,但是这个变量只能由一个字母组成,前面带上%%
:: 括号中是通配符,可以指定后缀名,*.*表示所有文件
for /R %DIR% %%f in (*.pro) do ( 
echo %%f
qmake -tp vc %%f
)
pause 
