package com.luowei.ndkthreadsample;

import android.util.Log;

import java.lang.ref.WeakReference;

/**
 * Created by luowei on 2017/12/2.
 */

public class NativeClass {
    static {
        System.loadLibrary("native-lib");
        native_init();
    }

    private static final String TAG = "NativeClass";
    private JavaClass transmit;

    public NativeClass() {
        native_setup(new WeakReference<>(this));
        Log.d(TAG, String.format("NativeClass:  %d", Thread.currentThread().getId()));
        transmit = new JavaClass();
    }

    public native static void native_init();

    public native void native_setup(Object nativeclass_this);

    public native void start_thread();

    public static void postEventFromNative(Object IDScanner_ref, final int code, Object card) {
        final NativeClass mp = (NativeClass) ((WeakReference) IDScanner_ref).get();
        if (mp == null) {
            return;
        }
        Log.d(TAG, String.format("postEventFromNative: code=%d, %d", code, Thread.currentThread().getId()));


        if (card != null) {
            CommData data = (CommData) card;
//            StringBuffer buffer = new StringBuffer();
//            for (int i = 0; i < 10; i++) {
//                buffer.append(data.buffer[i]).append(", ");
//                data.buffer[i] = (byte) (9 - i);
//            }
//            Log.d(TAG, String.format("postEventFromNative: card=%s, size=%d", buffer.toString(), data.size[0]));
            switch (code) {
                case JavaClass.SEND:
                    data.size[0] = mp.transmit.send(data.buffer, data.size[0]);
                    break;
                case JavaClass.RECV:
                    data.size[0] = mp.transmit.recv(data.buffer, data.size[0]);
                    break;
            }
        }
    }
}
