########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALLIB   = libcurl.a
MYCCOPTS    = -I$(SRCDIR) -I$(SRCDIR)/$(SMOOTH_PATH)/include/support -DBUILDING_LIBCURL -DCURL_STATICLIB

ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -DUSE_SCHANNEL -DUSE_WINDOWS_SSPI
endif

ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -DUSE_DARWINSSL
endif

# Enter object files here:
OBJECTS	    = asyn-ares.o asyn-thread.o base64.o conncache.o connect.o content_encoding.o cookie.o curl_addrinfo.o curl_des.o curl_endian.o curl_fnmatch.o curl_gethostname.o curl_gssapi.o curl_memrchr.o curl_multibyte.o curl_ntlm_core.o curl_ntlm_wb.o curl_rtmp.o curl_sasl.o curl_sspi.o curl_threads.o dict.o dotdot.o easy.o escape.o file.o fileinfo.o formdata.o ftp.o ftplistparser.o getenv.o getinfo.o gopher.o hash.o hmac.o hostasyn.o hostcheck.o hostip.o hostip4.o hostip6.o hostsyn.o http.o http_chunks.o http_digest.o http_negotiate.o http_ntlm.o http_proxy.o http2.o idn_win32.o if2ip.o imap.o inet_ntop.o inet_pton.o krb5.o ldap.o llist.o md4.o md5.o memdebug.o mprintf.o multi.o netrc.o non-ascii.o nonblock.o openldap.o parsedate.o pingpong.o pipeline.o pop3.o progress.o rand.o rtsp.o security.o select.o sendf.o share.o slist.o smb.o smtp.o socks.o socks_gssapi.o socks_sspi.o speedcheck.o splay.o ssh.o strcase.o strdup.o strerror.o strtok.o strtoofft.o telnet.o tftp.o timeval.o transfer.o url.o version.o warnless.o wildcard.o x509asn1.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += system_win32.o
endif

OBJECTS += vauth/cleartext.o vauth/cram.o vauth/digest.o vauth/digest_sspi.o vauth/krb5_gssapi.o vauth/krb5_sspi.o vauth/ntlm.o vauth/ntlm_sspi.o vauth/oauth2.o vauth/spnego_gssapi.o vauth/spnego_sspi.o vauth/vauth.o
OBJECTS += vtls/axtls.o vtls/cyassl.o vtls/gskit.o vtls/gtls.o vtls/mbedtls.o vtls/nss.o vtls/openssl.o vtls/polarssl.o vtls/polarssl_threadlock.o vtls/vtls.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += vtls/schannel.o
endif

ifeq ($(BUILD_OSX),True)
	OBJECTS += vtls/darwinssl.o
endif

## Do not change anything below this line. ##

include $(dir $(firstword $(MAKEFILE_LIST)))/$(SMOOTH_PATH)/Makefile-commands
