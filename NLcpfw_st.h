/********************************************************************
	created:	2010/11/19
	created:	19:11:2010   17:21
	filename: 	d:\Work\CommuLib\NLcpfw\inc\NLcpfw_st.h
	file path:	d:\Work\CommuLib\NLcpfw\inc
	file base:	NLcpfw_st
	file ext:	h
	author:		zencg
	
	purpose:	
*********************************************************************/


#ifndef __HEAD_FILE_NLcpfw_st_H__

#define __HEAD_FILE_NLcpfw_st_H__

//////////////////////////////////////////////////////////////////////////


#include <wtypes.h>
//////////////////////////////////////////////////////////////////////////

typedef void (*CPFW_FN_ENUM_PARAMS)(WCHAR *pstrParamDesc, WCHAR *pstrParamDefVal, WCHAR *pstrParamVal, int nParamType, void *pParam);

typedef void (*CPFW_FN_QUERY_PARAM_VALID_VALUE)(WCHAR *pstrVal, void *pParam);

typedef void *HPLUGDEV;

typedef struct
{
	UINT dwSize;
	int (*get_dev_desc)(WCHAR *pstrBuf, UINT dwLen);
	HPLUGDEV (*open)(WCHAR *pstrParam);
	int (*close)(HPLUGDEV hdev);
	int (*set_properties)(HPLUGDEV hdev, WCHAR pstrParam);
	int (*get_dev_state)(HPLUGDEV hdev);
	int (*read)(HPLUGDEV hdev, void *pBuf, UINT dwBufLen, UINT timeout);
	int (*write)(HPLUGDEV hdev, const void *pBuf, UINT dwBufLen, UINT timeout);
	int (*clear)(HPLUGDEV hdev);
	int (*get_params_count)(void);
	int (*enum_params)(CPFW_FN_ENUM_PARAMS pfnEnumParams, void* pParam);
	int (*query_param_valid_value)(WCHAR *pstrParamDesc, CPFW_FN_QUERY_PARAM_VALID_VALUE pfnQueryVal, void *pParam);
	int (*ShowConfigDlg)(WCHAR *pstrParam, UINT dwBufLen);
}NLCPFW_PLUG_API, *PNLCPFW_PLUG_API;

//////////////////////////////////////////////////////////////////////////

typedef struct
{
	void* hDev;
	int nMode;
	PNLCPFW_PLUG_API DevAPI;
	HINSTANCE hPlugDll;
	void *exData;
}NLCPFW,*HNLCPFW;




typedef PNLCPFW_PLUG_API (*PFN_GET_CPFW_PLUG_API)(void);

typedef int (*PFN_GET_CPFW_PLUG_DEV_NAME)(WCHAR *pstrBuf, UINT dwLen);


//////////////////////////////////////////////////////////////////////////

#endif		//#ifndef __HEAD_FILE_NLcpfw_st_H__

