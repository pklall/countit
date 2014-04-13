package com.example.countit;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class DrawingView extends View {
	
	private final Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	private float circleRadius = 25;
	private float circleX = -1.0f;
	private float circleY = -1.0f;
	
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
		if(circleX > 0 && circleY > 0){
			canvas.drawCircle(circleX, circleY, circleRadius, paint);
		}
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			circleX = event.getX();
			circleY = event.getY();
			invalidate();
		}
		return false;
	}
}
