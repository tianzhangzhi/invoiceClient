/********************************************************************
	created:	2010/11/19
	created:	19:11:2010   18:52
	filename: 	d:\Work\CommuLib\NLcpfw\inc\NLcpfw.h
	file path:	d:\Work\CommuLib\NLcpfw\inc
	file base:	NLcpfw
	file ext:	h
	author:		zencg
	
	purpose:	
*********************************************************************/

#ifndef __HEAD_FILE_NLcpfw_H__

#define __HEAD_FILE_NLcpfw_H__

//////////////////////////////////////////////////////////////////////////

#include "NLcpfw_st.h"

//////////////////////////////////////////////////////////////////////////

#define CPFW_OM_NORMAL		0
#define CPFW_OM_KEEP		1


//////////////////////////////////////////////////////////////////////////

#ifdef DLL_CPFW_EXPORT
#undef DLL_CPFW_EXPORT
#define DLL_CPFW_EXPORT __declspec(dllexport)
#else
#define DLL_CPFW_EXPORT __declspec(dllimport)
#endif


#ifdef WIN32
#define NLCPFW_API	WINAPI
#else
#define NLCPFW_API
#endif


typedef void (NLCPFW_API *CPFW_NTF_DEV_STATE)(int nState, void *pParam);				//端口状态变化回调
typedef void (NLCPFW_API *CPFW_NTF_DATA)(void *pBuf, UINT dwDataLen, void *pParam);	//数据到达通知回调
typedef void (NLCPFW_API *CPFW_FN_ENUM_PORTS)(WCHAR *pstrPortType, WCHAR *pstrPortDesc, void *pParam);		//查询通讯端口名称与描述回调


#ifdef __cplusplus
extern "C"{
#endif

DLL_CPFW_EXPORT int NLCPFW_API cpfw_QueryVersion(HNLCPFW hPort, WCHAR *pwStrBuf, UINT dwBufLen);		//BufLen指出StrBuf可以保存的字符数，而不是字节数，成功返回零

DLL_CPFW_EXPORT int NLCPFW_API cpfw_QueryLibFileInfo(HNLCPFW hPort, WCHAR *pwStrBuf, UINT dwBufLen);	//同上

DLL_CPFW_EXPORT int NLCPFW_API cpfw_QueryPhyDevName(HNLCPFW hPort, WCHAR *pwStrBuf, UINT dwBufLen);	//同上

DLL_CPFW_EXPORT int NLCPFW_API cpfw_QueryDevStatus(HNLCPFW hPort);

DLL_CPFW_EXPORT HNLCPFW NLCPFW_API cpfw_EasyOpen(WCHAR *pwFileName);

DLL_CPFW_EXPORT HNLCPFW NLCPFW_API cpfw_open(WCHAR *pwStrPort, WCHAR *pwStrParam, int nMode = CPFW_OM_NORMAL);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_close(HNLCPFW hPort);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_set_port_properties(HNLCPFW hPort, WCHAR *pwStrProperties);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_read(HNLCPFW hPort, void *pBuf, UINT dwReadCnt, UINT timeout, BOOL bWaitAllData);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_write(HNLCPFW hPort, const void *pBuf, UINT dwWriteCnt, UINT timeout);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_get_rec_buf_count(HNLCPFW hPort);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_get_rec_buf(HNLCPFW hPort, void *buf, int len);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_clear(HNLCPFW hPort);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_register_notify(HNLCPFW hPort, CPFW_NTF_DEV_STATE pfnDevStateNotify, CPFW_NTF_DATA pfnRecData, void *pParam);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_unregister_notify(HNLCPFW hPort, BOOL bUnRegDevStateNotify, BOOL bUnRegRecDataNotify);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_enum_support_ports(CPFW_FN_ENUM_PORTS pfnEnumPorts, void *pParam);

DLL_CPFW_EXPORT int NLCPFW_API cpfw_translate_err_code(int nErr, WCHAR *pstrErrMsg, UINT dwBufLen);

#ifdef __cplusplus
};
#endif
//////////////////////////////////////////////////////////////////////////


#define CPFW_ERR_SUCCEED		0
#define CPFW_ERR_INVALID_PARAM	-1
#define CPFW_ERR_BUF_NOT_ENOUGH	-2
#define CPFW_ERR_TIMEOUT		-3
#define CPFW_ERR_ALREADY_OPEN	-4
#define CPFW_ERR_FILE_NOT_FOUND	-5
#define CPFW_ERR_DISCONNECT		-6
#define CPFW_ERR_MISDATA		-7		//用于非法数据，或校验错
#define CPFW_ERR_NOT_MEMORY		-8
#define CPFW_ERR_USER_BREAK		-9
#define CPFW_ERR_INVALID_CMD	-10
#define CPFW_ERR_DEV_NOT_FOUND	-11
#define CPFW_ERR_ALREADY_EXIST	-12
#define CPFW_ERR_NOT_SUPPORT	-13
#define CPFW_ERR_UNEXPECTED		-100

//////////////////////////////////////////////////////////////////////////

#define CPFW_STATE_INIT			0
#define CPFW_STATE_DISCONNECT	1
#define CPFW_STATE_CONNECT		2
#define CPFW_STATE_DESTORY		3

//////////////////////////////////////////////////////////////////////////

#define CPFW_TIMEOUT_NOWAIT			0
#define CPFW_TIMEOUT_INFINITE		INFINITE


#endif		//#ifndef __HEAD_FILE_NLcpfw_H__
