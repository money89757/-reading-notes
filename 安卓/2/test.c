

#include  <stdio.h>
#include  <jni.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


	


int  fd;
jint  c_open( JNIEnv  *env,jclass cls)
{



	printf(" open file  success  \n");

	return  1;


}
	

jstring  c_string(JNIEnv  * env,jclass cls,jstring  prompt)
{

	char buf[128];
	const jbyte *str;
	str = (*env)->GetStringUTFChars(env, prompt, NULL);
	if (str == NULL) {
		return NULL; /* OutOfMemoryError already thrown */
	}
	printf("%s\n", str);
	(*env)->ReleaseStringUTFChars(env, prompt, str);
	/* We assume here that the user does not type more than
	 * * 127 characters */
	return (*env)->NewStringUTF(env, "no zuo no die");




}

JNINativeMethod  methods[]  = {

	{
	
		"hello_open","()I",(void  *)c_open,
	
	},
	{
	
		"hello_string","(Ljava/lang/String;)Ljava/lang/String;",(void  *)c_string,
	
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
