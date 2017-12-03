#include "UsbDriver_CCallJava.h"

#include <android/log.h>


JavaVM *g_jvm = 0;
JNIEnv *jniEnv = 0;
jclass UsbDriverCCallJava;

jmethodID openDevice;
jmethodID closeDevice;
jmethodID sendData;
jmethodID recvData;
jmethodID sendControl;
jmethodID recvControl;

#define TAG         "IDS600_Driver"
#define JAVA_PATH   "com/IDS600/UsbDriver/UsbDriver"


JNIEnv* getJNIEnv() 
{
    JNIEnv *env = 0;

    if((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK)
    {
        __android_log_print(ANDROID_LOG_INFO, TAG, "getJNIEnv");
        return NULL;
    }
    return env;
}

void detachJNI() 
{
    (*g_jvm)->DetachCurrentThread(g_jvm);
}

/*
 * 设置JIN 环境
 */
void SetJniEnv(int env)
{
    g_jvm = (JavaVM *)env;
    jniEnv = getJNIEnv ();//(JNIEnv*)env;
    UsbDriverCCallJava = (*jniEnv)->FindClass(jniEnv, JAVA_PATH);
}

void ReleaseUsbDriver()
{
    //__android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 1");
    if (jniEnv == NULL) 
    {
        return;
    }

    //__android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 2");
    if(UsbDriverCCallJava != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, UsbDriverCCallJava);
    }
    //__android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 3");
#if 0
    if(openDevice != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, openDevice);
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 4");

    if(closeDevice != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, closeDevice);
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 5");

    if(sendData != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, sendData);
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 6");

    if(recvData != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, recvData);
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 7");
    if(sendControl != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, sendControl);
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "ReleaseUsbDriver 8");
    if(recvControl != NULL)
    {
        (*jniEnv)->DeleteLocalRef(jniEnv, recvControl);
    }
#endif

    detachJNI();
    
}


/*
 * 初始化 类、对象、方法
 */
int InitUsbDriver() 
{
	//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  1");

	if (jniEnv == NULL) 
    {
		return 0;
	}

	//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  2 ok");

	
    openDevice = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "OpenDevice", "(II)I");
        if (openDevice == NULL) 
        {
            ReleaseUsbDriver();
            return -2;
        }
        //__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  3 ok");

		closeDevice = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "CloseDevice", "()I");
        if (closeDevice == NULL) 
        {
            ReleaseUsbDriver();
            return -3;
        }
        //__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  4 ok");


		sendData = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "SendData", "([BI[II)I");
		if (sendData == NULL) 
        {
			ReleaseUsbDriver();
			return -4;
		}
		//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  5 ok");

	
		recvData = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "RecvData", "([BI[II)I");
		if (recvData == NULL) 
        {
			ReleaseUsbDriver();
			return -5;
		}
		//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  6 ok");
	
		sendControl = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "SendControl", "([BIIII)I");
		if (sendControl == NULL) 
        {
			ReleaseUsbDriver();
			return -6;
		}
		//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  7 ok");


		recvControl = (*jniEnv)->GetStaticMethodID(jniEnv, UsbDriverCCallJava, "RecvControl", "([BIIII)I");
		if (recvControl == NULL) 
        {
			ReleaseUsbDriver();
			return -7;
		}
		//__android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  8 ok");

	return 1;

}



/**
 * jni_open_device ---- 调用 Java 方法
 */
int jni_open_device(int iVid, int iPid) 
{

	int result = InitUsbDriver();
	if (result != 1) 
    {
		return -1;
	}

	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, openDevice,iVid, iPid);

    ReleaseUsbDriver ();
	return result;

}

/**
 * jni_close_device ---- 调用 Java 方法
 */
int jni_close_device() 
{
    int result = InitUsbDriver();
	if (result != 1) 
    {
		return -1;
	}

	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, closeDevice);

    ReleaseUsbDriver ();
	return result;

}


int jni_bulk_sendData(unsigned char *pucData, int iSendLen, int *piWritten, int iTimeOut) 
{
    __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_sendData_1");
	int result = InitUsbDriver();
	if (result != 1) 
    {
        __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_sendData_2");
		return -1;
	}

	jbyteArray jArData      = (*jniEnv)->NewByteArray(jniEnv,  iSendLen+ 1);
	jbyte       *jpArData   = (*jniEnv)->GetByteArrayElements(jniEnv, jArData, 0);

    jintArray   jArWritten  = (*jniEnv)->NewIntArray(jniEnv, 2);
    int         *jpArWritten= (*jniEnv)->GetIntArrayElements(jniEnv, jArWritten, 0);

    (*jniEnv)->SetByteArrayRegion(jniEnv, jArData, 0, iSendLen, (const jbyte *) pucData);
    
	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, sendData, jArData, iSendLen, jArWritten, iTimeOut);

    (*jniEnv)->GetIntArrayRegion(jniEnv, jArWritten, 0, 1, piWritten);

	(*jniEnv)->ReleaseByteArrayElements(jniEnv, jArData, jpArData, 0);
	(*jniEnv)->DeleteLocalRef(jniEnv, jArData);

    (*jniEnv)->ReleaseIntArrayElements(jniEnv, jArWritten, jpArWritten, 0);
    (*jniEnv)->DeleteLocalRef(jniEnv, jArWritten);


    ReleaseUsbDriver ();
    __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_sendData_3 %d", *piWritten);
	return result;

}

/**
 * jni_bulk_recvData ---- 调用 Java 方法
 */
int jni_bulk_recvData(unsigned char *pucData, int iBufSize, int *piRecved, int iTimeOut)
{
    __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_recvData_1");
	int result = InitUsbDriver();
	if (result != 1) 
    {
        __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_recvData_2");
		return -1;
	}
    
	jbyteArray  jArData  = (*jniEnv)->NewByteArray(jniEnv, iBufSize);
    jbyte       *jpArData= (*jniEnv)->GetByteArrayElements(jniEnv, jArData, 0);

	jintArray   jArLen   = (*jniEnv)->NewIntArray(jniEnv, 2);	
	int         *jpArLen = (*jniEnv)->GetIntArrayElements(jniEnv, jArLen, 0);

    jintArray   jArRecved  = (*jniEnv)->NewIntArray(jniEnv, 2);
    int         *jpArRecved= (*jniEnv)->GetIntArrayElements(jniEnv, jArRecved, 0);
	
	jpArLen[0] = iBufSize;

	//__android_log_print(ANDROID_LOG_INFO, TAG, "jni_bulk_recvData Begin");
	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, recvData, jArData, jArLen, jArRecved, iTimeOut);
	//__android_log_print(ANDROID_LOG_INFO, TAG, "jni_bulk_recvData End");
	
    (*jniEnv)->GetIntArrayRegion(jniEnv, jArLen, 0, 1, piRecved);
    (*jniEnv)->GetByteArrayRegion(jniEnv, jArData, 0, *piRecved, pucData);

	(*jniEnv)->ReleaseByteArrayElements(jniEnv, jArData, jpArData, 0);
    (*jniEnv)->DeleteLocalRef(jniEnv, jArData);

	(*jniEnv)->ReleaseIntArrayElements(jniEnv, jArLen, jpArLen, 0);
	(*jniEnv)->DeleteLocalRef(jniEnv, jArLen);

    (*jniEnv)->ReleaseIntArrayElements(jniEnv, jArRecved, jpArRecved, 0);
    (*jniEnv)->DeleteLocalRef(jniEnv, jArRecved);

    ReleaseUsbDriver ();
    __android_log_print(ANDROID_LOG_INFO, TAG, "InitUsbDriver Begin  jni_bulk_recvData_3___%d", *piRecved);
	return *piRecved;

}

/**
 * jni_control_send ---- 调用 Java 方法
 */
int jni_control_send(unsigned char *pucData, int iSendLen, int iOffset, int iIndex, int iTimeOut) 
{
	int result = InitUsbDriver();
	if (result != 1) 
    {
		return -1;
	}

	jbyteArray  jArData   = (*jniEnv)->NewByteArray(jniEnv, iSendLen+ 1);
	jbyte       *jpArData = (*jniEnv)->GetByteArrayElements(jniEnv, jArData, 0);

    (*jniEnv)->SetByteArrayRegion(jniEnv, jArData, 0, iSendLen, (const jbyte *) pucData);
                    
	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, sendControl, jArData, iSendLen, iOffset, iIndex, iTimeOut);
    
	(*jniEnv)->ReleaseByteArrayElements(jniEnv, jArData, jpArData, 0);
	(*jniEnv)->DeleteLocalRef(jniEnv, jArData);

    ReleaseUsbDriver ();
	return result;
}

/**
 * jni_control_recv ---- 调用 Java 方法
 */
int jni_control_recv(unsigned char *pucData, int iRecvLen, int iOffset, int iIndex, int iTimeOut)
{
	int result = InitUsbDriver();
	if (result != 1) 
    {
		return -1;
	}

	jbyteArray jArData      = (*jniEnv)->NewByteArray(jniEnv, iRecvLen + 1);
	jbyte       *jpArData   = (*jniEnv)->GetByteArrayElements(jniEnv, jArData, 0);
                              

	result = (*jniEnv)->CallStaticIntMethod(jniEnv, UsbDriverCCallJava, recvControl, jArData, iRecvLen, iOffset, iIndex, iTimeOut);

	//memcpy(pucData, jpArData, iRecvLen);
    (*jniEnv)->GetByteArrayRegion(jniEnv, jArData, 0, iRecvLen, pucData);

    //__android_log_print(ANDROID_LOG_INFO, TAG, "%s__%d_#############%d______________Len:%d", __FUNCTION__, __LINE__, pucData[0], iRecvLen);

	(*jniEnv)->ReleaseByteArrayElements(jniEnv, jArData, jpArData, 0);
	(*jniEnv)->DeleteLocalRef(jniEnv, jArData);

    ReleaseUsbDriver ();
	return result;
}




