package com.example.countit;

import java.nio.Buffer;
import java.util.List;

import android.graphics.Bitmap;

public class CounterJNIWrapper {
	static {
		System.loadLibrary("counter-jni");
	}
	
	public static int processImage(
			List<Double> pointsX,
			List<Double> pointsY,
			Bitmap input,
			Bitmap output) {
		return 0;
	}

	public static native int processImage(int imgWidth, int imgHeight,
			int numPoints, Buffer pointBuf, Buffer imgInputBuf,
			Buffer imgOverlayBuf);
}
