#ifndef MAILSENDER_H
#define MAILSENDER_H
#include <vector>
#include <string>
#include <curl.h>
#include <iostream>
typedef unsigned int uint32_t;
/*
邮件发送类，
1，默认所有数据使用base64编码，目前只实现了base64编码邮件类型
2，所有文字使用utf8字符集编码
*/
enum mimeContentType
{
    MIME_CONTENT_TYPE_NOTHING,
    MIME_CONTENT_TYPE_EVY,
    MIME_CONTENT_TYPE_FIF,
    MIME_CONTENT_TYPE_SPL,
    MIME_CONTENT_TYPE_HTA,
    MIME_CONTENT_TYPE_ACX,
    MIME_CONTENT_TYPE_HQX,
    MIME_CONTENT_TYPE_DOC,
    MIME_CONTENT_TYPE_DOT,
    MIME_CONTENT_TYPE_BIN,
    MIME_CONTENT_TYPE_CLASS,
    MIME_CONTENT_TYPE_DMS,
    MIME_CONTENT_TYPE_EXE,
    MIME_CONTENT_TYPE_LHA,
    MIME_CONTENT_TYPE_LZH,
    MIME_CONTENT_TYPE_ODA,
    MIME_CONTENT_TYPE_AXS,
    MIME_CONTENT_TYPE_PDF,
    MIME_CONTENT_TYPE_PRF,
    MIME_CONTENT_TYPE_P10,
    MIME_CONTENT_TYPE_CRL,
    MIME_CONTENT_TYPE_AI,
    MIME_CONTENT_TYPE_EPS,
    MIME_CONTENT_TYPE_PS,
    MIME_CONTENT_TYPE_RTF,
    MIME_CONTENT_TYPE_SETPAY,
    MIME_CONTENT_TYPE_SETREG,
    MIME_CONTENT_TYPE_XLA,
    MIME_CONTENT_TYPE_XLC,
    MIME_CONTENT_TYPE_XLM,
    MIME_CONTENT_TYPE_XLS,
    MIME_CONTENT_TYPE_XLT,
    MIME_CONTENT_TYPE_XLW,
    MIME_CONTENT_TYPE_MSG,
    MIME_CONTENT_TYPE_SST,
    MIME_CONTENT_TYPE_CAT,
    MIME_CONTENT_TYPE_STL,
    MIME_CONTENT_TYPE_POT,
    MIME_CONTENT_TYPE_PPS,
    MIME_CONTENT_TYPE_PPT,
    MIME_CONTENT_TYPE_MPP,
    MIME_CONTENT_TYPE_WCM,
    MIME_CONTENT_TYPE_WDB,
    MIME_CONTENT_TYPE_WKS,
    MIME_CONTENT_TYPE_WPS,
    MIME_CONTENT_TYPE_HLP,
    MIME_CONTENT_TYPE_BCPIO,
    MIME_CONTENT_TYPE_XCDF,
    MIME_CONTENT_TYPE_Z,
    MIME_CONTENT_TYPE_TGZ,
    MIME_CONTENT_TYPE_CPIO,
    MIME_CONTENT_TYPE_CSH,
    MIME_CONTENT_TYPE_DCR,
    MIME_CONTENT_TYPE_DIR,
    MIME_CONTENT_TYPE_DXR,
    MIME_CONTENT_TYPE_DVI,
    MIME_CONTENT_TYPE_GTAR,
    MIME_CONTENT_TYPE_GZ,
    MIME_CONTENT_TYPE_HDF,
    MIME_CONTENT_TYPE_INS,
    MIME_CONTENT_TYPE_ISP,
    MIME_CONTENT_TYPE_III,
    MIME_CONTENT_TYPE_JS,
    MIME_CONTENT_TYPE_LATEX,
    MIME_CONTENT_TYPE_MDB,
    MIME_CONTENT_TYPE_CRD,
    MIME_CONTENT_TYPE_CLP,
    MIME_CONTENT_TYPE_DLL,
    MIME_CONTENT_TYPE_M13,
    MIME_CONTENT_TYPE_M14,
    MIME_CONTENT_TYPE_MVB,
    MIME_CONTENT_TYPE_WMF,
    MIME_CONTENT_TYPE_MNY,
    MIME_CONTENT_TYPE_PUB,
    MIME_CONTENT_TYPE_SCD,
    MIME_CONTENT_TYPE_TRM,
    MIME_CONTENT_TYPE_WRI,
    MIME_CONTENT_TYPE_CDF,
    MIME_CONTENT_TYPE_NC,
    MIME_CONTENT_TYPE_PMA,
    MIME_CONTENT_TYPE_PMC,
    MIME_CONTENT_TYPE_PML,
    MIME_CONTENT_TYPE_PMR,
    MIME_CONTENT_TYPE_PMW,
    MIME_CONTENT_TYPE_PNG,
    MIME_CONTENT_TYPE_P12,
    MIME_CONTENT_TYPE_PFX,
    MIME_CONTENT_TYPE_P7B,
    MIME_CONTENT_TYPE_SPC,
    MIME_CONTENT_TYPE_P7R,
    MIME_CONTENT_TYPE_P7C,
    MIME_CONTENT_TYPE_P7M,
    MIME_CONTENT_TYPE_P7S,
    MIME_CONTENT_TYPE_SH,
    MIME_CONTENT_TYPE_SHAR,
    MIME_CONTENT_TYPE_SWF,
    MIME_CONTENT_TYPE_SIT,
    MIME_CONTENT_TYPE_SV4CPIO,
    MIME_CONTENT_TYPE_SV4CRC,
    MIME_CONTENT_TYPE_TAR,
    MIME_CONTENT_TYPE_TCL,
    MIME_CONTENT_TYPE_TEX,
    MIME_CONTENT_TYPE_TEXI,
    MIME_CONTENT_TYPE_TEXINFO,
    MIME_CONTENT_TYPE_ROFF,
    MIME_CONTENT_TYPE_T,
    MIME_CONTENT_TYPE_TR,
    MIME_CONTENT_TYPE_MAN,
    MIME_CONTENT_TYPE_ME,
    MIME_CONTENT_TYPE_MS,
    MIME_CONTENT_TYPE_USTAR,
    MIME_CONTENT_TYPE_SRC,
    MIME_CONTENT_TYPE_CER,
    MIME_CONTENT_TYPE_CRT,
    MIME_CONTENT_TYPE_DER,
    MIME_CONTENT_TYPE_PKO,
    MIME_CONTENT_TYPE_ZIP,
    MIME_CONTENT_TYPE_AU,
    MIME_CONTENT_TYPE_SND,
    MIME_CONTENT_TYPE_MID,
    MIME_CONTENT_TYPE_RMI,
    MIME_CONTENT_TYPE_MP3,
    MIME_CONTENT_TYPE_AIF,
    MIME_CONTENT_TYPE_AIFC,
    MIME_CONTENT_TYPE_AIFF,
    MIME_CONTENT_TYPE_M3U,
    MIME_CONTENT_TYPE_RA,
    MIME_CONTENT_TYPE_RAM,
    MIME_CONTENT_TYPE_WAV,
    MIME_CONTENT_TYPE_BMP,
    MIME_CONTENT_TYPE_COD,
    MIME_CONTENT_TYPE_GIF,
    MIME_CONTENT_TYPE_IEF,
    MIME_CONTENT_TYPE_JPE,
    MIME_CONTENT_TYPE_JPEG,
    MIME_CONTENT_TYPE_JPG,
    MIME_CONTENT_TYPE_JFIF,
    MIME_CONTENT_TYPE_SVG,
    MIME_CONTENT_TYPE_TIF,
    MIME_CONTENT_TYPE_TIFF,
    MIME_CONTENT_TYPE_RAS,
    MIME_CONTENT_TYPE_CMX,
    MIME_CONTENT_TYPE_ICO,
    MIME_CONTENT_TYPE_PNM,
    MIME_CONTENT_TYPE_PBM,
    MIME_CONTENT_TYPE_PGM,
    MIME_CONTENT_TYPE_PPM,
    MIME_CONTENT_TYPE_RGB,
    MIME_CONTENT_TYPE_XBM,
    MIME_CONTENT_TYPE_XPM,
    MIME_CONTENT_TYPE_XWD,
    MIME_CONTENT_TYPE_MHT,
    MIME_CONTENT_TYPE_MHTML,
    MIME_CONTENT_TYPE_NWS,
    MIME_CONTENT_TYPE_CSS,
    MIME_CONTENT_TYPE_323,
    MIME_CONTENT_TYPE_HTM,
    MIME_CONTENT_TYPE_HTML,
    MIME_CONTENT_TYPE_STM,
    MIME_CONTENT_TYPE_ULS,
    MIME_CONTENT_TYPE_BAS,
    MIME_CONTENT_TYPE_C,
    MIME_CONTENT_TYPE_H,
    MIME_CONTENT_TYPE_TXT,
    MIME_CONTENT_TYPE_RTX,
    MIME_CONTENT_TYPE_SCT,
    MIME_CONTENT_TYPE_TSV,
    MIME_CONTENT_TYPE_HTT,
    MIME_CONTENT_TYPE_HTC,
    MIME_CONTENT_TYPE_ETX,
    MIME_CONTENT_TYPE_VCF,
    MIME_CONTENT_TYPE_MP2,
    MIME_CONTENT_TYPE_MPA,
    MIME_CONTENT_TYPE_MPE,
    MIME_CONTENT_TYPE_MPEG,
    MIME_CONTENT_TYPE_MPG,
    MIME_CONTENT_TYPE_MPV2,
    MIME_CONTENT_TYPE_MOV,
    MIME_CONTENT_TYPE_QT,
    MIME_CONTENT_TYPE_LSF,
    MIME_CONTENT_TYPE_LSX,
    MIME_CONTENT_TYPE_ASF,
    MIME_CONTENT_TYPE_ASR,
    MIME_CONTENT_TYPE_ASX,
    MIME_CONTENT_TYPE_AVI,
    MIME_CONTENT_TYPE_MOVIE,
    MIME_CONTENT_TYPE_FLR,
    MIME_CONTENT_TYPE_VRML,
    MIME_CONTENT_TYPE_WRL,
    MIME_CONTENT_TYPE_WRZ,
    MIME_CONTENT_TYPE_XAF,
    MIME_CONTENT_TYPE_XOF
};


enum  mimeMailType
{
    MULTIPART_MIXED,
    MULTIPART_RELATED,
    MULTIPART_ALTERNATIVE
};


/*邮件的正文数据结构体，其只有两种情况，text/plain和text/html
,使用base64编码过后的数据
*/
struct struct_mailContent
{
    std::string boundary;
    // text/plain或text/html
    mimeContentType contentType;
    std::string content;
};



/*收件人数据结构体,使用base64编码过后的数据*/
struct struct_recipients
{
    std::string recipientsName;
    std::string recipientsEmail;
};

/*邮件内嵌资源的ID结构体
之所以需要使用结构体是因为当给内嵌资源对象设置内嵌资源ID的时候，
程序并不知道那个ID没有被占用，所以需要设置索引和是否被占用的标志，
如果被占用则使用其他的ID
*/
struct struct_mailInlineResourceId
{
    uint32_t index;
    bool isOccupied;
    std::string id;
};
/*邮件内嵌资源数据结构体,使用base64编码过后的数据*/
struct struct_mailInlineResource
{
    std::string fileName;
    mimeContentType contentType;
    std::string content;
    std::string contentId;
};
/*curl读取要发送的邮件内容回调函数使用的结构体*/
struct struct_callback
{
    //要发送的数据字符串
    std::string * content;
    //记录已经被回调函数发送的数据长度，
    //如果长度与字符串长度一致，则需要结束发送
    size_t sendLength;
};


class mailSender
{
public:

    /** \brief 构造函数
     *
     * \param senderAccount_ std::string 用来发送邮件的邮箱帐户名
     * \param senderPassword_ std::string 用来发送邮件的邮箱账户密码
     * \param senderServer_ std::string 用来发送邮件的邮箱smtp服务器地址
     *
     */
    mailSender(std::string senderAccount_,std::string senderPassword_,std::string senderServer_);

    /** \brief 添加收件人
     *
     * \param to_name string 收件人姓名或称呼
     * \param to_email string 收件人的邮件地址，形如 kooba5@126.com，没有<>符号
     * \return void
     *
     */
    void addTo(std::string to_name,std::string to_email);

    /** \brief 添加抄送人
     *
     * \param cc_name string 抄送人的姓名或称呼
     * \param cc_email string 抄送人的邮件地址，形如 kooba5@126.com，没有<>符号
     * \return void
     *
     */
    void addCc(std::string cc_name,std::string cc_email);

    /** \brief 添加密送人
     *
     * \param bcc_name string 密送人的姓名或称呼
     * \param bcc_email string 密送人的邮件地址，形如 kooba5@126.com，没有<>符号
     * \return void
     *
     */
    void addBcc(std::string bcc_name,std::string bcc_email);

    /** \brief 设置邮件发送人及其地址
     *
     * \param from_name string 发送人的姓名或称呼
     * \param from_email string 发送人的邮件地址，形如 kooba5@126.com，没有<>符号
     * \return void
     *
     */
    void setFrom(std::string from_name,std::string from_email);

    /** \brief 设置邮件主题
     *
     * \param subject string 邮件主题
     * \return void
     *
     */
    void setSubject(std::string subject);

    /** \brief 从内存中添加内嵌资源
     *
     * \param sourceId const char* 资源ID
     * \param fileName const char* 资源文件名，非全路径文件名
     * \param fileData char* 内嵌资源的数据指针
     * \param dataLength uint32_t 内嵌资源的数据长度
     * \param contentType mimeContentType 目标类型
     * \return void
     *
     */
    void addInlineResource(const char* sourceId,const char* fileName,unsigned char* fileData,uint32_t dataLength,mimeContentType contentType);

    /** \brief 从文件添加内嵌资源
     *
     * \param sourceId const char* 资源ID
     * \param fileName const char*
     * \param contentType mimeContentType 目标类型
     * \return void
     *
     */
    void addInlineResource(const char* sourceId,const char* fileName,mimeContentType contentType);


    /** \brief 从内存中添加纯文本邮件内容
     *
     * \param data unsigned char* 邮件内容数据指针
     * \param dataLength uint32_t 邮件内容数据长度
     * \param contentType mimeContentType 邮件内容类型
     * \return void
     *
     */
    void addEmailContent(unsigned char* data,uint32_t dataLength,mimeContentType contentType);

    /** \brief 从文件添加存纯文本邮件内容
     *
     * \param fileName const char* 文件名，全路径文件名
     * \param contentType mimeContentType 邮件内容类型
     * \return void
     *
     */
    void addEmailContent(const char* fileName,mimeContentType contentType);

    /** \brief 返回附件的Content-Type，注意这里只返回类似application/x-zip-compressed
    *而不是类似Content-Type: application/x-zip-compressed;
    * \param contentType mimeContentType
    * \return string
    *
    */
    std::string getConentTypeString(mimeContentType contentType);


    /** \brief 将邮件整理成一个完整的文档数据
     *
     * \return std::string 邮件正文内容
     *
     */
    std::string composeEmail();

    /** \brief 回调函数，将数据拷贝到指定的指针中，以便curl将邮件正文发送出去
     *
     * \param ptr void* curl发送的邮件内容的数据指针
     * \param size uint32_t 邮件内容的数据长度
     * \param nmemb uint32_t
     * \param userp void* 需要传递给curl的邮件数据
     * \return uint32_t 需要发送的邮件数据的长度
     *
     */
    static uint32_t payload_source(void *ptr, uint32_t size, uint32_t nmemb, void * userp);

    /** \brief 发送邮件
     *
     * \return void
     *
     */
    void sendMail();


    virtual ~mailSender();

private:
    //发送邮件的邮箱
    std::string senderAccount;
    //发送邮件的账户密码
    std::string senderPassword;
    //返送邮件的服务器地址，smtp服务器地址
    std::string senderServer;
    //标记是否有text/plain邮件正文
    bool hasTextPlainContent;
    //标记是否有text/html邮件正文
    bool hasTextHtmlContent;
    //收件人地址
    std::vector <struct_recipients> mailTo;
    //抄送人地址
    std::vector <struct_recipients> mailCc;
    //密送人地址
    std::vector <struct_recipients> mailBcc;
    //邮件发送地址，邮件从该地址发出
    struct_recipients mailFrom;
    //邮件主题,base64编码过后的
    std::string mailSubject;

    //内嵌资源,使用base64编码，并进行了这行
    std::vector <struct_mailInlineResource> mailInlineResource;

    //邮件纯文本正文,使用base64编码，并进行了这行
    struct_mailContent mailPlainTextContent;

    //邮件html文本正文,使用base64编码，并进行了这行
    struct_mailContent mailHtmlTextContent;

    //related层级的Boundary
    std::string relatedBoundary;
    //alternative的Boundary
    std::string alternativeBoundary;
};

#endif // MAILSENDER_H

