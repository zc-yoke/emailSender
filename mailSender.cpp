#include "mailSender.h"
#include "b64.h"
#include <time.h> //time()
#include <ctime>
#include <fstream>
#include <assert.h>
mailSender::mailSender(std::string senderAccount_,std::string senderPassword_,std::string senderServer_)
    :senderAccount(senderAccount_),senderPassword(senderPassword_),senderServer(senderServer_)
{

    hasTextPlainContent=false;
    hasTextHtmlContent=false;
    //初始化随机种子
    srand((int) time(NULL));

    /*无论本邮件是否存在附件或者是内嵌资源都为multipart/mixed，
    multipart/related，multipart/alternative生成一个boundary*/
    //初始化二个boundary
    //related boundary
    relatedBoundary="relatedBoundary_ABCDEFG";
    //alternative boundary
    alternativeBoundary="alternativeBoundary_ABCDEFG";
}

mailSender::~mailSender()
{

}

void mailSender::addTo(std::string to_name,std::string to_email)
{
    struct_recipients tempRecipients;
    //收件人姓名，用base64编码
    char* enc=b64_encode((unsigned char*)(to_name.c_str()),to_name.length()+1);
    tempRecipients.recipientsName=enc;
    //释放内存
    free(enc);

    //收件人邮件地址
    tempRecipients.recipientsEmail=to_email;
    //将地址压入容器
    mailTo.push_back(tempRecipients);
}

void mailSender::addCc(std::string cc_name,std::string cc_email)
{
    struct_recipients tempRecipients;
    //收件人姓名，用base64编码
    char* enc=b64_encode((unsigned char*)(cc_name.c_str()),cc_name.length()+1);
    tempRecipients.recipientsName=enc;
    //释放内存
    free(enc);
    //收件人邮件地址
    tempRecipients.recipientsEmail=cc_email;
    //将地址压入容器
    mailCc.push_back(tempRecipients);
}

void mailSender::addBcc(std::string bcc_name,std::string bcc_email)
{
    struct_recipients tempRecipients;
    //收件人姓名，用base64编码
    char* enc=b64_encode((unsigned char*)(bcc_name.c_str()),bcc_name.length()+1);
    tempRecipients.recipientsName=enc;
    //释放内存
    free(enc);
    //收件人邮件地址
    tempRecipients.recipientsEmail=bcc_email;
    //将地址压入容器
    mailBcc.push_back(tempRecipients);
}

void mailSender::setFrom(std::string from_name,std::string from_email)
{
    //收件人姓名，用base64编码
    char* enc=b64_encode((unsigned char*)(from_name.c_str()),from_name.length()+1);
    mailFrom.recipientsName=enc;
    //释放内存
    free(enc);
    //收件人邮件地址
    mailFrom.recipientsEmail=from_email;
}

void mailSender::setSubject(std::string subject)
{
    mailSubject="";
    //转码
    mailSubject="=?UTF-8?B?";
    char* enc=b64_encode((unsigned char*)subject.c_str(),subject.length());
    mailSubject+=enc;
    free(enc);
    mailSubject+="?=";
}

void mailSender::addInlineResource(const char* sourceId,const char* fileName,unsigned char* fileData, uint32_t dataLength, mimeContentType contentType)
{
    struct_mailInlineResource tempInlineResource;
    //fileName
    tempInlineResource.fileName="car.png";//fileName;
    //content type
    tempInlineResource.contentType=contentType;
    //content
    //将fileData用base64编码
    char* encoded=b64_encode(fileData,dataLength);
    free(fileData);
    std::string mailContent;
    uint32_t qtyRow=strlen(encoded)/120;
    char tempContent[121]= {0};
    for(uint32_t row=0; row<qtyRow; ++row)
    {
        memcpy(tempContent,encoded+row*120,120);
        mailContent+=tempContent;
        mailContent+="\n";
    }
    memset(tempContent,0,121);
    memcpy(tempContent,encoded+qtyRow*120,strlen(encoded)-qtyRow*120);
    free(encoded);
    mailContent+=tempContent;
    mailContent+="\n";

    tempInlineResource.content=mailContent;
    //contentId
    tempInlineResource.contentId=sourceId;

    //将附件数据压入容器
    mailInlineResource.push_back(tempInlineResource);
}

void mailSender::addInlineResource(const char* sourceId,const char* fileName, mimeContentType contentType)
{
    //读取文件内容
    std::ifstream inlineResourceStream(fileName,std::ios::binary|std::ios::in);
    if(inlineResourceStream.is_open()==false)
    {
// TODO (Administrator#1#): 此处需要进行错误处理
        return;
    }
    //获得文件大小
    inlineResourceStream.seekg(0, std::ios_base::end);
    uint32_t fileSize=inlineResourceStream.tellg();
    //将读取位置设置在文件头部
    inlineResourceStream.seekg(0, std::ios::beg);
    unsigned char *buff = new unsigned char [fileSize];
    inlineResourceStream.read((char*)buff,fileSize);
    inlineResourceStream.close();

    addInlineResource(sourceId,fileName,buff,fileSize,contentType);
}

void mailSender::addEmailContent(unsigned char* data,uint32_t dataLength,mimeContentType contentType)
{
    if(contentType==MIME_CONTENT_TYPE_TXT)
    {
        hasTextPlainContent=true;
        //boundary
        mailPlainTextContent.boundary=alternativeBoundary;
        //content
        //base64编码
        char* encoded=b64_encode(data,dataLength);
        free(data);
        //将字符串按照每行120字符排列
        std::string mailContent;
        uint32_t qtyRow=strlen(encoded)/120;
        char tempContent[121]= {0};
        for(uint32_t row=0; row<qtyRow; ++row)
        {
            memcpy(tempContent,encoded+row*120,120);
            mailContent+=tempContent;
            mailContent+="\n";
        }
        memset(tempContent,0,121);
        memcpy(tempContent,encoded+qtyRow*120,strlen(encoded)-qtyRow*120);
        free(encoded);
        mailContent+=tempContent;
        mailContent+="\n";

        mailPlainTextContent.content=mailContent;
        //contentType
        mailPlainTextContent.contentType=MIME_CONTENT_TYPE_TXT;
        return;
    }
    if(contentType==MIME_CONTENT_TYPE_HTML)
    {
        hasTextHtmlContent=true;
        //boundary
        mailHtmlTextContent.boundary=alternativeBoundary;
        //content
        //base64编码
        char* encoded=b64_encode(data,dataLength);
        free(data);
        //将字符串按照每行120字符排列
        std::string mailContent;
        uint32_t qtyRow=strlen(encoded)/120;
        char tempContent[121]= {0};
        for(uint32_t row=0; row<qtyRow; ++row)
        {
            memcpy(tempContent,encoded+row*120,120);
            mailContent+=tempContent;
            mailContent+="\n";
        }
        memcpy(tempContent,encoded+qtyRow*120,strlen(encoded)-qtyRow*120);
        memset(tempContent,0,121);
        free(encoded);
        mailContent+=tempContent;
        mailContent+="\n";

        mailHtmlTextContent.content=mailContent;

        //contentType
        mailHtmlTextContent.contentType=MIME_CONTENT_TYPE_HTML;
        return;
    }
}

void mailSender::addEmailContent(const char* fileName,mimeContentType contentType)
{
    //读取文件内容
    std::ifstream plainTextEmailContentStream(fileName,std::ios::in|std::ios::binary);
    if(plainTextEmailContentStream.is_open()==false)
    {
// TODO (Administrator#1#): 此处需要进行错误处理
        return;
    }
    //获得文件大小
    plainTextEmailContentStream.seekg(0, std::ios_base::end);
    uint32_t fileSize=plainTextEmailContentStream.tellg();
    //将读取位置设置在文件头部
    plainTextEmailContentStream.seekg(0, std::ios::beg);
    unsigned char *buff = new unsigned char [fileSize];
    memset(buff,0,fileSize);
    plainTextEmailContentStream.read((char*)buff,fileSize);
    plainTextEmailContentStream.close();

    addEmailContent(buff,fileSize,contentType);
}

std::string mailSender::getConentTypeString(mimeContentType contentType)
{
    switch (contentType)
    {
    case MIME_CONTENT_TYPE_EVY:
        return "application/envoy";
    case MIME_CONTENT_TYPE_FIF:
        return "application/fractals";
    case MIME_CONTENT_TYPE_SPL:
        return "application/futuresplash";
    case MIME_CONTENT_TYPE_HTA:
        return "application/hta";
    case MIME_CONTENT_TYPE_ACX:
        return "application/internet-property-stream";
    case MIME_CONTENT_TYPE_HQX:
        return "application/mac-binhex40";
    case MIME_CONTENT_TYPE_DOC:
        return "application/msword";
    case MIME_CONTENT_TYPE_DOT:
        return "application/msword";
    case MIME_CONTENT_TYPE_BIN:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_CLASS:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_DMS:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_EXE:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_LHA:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_LZH:
        return "application/octet-stream";
    case MIME_CONTENT_TYPE_ODA:
        return "application/oda";
    case MIME_CONTENT_TYPE_AXS:
        return "application/olescript";
    case MIME_CONTENT_TYPE_PDF:
        return "application/pdf";
    case MIME_CONTENT_TYPE_PRF:
        return "application/pics-rules";
    case MIME_CONTENT_TYPE_P10:
        return "application/pkcs10";
    case MIME_CONTENT_TYPE_CRL:
        return "application/pkix-crl";
    case MIME_CONTENT_TYPE_AI:
        return "application/postscript";
    case MIME_CONTENT_TYPE_EPS:
        return "application/postscript";
    case MIME_CONTENT_TYPE_PS:
        return "application/postscript";
    case MIME_CONTENT_TYPE_RTF:
        return "application/rtf";
    case MIME_CONTENT_TYPE_SETPAY:
        return "application/set-payment-initiation";
    case MIME_CONTENT_TYPE_SETREG:
        return "application/set-registration-initiation";
    case MIME_CONTENT_TYPE_XLA:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_XLC:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_XLM:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_XLS:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_XLT:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_XLW:
        return "application/vnd.ms-excel";
    case MIME_CONTENT_TYPE_MSG:
        return "application/vnd.ms-outlook";
    case MIME_CONTENT_TYPE_SST:
        return "application/vnd.ms-pkicertstore";
    case MIME_CONTENT_TYPE_CAT:
        return "application/vnd.ms-pkiseccat";
    case MIME_CONTENT_TYPE_STL:
        return "application/vnd.ms-pkistl";
    case MIME_CONTENT_TYPE_POT:
        return "application/vnd.ms-powerpoint";
    case MIME_CONTENT_TYPE_PPS:
        return "application/vnd.ms-powerpoint";
    case MIME_CONTENT_TYPE_PPT:
        return "application/vnd.ms-powerpoint";
    case MIME_CONTENT_TYPE_MPP:
        return "application/vnd.ms-project";
    case MIME_CONTENT_TYPE_WCM:
        return "application/vnd.ms-works";
    case MIME_CONTENT_TYPE_WDB:
        return "application/vnd.ms-works";
    case MIME_CONTENT_TYPE_WKS:
        return "application/vnd.ms-works";
    case MIME_CONTENT_TYPE_WPS:
        return "application/vnd.ms-works";
    case MIME_CONTENT_TYPE_HLP:
        return "application/winhlp";
    case MIME_CONTENT_TYPE_BCPIO:
        return "application/x-bcpio";
    case MIME_CONTENT_TYPE_XCDF:
        return "application/x-cdf";
    case MIME_CONTENT_TYPE_Z:
        return "application/x-compress";
    case MIME_CONTENT_TYPE_TGZ:
        return "application/x-compressed";
    case MIME_CONTENT_TYPE_CPIO:
        return "application/x-cpio";
    case MIME_CONTENT_TYPE_CSH:
        return "application/x-csh";
    case MIME_CONTENT_TYPE_DCR:
        return "application/x-director";
    case MIME_CONTENT_TYPE_DIR:
        return "application/x-director";
    case MIME_CONTENT_TYPE_DXR:
        return "application/x-director";
    case MIME_CONTENT_TYPE_DVI:
        return "application/x-dvi";
    case MIME_CONTENT_TYPE_GTAR:
        return "application/x-gtar";
    case MIME_CONTENT_TYPE_GZ:
        return "application/x-gzip";
    case MIME_CONTENT_TYPE_HDF:
        return "application/x-hdf";
    case MIME_CONTENT_TYPE_INS:
        return "application/x-internet-signup";
    case MIME_CONTENT_TYPE_ISP:
        return "application/x-internet-signup";
    case MIME_CONTENT_TYPE_III:
        return "application/x-iphone";
    case MIME_CONTENT_TYPE_JS:
        return "application/x-javascript";
    case MIME_CONTENT_TYPE_LATEX:
        return "application/x-latex";
    case MIME_CONTENT_TYPE_MDB:
        return "application/x-msaccess";
    case MIME_CONTENT_TYPE_CRD:
        return "application/x-mscardfile";
    case MIME_CONTENT_TYPE_CLP:
        return "application/x-msclip";
    case MIME_CONTENT_TYPE_DLL:
        return "application/x-msdownload";
    case MIME_CONTENT_TYPE_M13:
        return "application/x-msmediaview";
    case MIME_CONTENT_TYPE_M14:
        return "application/x-msmediaview";
    case MIME_CONTENT_TYPE_MVB:
        return "application/x-msmediaview";
    case MIME_CONTENT_TYPE_WMF:
        return "application/x-msmetafile";
    case MIME_CONTENT_TYPE_MNY:
        return "application/x-msmoney";
    case MIME_CONTENT_TYPE_PUB:
        return "application/x-mspublisher";
    case MIME_CONTENT_TYPE_SCD:
        return "application/x-msschedule";
    case MIME_CONTENT_TYPE_TRM:
        return "application/x-msterminal";
    case MIME_CONTENT_TYPE_WRI:
        return "application/x-mswrite";
    case MIME_CONTENT_TYPE_CDF:
        return "application/x-netcdf";
    case MIME_CONTENT_TYPE_NC:
        return "application/x-netcdf";
    case MIME_CONTENT_TYPE_PMA:
        return "application/x-perfmon";
    case MIME_CONTENT_TYPE_PMC:
        return "application/x-perfmon";
    case MIME_CONTENT_TYPE_PML:
        return "application/x-perfmon";
    case MIME_CONTENT_TYPE_PMR:
        return "application/x-perfmon";
    case MIME_CONTENT_TYPE_PMW:
        return "application/x-perfmon";
    case MIME_CONTENT_TYPE_PNG:
        return "image/png";
    case MIME_CONTENT_TYPE_P12:
        return "application/x-pkcs12";
    case MIME_CONTENT_TYPE_PFX:
        return "application/x-pkcs12";
    case MIME_CONTENT_TYPE_P7B:
        return "application/x-pkcs7-certificates";
    case MIME_CONTENT_TYPE_SPC:
        return "application/x-pkcs7-certificates";
    case MIME_CONTENT_TYPE_P7R:
        return "application/x-pkcs7-certreqresp";
    case MIME_CONTENT_TYPE_P7C:
        return "application/x-pkcs7-mime";
    case MIME_CONTENT_TYPE_P7M:
        return "application/x-pkcs7-mime";
    case MIME_CONTENT_TYPE_P7S:
        return "application/x-pkcs7-signature";
    case MIME_CONTENT_TYPE_SH:
        return "application/x-sh";
    case MIME_CONTENT_TYPE_SHAR:
        return "application/x-shar";
    case MIME_CONTENT_TYPE_SWF:
        return "application/x-shockwave-flash";
    case MIME_CONTENT_TYPE_SIT:
        return "application/x-stuffit";
    case MIME_CONTENT_TYPE_SV4CPIO:
        return "application/x-sv4cpio";
    case MIME_CONTENT_TYPE_SV4CRC:
        return "application/x-sv4crc";
    case MIME_CONTENT_TYPE_TAR:
        return "application/x-tar";
    case MIME_CONTENT_TYPE_TCL:
        return "application/x-tcl";
    case MIME_CONTENT_TYPE_TEX:
        return "application/x-tex";
    case MIME_CONTENT_TYPE_TEXI:
        return "application/x-texinfo";
    case MIME_CONTENT_TYPE_TEXINFO:
        return "application/x-texinfo";
    case MIME_CONTENT_TYPE_ROFF:
        return "application/x-troff";
    case MIME_CONTENT_TYPE_T:
        return "application/x-troff";
    case MIME_CONTENT_TYPE_TR:
        return "application/x-troff";
    case MIME_CONTENT_TYPE_MAN:
        return "application/x-troff-man";
    case MIME_CONTENT_TYPE_ME:
        return "application/x-troff-me";
    case MIME_CONTENT_TYPE_MS:
        return "application/x-troff-ms";
    case MIME_CONTENT_TYPE_USTAR:
        return "application/x-ustar";
    case MIME_CONTENT_TYPE_SRC:
        return "application/x-wais-source";
    case MIME_CONTENT_TYPE_CER:
        return "application/x-x509-ca-cert";
    case MIME_CONTENT_TYPE_CRT:
        return "application/x-x509-ca-cert";
    case MIME_CONTENT_TYPE_DER:
        return "application/x-x509-ca-cert";
    case MIME_CONTENT_TYPE_PKO:
        return "application/ynd.ms-pkipko";
    case MIME_CONTENT_TYPE_ZIP:
        return "application/zip";
    case MIME_CONTENT_TYPE_AU:
        return "audio/basic";
    case MIME_CONTENT_TYPE_SND:
        return "audio/basic";
    case MIME_CONTENT_TYPE_MID:
        return "audio/mid";
    case MIME_CONTENT_TYPE_RMI:
        return "audio/mid";
    case MIME_CONTENT_TYPE_MP3:
        return "audio/mpeg";
    case MIME_CONTENT_TYPE_AIF:
        return "audio/x-aiff";
    case MIME_CONTENT_TYPE_AIFC:
        return "audio/x-aiff";
    case MIME_CONTENT_TYPE_AIFF:
        return "audio/x-aiff";
    case MIME_CONTENT_TYPE_M3U:
        return "audio/x-mpegurl";
    case MIME_CONTENT_TYPE_RA:
        return "audio/x-pn-realaudio";
    case MIME_CONTENT_TYPE_RAM:
        return "audio/x-pn-realaudio";
    case MIME_CONTENT_TYPE_WAV:
        return "audio/x-wav";
    case MIME_CONTENT_TYPE_BMP:
        return "image/bmp";
    case MIME_CONTENT_TYPE_COD:
        return "image/cis-cod";
    case MIME_CONTENT_TYPE_GIF:
        return "image/gif";
    case MIME_CONTENT_TYPE_IEF:
        return "image/ief";
    case MIME_CONTENT_TYPE_JPE:
        return "image/jpeg";
    case MIME_CONTENT_TYPE_JPEG:
        return "image/jpeg";
    case MIME_CONTENT_TYPE_JPG:
        return "image/jpeg";
    case MIME_CONTENT_TYPE_JFIF:
        return "image/pipeg";
    case MIME_CONTENT_TYPE_SVG:
        return "image/svg+xml";
    case MIME_CONTENT_TYPE_TIF:
        return "image/tiff";
    case MIME_CONTENT_TYPE_TIFF:
        return "image/tiff";
    case MIME_CONTENT_TYPE_RAS:
        return "image/x-cmu-raster";
    case MIME_CONTENT_TYPE_CMX:
        return "image/x-cmx";
    case MIME_CONTENT_TYPE_ICO:
        return "image/x-icon";
    case MIME_CONTENT_TYPE_PNM:
        return "image/x-portable-anymap";
    case MIME_CONTENT_TYPE_PBM:
        return "image/x-portable-bitmap";
    case MIME_CONTENT_TYPE_PGM:
        return "image/x-portable-graymap";
    case MIME_CONTENT_TYPE_PPM:
        return "image/x-portable-pixmap";
    case MIME_CONTENT_TYPE_RGB:
        return "image/x-rgb";
    case MIME_CONTENT_TYPE_XBM:
        return "image/x-xbitmap";
    case MIME_CONTENT_TYPE_XPM:
        return "image/x-xpixmap";
    case MIME_CONTENT_TYPE_XWD:
        return "image/x-xwindowdump";
    case MIME_CONTENT_TYPE_MHT:
        return "message/rfc822";
    case MIME_CONTENT_TYPE_MHTML:
        return "message/rfc822";
    case MIME_CONTENT_TYPE_NWS:
        return "message/rfc822";
    case MIME_CONTENT_TYPE_CSS:
        return "text/css";
    case MIME_CONTENT_TYPE_323:
        return "text/h323";
    case MIME_CONTENT_TYPE_HTM:
        return "text/html";
    case MIME_CONTENT_TYPE_HTML:
        return "text/html";
    case MIME_CONTENT_TYPE_STM:
        return "text/html";
    case MIME_CONTENT_TYPE_ULS:
        return "text/iuls";
    case MIME_CONTENT_TYPE_BAS:
        return "text/plain";
    case MIME_CONTENT_TYPE_C:
        return "text/plain";
    case MIME_CONTENT_TYPE_H:
        return "text/plain";
    case MIME_CONTENT_TYPE_TXT:
        return "text/plain";
    case MIME_CONTENT_TYPE_RTX:
        return "text/richtext";
    case MIME_CONTENT_TYPE_SCT:
        return "text/scriptlet";
    case MIME_CONTENT_TYPE_TSV:
        return "text/tab-separated-values";
    case MIME_CONTENT_TYPE_HTT:
        return "text/webviewhtml";
    case MIME_CONTENT_TYPE_HTC:
        return "text/x-component";
    case MIME_CONTENT_TYPE_ETX:
        return "text/x-setext";
    case MIME_CONTENT_TYPE_VCF:
        return "text/x-vcard";
    case MIME_CONTENT_TYPE_MP2:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MPA:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MPE:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MPEG:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MPG:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MPV2:
        return "video/mpeg";
    case MIME_CONTENT_TYPE_MOV:
        return "video/quicktime";
    case MIME_CONTENT_TYPE_QT:
        return "video/quicktime";
    case MIME_CONTENT_TYPE_LSF:
        return "video/x-la-asf";
    case MIME_CONTENT_TYPE_LSX:
        return "video/x-la-asf";
    case MIME_CONTENT_TYPE_ASF:
        return "video/x-ms-asf";
    case MIME_CONTENT_TYPE_ASR:
        return "video/x-ms-asf";
    case MIME_CONTENT_TYPE_ASX:
        return "video/x-ms-asf";
    case MIME_CONTENT_TYPE_AVI:
        return "video/x-msvideo";
    case MIME_CONTENT_TYPE_MOVIE:
        return "video/x-sgi-movie";
    case MIME_CONTENT_TYPE_FLR:
        return "x-world/x-vrml";
    case MIME_CONTENT_TYPE_VRML:
        return "x-world/x-vrml";
    case MIME_CONTENT_TYPE_WRL:
        return "x-world/x-vrml";
    case MIME_CONTENT_TYPE_WRZ:
        return "x-world/x-vrml";
    case MIME_CONTENT_TYPE_XAF:
        return "x-world/x-vrml";
    case MIME_CONTENT_TYPE_XOF:
        return "x-world/x-vrml";
    default:
        return nullptr;
    }
    return nullptr;
}


std::string mailSender::composeEmail()
{
    //判断是否有内嵌资源的标记
    bool hasInlineSource=false;
    if(!mailInlineResource.empty())
        hasInlineSource=true;

    //整个邮件内容的存储变量
    std::string emailTotalContent="";

    //Date
    {
        time_t t;
        struct tm *p;
        t=time(NULL);
        //localtime是获取本地时间的函数
        p=localtime(&t);//
        char nowDate[128]= {0};
        //Sun, 17 Mar 2019 22:16:29 +0800
        strftime(nowDate, 80, "%a,%d %b %Y %H::%M::%S +0800",p);
        emailTotalContent+=nowDate;
        emailTotalContent+="\n";
    }

    //From
    emailTotalContent+="From: =?UTF-8?B?";
    emailTotalContent+=mailFrom.recipientsName;
    emailTotalContent+="?=<";
    emailTotalContent+=mailFrom.recipientsEmail;
    emailTotalContent+=">\n";


    //To
    emailTotalContent+="To: ";
    for(auto itor=mailTo.begin(); itor!=mailTo.end(); ++itor)
    {
        emailTotalContent+="=?UTF-8?B?";
        emailTotalContent+=(*itor).recipientsName;
        emailTotalContent+="?= <";
        emailTotalContent+=(*itor).recipientsEmail;
        emailTotalContent+=">\n";
    }

    //Cc
    emailTotalContent+="Cc: ";
    for(auto itor=mailCc.begin(); itor!=mailCc.end(); ++itor)
    {
        emailTotalContent+="=?UTF-8?B?";
        emailTotalContent+=(*itor).recipientsName;
        emailTotalContent+="?= <";
        emailTotalContent+=(*itor).recipientsEmail;
        emailTotalContent+=">\n";
    }

    //Bcc
    emailTotalContent+="Bcc: ";
    for(auto itor=mailBcc.begin(); itor!=mailBcc.end(); ++itor)
    {
        emailTotalContent+="=?UTF-8?B?";
        emailTotalContent+=(*itor).recipientsName;
        emailTotalContent+="?= <";
        emailTotalContent+=(*itor).recipientsEmail;
        emailTotalContent+=">\n";
    }

    //Subject
    emailTotalContent+="Subject: ";
    emailTotalContent+=mailSubject;
    emailTotalContent+="\n";

    //Mime-Version: 1.0
    emailTotalContent+="Mime-Version: 1.0\n";

    /*<邮件的主Content-Type>*/
    //如果有内嵌资源，则Content-Type:multipart/related
    //包含内嵌资源
    if(hasInlineSource)
        emailTotalContent+="Content-Type: multipart/related;\n";
    //既不是multipart/mixed也不是multipart/related
    //则无论有text/plain和text/html 其中一个或两个都有，其Content-Type: multipart/alternative;
    else
        emailTotalContent+="Content-Type: multipart/alternative;\n";
    /*</邮件的主Content-Type>*/

    /*<邮件的主boundary>*/
    //判断是否有内嵌资源，如果有则Content-Type:multipart/related
    //包含内嵌资源
    if(hasInlineSource)
    {
        emailTotalContent+="\tboundary=";
        emailTotalContent+=relatedBoundary;
        emailTotalContent+="\n";
    }
    //如果没有内嵌资源
    //则无论有text/plain和text/html 其中一个或两个都有，其boundary都是multipart/alternative
    else
    {
        emailTotalContent+="\tboundary=";
        emailTotalContent+=alternativeBoundary;
        emailTotalContent+="\n";
    }
    /*</邮件的主boundary>*/

    /*<Message-ID>*/

    char* messageId=new char[16]();
    for(int nbr=0; nbr<15; ++nbr)
    {
        messageId[nbr]=rand()%('z'-'a'+1)+'a';
    }
    emailTotalContent+="Message-ID: ";
    emailTotalContent+=messageId;
    emailTotalContent+="\n";
    delete[] messageId;
    /*</Message-ID>*/

    /*按照从低到高的级别生成内容，则内容生成顺序如下：
    1，邮件正文内容，multipart/alternative部分
    2，邮件正文+内嵌资源 内容，multipart/related部分
    */
    /*<multipart/related>*/
    std::string relatedContent="";
    {
        /*<multipart/alternative>*/
        std::string alternativeContent="";
        {
            /*<纯文本内容>*/
            std::string textPlainContent="";
            if(hasTextPlainContent)
            {
                //内容前的空行
                textPlainContent+="\n";
                //开始的boundary
                textPlainContent="--";
                textPlainContent+=mailPlainTextContent.boundary;
                textPlainContent+="\n";
                //Content-Type和charset
                textPlainContent+="Content-Type: text/plain; charset=utf-8\n";
                //Content-Transfer-Encoding
                textPlainContent+="Content-Transfer-Encoding: base64\n\n";
                //正文内容
                textPlainContent+=mailPlainTextContent.content;
            }
            /*</纯文本内容>*/

            /*<html文本内容>*/
            std::string textHtmlContent="";
            if(hasTextHtmlContent)
            {
                //内容前的空行
                textHtmlContent+="\n";
                //开始的boundary
                textHtmlContent="--";
                textHtmlContent+=mailHtmlTextContent.boundary;
                textHtmlContent+="\n";
                //Content-Type和charset
                textHtmlContent+="Content-Type: text/html; charset=utf-8\n";
                //Content-Transfer-Encoding
                textHtmlContent+="Content-Transfer-Encoding: base64\n\n";
                //正文内容
                textHtmlContent+=mailHtmlTextContent.content;
            }
            /*</html文本内容>*/

            /*将纯文本内容和html文本内容打包成multipart/alternative内容*/
            //multipart/alternative的头部
            //开始前的空行
            alternativeContent+="\n";
            if(hasInlineSource)
            {
                //开始boundary
                alternativeContent+="--";
                alternativeContent+=relatedBoundary;
                alternativeContent+="\n";
                alternativeContent+="Content-Type: multipart/alternative;\n";
                alternativeContent+="\t boundary=\"";
                alternativeContent+=alternativeBoundary;
                alternativeContent+="\"\n\n";
            }

            //multipart/alternative的身体
            alternativeContent+=textPlainContent;
            alternativeContent+=textHtmlContent;

            //multipart/alternative的结束符
            alternativeContent+="--";
            alternativeContent+=alternativeBoundary;
            alternativeContent+="--";
        }
        /*</multipart/alternative>*/
        /*<内嵌资源>*/
        std::string inlineSourceContent="";
        if(hasInlineSource)
        {
            for(auto itor=mailInlineResource.begin(); itor!=mailInlineResource.end(); ++itor)
            {
                //boundary
                inlineSourceContent+="--";
                inlineSourceContent+=relatedBoundary;
                inlineSourceContent+="\n";
                //Content-Type和name
                inlineSourceContent+="Content-Type: ";
                inlineSourceContent+=getConentTypeString((*itor).contentType);
                inlineSourceContent+="; name=\"";
                inlineSourceContent+=(*itor).fileName;
                inlineSourceContent+="\"\n";
                //Content-Transfer-Encoding
                inlineSourceContent+="Content-Transfer-Encoding: base64\n";
                //Content-Disposition和filename
                inlineSourceContent+="Content-Disposition: inline; filename=\"";
                inlineSourceContent+=(*itor).fileName;
                inlineSourceContent+="\"\n";
                //Content-ID
                inlineSourceContent+="Content-ID: <";
                inlineSourceContent+=(*itor).contentId;
                inlineSourceContent+=">\n";
                //空行
                inlineSourceContent+="\n";
                //内嵌资源数据
                inlineSourceContent+=(*itor).content;
            }
            /*<内嵌资源>*/

            /*将multipart/alternative部分的数据和内嵌资源数据打包成multipart/related内容*/
            //multipart/alternative的头部
            //如果有内嵌资源则开始boundary就是multipart/related的boundary
            //multipart/alternative的身体
            relatedContent+=alternativeContent;
            //内嵌资源
            relatedContent+="\n\n";
            relatedContent+=inlineSourceContent;
            //结束boundary
            relatedContent+="--";
            relatedContent+=relatedBoundary;
            relatedContent+="--";
        }
        else//如果没有
            relatedContent+=alternativeContent;


    }
    /*</multipart/related>*/
    emailTotalContent+=relatedContent;
    return emailTotalContent;
}
uint32_t mailSender::payload_source(void *ptr, uint32_t size, uint32_t nmemb, void * userp)
{
    struct_callback* data=(struct_callback*)userp;
    //每次拷贝进去300个字节
    const size_t lengthPerTime=300;
    const size_t totalLength=data->content->length();
    //如果剩下的字符串大于300字节则发送300个字节

    if(totalLength-data->sendLength>=lengthPerTime)
    {
        const char* readLocationPointer=data->content->c_str()+data->sendLength;
        memcpy(ptr,readLocationPointer,lengthPerTime);
        data->sendLength=data->sendLength+lengthPerTime;
        return lengthPerTime;
    }
    else if(totalLength-data->sendLength>0 && totalLength-data->sendLength<lengthPerTime)
    {
        const char* readLocationPointer=data->content->c_str()+data->sendLength;
        memcpy(ptr,readLocationPointer,totalLength-data->sendLength);
        size_t sendLength=totalLength-data->sendLength;
        data->sendLength=data->sendLength+totalLength-data->sendLength;
        return sendLength;
    }
    //没有数据可以发送了
    else
        return 0;
}

void mailSender::sendMail()
{
    CURL *curl;
    CURLcode res = CURLE_OK;
    struct curl_slist *recipients = NULL;

    std::string emailContent=composeEmail();


    curl = curl_easy_init();
    if(curl)
    {
        /* Set username and password */
        curl_easy_setopt(curl, CURLOPT_USERNAME, senderAccount.c_str());
        //QQ邮箱的授权码为登录密码：mbuttlectpgrbddj
        //126邮箱的授权码为登录密码：UTGE89UYT3JKhER
        curl_easy_setopt(curl, CURLOPT_PASSWORD, senderPassword.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); // allows emails to be sent

        /* This is the URL for your mailserver. Note the use of smtps:// rather
         * than smtp:// to request a SSL based connection. */
        curl_easy_setopt(curl, CURLOPT_URL, senderServer.c_str());
        //curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.126.com:465");

        /* If you want to connect to a site who isn't using a certificate that is
         * signed by one of the certs in the CA bundle you have, you can skip the
         * verification of the server's certificate. This makes the connection
         * A LOT LESS SECURE.
         *
         * If you have a CA cert for the server stored someplace else than in the
         * default bundle, then the CURLOPT_CAPATH option might come handy for
         * you. */
#ifdef SKIP_PEER_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

        /* If the site you're connecting to uses a different host name that what
         * they have mentioned in their server certificate's commonName (or
         * subjectAltName) fields, libcurl will refuse to connect. You can skip
         * this check, but this will make the connection less secure. */
#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        /* Note that this option isn't strictly required, omitting it will result
         * in libcurl sending the MAIL FROM command with empty sender data. All
         * autoresponses should have an empty reverse-path, and should be directed
         * to the address in the reverse-path which triggered them. Otherwise,
         * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
         * details.
         */
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, senderAccount.c_str());

        /* Add two recipients, in this particular case they correspond to the
         * To: and Cc: addressees in the header, but they could be any kind of
         * recipient. */
        //将所有主送人人，抄送人，密送人都添加到这个指针中
        //主送人
        for(auto itor=mailTo.begin(); itor!=mailTo.end(); ++itor)
            recipients = curl_slist_append(recipients,(*itor).recipientsEmail.c_str());
        //抄送人
        for(auto itor=mailCc.begin(); itor!=mailCc.end(); ++itor)
            recipients = curl_slist_append(recipients,(*itor).recipientsEmail.c_str());
        //密送人
        for(auto itor=mailBcc.begin(); itor!=mailBcc.end(); ++itor)
            recipients = curl_slist_append(recipients,(*itor).recipientsEmail.c_str());

        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        /* We're using a callback function to specify the payload (the headers and
         * body of the message). You could just use the CURLOPT_READDATA option to
         * specify a FILE pointer to read from. */
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);

        struct_callback callBackData;
        callBackData.content=&emailContent;
        callBackData.sendLength=0;
        curl_easy_setopt(curl, CURLOPT_READDATA, &callBackData);

        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Since the traffic will be encrypted, it is very useful to turn on debug
         * information within libcurl to see what is happening during the
         * transfer */
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);
    }

    //return (int)res;

}
