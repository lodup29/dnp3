/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_automatak_dnp3_impl_ChannelImpl */

#ifndef _Included_com_automatak_dnp3_impl_ChannelImpl
#define _Included_com_automatak_dnp3_impl_ChannelImpl
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_automatak_dnp3_impl_ChannelImpl
 * Method:    shutdown_native
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
(JNIEnv*, jobject, jlong);

/*
 * Class:     com_automatak_dnp3_impl_ChannelImpl
 * Method:    add_native_state_change_listener
 * Signature: (JLcom/automatak/dnp3/impl/ChannelStateProxy;)V
 */
JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_add_1native_1state_1change_1listener
(JNIEnv*, jobject, jlong, jobject);

/*
 * Class:     com_automatak_dnp3_impl_ChannelImpl
 * Method:    get_native_master
 * Signature: (JLjava/lang/String;ILcom/automatak/dnp3/impl/DataObserverAdapter;Lcom/automatak/dnp3/MasterStackConfig;)J
 */
JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv*, jobject, jlong, jstring, jint, jobject, jobject);

/*
 * Class:     com_automatak_dnp3_impl_ChannelImpl
 * Method:    get_native_slave
 * Signature: (JLjava/lang/String;ILcom/automatak/dnp3/impl/CommandHandlerAdapter;Lcom/automatak/dnp3/OutstationStackConfig;)J
 */
JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
(JNIEnv*, jobject, jlong, jstring, jint, jobject, jobject);

#ifdef __cplusplus
}
#endif
#endif
