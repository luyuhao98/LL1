# 简介

用户友好的C++ LL(1)语法分析程序

# 文件目录：
LL1.pp　　　　　　　　　　　　　 LL(1)语法分析程序\
LL1　　　　　　　　　　　　　　　Linux下可执行文件，由g++7.3.0编译\
Grammar.txt　　　　　　　　　　　文法文件\
out　　　　　　　　　　　　　　　　\
｜——**document.html**　　　　　　　 **文档**，html格式,请用浏览器打开(推荐Chrome，不推荐IE)\
｜——[other　files]　　　　　　　文档的格式素材文件\
document　　　　　　　　　　　　　\
｜——document.mdk　　　　　　　　生成文档的madoko文件\
｜——[other　files]　　　　　　　文档的格式素材文件\

# 使用方法

1. 编辑文法文件    Grammar.txt
2. 运行可执行文件　lex/lex.exe

# 修改文档　(需要madoko环境)
1. 编辑`document/document.mdk `
2. 回到主目录运行 `madoko -v document/document.mdk`
3. 观察`out/document.html`

# [Github](https://github.com/luyuhao98/LL1)
`ssh:　git@github.com:luyuhao98/LL1.git`
