#include "mailSender.h"
#include <fstream>
#include <iostream>
int main()
{
    char pause[2];
    std::cout<<"start to test email send library"<<std::endl;
    std::cout<<"input y continue to send the text plain email:";
    std::cin>>pause;

    mailSender myEmail("715236311@qq.com","mbuttlectpgrbddj","smtps://smtp.qq.com:465");
    myEmail.setFrom("张三，发送人","715236311@qq.com");
    myEmail.addTo("李四，主送人","kooba5@126.com");
    myEmail.addCc("赵五，抄送人","784537082@qq.com");
    myEmail.addBcc("孙六，密送人","pushforme@outlook.com");
    myEmail.setSubject("这是一个纯文本的测试邮件");

    const char* emailTextPlainContent="here is an pure text";

    myEmail.addEmailContent((unsigned char*)emailTextPlainContent,strlen(emailTextPlainContent),MIME_CONTENT_TYPE_TXT);
    std::string emailContent=myEmail.composeEmail();
    //将compose出来的数据写入文件
    std::ofstream foutTextPlain("textPlainEmail(test_out).txt",std::ios::out | std::ios::trunc);
    foutTextPlain<<emailContent;
    foutTextPlain.close();
    //myEmail.sendMail();
    std::cout<<"success to send the text plain email"<<std::endl;
    //==============发送html邮件==============
    std::cout<<"input y continue to send text html email:";
    std::cin>>pause;
    myEmail.setSubject("这是一个无内嵌数据的html测试邮件");
    const char* fileName="html(test_in).html";
    myEmail.addEmailContent(fileName,MIME_CONTENT_TYPE_HTML);
    emailContent=myEmail.composeEmail();
    //将compose出来的数据写入文件
    std::ofstream foutTextHtml("html(test_out).txt",std::ios::out | std::ios::trunc);
    if(!foutTextHtml.is_open())
    {
        std::cout<<"failed to open the html file"<<std::endl;
        std::cin>>pause;
        return -1;
    }
    foutTextHtml<<emailContent;
    foutTextHtml.close();
    //myEmail.sendMail();
    std::cout<<"success to send the text html mail"<<std::endl;
    //==============发送html带内嵌资源的邮件==============
    std::cout<<"input y to send html with inline source email:";
    std::cin>>pause;
    myEmail.setSubject("这是一个带内嵌资源的html的测试邮件");
    const char* htmlWithSoucefileName="htmlWithInlineSource(test_in).html";
    myEmail.addEmailContent(htmlWithSoucefileName,MIME_CONTENT_TYPE_HTML);
    //添加内嵌资源
    const char* sourceId="inlineSourceId_ABC";
    myEmail.addInlineResource(sourceId,"bigCar(test_in).png",MIME_CONTENT_TYPE_PNG);
    emailContent=myEmail.composeEmail();
    //将compose出来的数据写入文件
    std::ofstream foutTextHtmlWithSource("htmlWithInlineSource(test_out).txt",std::ios::out | std::ios::trunc);
    if(!foutTextHtmlWithSource.is_open())
    {
        std::cout<<"failed to open the html file"<<std::endl;
        std::cin>>pause;
        return -1;
    }
    foutTextHtmlWithSource<<emailContent;
    foutTextHtmlWithSource.close();
    myEmail.sendMail();
    std::cout<<"html with source email has sent"<<std::endl;
    std::cout<<"input y to exit test email library:";
    std::cin>>pause;
    return 0;
}
