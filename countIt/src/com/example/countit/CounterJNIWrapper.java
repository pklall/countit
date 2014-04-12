package com.example.countit;

public class CounterJNIWrapper {
	static {
		System.loadLibrary("counter-jni");
	}
	
	public static native int processImage();
}
