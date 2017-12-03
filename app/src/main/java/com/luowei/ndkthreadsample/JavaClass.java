package com.luowei.ndkthreadsample;

import android.util.Log;

/**
 * Created by luowei on 2017/12/2.
 */

public class JavaClass {
    public static final int SEND = 1;
    public static final int RECV = 2;

    int send(byte[] buffer, int length) {
        StringBuilder stringBuffer = new StringBuilder();
        for (int i = 0; i < length; i++) {
            stringBuffer.append(buffer[i]).append(", ");
        }
        Log.d("JavaClass", "send:" + stringBuffer.toString());
        return length;
    }

    int recv(byte[] buffer, int length) {
        for (int i = 0; i < 10; i++) {
            buffer[i] = (byte) (buffer[i] + 100);
        }
        StringBuilder stringBuffer = new StringBuilder();
        for (int i = 0; i < length; i++) {
            stringBuffer.append(buffer[i]).append(", ");
        }
        Log.d("JavaClass", "recv:" + stringBuffer.toString());
        return 10;
    }
}
