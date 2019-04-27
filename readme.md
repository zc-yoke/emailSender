# 简介
这是一个邮件发送模块，可以发送纯文本邮件，html格式邮件（可内嵌资源）
# 依赖
libcurl
# 测试平台
win7 64 mingW32 curl-7.64.0
# 编译方法
- 下载curl后解压到电脑目录
- 打开makefile文件，修改curl_include_dir和curl_lib_dir的值
- 打开testEmail.cpp 修改其中的发件人，收件人等地址和邮箱账户密码
- 使用gnu make工具make