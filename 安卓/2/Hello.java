public  class   Hello {
	//加载动态库

	static {
	
		System.loadLibrary("test");
	}
  //申明函数，函数的实现在动态库中实现
	public static native  int   hello_open();
	public static native  String  hello_string(String  str);
	public static void main (String [] args)
	{
		/* code */
		
		int [] array = {1,2,3};
		Hello  hello  = new  Hello();

		System.out.println(hello.array);


	}





}
