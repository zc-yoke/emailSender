#curl配置
curl_dir=../curl
curl_inc_dir=curl_dir
curl_lib_dir=curl_dir/lib
curl_lflag=-lcurl -lcurldll -lwldap32 -lws2_32

#base64配置
base64_dir=../base64
base64_inc_dir=$(base64_dir)
base64_lib_dir=$(base64_dir)
base64_lflag=-lbase64


all: libemailSender email_debug

email_debug:libemailSender.a mailSender.h textPlainEmail.cpp
	g++ -std=c++11 -static -o email_debug textPlainEmail.cpp -I./include  -L. -lemailSender \
	-I $(base64_inc_dir) -L $(base64_lib_dir) $(base64_lflag) \
	-I $(curl_inc_dir) -L $(curl_lib_dir) $(curl_lflag)
	
libemailSender:mailSender.o
	ar -rc libemailSender.a mailSender.o
	
mailSender.o:mailSender.h mailSender.cpp
	g++ -c mailSender.h mailSender.cpp  \
	-I $(base64_inc_dir) -L $(base64_lib_dir) $(base64_lflag) \
	-I $(curl_inc_dir) -L $(curl_lib_dir) $(curl_lflag)
	


	
.PHONY:clean
clean:
	rm -f *.exe *test_out* *.a *.o *.gch
	rm -r .objs bin obj
	rm -rf .objs bin obj