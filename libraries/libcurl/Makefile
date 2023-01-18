########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libcurl.a
MYCCOPTS    = -I"$(SRCDIR)" -I"$(SRCDIR)"/$(SMOOTH_PATH)/include/support -DBUILDING_LIBCURL -DCURL_STATICLIB

ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -DUSE_SCHANNEL -DUSE_WINDOWS_SSPI -DHAVE_MINGW_ORIGINAL
endif

ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -DUSE_SECTRANSP
endif

# Enter object files here:
OBJECTS	    = altsvc.o asyn-ares.o asyn-thread.o base64.o bufref.o c-hyper.o cfilters.o conncache.o connect.o content_encoding.o cookie.o curl_addrinfo.o curl_des.o curl_endian.o curl_fnmatch.o curl_get_line.o curl_gethostname.o curl_gssapi.o curl_memrchr.o curl_multibyte.o curl_ntlm_core.o curl_ntlm_wb.o curl_path.o curl_range.o curl_rtmp.o curl_sasl.o curl_sspi.o curl_threads.o dict.o doh.o dynbuf.o easy.o easygetopt.o easyoptions.o escape.o file.o fileinfo.o fopen.o formdata.o ftp.o ftplistparser.o getenv.o getinfo.o gopher.o hash.o headers.o hmac.o hostasyn.o hostip.o hostip4.o hostip6.o hostsyn.o hsts.o http.o http_aws_sigv4.o http_chunks.o http_digest.o http_negotiate.o http_ntlm.o http_proxy.o http2.o idn.o if2ip.o imap.o inet_ntop.o inet_pton.o krb5.o ldap.o llist.o md4.o md5.o memdebug.o mime.o mprintf.o mqtt.o multi.o netrc.o nonblock.o noproxy.o openldap.o parsedate.o pingpong.o pop3.o progress.o psl.o rand.o rename.o rtsp.o select.o sendf.o setopt.o sha256.o share.o slist.o smb.o smtp.o socks.o socks_gssapi.o socks_sspi.o speedcheck.o socketpair.o splay.o strcase.o strdup.o strerror.o strtok.o strtoofft.o telnet.o tftp.o timediff.o timeval.o transfer.o url.o urlapi.o version.o warnless.o wildcard.o ws.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += system_win32.o version_win32.o
endif

OBJECTS += vauth/cleartext.o vauth/cram.o vauth/digest.o vauth/digest_sspi.o vauth/gsasl.o vauth/krb5_gssapi.o vauth/krb5_sspi.o vauth/ntlm.o vauth/ntlm_sspi.o vauth/oauth2.o vauth/spnego_gssapi.o vauth/spnego_sspi.o vauth/vauth.o
OBJECTS += vquic/msh3.o vquic/ngtcp2.o vquic/quiche.o vquic/vquic.o
OBJECTS += vssh/libssh.o vssh/libssh2.o vssh/wolfssh.o
OBJECTS += vtls/bearssl.o vtls/gskit.o vtls/gtls.o vtls/hostcheck.o vtls/keylog.o vtls/mbedtls.o vtls/mbedtls_threadlock.o vtls/nss.o vtls/openssl.o vtls/rustls.o vtls/vtls.o vtls/wolfssl.o vtls/x509asn1.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += vtls/schannel.o vtls/schannel_verify.o
endif

ifeq ($(BUILD_OSX),True)
	OBJECTS += vtls/sectransp.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
