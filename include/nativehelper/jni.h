/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * JNI specification, as defined by Sun:
 * http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/jniTOC.html
 *
 * Everything here is expected to be VM-neutral.
 */

#ifndef JNI_H_
#define JNI_H_

#include <stdarg.h>
#include <stdint.h>

/* Primitive types that match up with Java equivalents. */
typedef uint8_t  jboolean; /* unsigned 8 bits */
typedef int8_t   jbyte;    /* signed 8 bits */
typedef uint16_t jchar;    /* unsigned 16 bits */
typedef int16_t  jshort;   /* signed 16 bits */
typedef int32_t  jint;     /* signed 32 bits */
typedef int64_t  jlong;    /* signed 64 bits */
typedef float    jfloat;   /* 32-bit IEEE 754 */
typedef double   jdouble;  /* 64-bit IEEE 754 */

typedef uint32_t u4;

/* "cardinal indices and sizes" */
typedef jint     jsize;

#ifdef __cplusplus
/*
 * Reference types, in C++
 */
class _jobject {};
class _jclass : public _jobject {};
class _jstring : public _jobject {};
class _jarray : public _jobject {};
class _jobjectArray : public _jarray {};
class _jbooleanArray : public _jarray {};
class _jbyteArray : public _jarray {};
class _jcharArray : public _jarray {};
class _jshortArray : public _jarray {};
class _jintArray : public _jarray {};
class _jlongArray : public _jarray {};
class _jfloatArray : public _jarray {};
class _jdoubleArray : public _jarray {};
class _jthrowable : public _jobject {};

typedef _jobject*       jobject;
typedef _jclass*        jclass;
typedef _jstring*       jstring;
typedef _jarray*        jarray;
typedef _jobjectArray*  jobjectArray;
typedef _jbooleanArray* jbooleanArray;
typedef _jbyteArray*    jbyteArray;
typedef _jcharArray*    jcharArray;
typedef _jshortArray*   jshortArray;
typedef _jintArray*     jintArray;
typedef _jlongArray*    jlongArray;
typedef _jfloatArray*   jfloatArray;
typedef _jdoubleArray*  jdoubleArray;
typedef _jthrowable*    jthrowable;
typedef _jobject*       jweak;


#else /* not __cplusplus */

/*
 * Reference types, in C.
 */
typedef void*           jobject;
typedef jobject         jclass;
typedef jobject         jstring;
typedef jobject         jarray;
typedef jarray          jobjectArray;
typedef jarray          jbooleanArray;
typedef jarray          jbyteArray;
typedef jarray          jcharArray;
typedef jarray          jshortArray;
typedef jarray          jintArray;
typedef jarray          jlongArray;
typedef jarray          jfloatArray;
typedef jarray          jdoubleArray;
typedef jobject         jthrowable;
typedef jobject         jweak;

#endif /* not __cplusplus */

struct _jfieldID;                       /* opaque structure */
typedef struct _jfieldID* jfieldID;     /* field IDs */

struct _jmethodID;                      /* opaque structure */
typedef struct _jmethodID* jmethodID;   /* method IDs */

struct JNIInvokeInterface;

typedef union jvalue {
    jboolean    z;
    jbyte       b;
    jchar       c;
    jshort      s;
    jint        i;
    jlong       j;
    jfloat      f;
    jdouble     d;
    jobject     l;
} jvalue;

typedef enum jobjectRefType {
    JNIInvalidRefType = 0,
    JNILocalRefType = 1,
    JNIGlobalRefType = 2,
    JNIWeakGlobalRefType = 3
} jobjectRefType;

typedef struct {
    const char* name;
    const char* signature;
    void*       fnPtr;
} JNINativeMethod;

struct _JNIEnv;
struct _JavaVM;
typedef const struct JNINativeInterface* C_JNIEnv;

#if defined(__cplusplus)
typedef _JNIEnv JNIEnv;
typedef _JavaVM JavaVM;
#else
typedef const struct JNINativeInterface* JNIEnv;
typedef const struct JNIInvokeInterface* JavaVM;
#endif

/*
 * Table of interface function pointers.
 */
struct JNINativeInterface {
    void*       reserved0;
    void*       reserved1;
    void*       reserved2;
    void*       reserved3;

    jint        (*GetVersion)(JNIEnv *);

    jclass      (*DefineClass)(JNIEnv*, const char*, jobject, const jbyte*,
                        jsize);
    jclass      (*FindClass)(JNIEnv*, const char*);

    jmethodID   (*FromReflectedMethod)(JNIEnv*, jobject);
    jfieldID    (*FromReflectedField)(JNIEnv*, jobject);
    /* spec doesn't show jboolean parameter */
    jobject     (*ToReflectedMethod)(JNIEnv*, jclass, jmethodID, jboolean);

    jclass      (*GetSuperclass)(JNIEnv*, jclass);
    jboolean    (*IsAssignableFrom)(JNIEnv*, jclass, jclass);

    /* spec doesn't show jboolean parameter */
    jobject     (*ToReflectedField)(JNIEnv*, jclass, jfieldID, jboolean);

    jint        (*Throw)(JNIEnv*, jthrowable);
    jint        (*ThrowNew)(JNIEnv *, jclass, const char *);
    jthrowable  (*ExceptionOccurred)(JNIEnv*);
    void        (*ExceptionDescribe)(JNIEnv*);
    void        (*ExceptionClear)(JNIEnv*);
    void        (*FatalError)(JNIEnv*, const char*);

    jint        (*PushLocalFrame)(JNIEnv*, jint);
    jobject     (*PopLocalFrame)(JNIEnv*, jobject);

    jobject     (*NewGlobalRef)(JNIEnv*, jobject);
    void        (*DeleteGlobalRef)(JNIEnv*, jobject);
    void        (*DeleteLocalRef)(JNIEnv*, jobject);
    jboolean    (*IsSameObject)(JNIEnv*, jobject, jobject);

    jobject     (*NewLocalRef)(JNIEnv*, jobject);
    jint        (*EnsureLocalCapacity)(JNIEnv*, jint);

    jobject     (*AllocObject)(JNIEnv*, jclass);
    jobject     (*NewObject)(JNIEnv*, jclass, jmethodID, ...);
    jobject     (*NewObjectV)(JNIEnv*, jclass, jmethodID, va_list);
    jobject     (*NewObjectA)(JNIEnv*, jclass, jmethodID, jvalue*);

    jclass      (*GetObjectClass)(JNIEnv*, jobject);
    jboolean    (*IsInstanceOf)(JNIEnv*, jobject, jclass);
    jmethodID   (*GetMethodID)(JNIEnv*, jclass, const char*, const char*);

    jobject     (*CallObjectMethod)(JNIEnv*, jobject, jmethodID, ...);
    jobject     (*CallObjectMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jobject     (*CallObjectMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jboolean    (*CallBooleanMethod)(JNIEnv*, jobject, jmethodID, ...);
    jboolean    (*CallBooleanMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jboolean    (*CallBooleanMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jbyte       (*CallByteMethod)(JNIEnv*, jobject, jmethodID, ...);
    jbyte       (*CallByteMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jbyte       (*CallByteMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jchar       (*CallCharMethod)(JNIEnv*, jobject, jmethodID, ...);
    jchar       (*CallCharMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jchar       (*CallCharMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jshort      (*CallShortMethod)(JNIEnv*, jobject, jmethodID, ...);
    jshort      (*CallShortMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jshort      (*CallShortMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jint        (*CallIntMethod)(JNIEnv*, jobject, jmethodID, ...);
    jint        (*CallIntMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jint        (*CallIntMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jlong       (*CallLongMethod)(JNIEnv*, jobject, jmethodID, ...);
    jlong       (*CallLongMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jlong       (*CallLongMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jfloat      (*CallFloatMethod)(JNIEnv*, jobject, jmethodID, ...);
    jfloat      (*CallFloatMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jfloat      (*CallFloatMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    jdouble     (*CallDoubleMethod)(JNIEnv*, jobject, jmethodID, ...);
    jdouble     (*CallDoubleMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    jdouble     (*CallDoubleMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);
    void        (*CallVoidMethod)(JNIEnv*, jobject, jmethodID, ...);
    void        (*CallVoidMethodV)(JNIEnv*, jobject, jmethodID, va_list);
    void        (*CallVoidMethodA)(JNIEnv*, jobject, jmethodID, jvalue*);

    jobject     (*CallNonvirtualObjectMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jobject     (*CallNonvirtualObjectMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jobject     (*CallNonvirtualObjectMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jboolean    (*CallNonvirtualBooleanMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jboolean    (*CallNonvirtualBooleanMethodV)(JNIEnv*, jobject, jclass,
                         jmethodID, va_list);
    jboolean    (*CallNonvirtualBooleanMethodA)(JNIEnv*, jobject, jclass,
                         jmethodID, jvalue*);
    jbyte       (*CallNonvirtualByteMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jbyte       (*CallNonvirtualByteMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jbyte       (*CallNonvirtualByteMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jchar       (*CallNonvirtualCharMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jchar       (*CallNonvirtualCharMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jchar       (*CallNonvirtualCharMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jshort      (*CallNonvirtualShortMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jshort      (*CallNonvirtualShortMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jshort      (*CallNonvirtualShortMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jint        (*CallNonvirtualIntMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jint        (*CallNonvirtualIntMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jint        (*CallNonvirtualIntMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jlong       (*CallNonvirtualLongMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jlong       (*CallNonvirtualLongMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jlong       (*CallNonvirtualLongMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jfloat      (*CallNonvirtualFloatMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jfloat      (*CallNonvirtualFloatMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jfloat      (*CallNonvirtualFloatMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    jdouble     (*CallNonvirtualDoubleMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    jdouble     (*CallNonvirtualDoubleMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    jdouble     (*CallNonvirtualDoubleMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);
    void        (*CallNonvirtualVoidMethod)(JNIEnv*, jobject, jclass,
                        jmethodID, ...);
    void        (*CallNonvirtualVoidMethodV)(JNIEnv*, jobject, jclass,
                        jmethodID, va_list);
    void        (*CallNonvirtualVoidMethodA)(JNIEnv*, jobject, jclass,
                        jmethodID, jvalue*);

    jfieldID    (*GetFieldID)(JNIEnv*, jclass, const char*, const char*);

    jobject     (*GetObjectField)(JNIEnv*, jobject, jfieldID);
    jboolean    (*GetBooleanField)(JNIEnv*, jobject, jfieldID);
    jbyte       (*GetByteField)(JNIEnv*, jobject, jfieldID);
    jchar       (*GetCharField)(JNIEnv*, jobject, jfieldID);
    jshort      (*GetShortField)(JNIEnv*, jobject, jfieldID);
    jint        (*GetIntField)(JNIEnv*, jobject, jfieldID);
    jlong       (*GetLongField)(JNIEnv*, jobject, jfieldID);
    jfloat      (*GetFloatField)(JNIEnv*, jobject, jfieldID);
    jdouble     (*GetDoubleField)(JNIEnv*, jobject, jfieldID);

    void        (*SetObjectField)(JNIEnv*, jobject, jfieldID, jobject);
    void        (*SetBooleanField)(JNIEnv*, jobject, jfieldID, jboolean);
    void        (*SetByteField)(JNIEnv*, jobject, jfieldID, jbyte);
    void        (*SetCharField)(JNIEnv*, jobject, jfieldID, jchar);
    void        (*SetShortField)(JNIEnv*, jobject, jfieldID, jshort);
    void        (*SetIntField)(JNIEnv*, jobject, jfieldID, jint);
    void        (*SetLongField)(JNIEnv*, jobject, jfieldID, jlong);
    void        (*SetFloatField)(JNIEnv*, jobject, jfieldID, jfloat);
    void        (*SetDoubleField)(JNIEnv*, jobject, jfieldID, jdouble);

    jmethodID   (*GetStaticMethodID)(JNIEnv*, jclass, const char*, const char*);

    jobject     (*CallStaticObjectMethod)(JNIEnv*, jclass, jmethodID, ...);
    jobject     (*CallStaticObjectMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jobject     (*CallStaticObjectMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jboolean    (*CallStaticBooleanMethod)(JNIEnv*, jclass, jmethodID, ...);
    jboolean    (*CallStaticBooleanMethodV)(JNIEnv*, jclass, jmethodID,
                        va_list);
    jboolean    (*CallStaticBooleanMethodA)(JNIEnv*, jclass, jmethodID,
                        jvalue*);
    jbyte       (*CallStaticByteMethod)(JNIEnv*, jclass, jmethodID, ...);
    jbyte       (*CallStaticByteMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jbyte       (*CallStaticByteMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jchar       (*CallStaticCharMethod)(JNIEnv*, jclass, jmethodID, ...);
    jchar       (*CallStaticCharMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jchar       (*CallStaticCharMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jshort      (*CallStaticShortMethod)(JNIEnv*, jclass, jmethodID, ...);
    jshort      (*CallStaticShortMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jshort      (*CallStaticShortMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jint        (*CallStaticIntMethod)(JNIEnv*, jclass, jmethodID, ...);
    jint        (*CallStaticIntMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jint        (*CallStaticIntMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jlong       (*CallStaticLongMethod)(JNIEnv*, jclass, jmethodID, ...);
    jlong       (*CallStaticLongMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jlong       (*CallStaticLongMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jfloat      (*CallStaticFloatMethod)(JNIEnv*, jclass, jmethodID, ...);
    jfloat      (*CallStaticFloatMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jfloat      (*CallStaticFloatMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    jdouble     (*CallStaticDoubleMethod)(JNIEnv*, jclass, jmethodID, ...);
    jdouble     (*CallStaticDoubleMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    jdouble     (*CallStaticDoubleMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);
    void        (*CallStaticVoidMethod)(JNIEnv*, jclass, jmethodID, ...);
    void        (*CallStaticVoidMethodV)(JNIEnv*, jclass, jmethodID, va_list);
    void        (*CallStaticVoidMethodA)(JNIEnv*, jclass, jmethodID, jvalue*);

    jfieldID    (*GetStaticFieldID)(JNIEnv*, jclass, const char*,
                        const char*);

    jobject     (*GetStaticObjectField)(JNIEnv*, jclass, jfieldID);
    jboolean    (*GetStaticBooleanField)(JNIEnv*, jclass, jfieldID);
    jbyte       (*GetStaticByteField)(JNIEnv*, jclass, jfieldID);
    jchar       (*GetStaticCharField)(JNIEnv*, jclass, jfieldID);
    jshort      (*GetStaticShortField)(JNIEnv*, jclass, jfieldID);
    jint        (*GetStaticIntField)(JNIEnv*, jclass, jfieldID);
    jlong       (*GetStaticLongField)(JNIEnv*, jclass, jfieldID);
    jfloat      (*GetStaticFloatField)(JNIEnv*, jclass, jfieldID);
    jdouble     (*GetStaticDoubleField)(JNIEnv*, jclass, jfieldID);

    void        (*SetStaticObjectField)(JNIEnv*, jclass, jfieldID, jobject);
    void        (*SetStaticBooleanField)(JNIEnv*, jclass, jfieldID, jboolean);
    void        (*SetStaticByteField)(JNIEnv*, jclass, jfieldID, jbyte);
    void        (*SetStaticCharField)(JNIEnv*, jclass, jfieldID, jchar);
    void        (*SetStaticShortField)(JNIEnv*, jclass, jfieldID, jshort);
    void        (*SetStaticIntField)(JNIEnv*, jclass, jfieldID, jint);
    void        (*SetStaticLongField)(JNIEnv*, jclass, jfieldID, jlong);
    void        (*SetStaticFloatField)(JNIEnv*, jclass, jfieldID, jfloat);
    void        (*SetStaticDoubleField)(JNIEnv*, jclass, jfieldID, jdouble);

    jstring     (*NewString)(JNIEnv*, const jchar*, jsize);
    jsize       (*GetStringLength)(JNIEnv*, jstring);
    const jchar* (*GetStringChars)(JNIEnv*, jstring, jboolean*);
    void        (*ReleaseStringChars)(JNIEnv*, jstring, const jchar*);
    jstring     (*NewStringUTF)(JNIEnv*, const char*);
    jsize       (*GetStringUTFLength)(JNIEnv*, jstring);
    /* JNI spec says this returns const jbyte*, but that's inconsistent */
    const char* (*GetStringUTFChars)(JNIEnv*, jstring, jboolean*);
    void        (*ReleaseStringUTFChars)(JNIEnv*, jstring, const char*);
    jsize       (*GetArrayLength)(JNIEnv*, jarray);
    jobjectArray (*NewObjectArray)(JNIEnv*, jsize, jclass, jobject);
    jobject     (*GetObjectArrayElement)(JNIEnv*, jobjectArray, jsize);
    void        (*SetObjectArrayElement)(JNIEnv*, jobjectArray, jsize, jobject);

    jbooleanArray (*NewBooleanArray)(JNIEnv*, jsize);
    jbyteArray    (*NewByteArray)(JNIEnv*, jsize);
    jcharArray    (*NewCharArray)(JNIEnv*, jsize);
    jshortArray   (*NewShortArray)(JNIEnv*, jsize);
    jintArray     (*NewIntArray)(JNIEnv*, jsize);
    jlongArray    (*NewLongArray)(JNIEnv*, jsize);
    jfloatArray   (*NewFloatArray)(JNIEnv*, jsize);
    jdoubleArray  (*NewDoubleArray)(JNIEnv*, jsize);

    jboolean*   (*GetBooleanArrayElements)(JNIEnv*, jbooleanArray, jboolean*);
    jbyte*      (*GetByteArrayElements)(JNIEnv*, jbyteArray, jboolean*);
    jchar*      (*GetCharArrayElements)(JNIEnv*, jcharArray, jboolean*);
    jshort*     (*GetShortArrayElements)(JNIEnv*, jshortArray, jboolean*);
    jint*       (*GetIntArrayElements)(JNIEnv*, jintArray, jboolean*);
    jlong*      (*GetLongArrayElements)(JNIEnv*, jlongArray, jboolean*);
    jfloat*     (*GetFloatArrayElements)(JNIEnv*, jfloatArray, jboolean*);
    jdouble*    (*GetDoubleArrayElements)(JNIEnv*, jdoubleArray, jboolean*);

    void        (*ReleaseBooleanArrayElements)(JNIEnv*, jbooleanArray,
                        jboolean*, jint);
    void        (*ReleaseByteArrayElements)(JNIEnv*, jbyteArray,
                        jbyte*, jint);
    void        (*ReleaseCharArrayElements)(JNIEnv*, jcharArray,
                        jchar*, jint);
    void        (*ReleaseShortArrayElements)(JNIEnv*, jshortArray,
                        jshort*, jint);
    void        (*ReleaseIntArrayElements)(JNIEnv*, jintArray,
                        jint*, jint);
    void        (*ReleaseLongArrayElements)(JNIEnv*, jlongArray,
                        jlong*, jint);
    void        (*ReleaseFloatArrayElements)(JNIEnv*, jfloatArray,
                        jfloat*, jint);
    void        (*ReleaseDoubleArrayElements)(JNIEnv*, jdoubleArray,
                        jdouble*, jint);

    void        (*GetBooleanArrayRegion)(JNIEnv*, jbooleanArray,
                        jsize, jsize, jboolean*);
    void        (*GetByteArrayRegion)(JNIEnv*, jbyteArray,
                        jsize, jsize, jbyte*);
    void        (*GetCharArrayRegion)(JNIEnv*, jcharArray,
                        jsize, jsize, jchar*);
    void        (*GetShortArrayRegion)(JNIEnv*, jshortArray,
                        jsize, jsize, jshort*);
    void        (*GetIntArrayRegion)(JNIEnv*, jintArray,
                        jsize, jsize, jint*);
    void        (*GetLongArrayRegion)(JNIEnv*, jlongArray,
                        jsize, jsize, jlong*);
    void        (*GetFloatArrayRegion)(JNIEnv*, jfloatArray,
                        jsize, jsize, jfloat*);
    void        (*GetDoubleArrayRegion)(JNIEnv*, jdoubleArray,
                        jsize, jsize, jdouble*);

    /* spec shows these without const; some jni.h do, some don't */
    void        (*SetBooleanArrayRegion)(JNIEnv*, jbooleanArray,
                        jsize, jsize, const jboolean*);
    void        (*SetByteArrayRegion)(JNIEnv*, jbyteArray,
                        jsize, jsize, const jbyte*);
    void        (*SetCharArrayRegion)(JNIEnv*, jcharArray,
                        jsize, jsize, const jchar*);
    void        (*SetShortArrayRegion)(JNIEnv*, jshortArray,
                        jsize, jsize, const jshort*);
    void        (*SetIntArrayRegion)(JNIEnv*, jintArray,
                        jsize, jsize, const jint*);
    void        (*SetLongArrayRegion)(JNIEnv*, jlongArray,
                        jsize, jsize, const jlong*);
    void        (*SetFloatArrayRegion)(JNIEnv*, jfloatArray,
                        jsize, jsize, const jfloat*);
    void        (*SetDoubleArrayRegion)(JNIEnv*, jdoubleArray,
                        jsize, jsize, const jdouble*);

    jint        (*RegisterNatives)(JNIEnv*, jclass, const JNINativeMethod*,
                        jint);
    jint        (*UnregisterNatives)(JNIEnv*, jclass);
    jint        (*MonitorEnter)(JNIEnv*, jobject);
    jint        (*MonitorExit)(JNIEnv*, jobject);
    jint        (*GetJavaVM)(JNIEnv*, JavaVM**);

    void        (*GetStringRegion)(JNIEnv*, jstring, jsize, jsize, jchar*);
    void        (*GetStringUTFRegion)(JNIEnv*, jstring, jsize, jsize, char*);

    void*       (*GetPrimitiveArrayCritical)(JNIEnv*, jarray, jboolean*);
    void        (*ReleasePrimitiveArrayCritical)(JNIEnv*, jarray, void*, jint);

    const jchar* (*GetStringCritical)(JNIEnv*, jstring, jboolean*);
    void        (*ReleaseStringCritical)(JNIEnv*, jstring, const jchar*);

    jweak       (*NewWeakGlobalRef)(JNIEnv*, jobject);
    void        (*DeleteWeakGlobalRef)(JNIEnv*, jweak);

    jboolean    (*ExceptionCheck)(JNIEnv*);

    jobject     (*NewDirectByteBuffer)(JNIEnv*, void*, jlong);
    void*       (*GetDirectBufferAddress)(JNIEnv*, jobject);
    jlong       (*GetDirectBufferCapacity)(JNIEnv*, jobject);

    /* added in JNI 1.6 */
    jobjectRefType (*GetObjectRefType)(JNIEnv*, jobject);

	const char*	(*GetArrayType)(JNIEnv*, jarray jarr);
  
    jobject     (*NewTaintedObjectA)(JNIEnv*, jclass, jmethodID, u4, jvalue*, u4*);

    jobject     (*CallObjectTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jboolean    (*CallBooleanTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jbyte       (*CallByteTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jchar       (*CallCharTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jshort      (*CallShortTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jint        (*CallIntTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jlong       (*CallLongTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jfloat      (*CallFloatTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    jdouble     (*CallDoubleTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);
    void        (*CallVoidTaintedMethodA)(JNIEnv*, jobject, u4, jmethodID, u4*, jvalue*, u4*);

    jobject     (*CallNonvirtualObjectTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jboolean    (*CallNonvirtualBooleanTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jbyte       (*CallNonvirtualByteTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jchar       (*CallNonvirtualCharTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jshort      (*CallNonvirtualShortTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jint        (*CallNonvirtualIntTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jlong       (*CallNonvirtualLongTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jfloat      (*CallNonvirtualFloatTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    jdouble     (*CallNonvirtualDoubleTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);
    void        (*CallNonvirtualVoidTaintedMethodA)(JNIEnv*, jobject, u4, jclass, jmethodID, u4*, jvalue*, u4*);

    jobject     (*GetObjectTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jboolean    (*GetBooleanTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jbyte       (*GetByteTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jchar       (*GetCharTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jshort      (*GetShortTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jint        (*GetIntTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jlong       (*GetLongTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jfloat      (*GetFloatTaintedField)(JNIEnv*, jobject, jfieldID, u4*);
    jdouble     (*GetDoubleTaintedField)(JNIEnv*, jobject, jfieldID, u4*);

    void        (*SetObjectTaintedField)(JNIEnv*, jobject, jfieldID, jobject, u4);
    void        (*SetBooleanTaintedField)(JNIEnv*, jobject, jfieldID, jboolean, u4);
    void        (*SetByteTaintedField)(JNIEnv*, jobject, jfieldID, jbyte, u4);
    void        (*SetCharTaintedField)(JNIEnv*, jobject, jfieldID, jchar, u4);
    void        (*SetShortTaintedField)(JNIEnv*, jobject, jfieldID, jshort, u4);
    void        (*SetIntTaintedField)(JNIEnv*, jobject, jfieldID, jint, u4);
    void        (*SetLongTaintedField)(JNIEnv*, jobject, jfieldID, jlong, u4);
    void        (*SetFloatTaintedField)(JNIEnv*, jobject, jfieldID, jfloat, u4);
    void        (*SetDoubleTaintedField)(JNIEnv*, jobject, jfieldID, jdouble, u4);

    jobject     (*CallStaticObjectTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jboolean    (*CallStaticBooleanTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jbyte       (*CallStaticByteTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jchar       (*CallStaticCharTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jshort      (*CallStaticShortTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jint        (*CallStaticIntTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jlong       (*CallStaticLongTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jfloat      (*CallStaticFloatTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    jdouble     (*CallStaticDoubleTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);
    void        (*CallStaticVoidTaintedMethodA)(JNIEnv*, jclass, jmethodID, u4*, jvalue*, u4*);

    jobject     (*GetStaticObjectTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jboolean    (*GetStaticBooleanTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jbyte       (*GetStaticByteTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jchar       (*GetStaticCharTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jshort      (*GetStaticShortTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jint        (*GetStaticIntTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jlong       (*GetStaticLongTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jfloat      (*GetStaticFloatTaintedField)(JNIEnv*, jclass, jfieldID, u4*);
    jdouble     (*GetStaticDoubleTaintedField)(JNIEnv*, jclass, jfieldID, u4*);

    void        (*SetStaticObjectTaintedField)(JNIEnv*, jclass, jfieldID, jobject, u4);
    void        (*SetStaticBooleanTaintedField)(JNIEnv*, jclass, jfieldID, jboolean, u4);
    void        (*SetStaticByteTaintedField)(JNIEnv*, jclass, jfieldID, jbyte, u4);
    void        (*SetStaticCharTaintedField)(JNIEnv*, jclass, jfieldID, jchar, u4);
    void        (*SetStaticShortTaintedField)(JNIEnv*, jclass, jfieldID, jshort, u4);
    void        (*SetStaticIntTaintedField)(JNIEnv*, jclass, jfieldID, jint, u4);
    void        (*SetStaticLongTaintedField)(JNIEnv*, jclass, jfieldID, jlong, u4);
    void        (*SetStaticFloatTaintedField)(JNIEnv*, jclass, jfieldID, jfloat, u4);
    void        (*SetStaticDoubleTaintedField)(JNIEnv*, jclass, jfieldID, jdouble, u4);

    jsize       (*GetTaintedStringLength)(JNIEnv*, jstring, u4*);
    const jchar* (*GetTaintedStringChars)(JNIEnv*, jstring, jboolean*, u4*);
    void        (*ReleaseTaintedStringChars)(JNIEnv*, jstring, u4, const jchar*);

    jstring     (*NewTaintedStringUTF)(JNIEnv*, const char*, u4);
    const char* (*GetTaintedStringUTFChars)(JNIEnv*, jstring, jboolean*, u4*);
    void        (*ReleaseTaintedStringUTFChars)(JNIEnv*, jstring, u4, const char*);
    jobjectArray (*NewTaintedObjectArray)(JNIEnv*, jsize, jclass, jobject, u4);

    jobject     (*GetTaintedObjectArrayElement)(JNIEnv*, jobjectArray, jsize, u4*);
    void        (*SetTaintedObjectArrayElement)(JNIEnv*, jobjectArray, jsize, jobject, u4);

    jboolean*   (*GetTaintedBooleanArrayElements)(JNIEnv*, jbooleanArray, jboolean*, u4*);
    jbyte*      (*GetTaintedByteArrayElements)(JNIEnv*, jbyteArray, jboolean*, u4*);
    jchar*      (*GetTaintedCharArrayElements)(JNIEnv*, jcharArray, jboolean*, u4*);
    jshort*     (*GetTaintedShortArrayElements)(JNIEnv*, jshortArray, jboolean*, u4*);
    jint*       (*GetTaintedIntArrayElements)(JNIEnv*, jintArray, jboolean*, u4*);
    jlong*      (*GetTaintedLongArrayElements)(JNIEnv*, jlongArray, jboolean*, u4*);
    jfloat*     (*GetTaintedFloatArrayElements)(JNIEnv*, jfloatArray, jboolean*, u4*);
    jdouble*    (*GetTaintedDoubleArrayElements)(JNIEnv*, jdoubleArray, jboolean*, u4*);

    void        (*ReleaseTaintedBooleanArrayElements)(JNIEnv*, jbooleanArray, jboolean*, jint, u4);
    void        (*ReleaseTaintedByteArrayElements)(JNIEnv*, jbyteArray, jbyte*, jint, u4);
    void        (*ReleaseTaintedCharArrayElements)(JNIEnv*, jcharArray, jchar*, jint, u4);
    void        (*ReleaseTaintedShortArrayElements)(JNIEnv*, jshortArray, jshort*, jint, u4);
    void        (*ReleaseTaintedIntArrayElements)(JNIEnv*, jintArray, jint*, jint, u4);
    void        (*ReleaseTaintedLongArrayElements)(JNIEnv*, jlongArray, jlong*, jint, u4);
    void        (*ReleaseTaintedFloatArrayElements)(JNIEnv*, jfloatArray, jfloat*, jint, u4);
    void        (*ReleaseTaintedDoubleArrayElements)(JNIEnv*, jdoubleArray, jdouble*, jint, u4);

    void        (*GetTaintedBooleanArrayRegion)(JNIEnv*, jbooleanArray, jsize, jsize, jboolean*, u4*);
    void        (*GetTaintedByteArrayRegion)(JNIEnv*, jbyteArray, jsize, jsize, jbyte*, u4*);
    void        (*GetTaintedCharArrayRegion)(JNIEnv*, jcharArray, jsize, jsize, jchar*, u4*);
    void        (*GetTaintedShortArrayRegion)(JNIEnv*, jshortArray, jsize, jsize, jshort*, u4*);
    void        (*GetTaintedIntArrayRegion)(JNIEnv*, jintArray, jsize, jsize, jint*, u4*);
    void        (*GetTaintedLongArrayRegion)(JNIEnv*, jlongArray, jsize, jsize, jlong*, u4*);
    void        (*GetTaintedFloatArrayRegion)(JNIEnv*, jfloatArray, jsize, jsize, jfloat*, u4*);
    void        (*GetTaintedDoubleArrayRegion)(JNIEnv*, jdoubleArray, jsize, jsize, jdouble*, u4*);

    void        (*SetTaintedBooleanArrayRegion)(JNIEnv*, jbooleanArray, jsize, jsize, const jboolean*, u4);
    void        (*SetTaintedByteArrayRegion)(JNIEnv*, jbyteArray, jsize, jsize, const jbyte*, u4);
    void        (*SetTaintedCharArrayRegion)(JNIEnv*, jcharArray, jsize, jsize, const jchar*, u4);
    void        (*SetTaintedShortArrayRegion)(JNIEnv*, jshortArray, jsize, jsize, const jshort*, u4);
    void        (*SetTaintedIntArrayRegion)(JNIEnv*, jintArray, jsize, jsize, const jint*, u4);
    void        (*SetTaintedLongArrayRegion)(JNIEnv*, jlongArray, jsize, jsize, const jlong*, u4);
    void        (*SetTaintedFloatArrayRegion)(JNIEnv*, jfloatArray, jsize, jsize, const jfloat*, u4);
    void        (*SetTaintedDoubleArrayRegion)(JNIEnv*, jdoubleArray, jsize, jsize, const jdouble*, u4);

    void        (*GetTaintedStringRegion)(JNIEnv*, jstring, jsize, jsize, jchar*, u4*);
    void        (*GetTaintedStringUTFRegion)(JNIEnv*, jstring, jsize, jsize, char*, u4*);

    void*       (*GetTaintedPrimitiveArrayCritical)(JNIEnv*, jarray, u4*, jboolean*);
    void        (*ReleaseTaintedPrimitiveArrayCritical)(JNIEnv*, jarray, void*, u4, jint);

    const jchar* (*GetTaintedStringCritical)(JNIEnv*, jstring, u4*, jboolean*);
    void        (*ReleaseTaintedStringCritical)(JNIEnv*, jstring, u4, const jchar*);

    jint        (*RegisterTaintedNatives)(JNIEnv*, jclass, const JNINativeMethod*, jint);

    jint        (*GetUTFCharsByteLength)(JNIEnv* env, jstring jstr);
};

/*
 * C++ object wrapper.
 *
 * This is usually overlaid on a C struct whose first element is a
 * JNINativeInterface*.  We rely somewhat on compiler behavior.
 */
struct _JNIEnv {
    /* do not rename this; it does not seem to be entirely opaque */
    const struct JNINativeInterface* functions;

#if defined(__cplusplus)

    jint GetVersion()
    { return functions->GetVersion(this); }

    jclass DefineClass(const char *name, jobject loader, const jbyte* buf,
        jsize bufLen)
    { return functions->DefineClass(this, name, loader, buf, bufLen); }

    jclass FindClass(const char* name)
    { return functions->FindClass(this, name); }

    jmethodID FromReflectedMethod(jobject method)
    { return functions->FromReflectedMethod(this, method); }

    jfieldID FromReflectedField(jobject field)
    { return functions->FromReflectedField(this, field); }

    jobject ToReflectedMethod(jclass cls, jmethodID methodID, jboolean isStatic)
    { return functions->ToReflectedMethod(this, cls, methodID, isStatic); }

    jclass GetSuperclass(jclass clazz)
    { return functions->GetSuperclass(this, clazz); }

    jboolean IsAssignableFrom(jclass clazz1, jclass clazz2)
    { return functions->IsAssignableFrom(this, clazz1, clazz2); }

    jobject ToReflectedField(jclass cls, jfieldID fieldID, jboolean isStatic)
    { return functions->ToReflectedField(this, cls, fieldID, isStatic); }

    jint Throw(jthrowable obj)
    { return functions->Throw(this, obj); }

    jint ThrowNew(jclass clazz, const char* message)
    { return functions->ThrowNew(this, clazz, message); }

    jthrowable ExceptionOccurred()
    { return functions->ExceptionOccurred(this); }

    void ExceptionDescribe()
    { functions->ExceptionDescribe(this); }

    void ExceptionClear()
    { functions->ExceptionClear(this); }

    void FatalError(const char* msg)
    { functions->FatalError(this, msg); }

    jint PushLocalFrame(jint capacity)
    { return functions->PushLocalFrame(this, capacity); }

    jobject PopLocalFrame(jobject result)
    { return functions->PopLocalFrame(this, result); }

    jobject NewGlobalRef(jobject obj)
    { return functions->NewGlobalRef(this, obj); }

    void DeleteGlobalRef(jobject globalRef)
    { functions->DeleteGlobalRef(this, globalRef); }

    void DeleteLocalRef(jobject localRef)
    { functions->DeleteLocalRef(this, localRef); }

    jboolean IsSameObject(jobject ref1, jobject ref2)
    { return functions->IsSameObject(this, ref1, ref2); }

    jobject NewLocalRef(jobject ref)
    { return functions->NewLocalRef(this, ref); }

    jint EnsureLocalCapacity(jint capacity)
    { return functions->EnsureLocalCapacity(this, capacity); }

    jobject AllocObject(jclass clazz)
    { return functions->AllocObject(this, clazz); }

    jobject NewObject(jclass clazz, jmethodID methodID, ...)
    {
        va_list args;
        va_start(args, methodID);
        jobject result = functions->NewObjectV(this, clazz, methodID, args);
        va_end(args);
        return result;
    }

    jobject NewObjectV(jclass clazz, jmethodID methodID, va_list args)
    { return functions->NewObjectV(this, clazz, methodID, args); }

    jobject NewObjectA(jclass clazz, jmethodID methodID, jvalue* args)
    { return functions->NewObjectA(this, clazz, methodID, args); }

    jclass GetObjectClass(jobject obj)
    { return functions->GetObjectClass(this, obj); }

    jboolean IsInstanceOf(jobject obj, jclass clazz)
    { return functions->IsInstanceOf(this, obj, clazz); }

    jmethodID GetMethodID(jclass clazz, const char* name, const char* sig)
    { return functions->GetMethodID(this, clazz, name, sig); }

#define CALL_TYPE_METHOD(_jtype, _jname)                                    \
    _jtype Call##_jname##Method(jobject obj, jmethodID methodID, ...)       \
    {                                                                       \
        _jtype result;                                                      \
        va_list args;                                                       \
        va_start(args, methodID);                                           \
        result = functions->Call##_jname##MethodV(this, obj, methodID,      \
                    args);                                                  \
        va_end(args);                                                       \
        return result;                                                      \
    }
#define CALL_TYPE_METHODV(_jtype, _jname)                                   \
    _jtype Call##_jname##MethodV(jobject obj, jmethodID methodID,           \
        va_list args)                                                       \
    { return functions->Call##_jname##MethodV(this, obj, methodID, args); }
#define CALL_TYPE_METHODA(_jtype, _jname)                                   \
    _jtype Call##_jname##MethodA(jobject obj, jmethodID methodID,           \
        jvalue* args)                                                       \
    { return functions->Call##_jname##MethodA(this, obj, methodID, args); }

#define CALL_TYPE(_jtype, _jname)                                           \
    CALL_TYPE_METHOD(_jtype, _jname)                                        \
    CALL_TYPE_METHODV(_jtype, _jname)                                       \
    CALL_TYPE_METHODA(_jtype, _jname)

    CALL_TYPE(jobject, Object)
    CALL_TYPE(jboolean, Boolean)
    CALL_TYPE(jbyte, Byte)
    CALL_TYPE(jchar, Char)
    CALL_TYPE(jshort, Short)
    CALL_TYPE(jint, Int)
    CALL_TYPE(jlong, Long)
    CALL_TYPE(jfloat, Float)
    CALL_TYPE(jdouble, Double)

    void CallVoidMethod(jobject obj, jmethodID methodID, ...)
    {
        va_list args;
        va_start(args, methodID);
        functions->CallVoidMethodV(this, obj, methodID, args);
        va_end(args);
    }
    void CallVoidMethodV(jobject obj, jmethodID methodID, va_list args)
    { functions->CallVoidMethodV(this, obj, methodID, args); }
    void CallVoidMethodA(jobject obj, jmethodID methodID, jvalue* args)
    { functions->CallVoidMethodA(this, obj, methodID, args); }

#define CALL_NONVIRT_TYPE_METHOD(_jtype, _jname)                            \
    _jtype CallNonvirtual##_jname##Method(jobject obj, jclass clazz,        \
        jmethodID methodID, ...)                                            \
    {                                                                       \
        _jtype result;                                                      \
        va_list args;                                                       \
        va_start(args, methodID);                                           \
        result = functions->CallNonvirtual##_jname##MethodV(this, obj,      \
                    clazz, methodID, args);                                 \
        va_end(args);                                                       \
        return result;                                                      \
    }
#define CALL_NONVIRT_TYPE_METHODV(_jtype, _jname)                           \
    _jtype CallNonvirtual##_jname##MethodV(jobject obj, jclass clazz,       \
        jmethodID methodID, va_list args)                                   \
    { return functions->CallNonvirtual##_jname##MethodV(this, obj, clazz,   \
        methodID, args); }
#define CALL_NONVIRT_TYPE_METHODA(_jtype, _jname)                           \
    _jtype CallNonvirtual##_jname##MethodA(jobject obj, jclass clazz,       \
        jmethodID methodID, jvalue* args)                                   \
    { return functions->CallNonvirtual##_jname##MethodA(this, obj, clazz,   \
        methodID, args); }

#define CALL_NONVIRT_TYPE(_jtype, _jname)                                   \
    CALL_NONVIRT_TYPE_METHOD(_jtype, _jname)                                \
    CALL_NONVIRT_TYPE_METHODV(_jtype, _jname)                               \
    CALL_NONVIRT_TYPE_METHODA(_jtype, _jname)

    CALL_NONVIRT_TYPE(jobject, Object)
    CALL_NONVIRT_TYPE(jboolean, Boolean)
    CALL_NONVIRT_TYPE(jbyte, Byte)
    CALL_NONVIRT_TYPE(jchar, Char)
    CALL_NONVIRT_TYPE(jshort, Short)
    CALL_NONVIRT_TYPE(jint, Int)
    CALL_NONVIRT_TYPE(jlong, Long)
    CALL_NONVIRT_TYPE(jfloat, Float)
    CALL_NONVIRT_TYPE(jdouble, Double)

    void CallNonvirtualVoidMethod(jobject obj, jclass clazz,
        jmethodID methodID, ...)
    {
        va_list args;
        va_start(args, methodID);
        functions->CallNonvirtualVoidMethodV(this, obj, clazz, methodID, args);
        va_end(args);
    }
    void CallNonvirtualVoidMethodV(jobject obj, jclass clazz,
        jmethodID methodID, va_list args)
    { functions->CallNonvirtualVoidMethodV(this, obj, clazz, methodID, args); }
    void CallNonvirtualVoidMethodA(jobject obj, jclass clazz,
        jmethodID methodID, jvalue* args)
    { functions->CallNonvirtualVoidMethodA(this, obj, clazz, methodID, args); }

    jfieldID GetFieldID(jclass clazz, const char* name, const char* sig)
    { return functions->GetFieldID(this, clazz, name, sig); }

    jobject GetObjectField(jobject obj, jfieldID fieldID)
    { return functions->GetObjectField(this, obj, fieldID); }
    jboolean GetBooleanField(jobject obj, jfieldID fieldID)
    { return functions->GetBooleanField(this, obj, fieldID); }
    jbyte GetByteField(jobject obj, jfieldID fieldID)
    { return functions->GetByteField(this, obj, fieldID); }
    jchar GetCharField(jobject obj, jfieldID fieldID)
    { return functions->GetCharField(this, obj, fieldID); }
    jshort GetShortField(jobject obj, jfieldID fieldID)
    { return functions->GetShortField(this, obj, fieldID); }
    jint GetIntField(jobject obj, jfieldID fieldID)
    { return functions->GetIntField(this, obj, fieldID); }
    jlong GetLongField(jobject obj, jfieldID fieldID)
    { return functions->GetLongField(this, obj, fieldID); }
    jfloat GetFloatField(jobject obj, jfieldID fieldID)
    { return functions->GetFloatField(this, obj, fieldID); }
    jdouble GetDoubleField(jobject obj, jfieldID fieldID)
    { return functions->GetDoubleField(this, obj, fieldID); }

    void SetObjectField(jobject obj, jfieldID fieldID, jobject value)
    { functions->SetObjectField(this, obj, fieldID, value); }
    void SetBooleanField(jobject obj, jfieldID fieldID, jboolean value)
    { functions->SetBooleanField(this, obj, fieldID, value); }
    void SetByteField(jobject obj, jfieldID fieldID, jbyte value)
    { functions->SetByteField(this, obj, fieldID, value); }
    void SetCharField(jobject obj, jfieldID fieldID, jchar value)
    { functions->SetCharField(this, obj, fieldID, value); }
    void SetShortField(jobject obj, jfieldID fieldID, jshort value)
    { functions->SetShortField(this, obj, fieldID, value); }
    void SetIntField(jobject obj, jfieldID fieldID, jint value)
    { functions->SetIntField(this, obj, fieldID, value); }
    void SetLongField(jobject obj, jfieldID fieldID, jlong value)
    { functions->SetLongField(this, obj, fieldID, value); }
    void SetFloatField(jobject obj, jfieldID fieldID, jfloat value)
    { functions->SetFloatField(this, obj, fieldID, value); }
    void SetDoubleField(jobject obj, jfieldID fieldID, jdouble value)
    { functions->SetDoubleField(this, obj, fieldID, value); }

    jmethodID GetStaticMethodID(jclass clazz, const char* name, const char* sig)
    { return functions->GetStaticMethodID(this, clazz, name, sig); }

#define CALL_STATIC_TYPE_METHOD(_jtype, _jname)                             \
    _jtype CallStatic##_jname##Method(jclass clazz, jmethodID methodID,     \
        ...)                                                                \
    {                                                                       \
        _jtype result;                                                      \
        va_list args;                                                       \
        va_start(args, methodID);                                           \
        result = functions->CallStatic##_jname##MethodV(this, clazz,        \
                    methodID, args);                                        \
        va_end(args);                                                       \
        return result;                                                      \
    }
#define CALL_STATIC_TYPE_METHODV(_jtype, _jname)                            \
    _jtype CallStatic##_jname##MethodV(jclass clazz, jmethodID methodID,    \
        va_list args)                                                       \
    { return functions->CallStatic##_jname##MethodV(this, clazz, methodID,  \
        args); }
#define CALL_STATIC_TYPE_METHODA(_jtype, _jname)                            \
    _jtype CallStatic##_jname##MethodA(jclass clazz, jmethodID methodID,    \
        jvalue* args)                                                       \
    { return functions->CallStatic##_jname##MethodA(this, clazz, methodID,  \
        args); }                                                            

#define CALL_STATIC_TYPE(_jtype, _jname)                                    \
    CALL_STATIC_TYPE_METHOD(_jtype, _jname)                                 \
    CALL_STATIC_TYPE_METHODV(_jtype, _jname)                                \
    CALL_STATIC_TYPE_METHODA(_jtype, _jname)

    CALL_STATIC_TYPE(jobject, Object)
    CALL_STATIC_TYPE(jboolean, Boolean)
    CALL_STATIC_TYPE(jbyte, Byte)
    CALL_STATIC_TYPE(jchar, Char)
    CALL_STATIC_TYPE(jshort, Short)
    CALL_STATIC_TYPE(jint, Int)
    CALL_STATIC_TYPE(jlong, Long)
    CALL_STATIC_TYPE(jfloat, Float)
    CALL_STATIC_TYPE(jdouble, Double)

    void CallStaticVoidMethod(jclass clazz, jmethodID methodID, ...)
    {
        va_list args;
        va_start(args, methodID);
        functions->CallStaticVoidMethodV(this, clazz, methodID, args);
        va_end(args);
    }
    void CallStaticVoidMethodV(jclass clazz, jmethodID methodID, va_list args)
    { functions->CallStaticVoidMethodV(this, clazz, methodID, args); }
    void CallStaticVoidMethodA(jclass clazz, jmethodID methodID, jvalue* args)
    { functions->CallStaticVoidMethodA(this, clazz, methodID, args); }

    jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig)
    { return functions->GetStaticFieldID(this, clazz, name, sig); }

    jobject GetStaticObjectField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticObjectField(this, clazz, fieldID); }
    jboolean GetStaticBooleanField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticBooleanField(this, clazz, fieldID); }
    jbyte GetStaticByteField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticByteField(this, clazz, fieldID); }
    jchar GetStaticCharField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticCharField(this, clazz, fieldID); }
    jshort GetStaticShortField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticShortField(this, clazz, fieldID); }
    jint GetStaticIntField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticIntField(this, clazz, fieldID); }
    jlong GetStaticLongField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticLongField(this, clazz, fieldID); }
    jfloat GetStaticFloatField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticFloatField(this, clazz, fieldID); }
    jdouble GetStaticDoubleField(jclass clazz, jfieldID fieldID)
    { return functions->GetStaticDoubleField(this, clazz, fieldID); }

    void SetStaticObjectField(jclass clazz, jfieldID fieldID, jobject value)
    { functions->SetStaticObjectField(this, clazz, fieldID, value); }
    void SetStaticBooleanField(jclass clazz, jfieldID fieldID, jboolean value)
    { functions->SetStaticBooleanField(this, clazz, fieldID, value); }
    void SetStaticByteField(jclass clazz, jfieldID fieldID, jbyte value)
    { functions->SetStaticByteField(this, clazz, fieldID, value); }
    void SetStaticCharField(jclass clazz, jfieldID fieldID, jchar value)
    { functions->SetStaticCharField(this, clazz, fieldID, value); }
    void SetStaticShortField(jclass clazz, jfieldID fieldID, jshort value)
    { functions->SetStaticShortField(this, clazz, fieldID, value); }
    void SetStaticIntField(jclass clazz, jfieldID fieldID, jint value)
    { functions->SetStaticIntField(this, clazz, fieldID, value); }
    void SetStaticLongField(jclass clazz, jfieldID fieldID, jlong value)
    { functions->SetStaticLongField(this, clazz, fieldID, value); }
    void SetStaticFloatField(jclass clazz, jfieldID fieldID, jfloat value)
    { functions->SetStaticFloatField(this, clazz, fieldID, value); }
    void SetStaticDoubleField(jclass clazz, jfieldID fieldID, jdouble value)
    { functions->SetStaticDoubleField(this, clazz, fieldID, value); }

    jstring NewString(const jchar* unicodeChars, jsize len)
    { return functions->NewString(this, unicodeChars, len); }

    jsize GetStringLength(jstring string)
    { return functions->GetStringLength(this, string); }

    const jchar* GetStringChars(jstring string, jboolean* isCopy)
    { return functions->GetStringChars(this, string, isCopy); }
  
    void ReleaseStringChars(jstring string, const jchar* chars)
    { functions->ReleaseStringChars(this, string, chars); }

    jstring NewStringUTF(const char* bytes)
    { return functions->NewStringUTF(this, bytes); }

    jsize GetStringUTFLength(jstring string)
    { return functions->GetStringUTFLength(this, string); }

    const char* GetStringUTFChars(jstring string, jboolean* isCopy)
    { return functions->GetStringUTFChars(this, string, isCopy); }

    void ReleaseStringUTFChars(jstring string, const char* utf)
    { functions->ReleaseStringUTFChars(this, string, utf); }

    jsize GetArrayLength(jarray array)
    { return functions->GetArrayLength(this, array); }

    jobjectArray NewObjectArray(jsize length, jclass elementClass,
                                jobject initialElement)
    { return functions->NewObjectArray(this, length, elementClass,
                                       initialElement); }
  
    jobject GetObjectArrayElement(jobjectArray array, jsize index)
    { return functions->GetObjectArrayElement(this, array, index); }

    void SetObjectArrayElement(jobjectArray array, jsize index, jobject value)
    { functions->SetObjectArrayElement(this, array, index, value); }

    jbooleanArray NewBooleanArray(jsize length)
    { return functions->NewBooleanArray(this, length); }
    jbyteArray NewByteArray(jsize length)
    { return functions->NewByteArray(this, length); }
    jcharArray NewCharArray(jsize length)
    { return functions->NewCharArray(this, length); }
    jshortArray NewShortArray(jsize length)
    { return functions->NewShortArray(this, length); }
    jintArray NewIntArray(jsize length)
    { return functions->NewIntArray(this, length); }
    jlongArray NewLongArray(jsize length)
    { return functions->NewLongArray(this, length); }
    jfloatArray NewFloatArray(jsize length)
    { return functions->NewFloatArray(this, length); }
    jdoubleArray NewDoubleArray(jsize length)
    { return functions->NewDoubleArray(this, length); }

    jboolean* GetBooleanArrayElements(jbooleanArray array, jboolean* isCopy)
    { return functions->GetBooleanArrayElements(this, array, isCopy); }
    jbyte* GetByteArrayElements(jbyteArray array, jboolean* isCopy)
    { return functions->GetByteArrayElements(this, array, isCopy); }
    jchar* GetCharArrayElements(jcharArray array, jboolean* isCopy)
    { return functions->GetCharArrayElements(this, array, isCopy); }
    jshort* GetShortArrayElements(jshortArray array, jboolean* isCopy)
    { return functions->GetShortArrayElements(this, array, isCopy); }
    jint* GetIntArrayElements(jintArray array, jboolean* isCopy)
    { return functions->GetIntArrayElements(this, array, isCopy); }
    jlong* GetLongArrayElements(jlongArray array, jboolean* isCopy)
    { return functions->GetLongArrayElements(this, array, isCopy); }
    jfloat* GetFloatArrayElements(jfloatArray array, jboolean* isCopy)
    { return functions->GetFloatArrayElements(this, array, isCopy); }
    jdouble* GetDoubleArrayElements(jdoubleArray array, jboolean* isCopy)
    { return functions->GetDoubleArrayElements(this, array, isCopy); }

    void ReleaseBooleanArrayElements(jbooleanArray array, jboolean* elems,
        jint mode)
    { functions->ReleaseBooleanArrayElements(this, array, elems, mode); }
    void ReleaseByteArrayElements(jbyteArray array, jbyte* elems,
        jint mode)
    { functions->ReleaseByteArrayElements(this, array, elems, mode); }
    void ReleaseCharArrayElements(jcharArray array, jchar* elems,
        jint mode)
    { functions->ReleaseCharArrayElements(this, array, elems, mode); }
    void ReleaseShortArrayElements(jshortArray array, jshort* elems,
        jint mode)
    { functions->ReleaseShortArrayElements(this, array, elems, mode); }
    void ReleaseIntArrayElements(jintArray array, jint* elems,
        jint mode)
    { functions->ReleaseIntArrayElements(this, array, elems, mode); }
    void ReleaseLongArrayElements(jlongArray array, jlong* elems,
        jint mode)
    { functions->ReleaseLongArrayElements(this, array, elems, mode); }
    void ReleaseFloatArrayElements(jfloatArray array, jfloat* elems,
        jint mode)
    { functions->ReleaseFloatArrayElements(this, array, elems, mode); }
    void ReleaseDoubleArrayElements(jdoubleArray array, jdouble* elems,
        jint mode)
    { functions->ReleaseDoubleArrayElements(this, array, elems, mode); }
  
    void GetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len,
        jboolean* buf)
    { functions->GetBooleanArrayRegion(this, array, start, len, buf); }
    void GetByteArrayRegion(jbyteArray array, jsize start, jsize len,
        jbyte* buf)
    { functions->GetByteArrayRegion(this, array, start, len, buf); }
    void GetCharArrayRegion(jcharArray array, jsize start, jsize len,
        jchar* buf)
    { functions->GetCharArrayRegion(this, array, start, len, buf); }
    void GetShortArrayRegion(jshortArray array, jsize start, jsize len,
        jshort* buf)
    { functions->GetShortArrayRegion(this, array, start, len, buf); }
    void GetIntArrayRegion(jintArray array, jsize start, jsize len,
        jint* buf)
    { functions->GetIntArrayRegion(this, array, start, len, buf); }
    void GetLongArrayRegion(jlongArray array, jsize start, jsize len,
        jlong* buf)
    { functions->GetLongArrayRegion(this, array, start, len, buf); }
    void GetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
        jfloat* buf)
    { functions->GetFloatArrayRegion(this, array, start, len, buf); }
    void GetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
        jdouble* buf)
    { functions->GetDoubleArrayRegion(this, array, start, len, buf); }
  
    void SetBooleanArrayRegion(jbooleanArray array, jsize start, jsize len,
        const jboolean* buf)
    { functions->SetBooleanArrayRegion(this, array, start, len, buf); }
    void SetByteArrayRegion(jbyteArray array, jsize start, jsize len,
        const jbyte* buf)
    { functions->SetByteArrayRegion(this, array, start, len, buf); }
    void SetCharArrayRegion(jcharArray array, jsize start, jsize len,
        const jchar* buf)
    { functions->SetCharArrayRegion(this, array, start, len, buf); }
    void SetShortArrayRegion(jshortArray array, jsize start, jsize len,
        const jshort* buf)
    { functions->SetShortArrayRegion(this, array, start, len, buf); }
    void SetIntArrayRegion(jintArray array, jsize start, jsize len,
        const jint* buf)
    { functions->SetIntArrayRegion(this, array, start, len, buf); }
    void SetLongArrayRegion(jlongArray array, jsize start, jsize len,
        const jlong* buf)
    { functions->SetLongArrayRegion(this, array, start, len, buf); }
    void SetFloatArrayRegion(jfloatArray array, jsize start, jsize len,
        const jfloat* buf)
    { functions->SetFloatArrayRegion(this, array, start, len, buf); }
    void SetDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
        const jdouble* buf)
    { functions->SetDoubleArrayRegion(this, array, start, len, buf); }

    jint RegisterNatives(jclass clazz, const JNINativeMethod* methods,
        jint nMethods)
    { return functions->RegisterNatives(this, clazz, methods, nMethods); }

    jint UnregisterNatives(jclass clazz)
    { return functions->UnregisterNatives(this, clazz); }

    jint MonitorEnter(jobject obj)
    { return functions->MonitorEnter(this, obj); }

    jint MonitorExit(jobject obj)
    { return functions->MonitorExit(this, obj); }

    jint GetJavaVM(JavaVM** vm)
    { return functions->GetJavaVM(this, vm); }

    void GetStringRegion(jstring str, jsize start, jsize len, jchar* buf)
    { functions->GetStringRegion(this, str, start, len, buf); }

    void GetStringUTFRegion(jstring str, jsize start, jsize len, char* buf)
    { return functions->GetStringUTFRegion(this, str, start, len, buf); }

    void* GetPrimitiveArrayCritical(jarray array, jboolean* isCopy)
    { return functions->GetPrimitiveArrayCritical(this, array, isCopy); }

    void ReleasePrimitiveArrayCritical(jarray array, void* carray, jint mode)
    { functions->ReleasePrimitiveArrayCritical(this, array, carray, mode); }

    const jchar* GetStringCritical(jstring string, jboolean* isCopy)
    { return functions->GetStringCritical(this, string, isCopy); }

    void ReleaseStringCritical(jstring string, const jchar* carray)
    { functions->ReleaseStringCritical(this, string, carray); }

    jweak NewWeakGlobalRef(jobject obj)
    { return functions->NewWeakGlobalRef(this, obj); }

    void DeleteWeakGlobalRef(jweak obj)
    { functions->DeleteWeakGlobalRef(this, obj); }

    jboolean ExceptionCheck()
    { return functions->ExceptionCheck(this); }

    jobject NewDirectByteBuffer(void* address, jlong capacity)
    { return functions->NewDirectByteBuffer(this, address, capacity); }

    void* GetDirectBufferAddress(jobject buf)
    { return functions->GetDirectBufferAddress(this, buf); }

    jlong GetDirectBufferCapacity(jobject buf)
    { return functions->GetDirectBufferCapacity(this, buf); }

    /* added in JNI 1.6 */
    jobjectRefType GetObjectRefType(jobject obj)
    { return functions->GetObjectRefType(this, obj); }

  	const char*	GetArrayType(jarray jarr)
	{ return functions->GetArrayType(this, jarr); }

    jobject NewTaintedObjectA(jclass clazz, jmethodID methodID, u4 objTaint, jvalue* args, u4* taints)
    { return functions->NewTaintedObjectA(this, clazz, methodID, objTaint, args, taints); }

    #define CALL_TYPE_TAINTED_METHODA(_jtype, _jname)                       \
    _jtype Call##_jname##TaintedMethodA(jobject obj, u4 objTaint,           \
             jmethodID methodID, u4* resultTaint, jvalue* args, u4* taints) \
    { return functions->Call##_jname##TaintedMethodA(this, obj, objTaint,   \
                                    methodID, resultTaint, args, taints); }

#define CALL_TAINTED_TYPE(_jtype, _jname)       \
  CALL_TYPE_TAINTED_METHODA(_jtype, _jname)

  CALL_TAINTED_TYPE(jobject, Object)
  CALL_TAINTED_TYPE(jboolean, Boolean)
  CALL_TAINTED_TYPE(jbyte, Byte)
  CALL_TAINTED_TYPE(jchar, Char)
  CALL_TAINTED_TYPE(jshort, Short)
  CALL_TAINTED_TYPE(jint, Int)
  CALL_TAINTED_TYPE(jlong, Long)
  CALL_TAINTED_TYPE(jfloat, Float)
  CALL_TAINTED_TYPE(jdouble, Double)

  void CallVoidTaintedMethodA(jobject obj, u4 objTaint, jmethodID methodID,
                              u4* resultTaint, jvalue* args, u4* taints)
  { functions->CallVoidTaintedMethodA(this, obj, objTaint, methodID, resultTaint, args, taints); }

#define CALL_NONVIRT_TYPE_TAINTED_METHODA(_jtype, _jname)                   \
    _jtype CallNonvirtual##_jname##TaintedMethodA(jobject obj, u4 objTaint, \
        jclass jclazz, jmethodID methodID, u4* resultTaint, jvalue* args,   \
         u4* taints)                                                        \
    { return functions->CallNonvirtual##_jname##TaintedMethodA(this, obj,   \
                 objTaint, jclazz, methodID, resultTaint, args, taints); }

#define CALL_NONVIRT_TAINTED_TYPE(_jtype, _jname)  \
    CALL_NONVIRT_TYPE_TAINTED_METHODA(_jtype, _jname)

    CALL_NONVIRT_TAINTED_TYPE(jobject, Object)
    CALL_NONVIRT_TAINTED_TYPE(jboolean, Boolean)
    CALL_NONVIRT_TAINTED_TYPE(jbyte, Byte)
    CALL_NONVIRT_TAINTED_TYPE(jchar, Char)
    CALL_NONVIRT_TAINTED_TYPE(jshort, Short)
    CALL_NONVIRT_TAINTED_TYPE(jint, Int)
    CALL_NONVIRT_TAINTED_TYPE(jlong, Long)
    CALL_NONVIRT_TAINTED_TYPE(jfloat, Float)
    CALL_NONVIRT_TAINTED_TYPE(jdouble, Double)

    void CallNonvirtualVoidTaintedMethodA(jobject obj, u4 objTaint,
                                          jclass jclazz, jmethodID methodID, u4* resultTaint, jvalue* args,
                                          u4* taints)
    { functions->CallNonvirtualVoidTaintedMethodA(this, obj,
        objTaint, jclazz, methodID, resultTaint, args, taints); }

  	jobject GetObjectTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetObjectTaintedField(this, obj, field, taint); }
	jboolean GetBooleanTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetBooleanTaintedField(this, obj, field, taint); }
	jbyte GetByteTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetByteTaintedField(this, obj, field, taint); }
	jchar GetCharTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetCharTaintedField(this, obj, field, taint); }
	jshort GetShortTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetShortTaintedField(this, obj, field, taint); }
	jint GetIntTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetIntTaintedField(this, obj, field, taint); }
	jlong GetLongTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetLongTaintedField(this, obj, field, taint); }
	jfloat GetFloatTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetFloatTaintedField(this, obj, field, taint); }
	jdouble GetDoubleTaintedField(jobject obj, jfieldID field, u4* taint)
	{ return functions->GetDoubleTaintedField(this, obj, field, taint); }

    void SetObjectTaintedField(jobject obj, jfieldID field, jobject val, u4 taint)
	{ return functions->SetObjectTaintedField(this, obj, field, val, taint); }
	void SetBooleanTaintedField(jobject obj, jfieldID field, jboolean val, u4 taint)
	{ return functions->SetBooleanTaintedField(this, obj, field, val, taint); }
	void SetByteTaintedField(jobject obj, jfieldID field, jbyte val, u4 taint)
	{ return functions->SetByteTaintedField(this, obj, field, val, taint); }
	void SetCharTaintedField(jobject obj, jfieldID field, jchar val, u4 taint)
	{ return functions->SetCharTaintedField(this, obj, field, val, taint); }
	void SetShortTaintedField(jobject obj, jfieldID field, jshort val, u4 taint)
	{ return functions->SetShortTaintedField(this, obj, field, val, taint); }
	void SetIntTaintedField(jobject obj, jfieldID field, jint val, u4 taint)
	{ return functions->SetIntTaintedField(this, obj, field, val, taint); }
	void SetLongTaintedField(jobject obj, jfieldID field, jlong val, u4 taint)
	{ return functions->SetLongTaintedField(this, obj, field, val, taint); }
	void SetFloatTaintedField(jobject obj, jfieldID field, jfloat val, u4 taint)
	{ return functions->SetFloatTaintedField(this, obj, field, val, taint); }
	void SetDoubleTaintedField(jobject obj, jfieldID field, jdouble val, u4 taint)
	{ return functions->SetDoubleTaintedField(this, obj, field, val, taint); }


#define CALL_STATIC_TYPE_TAINTED_METHODA(_jtype, _jname)                        \
    _jtype CallStatic##_jname##TaintedMethodA(jclass clazz, jmethodID methodID, \
                             u4* resultTaint, jvalue* args, u4* taints)     \
    { return functions->CallStatic##_jname##TaintedMethodA(this, clazz,     \
                         methodID, resultTaint, args, taints); }

#define CALL_STATIC_TAINTED_TYPE(_jtype, _jname)                                    \
    CALL_STATIC_TYPE_TAINTED_METHODA(_jtype, _jname)

    CALL_STATIC_TAINTED_TYPE(jobject, Object)
    CALL_STATIC_TAINTED_TYPE(jboolean, Boolean)
    CALL_STATIC_TAINTED_TYPE(jbyte, Byte)
    CALL_STATIC_TAINTED_TYPE(jchar, Char)
    CALL_STATIC_TAINTED_TYPE(jshort, Short)
    CALL_STATIC_TAINTED_TYPE(jint, Int)
    CALL_STATIC_TAINTED_TYPE(jlong, Long)
    CALL_STATIC_TAINTED_TYPE(jfloat, Float)
    CALL_STATIC_TAINTED_TYPE(jdouble, Double)

    void CallStaticVoidTaintedMethodA(jclass clazz, jmethodID methodID, u4* resultTaint, jvalue* args, u4* taints)
    { functions->CallStaticVoidTaintedMethodA(this, clazz, methodID, resultTaint, args, taints); }

    jobject GetStaticObjectTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticObjectTaintedField(this, clazz, fieldID, taint); }
    jboolean GetStaticBooleanTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticBooleanTaintedField(this, clazz, fieldID, taint); }
    jbyte GetStaticByteTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticByteTaintedField(this, clazz, fieldID, taint); }
    jchar GetStaticCharTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticCharTaintedField(this, clazz, fieldID, taint); }
    jshort GetStaticShortTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticShortTaintedField(this, clazz, fieldID, taint); }
    jint GetStaticIntTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticIntTaintedField(this, clazz, fieldID, taint); }
    jlong GetStaticLongTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticLongTaintedField(this, clazz, fieldID, taint); }
    jfloat GetStaticFloatTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticFloatTaintedField(this, clazz, fieldID, taint); }
    jdouble GetStaticDoubleTaintedField(jclass clazz, jfieldID fieldID, u4* taint)
    { return functions->GetStaticDoubleTaintedField(this, clazz, fieldID, taint); }

    void SetStaticObjectTaintedField(jclass clazz, jfieldID fieldID, jobject value, u4 taint)
    { functions->SetStaticObjectTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticBooleanTaintedField(jclass clazz, jfieldID fieldID, jboolean value, u4 taint)
    { functions->SetStaticBooleanTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticByteTaintedField(jclass clazz, jfieldID fieldID, jbyte value, u4 taint)
    { functions->SetStaticByteTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticCharTaintedField(jclass clazz, jfieldID fieldID, jchar value, u4 taint)
    { functions->SetStaticCharTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticShortTaintedField(jclass clazz, jfieldID fieldID, jshort value, u4 taint)
    { functions->SetStaticShortTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticIntTaintedField(jclass clazz, jfieldID fieldID, jint value, u4 taint)
    { functions->SetStaticIntTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticLongTaintedField(jclass clazz, jfieldID fieldID, jlong value, u4 taint)
    { functions->SetStaticLongTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticFloatTaintedField(jclass clazz, jfieldID fieldID, jfloat value, u4 taint)
    { functions->SetStaticFloatTaintedField(this, clazz, fieldID, value, taint); }
    void SetStaticDoubleTaintedField(jclass clazz, jfieldID fieldID, jdouble value, u4 taint)
    { functions->SetStaticDoubleTaintedField(this, clazz, fieldID, value, taint); }

    jsize GetTaintedStringLength(jstring string, u4* taint)
    { return functions->GetTaintedStringLength(this, string, taint); }

    const jchar* GetTaintedStringChars(jstring string, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedStringChars(this, string, isCopy, taint); }
  
    void ReleaseTaintedStringChars(jstring string, u4 taint, const jchar* chars)
    { functions->ReleaseTaintedStringChars(this, string, taint, chars); }

    jstring NewTaintedStringUTF(const char* bytes, u4 taint)
    { return functions->NewTaintedStringUTF(this, bytes, taint); }

    const char* GetTaintedStringUTFChars(jstring string, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedStringUTFChars(this, string, isCopy, taint); }

    void ReleaseTaintedStringUTFChars(jstring string, u4 taint, const char* utf)
    { functions->ReleaseTaintedStringUTFChars(this, string, taint, utf); }

    jobjectArray NewTaintedObjectArray(jsize length, jclass elementClass,
                                jobject initialElement, u4 taint)
    { return functions->NewTaintedObjectArray(this, length, elementClass,
                                              initialElement, taint); }
  
    jobject GetTaintedObjectArrayElement(jobjectArray array, jsize index, u4* taint)
    { return functions->GetTaintedObjectArrayElement(this, array, index, taint); }

    void SetTaintedObjectArrayElement(jobjectArray array, jsize index, jobject value, u4 taint)
    { functions->SetTaintedObjectArrayElement(this, array, index, value, taint); }

    jboolean* GetTaintedBooleanArrayElements(jbooleanArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedBooleanArrayElements(this, array, isCopy, taint); }
    jbyte* GetTaintedByteArrayElements(jbyteArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedByteArrayElements(this, array, isCopy, taint); }
    jchar* GetTaintedCharArrayElements(jcharArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedCharArrayElements(this, array, isCopy, taint); }
    jshort* GetTaintedShortArrayElements(jshortArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedShortArrayElements(this, array, isCopy, taint); }
    jint* GetTaintedIntArrayElements(jintArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedIntArrayElements(this, array, isCopy, taint); }
    jlong* GetTaintedLongArrayElements(jlongArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedLongArrayElements(this, array, isCopy, taint); }
    jfloat* GetTaintedFloatArrayElements(jfloatArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedFloatArrayElements(this, array, isCopy, taint); }
    jdouble* GetTaintedDoubleArrayElements(jdoubleArray array, jboolean* isCopy, u4* taint)
    { return functions->GetTaintedDoubleArrayElements(this, array, isCopy, taint); }

    void ReleaseTaintedBooleanArrayElements(jbooleanArray array, jboolean* elems,
                                            jint mode, u4 taint)
    { functions->ReleaseTaintedBooleanArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedByteArrayElements(jbyteArray array, jbyte* elems,
                                         jint mode, u4 taint)
    { functions->ReleaseTaintedByteArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedCharArrayElements(jcharArray array, jchar* elems,
                                         jint mode, u4 taint)
    { functions->ReleaseTaintedCharArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedShortArrayElements(jshortArray array, jshort* elems,
                                          jint mode, u4 taint)
    { functions->ReleaseTaintedShortArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedIntArrayElements(jintArray array, jint* elems,
                                        jint mode, u4 taint)
    { functions->ReleaseTaintedIntArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedLongArrayElements(jlongArray array, jlong* elems,
                                         jint mode, u4 taint)
    { functions->ReleaseTaintedLongArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedFloatArrayElements(jfloatArray array, jfloat* elems,
                                          jint mode, u4 taint)
    { functions->ReleaseTaintedFloatArrayElements(this, array, elems, mode, taint); }
    void ReleaseTaintedDoubleArrayElements(jdoubleArray array, jdouble* elems,
                                           jint mode, u4 taint)
    { functions->ReleaseTaintedDoubleArrayElements(this, array, elems, mode, taint); }

    void GetTaintedBooleanArrayRegion(jbooleanArray array, jsize start, jsize len, jboolean* buf, u4* taint)
    { functions->GetTaintedBooleanArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedByteArrayRegion(jbyteArray array, jsize start, jsize len, jbyte* buf, u4* taint)
    { functions->GetTaintedByteArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedCharArrayRegion(jcharArray array, jsize start, jsize len, jchar* buf, u4* taint)
    { functions->GetTaintedCharArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedShortArrayRegion(jshortArray array, jsize start, jsize len, jshort* buf, u4* taint)
    { functions->GetTaintedShortArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedIntArrayRegion(jintArray array, jsize start, jsize len, jint* buf, u4* taint)
    { functions->GetTaintedIntArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedLongArrayRegion(jlongArray array, jsize start, jsize len, jlong* buf, u4* taint)
    { functions->GetTaintedLongArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedFloatArrayRegion(jfloatArray array, jsize start, jsize len, jfloat* buf, u4* taint)
    { functions->GetTaintedFloatArrayRegion(this, array, start, len, buf, taint); }
    void GetTaintedDoubleArrayRegion(jdoubleArray array, jsize start, jsize len, jdouble* buf, u4* taint)
    { functions->GetTaintedDoubleArrayRegion(this, array, start, len, buf, taint); }
  
    void SetTaintedBooleanArrayRegion(jbooleanArray array, jsize start, jsize len,
                                      const jboolean* buf, u4 taint)
    { functions->SetTaintedBooleanArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedByteArrayRegion(jbyteArray array, jsize start, jsize len,
                                 const jbyte* buf, u4 taint)
    { functions->SetTaintedByteArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedCharArrayRegion(jcharArray array, jsize start, jsize len,
                            const jchar* buf, u4 taint)
    { functions->SetTaintedCharArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedShortArrayRegion(jshortArray array, jsize start, jsize len,
                                    const jshort* buf, u4 taint)
    { functions->SetTaintedShortArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedIntArrayRegion(jintArray array, jsize start, jsize len,
                           const jint* buf, u4 taint)
    { functions->SetTaintedIntArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedLongArrayRegion(jlongArray array, jsize start, jsize len,
                                   const jlong* buf, u4 taint)
    { functions->SetTaintedLongArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedFloatArrayRegion(jfloatArray array, jsize start, jsize len,
                                    const jfloat* buf, u4 taint)
    { functions->SetTaintedFloatArrayRegion(this, array, start, len, buf, taint); }
    void SetTaintedDoubleArrayRegion(jdoubleArray array, jsize start, jsize len,
                                     const jdouble* buf, u4 taint)
    { functions->SetTaintedDoubleArrayRegion(this, array, start, len, buf, taint); }

    void GetTaintedStringRegion(jstring str, jsize start, jsize len, jchar* buf, u4* taint)
    { functions->GetTaintedStringRegion(this, str, start, len, buf, taint); }

    void GetTaintedStringUTFRegion(jstring str, jsize start, jsize len, char* buf, u4* taint)
    { return functions->GetTaintedStringUTFRegion(this, str, start, len, buf, taint); }

    void* GetTaintedPrimitiveArrayCritical(jarray array, u4* taint, jboolean* isCopy)
    { return functions->GetTaintedPrimitiveArrayCritical(this, array, taint, isCopy); }

    void ReleaseTaintedPrimitiveArrayCritical(jarray array, void* carray, u4 taint, jint mode)
    { functions->ReleaseTaintedPrimitiveArrayCritical(this, array, carray, taint, mode); }

    const jchar* GetTaintedStringCritical(jstring string, u4* taint, jboolean* isCopy)
    { return functions->GetTaintedStringCritical(this, string, taint, isCopy); }

    void ReleaseTaintedStringCritical(jstring string, u4 taint, const jchar* carray)
    { functions->ReleaseTaintedStringCritical(this, string, taint, carray); }

	jint RegisterTaintedNatives(jclass jclazz, const JNINativeMethod* methods, jint nMethods)
	{ return functions->RegisterTaintedNatives(this, jclazz, methods, nMethods); }

	jint GetUTFCharsByteLength(jstring jstr)
	{ return functions->GetUTFCharsByteLength(this, jstr); }

#endif /*__cplusplus*/
};


/*
 * JNI invocation interface.
 */
struct JNIInvokeInterface {
    void*       reserved0;
    void*       reserved1;
    void*       reserved2;

    jint        (*DestroyJavaVM)(JavaVM*);
    jint        (*AttachCurrentThread)(JavaVM*, JNIEnv**, void*);
    jint        (*DetachCurrentThread)(JavaVM*);
    jint        (*GetEnv)(JavaVM*, void**, jint);
    jint        (*AttachCurrentThreadAsDaemon)(JavaVM*, JNIEnv**, void*);
};

/*
 * C++ version.
 */
struct _JavaVM {
    const struct JNIInvokeInterface* functions;

#if defined(__cplusplus)
    jint DestroyJavaVM()
    { return functions->DestroyJavaVM(this); }
    jint AttachCurrentThread(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThread(this, p_env, thr_args); }
    jint DetachCurrentThread()
    { return functions->DetachCurrentThread(this); }
    jint GetEnv(void** env, jint version)
    { return functions->GetEnv(this, env, version); }
    jint AttachCurrentThreadAsDaemon(JNIEnv** p_env, void* thr_args)
    { return functions->AttachCurrentThreadAsDaemon(this, p_env, thr_args); }
#endif /*__cplusplus*/
};

struct JavaVMAttachArgs {
    jint        version;    /* must be >= JNI_VERSION_1_2 */
    const char* name;       /* NULL or name of thread as modified UTF-8 str */
    jobject     group;      /* global ref of a ThreadGroup object, or NULL */
};
typedef struct JavaVMAttachArgs JavaVMAttachArgs;

/*
 * JNI 1.2+ initialization.  (As of 1.6, the pre-1.2 structures are no
 * longer supported.)
 */
typedef struct JavaVMOption {
    const char* optionString;
    void*       extraInfo;
} JavaVMOption;

typedef struct JavaVMInitArgs {
    jint        version;    /* use JNI_VERSION_1_2 or later */

    jint        nOptions;
    JavaVMOption* options;
    jboolean    ignoreUnrecognized;
} JavaVMInitArgs;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * VM initialization functions.
 *
 * Note these are the only symbols exported for JNI by the VM.
 */
jint JNI_GetDefaultJavaVMInitArgs(void*);
jint JNI_CreateJavaVM(JavaVM**, JNIEnv**, void*);
jint JNI_GetCreatedJavaVMs(JavaVM**, jsize, jsize*);

#define JNIIMPORT
#define JNIEXPORT  __attribute__ ((visibility ("default")))
#define JNICALL

/*
 * Prototypes for functions exported by loadable shared libs.  These are
 * called by JNI, not provided by JNI.
 */
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);
JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved);

#ifdef __cplusplus
}
#endif


/*
 * Manifest constants.
 */
#define JNI_FALSE   0
#define JNI_TRUE    1

#define JNI_VERSION_1_1 0x00010001
#define JNI_VERSION_1_2 0x00010002
#define JNI_VERSION_1_4 0x00010004
#define JNI_VERSION_1_6 0x00010006

#define JNI_OK          (0)         /* no error */
#define JNI_ERR         (-1)        /* generic error */
#define JNI_EDETACHED   (-2)        /* thread detached from the VM */
#define JNI_EVERSION    (-3)        /* JNI version error */

#define JNI_COMMIT      1           /* copy content, do not free buffer */
#define JNI_ABORT       2           /* free buffer w/o copying back */

#endif  /* JNI_H_ */
