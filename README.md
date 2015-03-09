# AppUpdate
##1： 外部依赖
  1> boost 库， 当前开发采用的是1.55版本<br\>
  2> avhttp 库，感谢jack的开源。该库的地址是: https://github.com/avplayer/avhttp<br\>
  3> zlib 库， 本库已经也已经集成在项目中。通过编译就可以了。本项目采用的是 1.2.5 版本<br\>
##2： 下载的配置文件字段说明
  1> name : 下载后文件保存的名字<br\>
  2> md5 : 下载文件的md5值，每次升级前会获取本地文件的md5，如果与服务器相同，则不进行下载<br\>
  3> url : 文件的下载地址<br\>
  4> description : 文件描述文案<br\>
##3： 项目路径说明
  AppUpdate\ ： 项目的主要文件<br\>
  include\avhttp\ : avhttp库，由于该库是 Http only 。因此拷贝到这里就可以了<br\>
  include\boost : boost库的文件<br\>
  include\zlib-1.2.5 : zlib库<br\>
  include\ :   项目的头文件存放处<br\>
  bin\ ： 项目生成的 exe 输出地方<br\>
  lib\boost : boost 的所有lib文件<br\>
  lib\zlib : zlib项目编译生成的lib<br\>
  
