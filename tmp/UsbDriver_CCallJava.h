#pragma once

#include <string.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif	


void SetJniEnv(int env);
/**
 * jni_open_device - 打开USB设备
 * @param iVid - USB的VID
 * @param iPid - USB的PID
 * return 0-成功，  -1-没有匹配的设备，-2-没有权限
 */
int jni_open_device(int iVid, int iPid);


/**
 * jni_close_device - 关闭USB设备
 * return 0-成功，  非0 失败
 */
int jni_close_device();


/**
 * jni_bulk_sendData - 发送数据
 * @param byData - 发送的数据
 * @param iSendLen - 发送的数据长度
 * @param iTimeOut - 超时时间
 * return 0-成功，  -1-设备没有打开，-2-发送命令失败
 */
int jni_bulk_sendData(unsigned char *pucData, int iSendLen, int *piWritten, int iTimeOut);


/**
 * jni_bulk_recvData - 接收数据
 * @param byData - 接收到的数据
 * @param iRecvLen - 输入需接收到的数据长度，输出实际接收到的数据
 * @param iTimeOut - 超时时间
 * return 0-成功，  -1-设备没有打开，-2-接收命令失败
 */
int jni_bulk_recvData(unsigned char *pucData, int iBufSize, int *piRecved, int iTimeOut);


/**
 * jni_bulk_recvData - 发送控制命令
 * @param byData - 控制命令数据
 * @param iSendLen - 发送的数据长度
 * @param iTimeOut - 超时时间
 * return 0-成功，  -1-设备没有打开，-2-发送命令失败
 */
int jni_control_send(unsigned char *pucData, int iSendLen, int iOffset, int iIndex, int iTimeOut);


/**
 * jni_control_recv - 接收控制命令
 * @param byData - 接收到的数据
 * @param iSendLen - 需接收到的数据长度
 * @param iTimeOut - 超时时间
 * return 0-成功，  -1-设备没有打开，-2-发送命令失败
 */
int jni_control_recv(unsigned char *pucBuf, int iRecvLen, int iOffset, int iIndex, int iTimeOut);

#ifdef __cplusplus
}
#endif