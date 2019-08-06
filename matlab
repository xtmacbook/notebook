mex mxGetEdges.cpp -ID:/3rdlib/opencv-3.4.6/vc14/install/include -LD:/3rdlib/opencv-3.4.6/vc14/install/x64/vc14/lib -lopencv_imgproc346 -lopencv_core346 -lopencv_imgcodecs346 -lopencv_img_hash346




冒号:
a:b -》  	表示[a,a+1,……,b]
a:c:b-》  	[a,a+c,……,a+n*c],其中n=fix((b-a)/c)
A(:)->    	表示以一列的方式显示A中所有元素
b=A(i, :) 	表示把矩阵A的第i行存入b
b=A( :,j) 	表示把矩阵A的第j列存入b
b=A(J:K) 	表示把矩阵A中[A(J),A(J+1),...,A(K)]这些元素存入b中
b=A（:,c:d）	表示把矩阵A的第c到第d列存入 b中，当然c，d大于A的列数则出错
b=A(2:3,1:2:5)	显示A的第二行与第三行的第一、三、五列: 从num1到num3 步长num2
A(i, 1:3)	
for i=0 : 2 :10 表示的是i从0开始，每次增长2，一直到10

.*(点乘)-> 
  作用：矩阵各个元素的乘法
  用法举例：A=[1 2 3],B=[4 5 6].
  A*B=错误结果，因为不符合矩阵相乘的条件，A.*B=1*4 2*5 3*6=[4 10 18]
