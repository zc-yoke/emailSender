TOP_PATH = $(shell pwd)
all: libemailSender email_debug

email_debug:libemailSender.a mailSender.h textPlainEmail.cpp
	g++ -std=c++11 -static -o email_debug textPlainEmail.cpp -I./include  -L. -lemailSender \
	-I ../../000_soft_develop_lib/libcurl_7_62/include \
	-L ../../000_soft_develop_lib/libcurl_7_62 \
	-lcurl -lcurldll -lwldap32 -lws2_32 \
	-L ../base64 \
	-I ../base64/include -lbase64
	
libemailSender:mailSender.o
	ar -rc libemailSender.a mailSender.o
	
mailSender.o:mailSender.h mailSender.cpp
	g++ -c mailSender.h mailSender.cpp  \
	-I ./include \
	-I ../../000_soft_develop_lib/libcurl_7_62/include \
	-L ../../000_soft_develop_lib/libcurl_7_62 \
	-lcurl -lcurldll -lwldap32 -lws2_32 \
	-L ../base64 \
	-I ../base64/include -lbase64
	


	
.PHONY:clean cleanall
clean:
	rm -f *.exe *test_out* *.a *.o
	rm -r .objs bin obj
	rm -rf .objs bin obj