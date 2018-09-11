主要由三部分组成：变量声明，管道状态technique, pass，渲染函数, 

1.  变量声明在文件开始，这些变量可以在运行时操作，格式如下，

type id : tag = {initiliaze value} ;  

其中 type 必须是 HLSL 可识别的类型, id 必须唯一, tag 用来表示变量的用途,  后面是初始值

2. 管道状态, 一个 fx 文件可以配置多个 technique,  一个 technique 可以配置多个 pass 特殊如下，

technique (tch_uniqueId)

{

     pass (pass_uniqueId)

     {

          Texture[0] = <diffuseTexture>;

          VertexShader = compile vs_2_2 vsMain()

          PiexlShader = compile ps_2_2 psMain()

     }

}

配置的 technique , pass 可以根据 显卡的支持，以及应用程序的需求，选择性的激活相应的 technique 和 pass

3. 渲染函数，   这个就是传统意义上的着色程序，定点着色，片段着色等
