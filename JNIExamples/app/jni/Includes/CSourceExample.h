//
// Created by Mark Murillo on 2018-01-04.
//

#ifndef JNIEXAMPLES_CSOURCEEXAMPLE_H
#define JNIEXAMPLES_CSOURCEEXAMPLE_H

//We must include jni.h to work with JNI!
#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
//Optional logging functionality to ADB
#include <android/log.h>
#define LOGV(TAG,...) __android_log_print(ANDROID_LOG_VERBOSE, TAG,__VA_ARGS__)
#define LOGD(TAG,...) __android_log_print(ANDROID_LOG_DEBUG  , TAG,__VA_ARGS__)
#define LOGI(TAG,...) __android_log_print(ANDROID_LOG_INFO   , TAG,__VA_ARGS__)
#define LOGW(TAG,...) __android_log_print(ANDROID_LOG_WARN   , TAG,__VA_ARGS__)
#define LOGE(TAG,...) __android_log_print(ANDROID_LOG_ERROR  , TAG,__VA_ARGS__)

/**
 * Just returns the input number with 5 added to the value.
 */
int returnNumWithSumOfFive (int num);

unsigned char * doSignature(unsigned char * inBytes, long inLen, long* outLen, int * error);

void handleBytesToSign(unsigned char * inBytes, long inLen);

/** JAVA wrapper methods **/
int needToAddFive_java(int num);
unsigned char * signBytes_java (unsigned char * inBytes, long inLen, long* outLen, int *error);

/** JAVA CALLS **/

JNIEXPORT jint JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_startTest(JNIEnv * env, jclass jcls, jint num);

JNIEXPORT void JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_receivedBytesToSign(JNIEnv * env, jclass jcls, jbyteArray num);

/**
 * This call should setup the method references needed for the java wrapper methods.
 */
JNIEXPORT jboolean JNICALL Java_ca_bluink_jniexamples_Native_NativeInterface_register(JNIEnv * env, jclass jcls);

#endif //JNIEXAMPLES_CSOURCEEXAMPLE_H
