//
// Created by Mark Murillo on 2018-01-04.
//

#include "CSourceExample.h"

JavaVM* g_vm;
jclass g_cls;
jmethodID g_needToAddFive;
jmethodID g_signBytes;
int g_callbackRegistered;

int needToAddFive_java(int num) {
    JNIEnv* g_env;
    int detached = 0;

    // double check it's all ok
    int getEnvStat = (*g_vm)->GetEnv(g_vm, (void **)&g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        detached = 1;
        LOGD("NATIVE", "Thread not attached. Attaching...\n");

        if ((*g_vm)->AttachCurrentThread(g_vm, &g_env, NULL) != 0) {
            LOGD("NATIVE", "Could not attach current thread.\n");
            return -1;
        }
    } else if (getEnvStat == JNI_EVERSION) {
        LOGD("NATIVE", "Version not supported\n");
        return -1;
    }

    if(!g_cls){
        LOGD("NATIVE", "Failed to find class!\n");
        return -1;
    }

    LOGD("NATIVE", "Calling static int method!\n");
    int result = (*g_env)->CallStaticIntMethod( g_env, g_cls, g_needToAddFive, num);

    if ( detached == 1 ) {
        (*g_vm)->DetachCurrentThread( g_vm );
    }

    return result;
}

int returnNumWithSumOfFive (int num) {
    LOGD("NATIVE", "returnNumWithSumOfFive called with %d\n", num );
    int result = needToAddFive_java(num);
    LOGD("NATIVE", "returnNumWithSumOfFive() result %d.", result);
    return result;
}

void handleBytesToSign(unsigned char * inBytes, long inLen) {
    LOGD("NATIVE", "handleBytesToSign() Called.");
    LOGD("NATIVE", "Optionally unwrapping bytes to do signature...");
    int error = 0;
    long outLen = 0;
    int i = 0;

    LOGD("NATIVE", "Bytes to sign:");

    for(i = 0; i < inLen; ++i) LOGD("NATIVE", "%c", inBytes[i]);
    LOGD("NATIVE", "Error code: %d", error);

    unsigned char * result = doSignature(inBytes, inLen, &outLen, &error);

    LOGD("NATIVE", "Signed bytes:");
    for(i = 0; i < outLen; ++i) LOGD("NATIVE", "%c", result[i]);
    LOGD("NATIVE", "Error code: %d", error);
    //Free result after we are done.
    free(result);
}

unsigned char * doSignature(unsigned char * inBytes, long inLen, long* outLen, int * error) {
    //We can request to do platform specific signature here with an ifdef!
    return signBytes_java(inBytes, inLen, outLen, error);
}

unsigned char * signBytes_java (unsigned char * inBytes, long inLen, long* outLen, int *error) {
    JNIEnv* g_env;
    int detached = 0;

    // double check it's all ok
    int getEnvStat = (*g_vm)->GetEnv(g_vm, (void **)&g_env, JNI_VERSION_1_6);

    if (getEnvStat == JNI_EDETACHED) {
        detached = 1;
        LOGD("NATIVE", "Thread not attached. Attaching...\n");

        if ((*g_vm)->AttachCurrentThread(g_vm, &g_env, NULL) != 0) {
            LOGD("NATIVE", "Could not attach current thread.\n");
            return -1;
        }
    } else if (getEnvStat == JNI_EVERSION) {
        LOGD("NATIVE", "Version not supported\n");
        return -1;
    }

    if(!g_cls){
        LOGD("NATIVE", "Failed to fine class!\n");
        return -1;
    }

    LOGD("NATIVE", "Calling static byte array method!\n");

    //Need to convert inBytes and error to their respective java counterparts.
    jbyteArray jinBytes = (*g_env)->NewByteArray(g_env, inLen);
    (*g_env)->SetByteArrayRegion(g_env, jinBytes, 0, inLen, (jbyte*)inBytes);

    jintArray jerror = (*g_env)->NewIntArray(g_env, 1);
    (*g_env)->SetIntArrayRegion(g_env, jerror, 0, 1, (jint*)error);

    //Now we call the sign bytes method which actually returns another jbyteArray
    jbyteArray jresult = (jbyteArray) (*g_env)->CallStaticObjectMethod( g_env, g_cls, g_signBytes, jinBytes, jerror);

    //Parse the byte array result.
    unsigned char * result = (unsigned char *) (*g_env)->GetByteArrayElements(g_env, jresult, 0);
    size_t length = (*g_env)->GetArrayLength(g_env, jresult);
    outLen[0] = length;

    //Parse the error array result.
    int* errorResult = (int *) (*g_env)->GetIntArrayElements(g_env, jerror, 0);
    error[0] = errorResult[0];


    //Need to optionally copy the result if we want it to persist in native after retrieving it from java.
    unsigned char * returnResult = (unsigned char *) malloc(length*sizeof(unsigned char));
    memcpy(returnResult, result, length);

    //Release java handles.
    (*g_env)->ReleaseByteArrayElements(g_env, jresult, (jbyte *)result, 0);
    (*g_env)->ReleaseIntArrayElements(g_env, jerror, (jint *)errorResult, 0);

    if ( detached == 1 ) {
        (*g_vm)->DetachCurrentThread( g_vm );
    }

    return returnResult;
}

JNIEXPORT jint JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_startTest(JNIEnv * env, jclass jcls, jint num) {
    return returnNumWithSumOfFive((int) num);
}

JNIEXPORT void JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_receivedBytesToSign(JNIEnv * env, jclass jcls, jbyteArray num) {
    unsigned char * bytes = (unsigned char *) (*env)->GetByteArrayElements(env, num, 0);
    size_t length = (*env)->GetArrayLength(env, num);
    handleBytesToSign(bytes, (long) length);
    (*env)->ReleaseByteArrayElements(env, num, (jbyte *)bytes, 0);
}

/**
 * This method provides the JVM context and sets up references to the java methods to call.
 * When referencing method signatures, use the following syntax:
 * B = byte
 * C = char
 * D = double
 * F = float
 * I = int
 * J = long
 * S = short
 * V = void
 * Z = boolean
 * Lfully-qualified-class = fully qualified class
 * [type = array of type
 * (argument types)return type = method type. If no arguments, use empty argument types: ().
 * If return type is void (or constructor) use (argument types)V.
 */
JNIEXPORT jboolean JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_register(JNIEnv * env, jclass jcls) {
    g_callbackRegistered = 0;
    //Save the JVM reference.
    (*env)->GetJavaVM(env, &g_vm);
    if(!g_vm){
        LOGD("Native", "Failed to retrieve JavaVM\n");
        return (jboolean) 0;
    }

    //Class responsible for holding the method ids to save references.
    g_cls = (jclass)(*env)->NewGlobalRef(env, jcls);

    //Setup method references here.
    g_needToAddFive = (*env)->GetStaticMethodID(env, g_cls, "needToAddFive", "(I)I");
    g_signBytes = (*env)->GetStaticMethodID(env, g_cls, "signBytes", "([B[I)[B");

    if (!g_needToAddFive) {
        LOGD("Native", "No such method!\n");
        return (jboolean) 0;
    }

    if (!g_signBytes) {
        LOGD("Native", "No such method!\n");
        return (jboolean) 0;
    }

    LOGD("Native", "Registration successful!\n");
    g_callbackRegistered = 1;
    return (jboolean)1;
}
