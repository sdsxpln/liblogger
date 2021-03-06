/*
Copyright (c) 2016, Yomei Otani <yomei.otani@gmai.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

//#define I_USE_MYSQL

#ifndef I_USE_MYSQL

// do not include mysql sdk's file

#else

#define MY_SQL_WRAOPPER_C
#include "mysql_wrapper.h"
#include "dll_client.h"

MYSQL *	(STDCALL *pf_mysql_init)(MYSQL *mysql)=NULL;
void (STDCALL *pf_mysql_close)(MYSQL *sock)=NULL;
MYSQL * (STDCALL *pf_mysql_real_connect)(MYSQL *mysql, const char *host,
	const char *user,const char *passwd,const char *db,unsigned int port,
	const char *unix_socket,unsigned long clientflag) = NULL;
int (STDCALL *pf_mysql_query)(MYSQL *mysql, const char *q)=NULL;
MYSQL_RES * (STDCALL *pf_mysql_use_result)(MYSQL *mysql)=NULL;
void (STDCALL *pf_mysql_free_result)(MYSQL_RES *result)=NULL;
MYSQL_ROW(STDCALL *pf_mysql_fetch_row)(MYSQL_RES *result) = NULL;


static void *h_mysql = NULL;

class mysql_wrapper {
public :
	mysql_wrapper();
	virtual ~mysql_wrapper();
};

mysql_wrapper::mysql_wrapper()
{
	h_mysql = dll_load(DLL_NAME("libmysql"));
	if (h_mysql == NULL)return;
	pf_mysql_init = (MYSQL * (STDCALL *)(MYSQL *))dll_access(h_mysql, "mysql_init");
	pf_mysql_close = (void (STDCALL *)(MYSQL *))dll_access(h_mysql, "mysql_close");
	pf_mysql_real_connect = (MYSQL * (STDCALL *)(MYSQL *, const char *,
		const char *, const char *, const char *, unsigned int ,
		const char *, unsigned long))dll_access(h_mysql, "mysql_real_connect");
	pf_mysql_query = (int (STDCALL *)(MYSQL *mysql, const char *))dll_access(h_mysql, "mysql_query");
	pf_mysql_use_result = (MYSQL_RES * (STDCALL *)(MYSQL *))dll_access(h_mysql, "mysql_use_result");
	pf_mysql_free_result = (void (STDCALL *)(MYSQL_RES *))dll_access(h_mysql, "mysql_free_result");
	pf_mysql_fetch_row = (MYSQL_ROW(STDCALL*)(MYSQL_RES *))dll_access(h_mysql, "mysql_fetch_row");
}

mysql_wrapper::~mysql_wrapper()
{
	if (h_mysql)dll_close(h_mysql);
	h_mysql = NULL;
}


static mysql_wrapper s_mysql_wrapper;


#endif



