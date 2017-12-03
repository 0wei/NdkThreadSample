#include <jni.h>
#include <string>
#include <android/log.h>

#define LOG_TAG "idCard_c"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define NUMTHREADS 5

struct fields_t {
    jmethodID post_event;
    jobject card_object;
    jclass card_clazz;
    char *basePath;
};
static fields_t fields;
static JavaVM *sVm;
static JNIEnv *env;

jclass mClass;
jobject mObject;
pthread_t pt[NUMTHREADS];
int thread_count = 0;

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    sVm = vm;
    sVm->GetEnv((void **) &env, JNI_VERSION_1_4);
    return JNI_VERSION_1_4;
}

void notify(JNIEnv *env, int code) {
    jobject pJobject = NULL;
    jclass clazz = fields.card_clazz;

    pJobject = fields.card_object;

    jfieldID filedBuffer = env->GetFieldID(clazz, "buffer", "[B");
    int bufferSize = 20;
    char cBuffer[bufferSize];
    for (int i = 0; i < bufferSize; i++) {
        cBuffer[i] = (char) i;
    }
    jbyteArray pBufferArray = env->NewByteArray(bufferSize);
    env->SetByteArrayRegion(pBufferArray, 0, bufferSize, (const jbyte *) cBuffer);
    env->SetObjectField(pJobject, filedBuffer, pBufferArray);

    jfieldID filedSize = env->GetFieldID(clazz, "size", "[I");
    int sizeLen = 1;
    int size[1];
    size[0] = bufferSize;

    jintArray pSizeArray = env->NewIntArray(sizeLen);
    env->SetIntArrayRegion(pSizeArray, 0, sizeLen, size);
    env->SetObjectField(pJobject, filedSize, pSizeArray);

    env->CallStaticVoidMethod(mClass, fields.post_event, mObject, code, pJobject);

    jbyte *jbuffer = env->GetByteArrayElements(pBufferArray, NULL);
    jint *jsize = env->GetIntArrayElements(pSizeArray, NULL);
    LOGD("jsize=%d",jsize[0]);
    for (int i = 0; i < jsize[0]; i++) {
        LOGD("buffer[%d]=%d",i,jbuffer[i]);
    }
    env->ReleaseByteArrayElements(pBufferArray, jbuffer, 0);
    env->ReleaseIntArrayElements(pSizeArray, jsize, 0);


}

//线程的run方法
void *thread_fun(void *args) {
    int c = 2;
    bool isAttached = false;
    while (c--) {
        //记录从jvm中申请JNIEnv*的状态
        int status;
        //用于存放申请过来的JNIEnv*
        JNIEnv *env;
        //用于标记线程的状态，用于开启，释放
        //获取当前状态，查看是否已经拥有过JNIEnv指针
        status = sVm->GetEnv((void **) &env, JNI_VERSION_1_4);
        LOGD("child thread get env status = %d", status);
        if (status < 0) {
            //将当前线程依附于java虚拟机：
            //这样能够得到一个JNIEnv*指针，
            //该线程也能够像java线程一样，在一定规则下运行
            //这个状态将持续直到调用detachCurrentThread方法
            LOGD("attach current thread");
            status = sVm->AttachCurrentThread(&env, NULL);
            if (status < 0)
                return NULL;
            isAttached = true;
        }
        //执行这个线程要做的事情

        notify(env, thread_count);
    }
    //执行完了释放
    if (isAttached)
        sVm->DetachCurrentThread();
    LOGD("thread is finish");
    return NULL;
}

void notifyChildThread(int code) {
    JNIEnv *env;
    sVm->AttachCurrentThread(&env, NULL);
//    sVm->GetEnv((void **) &env, JNI_VERSION_1_4);
    notify(env, code);
    sVm->DetachCurrentThread();
//    env->DeleteLocalRef(pJobject);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_luowei_ndkthreadsample_NativeClass_native_1init(JNIEnv *env, jclass type) {
    jclass clazz;
    clazz = env->FindClass("com/luowei/ndkthreadsample/NativeClass");
    if (clazz == NULL) {
        LOGE("can't find IDScanner");
        return;
    }
    fields.post_event = env->GetStaticMethodID(clazz, "postEventFromNative",
                                               "(Ljava/lang/Object;ILjava/lang/Object;)V");

//    LOGD("find CardInfoCn");
    clazz = env->FindClass("com/luowei/ndkthreadsample/CommData");

    fields.card_clazz = (jclass) env->NewGlobalRef(clazz);
    if (fields.card_clazz == NULL) {
        LOGE("can't find IDScanner");
        return;
    }
    LOGD("find CardInfoCn ok");
    jmethodID constrocMID = env->GetMethodID(fields.card_clazz, "<init>", "()V");
    jobject pJobject = env->NewObject(clazz, constrocMID);
    fields.card_object = env->NewGlobalRef(pJobject);

}extern "C"
JNIEXPORT void JNICALL
Java_com_luowei_ndkthreadsample_NativeClass_native_1setup(JNIEnv *env, jobject thiz,
                                                          jobject nativeclass_this) {
    jclass clazz = env->GetObjectClass(thiz);
    if (clazz == NULL) {
        LOGD("Can't find IDScanner");
//        jniThrowException(env, "java/lang/Exception", NULL);
        return;
    }
    mClass = (jclass) env->NewGlobalRef(clazz);
    // We use a weak reference so the MediaPlayer object can be garbage collected.
    // The reference is only used as a proxy for callbacks.
    mObject = env->NewGlobalRef(nativeclass_this);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_luowei_ndkthreadsample_NativeClass_start_1thread(JNIEnv *env, jobject instance) {

    pthread_create(&pt[thread_count], NULL, &thread_fun, (void *) thread_count);
    thread_count++;
}extern "C"
JNIEXPORT void JNICALL
Java_com_luowei_ndkthreadsample_NativeClass_setData(JNIEnv *env, jobject instance,
                                                    jbyteArray buffer_, jintArray size_) {
    jbyte *buffer = env->GetByteArrayElements(buffer_, NULL);
    jint *size = env->GetIntArrayElements(size_, NULL);

    env->ReleaseByteArrayElements(buffer_, buffer, 0);
    env->ReleaseIntArrayElements(size_, size, 0);
}