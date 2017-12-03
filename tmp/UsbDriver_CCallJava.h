#pragma once

#include <string.h>
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif	


void SetJniEnv(int env);
/**
 * jni_open_device - ��USB�豸
 * @param iVid - USB��VID
 * @param iPid - USB��PID
 * return 0-�ɹ���  -1-û��ƥ����豸��-2-û��Ȩ��
 */
int jni_open_device(int iVid, int iPid);


/**
 * jni_close_device - �ر�USB�豸
 * return 0-�ɹ���  ��0 ʧ��
 */
int jni_close_device();


/**
 * jni_bulk_sendData - ��������
 * @param byData - ���͵�����
 * @param iSendLen - ���͵����ݳ���
 * @param iTimeOut - ��ʱʱ��
 * return 0-�ɹ���  -1-�豸û�д򿪣�-2-��������ʧ��
 */
int jni_bulk_sendData(unsigned char *pucData, int iSendLen, int *piWritten, int iTimeOut);


/**
 * jni_bulk_recvData - ��������
 * @param byData - ���յ�������
 * @param iRecvLen - ��������յ������ݳ��ȣ����ʵ�ʽ��յ�������
 * @param iTimeOut - ��ʱʱ��
 * return 0-�ɹ���  -1-�豸û�д򿪣�-2-��������ʧ��
 */
int jni_bulk_recvData(unsigned char *pucData, int iBufSize, int *piRecved, int iTimeOut);


/**
 * jni_bulk_recvData - ���Ϳ�������
 * @param byData - ������������
 * @param iSendLen - ���͵����ݳ���
 * @param iTimeOut - ��ʱʱ��
 * return 0-�ɹ���  -1-�豸û�д򿪣�-2-��������ʧ��
 */
int jni_control_send(unsigned char *pucData, int iSendLen, int iOffset, int iIndex, int iTimeOut);


/**
 * jni_control_recv - ���տ�������
 * @param byData - ���յ�������
 * @param iSendLen - ����յ������ݳ���
 * @param iTimeOut - ��ʱʱ��
 * return 0-�ɹ���  -1-�豸û�д򿪣�-2-��������ʧ��
 */
int jni_control_recv(unsigned char *pucBuf, int iRecvLen, int iOffset, int iIndex, int iTimeOut);

#ifdef __cplusplus
}
#endif