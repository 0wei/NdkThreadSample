package com.IDS600.UsbDriver;

import android.util.Log;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;

import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbConstants;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbEndpoint;
import android.hardware.usb.UsbInterface;
import android.hardware.usb.UsbManager;
import android.widget.Toast;

public class UsbDriver {

	static private UsbManager mUsbManager;
	static private PendingIntent mPendingIntent;
	static private UsbDevice mUsbDevice;
	static private UsbInterface mUsbInterface;
	static private UsbEndpoint outPoint;
	static private UsbEndpoint inPoint;
	static private UsbDeviceConnection mConnection;
	static private Context mContext;
	static private String mPermission;
	static private int mUsbFd;

	static private int mMaxPacketSize;
	static private boolean bAttached = false;
	static private ArrayList mDevList = new ArrayList();
	static private int mListSize;

	public void InitContext(Context context, String permission) 
	{
		mContext		= context;
		mPermission = permission;
	}

	public static int OpenDevice(int iVid, int iPid) 
	{
		int iRet = 0;

		iVid = 0x05e3;
		iPid = 0x0102;
		String strDevList = String.format("%04X:%04X", new Object[] { Integer.valueOf(iVid), Integer.valueOf(iPid) });

		addDevList(strDevList);
		iRet = FindUsbList();

		return iRet;
	}

	public static int CloseDevice() 
	{
		if (mConnection != null) 
		{
			if (mUsbInterface != null) 
			{
				mConnection.releaseInterface(mUsbInterface);
				mUsbInterface = null;
			}

			mConnection.close();
			mConnection = null;
		}

		if (mUsbDevice != null) 
		{
			mUsbDevice = null;
		}

		if (mUsbManager != null) 
		{
			mUsbManager = null;
		}

		if (bAttached) 
		{
			mContext.unregisterReceiver(mUsbReceiver);
			bAttached = false;
		}

		return 0;

	}

	public boolean UsbFeatureSupported() {
		return this.mContext.getPackageManager().hasSystemFeature(
				"android.hardware.usb.host");
	}
	
	public static int getUsbFd(){
		return mUsbFd;
	}

	public static int FindUsbList() {
		mUsbManager = (UsbManager) mContext.getSystemService("usb");
		HashMap<String, UsbDevice> deviceList = mUsbManager.getDeviceList();
		if (deviceList.isEmpty()) {
			Toast.makeText(mContext, "No Device Or Device Not Match", 1).show();
			return -1;
		} else {
			Iterator<UsbDevice> deviceIterator = deviceList.values().iterator();
			while (deviceIterator.hasNext()) {
				UsbDevice device = (UsbDevice) deviceIterator.next();
				for (int i = 0; i < mListSize; ++i) {

					String strDevList = String.format(
							"%04X:%04X",
							new Object[] {
									Integer.valueOf(device.getVendorId()),
									Integer.valueOf(device.getProductId()) });

					if (strDevList.equals(mDevList.get(i))) {

						mPendingIntent = PendingIntent.getBroadcast(mContext,
								0, new Intent(mPermission), 0);
						mUsbManager.requestPermission(device, mPendingIntent);

						IntentFilter filter = new IntentFilter(mPermission);
						filter.addAction("android.hardware.usb.action.USB_DEVICE_DETACHED");
						mContext.registerReceiver(mUsbReceiver, filter);

						synchronized (UsbDriver.class) {
							if (mUsbManager.hasPermission(device)) {
								setDevice(device);
								return 1;
							} else {
								Toast.makeText(mContext, "No Perssion!", 1)
										.show();
								return -2;
							}
						}

					}

				}
			}
		}

		Log.i("UsbDriver", "String.format not match");
		return -1;
	}

	public static int SendData(byte[] bySendData, int iSendLen, int []piWritten, int iTimeOut) 
	{
		if (mConnection == null || outPoint == null) 
		{
			Log.e("UsbDriver", "SendData mConnection or outPoint  error");
			return -1;
		}

		// 发送准备命令
		piWritten[0] = mConnection.bulkTransfer(outPoint, bySendData, iSendLen, iTimeOut);
		if (piWritten[0] < 0) 
		{
			Log.e("UsbDriver", "SendData bulkTransfer error");
			return -2;
		}

		return 0;
	}

	public static int RecvData(byte[] byData, int iDataLen, int []piRecved, int iTimeOut)
	{
		if (mConnection == null || inPoint == null) 
		{
			Log.e("UsbDriver", "RecvData mConnection or inPoint  error");
			return -1;
		}
		
		int index = 0;
		byte[] tmpData = new byte[inPoint.getMaxPacketSize()];

		int inMax = inPoint.getMaxPacketSize();

		while (iDataLen > 0) 
		{
			if (iDataLen < inMax)
			{
				inMax = iDataLen;
			}
			
			int length = mConnection.bulkTransfer(inPoint, tmpData, inMax, iTimeOut);
			// 发送准备命令
			if (length <= 0) 
			{
				Log.e("UsbDriver", "RecvData bulkTransfer error");
				return -2;
			}
			else
			{
				iDataLen -= length;
				index += length;
			}
		}

		piRecved[0] = index;

		return 0;
	}

	public static int SendControl(byte[] bySendCMD, int iSendLen, int iOffset, int iIndex, int iTimeOut) 
	{
		if (mConnection == null) 
		{
			return -1;
		}
		int iRet = 0;
		// requestType
		// D0-4 0-device 1-interface 2-endpoint 3-other 4..31-reserved.
		// D5-6 0-standard 1-class 2-vendor 3-reserved
		// D7 0-host to device 1-device to host
		int requestType = 0x40;
		int req = (iSendLen > 2) ? 0x04 : 0x0C; // REQUEST_BUFFER-04
												// REQUEST_REGISTER-0c
		
		iRet = mConnection.controlTransfer(requestType, req, iOffset, iIndex, bySendCMD, iSendLen, iTimeOut);

		if (iRet < 0) 
		{
			Log.e("UsbDriver", "SendData controlTransfer error");
			return -2;
		}

		return 0;
	}

	public static int RecvControl(byte[] byRecvCMD, int iRecvLen, int iOffset, int iIndex, int iTimeOut) 
	{
		if (mConnection == null) 
		{
			return -1;
		}

		// requestType
		// D0-4 0-device 1-interface 2-endpoint 3-other 4..31-reserved.
		// D5-6 0-standard 1-class 2-vendor 3-reserved
		// D7 0-host to device 1-device to host
		int requestType = 0xC0;
		int req = (iRecvLen > 2) ? 0x04 : 0x0C; // REQUEST_BUFFER-04
												// REQUEST_REGISTER-0c
		int iRet = mConnection.controlTransfer(requestType, req, iOffset, iIndex,
				byRecvCMD, iRecvLen, iTimeOut);

		if (iRet < 0) 
		{
			Log.e("UsbDriver", "RecvData controlTransfer error");
			return -2;
		}
		return 0;
	}

	protected static BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {

		@Override
		public final void onReceive(Context arg0, Intent arg1) {
			String action = arg1.getAction();
			if ("android.hardware.usb.action.USB_DEVICE_ATTACHED"
					.equals(action)) {
				Log.i("UsbDriver", "Step1!\n");
			} else if (mPermission.equals(action)) {
				Log.i("UsbDriver", "Step2!\n");
				synchronized (UsbDriver.class) {
					UsbDevice device = (UsbDevice) arg1
							.getParcelableExtra("device");
					if (arg1.getBooleanExtra("permission", false)) {
						setDevice(device);
					} else {
						Log.i("UsbDriver", "permission denied");
					}
				}

			} else if ("android.hardware.usb.action.USB_DEVICE_DETACHED"
					.equals(action)) {
				Log.i("UsbDriver", "Step3!\n");
				UsbDevice device = (UsbDevice) arg1
						.getParcelableExtra("device");
				String strDeviceName = device.getDeviceName();
				Log.i("UsbDriver", strDeviceName);
				for (int i = 0; i < mListSize; ++i) {
					if (String.format(
							"%04X:%04X",
							new Object[] {
									Integer.valueOf(device.getVendorId()),
									Integer.valueOf(device.getProductId()) })
							.equals(mDevList.get(i))) {
						Toast.makeText(mContext, "Disconnect", 1).show();
						CloseDevice();
						System.exit(0);
					}
				}
			}
		}
	};

	private static void addDevList(String strList) {
		mDevList.add(strList);
		mListSize = mDevList.size();
	}

	private static void setDevice(UsbDevice device) {
		if (device != null) {
			
			if (mConnection != null) {
				if (mUsbInterface != null) {
					mConnection.releaseInterface(mUsbInterface);
					mUsbInterface = null;
				}

				mConnection.close();
				mUsbDevice = null;
				mUsbInterface = null;
			}

			mConnection = mUsbManager.openDevice(device);
			if (mConnection != null) {
				Log.i("UsbDriver", "UsbManager openDevice success");
				mUsbFd = mConnection.getFileDescriptor();
				mUsbInterface = device.getInterface(0);// USBEndpoint为读写数据所需的
				outPoint = mUsbInterface.getEndpoint(1);
				inPoint = mUsbInterface.getEndpoint(0);
				mConnection.claimInterface(mUsbInterface, true);
				// assignEndpoint(mUsbInterface);
			}

		}

	}

	// #define USB_ENDPOINT_XFER_CONTROL 0 --控制传输
	// #define USB_ENDPOINT_XFER_ISOC 1 --等时传输
	// #define USB_ENDPOINT_XFER_BULK 2 --块传输
	// #define USB_ENDPOINT_XFER_INT 3 --中断传输
	private void assignEndpoint(UsbInterface mUsbInterface) {

		if (mUsbInterface != null) {
			for (int i = 0; i < mUsbInterface.getEndpointCount(); i++) {
				UsbEndpoint ep = mUsbInterface.getEndpoint(i);
				if (ep.getDirection() == UsbConstants.USB_ENDPOINT_XFER_INT) {
					if (ep.getDirection() == UsbConstants.USB_DIR_OUT) {
						outPoint = ep; // 发送数据节点
					} else {
						inPoint = ep; // 接收数据节点
					}

				}
			}

			mConnection.claimInterface(mUsbInterface, true);

		} else {
			Log.e("UsbDriver", "UsbManager openDevice failed");
		}
	}
}
