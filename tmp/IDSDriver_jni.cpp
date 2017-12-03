#include <string.h>
#include <jni.h>

#include "../common/Android_Log.h"
#include "../common/ids.h"



#ifdef __cplusplus
extern "C" {
#endif

//===============================================
int C2jniString(jchar szjni[], char szc[])
{
    int i= 0;
    int j = 0;
    int szclen = strlen(szc);
    for(;j<szclen;i+=1,j++)
    {
	szjni[i] = szc[j];
    }
    return 0;
}

//===============================================
int Jni2cString(jchar szjni[], int jnilen, char szc[])
{
    int i= 0;
    int j = 0;
    for(;j<jnilen;i+=1,j++)
    {
	szc[j] = szjni[i];
    }
    return 0;
}
/*
JavaVM *g_JavaVM = 0;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
	g_JavaVM = vm;
}
*/
/**
 *  ����
 */
JNIEXPORT void JNICALL Java_com_IDS600_UsbDriver_JniDriver_testIDS(JNIEnv* env, jobject thiz, int fd)
{
	test(fd);
}

/**
 *  ���������ʼ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_init(JNIEnv* env, jobject thiz,
																    jcharArray imgPath, int fd)
{
	jchar* arrayBody = env->GetCharArrayElements(imgPath, 0);
	jsize alen = env->GetArrayLength(imgPath);
	
	char  tmpsPath[256] = {0};
    Jni2cString(arrayBody, alen, tmpsPath);

    fd = (int)env;
	int iRet = OpenDevice(tmpsPath, fd);
	
	JavaVM *JavaVM = 0;
	(*env)->GetJavaVM(env,&JavaVM);
	IDS_CS_SetUserData ((int)JavaVM);

	env->ReleaseCharArrayElements(imgPath,arrayBody, 0);
	return iRet;
}

/**
 *  �������򷴳�ʼ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_uninit(JNIEnv* env, jobject thiz)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = CloseDevice();
	
	return iRet;
}


/**
 *  У��ɨ���豸
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_calibrate(JNIEnv* env, jobject thiz)
{

	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = IDS_CS_Calibrate();

	return iRet;
}


/**
 *  ��ȡ�豸��Ϣ
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_getDevInfo(JNIEnv* env, jobject thiz,
																	 jcharArray szDrvVer, jcharArray szFWVer, jcharArray szHWVer )
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	jchar* arrayBody1 = env->GetCharArrayElements(szDrvVer, 0);
	jchar* arrayBody2 = env->GetCharArrayElements(szFWVer, 0);
	jchar* arrayBody3 = env->GetCharArrayElements(szHWVer, 0);

	char  tmpDrvVer[256] = {0};
	char  tmpFWVer[256] = {0};
	char  tmpHWVer[256] = {0};
	int iRet = IDS_CS_GetDevInfo(tmpDrvVer, tmpFWVer, tmpHWVer);
	if(iRet == 0)
	{
		LOGE("==============Java_com_IDS600_UsbDriver_JniDriver_getDevInfo+++++++\n");
		C2jniString(arrayBody1, tmpDrvVer);
		C2jniString(arrayBody2, tmpFWVer);
		C2jniString(arrayBody3, tmpHWVer);
	}
	else
	{
		LOGE("==============Java_com_IDS600_UsbDriver_JniDriver_getDevInfo---------------\n");
	}

	env->ReleaseCharArrayElements(szDrvVer,arrayBody1, 0);
	env->ReleaseCharArrayElements(szFWVer,arrayBody2, 0);
	env->ReleaseCharArrayElements(szHWVer,arrayBody3, 0);

	return iRet;
}

/**
 *  �豸��λ
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_reset(JNIEnv* env, jobject thiz)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = IDS_CS_Reset();

	return iRet;
}


/**
 *  �����豸,ͨ���˺����������״̬���Լ����ߺ���������
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_connect(JNIEnv* env, jobject thiz)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = IDS_CS_Connect();

	return iRet;
}


/**
 *  ��ȡ������״̬
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_getStatus(JNIEnv* env, jobject thiz,
																	 int iType, jcharArray szStatus)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	jchar* arrayBody1 = env->GetCharArrayElements(szStatus, 0);

	char  tmpStatus[256] = {0};
	int iRet = IDS_CS_GetStatus(iType, tmpStatus);
	if(iRet == 0)
	{
		C2jniString(arrayBody1, tmpStatus);

	}

	env->ReleaseCharArrayElements(szStatus,arrayBody1, 0);

	return iRet;
}


/**
 *  �ƿ�
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_move(JNIEnv* env, jobject thiz,
		                                                       int iPos,  int iTimeOut)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = IDS_CS_Move(iPos, iTimeOut);

	return iRet;
}


/**
 *  ɨ��ͼ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_scan(JNIEnv* env, jobject thiz,
												               int _Type, int _Mode, int _Side, int _Dpi,
												               int lBrightnessF, int lContrastF, int lGammaF,
												               int lBrightnessB, int lContrastB, int lGammaB,
												               jcharArray cFileNameFront,
												               jcharArray cFileNameBack,
												               jintArray penCardDir)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	jchar* arrayBody1 = env->GetCharArrayElements(cFileNameFront, 0);
	jchar* arrayBody2 = env->GetCharArrayElements(cFileNameBack, 0);
	jint* arrayBody3 = env->GetIntArrayElements(penCardDir, 0);

	char  tmpFileNameFront[256] = {0};
	char  tmpFileNameBack[256] = {0};

	int iRet = IDS_CS_Scan(_Type, _Mode, _Side, _Dpi, lBrightnessF, lContrastF, lGammaF,
			               lBrightnessB, lContrastB, lGammaB, tmpFileNameFront, tmpFileNameBack, (long*)arrayBody3);
	if(iRet == 0)
	{
		C2jniString(arrayBody1, tmpFileNameFront);
		C2jniString(arrayBody2, tmpFileNameBack);

	}

	env->ReleaseCharArrayElements(cFileNameFront,arrayBody1, 0);
	env->ReleaseCharArrayElements(cFileNameBack,arrayBody2, 0);
	env->ReleaseIntArrayElements(penCardDir,arrayBody3, 0);

	return iRet;
}

/**
 *  ��ȡRF
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_readRF(JNIEnv* env, jobject thiz,
		                                                         int _CardType,
		                                                         jcharArray pucType,
		                                                         jcharArray ucName, //����
		                                                         jcharArray ucGender,//�Ա�
		                                                         jcharArray ucNation,//����
		                                                         jcharArray ucBirth,//��������
		                                                         jcharArray ucAddress,//סַ
		                                                         jcharArray ucIDNumber,//�������֤��
		                                                         jcharArray ucIssued,//ǩ������
		                                                         jcharArray ucDateBegin,//��Ч����ʼ����
		                                                         jcharArray ucDateEnd,//��Ч�ڽ�ֹ����
		                                                         jcharArray cHeadName,//ָ��·�������֤ͷ��ͼ���ļ�����
		                                                         jcharArray cLeftName,//ָ��·��������ָ��ͼ���ļ�����
		                                                         jcharArray cRightName,//ָ��·��������ָ��ͼ���ļ�����
		                                                         int ulTimeout)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	jchar* arrayBody1 = env->GetCharArrayElements(pucType, 0);
	jchar* arrayBody2 = env->GetCharArrayElements(ucName, 0);
	jchar* arrayBody3 = env->GetCharArrayElements(ucGender, 0);
	jchar* arrayBody4 = env->GetCharArrayElements(ucNation, 0);
	jchar* arrayBody5 = env->GetCharArrayElements(ucBirth, 0);
	jchar* arrayBody6 = env->GetCharArrayElements(ucAddress, 0);
	jchar* arrayBody7 = env->GetCharArrayElements(ucIDNumber, 0);
	jchar* arrayBody8 = env->GetCharArrayElements(ucIssued, 0);
	jchar* arrayBody9 = env->GetCharArrayElements(ucDateBegin, 0);
	jchar* arrayBody10 = env->GetCharArrayElements(ucDateEnd, 0);
	jchar* arrayBody11 = env->GetCharArrayElements(cHeadName, 0);
	jchar* arrayBody12 = env->GetCharArrayElements(cLeftName, 0);
	jchar* arrayBody13 = env->GetCharArrayElements(cRightName, 0);

	//int iRet = 0;
	int iRet = IDS_CS_ReadRF(_CardType, (char*)arrayBody1, (char*)arrayBody2, (char*)arrayBody3, (char*)arrayBody4,
			                (char*)arrayBody5, (char*)arrayBody6, (char*)arrayBody7, (char*)arrayBody8, (char*)arrayBody9,
			                (char*)arrayBody10, (char*)arrayBody11, (char*)arrayBody12, (char*)arrayBody13, ulTimeout);


	env->ReleaseCharArrayElements(pucType,arrayBody1, 0);
	env->ReleaseCharArrayElements(ucName,arrayBody2, 0);
	env->ReleaseCharArrayElements(ucGender,arrayBody3, 0);
	env->ReleaseCharArrayElements(ucNation,arrayBody4, 0);
	env->ReleaseCharArrayElements(ucBirth,arrayBody5, 0);
	env->ReleaseCharArrayElements(ucAddress,arrayBody6, 0);
	env->ReleaseCharArrayElements(ucIDNumber,arrayBody7, 0);
	env->ReleaseCharArrayElements(ucIssued,arrayBody8, 0);
	env->ReleaseCharArrayElements(ucDateBegin,arrayBody9, 0);
	env->ReleaseCharArrayElements(ucDateEnd,arrayBody10, 0);
	env->ReleaseCharArrayElements(cHeadName,arrayBody11, 0);
	env->ReleaseCharArrayElements(cLeftName,arrayBody12, 0);
	env->ReleaseCharArrayElements(cRightName,arrayBody13, 0);
	return iRet;

}



/**
 *  Java �� ������native waitForCard ������ʵ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_waitForCard(JNIEnv* env, jobject thiz)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = WaitForCard();
	
	return iRet;
}


/**
 *  Java �� ������native scanAndReadIdCard ������ʵ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_scanAndReadIdCard(JNIEnv* env, jobject thiz,
																			int bScanImage, int bReadRF, jcharArray szIDCard)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	//jchar* arrayBody1 = env->GetCharArrayElements(szIDCard, 0);
	
	unsigned char *pucData = new unsigned char[3000*1000*2*3];

	int iRet = ScanAndReadIdCard(bScanImage, bReadRF, (char*)pucData);

	//env->ReleaseCharArrayElements(szIDCard,arrayBody1, 0);
	
	delete pucData;

	return iRet;
}

/**
 *  Java �� ������native backIdCard ������ʵ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_backIdCard(JNIEnv* env, jobject thiz)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = BackIdCard();
	
	return iRet;
}




/**
 *  Java �� ������native setScan ������ʵ��
 */
JNIEXPORT int JNICALL Java_com_IDS600_UsbDriver_JniDriver_setScan(JNIEnv* env, jobject thiz,
																  int iDpi, int iMode, int iSize, int iType)
{
	JavaVM *JavaVM = 0;
		(*env)->GetJavaVM(env,&JavaVM);
		IDS_CS_SetUserData ((int)JavaVM);

	int iRet = SetScan(iDpi, iMode, iSize, iType);
	
	return iRet;
}



#ifdef __cplusplus
}
#endif




