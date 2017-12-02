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

    public NativeClass() {
        native_setup(new WeakReference<>(this));
        Log.d(TAG, String.format("NativeClass:  %d", Thread.currentThread().getId()));

    }

    public native static void native_init();

    public native void native_setup(Object nativeclass_this);

    public native void start_thread();

    public static void postEventFromNative(Object IDScanner_ref, final int code, Object card) {
        final NativeClass mp = (NativeClass) ((WeakReference) IDScanner_ref).get();
        if (mp == null) {
            return;
        }
        Log.d(TAG, String.format("postEventFromNative: code=%d, %d", code,Thread.currentThread().getId()));
        if (card != null) {
//            Log.d(TAG, String.format("postEventFromNative: card=%s", ((CardInfoCn) card)));
        }
//        if (mp.mEventHandler != null) {
//            Message m = mp.mEventHandler.obtainMessage(code, 0, 0, card);
//            mp.mEventHandler.sendMessage(m);
//        }
    }
}
