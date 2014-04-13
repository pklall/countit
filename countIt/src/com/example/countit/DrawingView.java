package com.example.countit;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Matrix.ScaleToFit;
import android.graphics.Paint;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class DrawingView extends View {

	private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	private float circleRadius = 25;
	private float circleX = -1.0f;
	private float circleY = -1.0f;

	public final ArrayList<Double> pointX = new ArrayList<Double>();
	public final ArrayList<Double> pointY = new ArrayList<Double>();

	public Bitmap img;
	public Bitmap overlay;
	
	private static Matrix imgMatrix = new Matrix();
	private static Matrix revImgMatrix = new Matrix();

	public Runnable onClickCallback = null;

	public DrawingView(Context context) {
		super(context);
		paint.setColor(Color.BLUE);
	}

	public DrawingView(Context context, AttributeSet attrs) {
		super(context, attrs);
		paint.setColor(Color.BLUE);
	}

	@Override
	public void onDraw(Canvas canvas) {
		super.onDraw(canvas);

		RectF dst = new RectF();
		dst.left = 0;
		dst.top = 0;
		dst.right = canvas.getWidth();
		dst.bottom = canvas.getHeight();

		RectF src = new RectF();
		src.left = 0;
		src.top = 0;
		src.right = img.getWidth();
		src.bottom = img.getHeight();

		imgMatrix.setRectToRect(src, dst, ScaleToFit.CENTER);
		
		imgMatrix.invert(revImgMatrix);

		canvas.drawBitmap(img, imgMatrix, null);

		canvas.drawBitmap(overlay, imgMatrix, null);

		if (circleX > 0 && circleY > 0) {
			canvas.drawCircle(circleX, circleY, circleRadius, paint);
		}
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			circleX = event.getX();
			circleY = event.getY();
			invalidate();

			float[] point = new float[2];
			point[0] = circleX;
			point[1] = circleY;
			revImgMatrix.mapPoints(point);

			pointX.add(Double.valueOf(point[0]));
			pointY.add(Double.valueOf(point[1]));

			if (onClickCallback != null) {
				onClickCallback.run();
			}
		}

		return false;
	}
}
