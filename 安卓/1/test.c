#include  <stdio.h>
#include  <jni.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int  fd;
	
jint  c_open(JNIEnv  * env,jclass cls,jintArray arr)
{

	jint *carr;
	jint i,  sum = 0;
	carr = (*env)->GetIntArrayElements(env,arr,NULL);
	if (carr == NULL)
	{
		return 0;
	}
	for( i = 0; i < (*env)->GetArrayLength(env, arr); i++){
		sum += carr[i];
	}
	(*env)->ReleaseIntArrayElements(env, arr,carr,0);
	return sum;




}

JNINativeMethod  methods[]  = {

	{
	
		"hello_open","([I)I",(void  *)c_open,
	
	},

};
	
JNIEXPORT jint JNICALL 
JNI_OnLoad(JavaVM *jvm, void *reserved) {
	JNIEnv *env;
	jclass cls;
    //获取java运行环境
	if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
		return JNI_ERR; /* JNI version not supported */
	}

	//找类
	cls = (*env)->FindClass(env, "Hello");
	if (cls == NULL) {
		return JNI_ERR;
	}
	//建立映射关系
	(*env)->RegisterNatives(env,cls,methods,sizeof(methods)/sizeof(methods[0]));
	
	
	return JNI_VERSION_1_2;

}
