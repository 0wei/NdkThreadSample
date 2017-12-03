#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

#include "../common/apiIDS.h"
#include "../common/Android_Log.h"
#include "../common/ids.h"
#include "../common/idparse.h"

bool GetDevStatus (_IDS_STATUS_RESULT &stResult);
bool TransIDCard_Self(_IDS_CARD_INFO_CN _RFResult);

PST_IDS_FUN_TAB pstPfnTab = 0;
char gIdCardInfo[1024] = {0};


_IDS_SCAN_CONF    gScanConf ;

_IDS_RET IDS_CS_SetUserData (int iUserDataPtr)
{
	_IDS_RET enRet = _IDS_RET_ERR;

	    if (0 != pstPfnTab)
	    {
	        enRet = pstPfnTab->pSetUserData(iUserDataPtr);
	    }

	    return enRet;
}


_IDS_RET IDS_CS_Calibrate()
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        enRet = pstPfnTab->pCalibrate();
    }

    return enRet;
}

_IDS_RET IDS_CS_GetDevInfo (char* szDrvVer, char* szFWVer, char* szHWVer)
{
	_IDS_RET enRet = _IDS_RET_ERR;

	if(0 != pstPfnTab )
	{
		enRet = pstPfnTab->pConnect ();
		if (_IDS_RET_OK != enRet)
		{
			LOGE ("###################%s_%d\n", __FUNCTION__, __LINE__);
			return enRet;
		}

		_IDS_DEV_INFO _DevInfo;

		enRet = pstPfnTab->pGetDevInfo(&_DevInfo);
		if (_IDS_RET_OK == enRet) {
			LOGE("Connect::Deivce is connective\n");

			typedef struct {
				unsigned long ul4 :8;
				unsigned long ul3 :8;
				unsigned long ul2 :8;
				unsigned long ul1 :8;
			} _VERSION;

			if (szDrvVer != 0 && szFWVer != 0 && szHWVer != 0) {
				_VERSION* pVer = (_VERSION *) &_DevInfo.ulDrvVer;
				sprintf(szDrvVer, "%x.%x.%x.%x", pVer->ul1, pVer->ul2,
						pVer->ul3, pVer->ul4);

				pVer = (_VERSION *) &_DevInfo.ulFWVer;
				sprintf(szFWVer, "%x.%x.%x.%x", pVer->ul1, pVer->ul2, pVer->ul3,
						pVer->ul4);

				pVer = (_VERSION *) &_DevInfo.ulHWVer;
				sprintf(szHWVer, "%x.%x.%x.%x", pVer->ul1, pVer->ul2, pVer->ul3,
						pVer->ul4);
			}

		}
	}

    return enRet;
}


_IDS_RET IDS_CS_Reset()
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        enRet = pstPfnTab->pReset();
    }
    return enRet;
}

_IDS_RET IDS_CS_Connect()
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        enRet = pstPfnTab->pConnect();
    }
    return enRet;
}


_IDS_RET IDS_CS_GetStatus(int iType, char* szStatus) {

	_IDS_RET enRet = _IDS_RET_ERR;

	if (0 != pstPfnTab) {
		enRet = pstPfnTab->pConnect();

		if (_IDS_RET_OK != enRet) {
			return enRet;
		}

		_IDS_STATUS_RESULT stResult;
		enRet = pstPfnTab->pGetStatus((_IDS_DEV_STATUS_TYPE) iType, &stResult);
		if (_IDS_RET_OK == enRet) {
			int ii = 0;
			szStatus[ii++] = stResult._Sensor.ulCOVER;
			szStatus[ii++] = stResult._Sensor.ulSensor1;
			szStatus[ii++] = stResult._Sensor.ulSensor2;
			szStatus[ii++] = stResult._Sensor.ulSensor3;
			szStatus[ii++] = stResult._Sensor.ulSensor4;

		}
	}

	return enRet;
}


//移卡
_IDS_RET IDS_CS_Move(unsigned long const _Pos, unsigned long const ulTimeout)
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        enRet = pstPfnTab->pMove((_IDS_MOVE_POS)_Pos, ulTimeout);
    }
    return enRet;
}

//扫描图像
_IDS_RET IDS_CS_Scan(long _Type, long _Mode, long _Side, long _Dpi,
                         long lBrightnessF, long lContrastF, long lGammaF,
                         long lBrightnessB, long lContrastB, long lGammaB,
                         char cFileNameFront[260],
                         char cFileNameBack[260],
                         long *penCardDir)
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        _IDS_SCAN_CONF _ScanConf;
        _IDS_SCAN_RESULT _Result;

        memset (&_ScanConf, 0, sizeof (_IDS_SCAN_CONF));
        _ScanConf._Type  = (_IDS_CARD_TYPE)_Type;
        _ScanConf._Mode  = (_IDS_SCAN_MODE)_Mode;
        _ScanConf._Side  = (_IDS_SCAN_SIDE)_Side;
        _ScanConf._Dpi   = (_IDS_SCAN_DPI)_Dpi;
        _ScanConf._Front.lBrightness = lBrightnessF;
        _ScanConf._Front.lContrast   = lContrastF;
        _ScanConf._Front.lGamma      = lGammaF;
        _ScanConf._Back.lBrightness  = lBrightnessB;
        _ScanConf._Back.lContrast    = lContrastB;
        _ScanConf._Back.lGamma       = lGammaB;

        memset (&_Result, 0, sizeof (_IDS_SCAN_RESULT));
        if (_IDS_RET_OK == (enRet = pstPfnTab->pScan (_ScanConf, &_Result)))
        {
            strcpy (cFileNameFront, _Result.cFileNameFront);
            strcpy (cFileNameBack, _Result.cFileNameBack);
            *penCardDir = _Result.enCardDir;
        }
    }
    return enRet;
}


_IDS_RET IDS_CS_ReadRF(long const _CardType,
                            char *pucType,
                            char ucName[60],              //姓名
                            char ucGender[10],             //性别
                            char ucNation[100],            //民族
                            char ucBirth[16],            //出生日期
                            char ucAddress[100],           //住址
                            char ucIDNumber[36],          //公民身份证号
                            char ucIssued[80],           //签发机关
                            char ucDateBegin[16],          //有效期起始日期
                            char ucDateEnd[16],          //有效期截止日期
                            char          cHeadName[260],           //指定路径中身份证头部图像文件名称
                            char          cLeftName[260],           //指定路径中左手指纹图像文件名称
                            char          cRightName[260],          //指定路径中右手指纹图像文件名称
                            unsigned long const ulTimeout)
{
    _IDS_RET enRet = _IDS_RET_ERR;

    if (0 != pstPfnTab)
    {
        _IDS_CARD_INFO_RF _RFResult;

        memset (&_RFResult, 0, sizeof (_RFResult));
        if (_IDS_RET_OK == (enRet = pstPfnTab->pReadRF((_IDS_CARD_TYPE)_CardType, &_RFResult, ulTimeout)))
        {
        	TransIDCard_Self(_RFResult.CardInfoCN);
            *pucType = _RFResult.ucType;
            memcpy (ucName, _RFResult.CardInfoCN.ucName, sizeof (_RFResult.CardInfoCN.ucName));
            memcpy (ucGender, _RFResult.CardInfoCN.ucGender, sizeof (_RFResult.CardInfoCN.ucGender));
            memcpy (ucNation, _RFResult.CardInfoCN.ucNation, sizeof (_RFResult.CardInfoCN.ucNation));
            memcpy (ucBirth, _RFResult.CardInfoCN.ucBirth, sizeof (_RFResult.CardInfoCN.ucBirth));
            memcpy (ucAddress, _RFResult.CardInfoCN.ucAddress, sizeof (_RFResult.CardInfoCN.ucAddress));
            memcpy (ucIDNumber, _RFResult.CardInfoCN.ucIDNumber, sizeof (_RFResult.CardInfoCN.ucIDNumber));
            memcpy (ucIssued, _RFResult.CardInfoCN.ucIssued, sizeof (_RFResult.CardInfoCN.ucIssued));
            memcpy (ucDateBegin, _RFResult.CardInfoCN.ucDateBegin, sizeof (_RFResult.CardInfoCN.ucDateBegin));
            memcpy (ucDateEnd, _RFResult.CardInfoCN.ucDateEnd, sizeof (_RFResult.CardInfoCN.ucDateEnd));
            memcpy (cHeadName, _RFResult.CardInfoCN.cHeadName, sizeof (_RFResult.CardInfoCN.cHeadName));
            memcpy (cLeftName, _RFResult.CardInfoCN.cLeftName, sizeof (_RFResult.CardInfoCN.cLeftName));
            memcpy (cRightName, _RFResult.CardInfoCN.cRightName, sizeof (_RFResult.CardInfoCN.cRightName));
        }
    }
    return enRet;
}





bool SetScan(int iDpi, int iMode, int iSize, int iType)
{
	if(iDpi != 150 || iDpi != 300)
	{
		return false;
	}
	
	if(iMode > 4 || iMode < 0)
	{
		return false;
	}
	
	if(iSize > 3 || iSize < 1)
	{
		return false;
	}
	
	if(iType > 2 || iType < 1)
	{
		return false;
	}

	gScanConf._Dpi  = _IDS_SCAN_DPI(iDpi);
	gScanConf._Mode = _IDS_SCAN_MODE(iMode);
	gScanConf._Side = _IDS_SCAN_SIDE(iSize);
	gScanConf._Type = _IDS_CARD_TYPE(iType);
	return true;
}


bool GetDevStatus (_IDS_STATUS_RESULT &stResult)
{
	if(pstPfnTab == 0)
	{
		return false;
	}
	
	static bool s_bDevOnLine = false;
	
	if (_IDS_RET_OK != pstPfnTab->pConnect ())
    {
        s_bDevOnLine = false;
    }
    else if (false == s_bDevOnLine)
    {
        s_bDevOnLine = true;
    }
	
	 if (true == s_bDevOnLine)
	 {
		 return (_IDS_RET_OK == pstPfnTab->pGetStatus(_IDS_DEV_STATUS_TYPE_DS, &stResult));
	 }

	return s_bDevOnLine;
}

bool CoverFlagIs ( _IDS_STATUS_DS_FLAG const enFlag)
{
	if(pstPfnTab == 0)
	{
		return false;
	}
	
    _IDS_STATUS_RESULT stResult;

    memset (&stResult, 0, sizeof (stResult));
    return (GetDevStatus (stResult) && (enFlag == stResult._Sensor.ulCOVER));
}

bool SensorFlagIs ( EN_DS_FLAG enSensorFlag, _IDS_STATUS_DS_FLAG const enFlag)
{
	if(pstPfnTab == 0)
	{
		return false;
	}
	
    bool                bRet = false;
    _IDS_STATUS_RESULT  stResult;

    memset (&stResult, 0, sizeof (stResult));

    if (GetDevStatus (stResult))
    {
        do
        {
            if ((enSensorFlag & EN_DS_FLAG_SENSOR_1) && enFlag != stResult._Sensor.ulSensor1)
            {
                break;
            }
            if ((enSensorFlag & EN_DS_FLAG_SENSOR_2) && enFlag != stResult._Sensor.ulSensor2)
            {
                break;
            }
            if ((enSensorFlag & EN_DS_FLAG_SENSOR_3) && enFlag != stResult._Sensor.ulSensor3)
            {
                break;
            }
            if ((enSensorFlag & EN_DS_FLAG_SENSOR_4) && enFlag != stResult._Sensor.ulSensor4)
            {
                break;
            }
            bRet = true;
        }while (0);
    }
    return bRet;
}

bool TransIDCard_Self(_IDS_CARD_INFO_CN _RFResult)
{
	BYTE byTempData[1024] = {0};
	char byGetTrans[256] = {0};

	//韬唤璇佷俊鎭�
	UnicodeByte2Char(_RFResult.ucName, 30, byGetTrans);  // 濮撳悕
	RemovSpace(byGetTrans);
	memset(_RFResult.ucName, 0 ,sizeof(_RFResult.ucName));
	strcpy((char*)_RFResult.ucName, byGetTrans);


	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucGender, 2, byGetTrans); // 鎬у埆
	if(!TransSex(byGetTrans))
	{
		return false;
	}
	memset(_RFResult.ucGender, 0 ,sizeof(_RFResult.ucGender));
	strcpy((char*)_RFResult.ucGender, byGetTrans);

	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucNation, 4, byGetTrans); // 姘戞棌
	TransNation(byGetTrans);
	memset(_RFResult.ucNation, 0 ,sizeof(_RFResult.ucNation));
	strcpy((char*)_RFResult.ucNation, byGetTrans);

	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucBirth, 16, byGetTrans); // 鐢熸棩
	memset(_RFResult.ucBirth, 0 ,sizeof(_RFResult.ucBirth));
	strcpy((char*)_RFResult.ucBirth, byGetTrans);

	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucAddress, 70, byGetTrans); // 鍦板潃
	RemovSpace(byGetTrans);
	memset(_RFResult.ucAddress, 0 ,sizeof(_RFResult.ucAddress));
	strcpy((char*)_RFResult.ucAddress, byGetTrans);



	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucIDNumber, 36, byGetTrans); // 韬唤璇佸彿鐮�
	memset(_RFResult.ucIDNumber, 0 ,sizeof(_RFResult.ucIDNumber));
	strcpy((char*)_RFResult.ucIDNumber, byGetTrans);


	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucIssued, 30, byGetTrans); // 绛捐瘉鍗曚綅
	RemovSpace(byGetTrans);
	memset(_RFResult.ucIssued, 0 ,sizeof(_RFResult.ucIssued));
	strcpy((char*)_RFResult.ucIssued, byGetTrans);


	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucDateBegin, 16, byGetTrans); // 鏈夋晥璧峰鏃ユ湡
	memset(_RFResult.ucDateBegin, 0 ,sizeof(_RFResult.ucDateBegin));
	strcpy((char*)_RFResult.ucDateBegin, byGetTrans);



	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucDateEnd, 16, byGetTrans); // 鏈夋晥缁撴潫鏃ユ湡
	memset(_RFResult.ucDateEnd, 0 ,sizeof(_RFResult.ucDateEnd));
	strcpy((char*)_RFResult.ucDateEnd, byGetTrans);

	return true;
}

bool TransIDCard(_IDS_CARD_INFO_CN _RFResult, char szIDCard[])
{
	BYTE byTempData[1024] = {0};
	char szIdInfo[1024] = {0};
	char byGetTrans[256] = {0};
	char szSeparate[] = {"|"};



	//韬唤璇佷俊鎭�
	UnicodeByte2Char(_RFResult.ucName, 30, byGetTrans);  // 濮撳悕
	RemovSpace(byGetTrans);
	strcat(szIdInfo, byGetTrans);
	
	strcat(szIdInfo, szSeparate);
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucGender, 2, byGetTrans); // 鎬у埆
	if(!TransSex(byGetTrans))
	{
		return false;
	}
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucNation, 4, byGetTrans); // 姘戞棌
	TransNation(byGetTrans);
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucBirth, 16, byGetTrans); // 鐢熸棩
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucAddress, 70, byGetTrans); // 鍦板潃
	RemovSpace(byGetTrans);
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucIDNumber, 36, byGetTrans); // 韬唤璇佸彿鐮�
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucIssued, 30, byGetTrans); // 绛捐瘉鍗曚綅
	RemovSpace(byGetTrans);
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);

	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucDateBegin, 16, byGetTrans); // 鏈夋晥璧峰鏃ユ湡
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	

	
	memset(byGetTrans, 0 ,sizeof(byGetTrans));
	UnicodeByte2Char(_RFResult.ucDateEnd, 16, byGetTrans); // 鏈夋晥缁撴潫鏃ユ湡
	strcat(szIdInfo, byGetTrans);
	strcat(szIdInfo, szSeparate);
	
	//TraceLog(szIdInfo);
	strcpy(szIDCard, szIdInfo);

	return true;
}

bool ScanAndReadIdCard (bool bScanImage, bool bReadRF, char szIDCard[])
{
	if(pstPfnTab == 0)
	{
		return false;
	}
	
    bool bRet = false;

    if (0 != pstPfnTab)
    {
        bRet = true;

        LOGE("==============ScanAndReadIdCard//////////////////////");

        //褰撻渶瑕佽鍙朢F鏁版嵁鏃讹紝鍙戦�佽鍙栧懡浠�
        if (bReadRF)
        {
            bRet = (_IDS_RET_OK == pstPfnTab->pReadRF (_IDS_CARD_TYPE_ID_CN, 0, 0));
        }

        //绉诲姩鍗″埌鎸囧畾寰呮壂鎻忎綅缃�
        bRet &= (_IDS_RET_OK == pstPfnTab->pMove(_IDS_MOVE_POS_SCAN, 10000));

        //褰撻渶瑕佽鍙朢F鏁版嵁鏃讹紝寤惰繜绛夊緟璇诲崱
        if (bRet && bReadRF)
        {
            usleep (300 * 1000);
        }

        //褰撻渶瑕佽繘琛屽浘鍍忔壂鎻忔椂锛屽惎鍔ㄦ壂鎻忓鐞�
        if (bRet && bScanImage)
        {
            _IDS_SCAN_CONF      stScanConf;

            memset (&stScanConf, 0, sizeof (stScanConf));
			memcpy(&stScanConf, &gScanConf, sizeof(_IDS_SCAN_CONF));

            _IDS_SCAN_RESULT    stScanResult;
            memset (&stScanResult, 0, sizeof (stScanResult));

            if (bRet = (_IDS_RET_OK == pstPfnTab->pScan(stScanConf, &stScanResult)))
            {
                printf ("==============> %s___%d___Scan Finish\n", __FUNCTION__, __LINE__);
            }

        }

        //鎵弿瀹屾垚鍚庯紝鑾峰彇搴曞眰鎺у埗鍣ㄨ鍒癛F鏁版嵁
        if (bReadRF)
        {

            _IDS_CARD_INFO_RF _RFResult;
            
            memset (&_RFResult, 0, sizeof (_RFResult));
            switch (pstPfnTab->pReadRF(_IDS_CARD_TYPE_ID_CN, &_RFResult, 0))
            {
            case _IDS_RET_OK:
                {
                    LOGE ("==============> %s___%d___Read RF is success\n", __FUNCTION__, __LINE__);
					TransIDCard(_RFResult.CardInfoCN, szIDCard);
					LOGE ("==============> szIDCard:%s \n", szIDCard);
                }
                break;
            case _IDS_RET_ERR_RF:
                {
                    LOGE ("==============> %s___%d___Read RF is fail\n", __FUNCTION__, __LINE__);
                }
                break;
            default:
                {
                }
                break;
            }
        }  
    }
    return bRet;
}



bool OpenDevice (char* imgPath, int fd)
{   
	LOGE ("===============%s======\n", imgPath);
	CloseDevice ();
	
    bool            bRet = false;
    _IDS_DEV_DESC   stDevDesc = {0x05e3, 0x0102, 0};
	stDevDesc.iFileDesc = fd;

    pstPfnTab = Dll_IDS_Entry ();
    if (0 != pstPfnTab)
    {
        if (_IDS_RET_OK == pstPfnTab->pInit(imgPath, &stDevDesc))
        {
            bRet = true;
        }
        else
        {
            pstPfnTab = 0;
        }
    }
	
	memset (&gScanConf, 0, sizeof (gScanConf));
	gScanConf._Dpi  = _IDS_SCAN_DPI_300;
    gScanConf._Mode = _IDS_SCAN_MODE_COLOR;
    gScanConf._Side = _IDS_SCAN_SIDE_DUP;
    gScanConf._Type = _IDS_CARD_TYPE_ID_CN;

    return bRet;
}

bool CloseDevice ()
{
    bool bRet = false;

    if (0 != pstPfnTab)
    {
        pstPfnTab->pUninit();
        pstPfnTab = 0;
        bRet = true;
    }

    return bRet;
}


bool WaitForCard ()
{

    bool bRet = false;

    if (0 != pstPfnTab)
    {
        do
        {
            //妫�鏌ヤ笂鐩栫姸鎬侊紝琚墦寮�鏃朵笉鍙互鎵弿
            if (true == CoverFlagIs (_IDS_STATUS_DS_FLAG_LOW))
            {
                LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);
                break;
            }
            
            //妫�鏌ユ壂鎻忓墠涓嶅彲浠ラ伄鎸＄殑浼犳劅鍣�
            if (SensorFlagIs((EN_DS_FLAG)(EN_DS_FLAG_SENSOR_2 | EN_DS_FLAG_SENSOR_3 | EN_DS_FLAG_SENSOR_4), _IDS_STATUS_DS_FLAG_LOW))
            {
                LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);
                break;
            }

            //妫�鏌ユ壂鎻忓墠蹇呴』閬尅鐨勪紶鎰熷櫒
            bRet = SensorFlagIs(EN_DS_FLAG_SENSOR_1, _IDS_STATUS_DS_FLAG_LOW);
        }while (0);
    }
    return bRet;
}

bool ReadIdcard(PST_IDS_FUN_TAB pstPfnTab)
{
    return false;
}

bool ScanIdCard (PST_IDS_FUN_TAB pstPfnTab)
{
    return false;
}

bool BackIdCard()
{
	if(pstPfnTab == 0)
	{
		return false;
	}
	
    bool bRet = true;

    if (_IDS_RET_ERR_JAM == pstPfnTab->pMove(_IDS_MOVE_POS_INSERT, 10000))
    {
        bRet = false;
    }  
    return bRet;
}

int test(int fd)
{   
    PST_IDS_FUN_TAB pstPfnTab = 0;
	char szIDCard[1024] = {0};

	
    if (OpenDevice ("/sdcard/android", fd))
    {
        LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);
#if 0
		pstPfnTab->pUpgrader();
		LOGE ("===============Upgrader: %s_%d___\n", __FUNCTION__, __LINE__);
#else
        while (1)
        {
            if (WaitForCard ())
            {
                LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);

                if (ScanAndReadIdCard ( true, true, szIDCard))
				{
					 LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);
					BackIdCard ();
				}
				 LOGE ("===============%s_%d___\n", __FUNCTION__, __LINE__);
            }
            else
            {
                usleep (50 * 1000);
            }
        }
#endif
        CloseDevice ();
    }
    else
    {
        printf ("===============%s_%d__\n", __FUNCTION__, __LINE__);
    }    
}
