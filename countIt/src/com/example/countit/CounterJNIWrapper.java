package com.example.countit;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.List;

import android.graphics.Bitmap;

public class CounterJNIWrapper {
	static {
		System.loadLibrary("counter-jni");
	}

	public static int processImage(List<Double> pointsX, List<Double> pointsY,
			Bitmap input, Bitmap output) {
		ByteBuffer inputBB = ByteBuffer.allocateDirect(input.getByteCount());
		ByteBuffer outputBB = ByteBuffer.allocateDirect(input.getByteCount());
		
		inputBB.order(ByteOrder.nativeOrder());
		outputBB.order(ByteOrder.nativeOrder());
		
		input.copyPixelsToBuffer(inputBB);

		int numPoints = pointsX.size();

		ByteBuffer pointBufByte = ByteBuffer.allocateDirect(4 * numPoints * 2);
		pointBufByte.order(ByteOrder.nativeOrder());

		FloatBuffer pointBuf = pointBufByte.asFloatBuffer();
		
		for (int i = 0; i < numPoints; i++) {
			float x = (float) pointsX.get(i).doubleValue();
			float y = (float) pointsY.get(i).doubleValue();

			pointBuf.put(i * 2 + 0, x);
			pointBuf.put(i * 2 + 1, y);
		}

		int count = processImage(input.getWidth(), input.getHeight(),
				numPoints, pointBuf, inputBB, outputBB);
		
		output.copyPixelsFromBuffer(outputBB);

		return count;
	}

	public static native int processImage(int imgWidth, int imgHeight,
			int numPoints, Buffer pointBuf, Buffer imgInputBuf,
			Buffer imgOverlayBuf);
	
	public static native int jnitest();
}
