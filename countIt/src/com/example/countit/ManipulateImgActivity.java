package com.example.countit;

import java.io.FileNotFoundException;
import java.io.IOException;

import android.support.v7.app.ActionBarActivity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Path;
import android.graphics.PixelFormat;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.ImageView;

public class ManipulateImgActivity extends ActionBarActivity implements
		SurfaceHolder.Callback {

	/*
	 * DrawingView dv; private Paint mPaint;
	 * 
	 * // private DrawingManager mDrawingManager=null;
	 * 
	 * @Override protected void onCreate(Bundle savedInstanceState) {
	 * super.onCreate(savedInstanceState); dv = new DrawingView(this);
	 * setContentView(dv); mPaint = new Paint(); mPaint.setAntiAlias(true);
	 * mPaint.setDither(true); mPaint.setColor(Color.GREEN);
	 * mPaint.setStyle(Paint.Style.STROKE);
	 * mPaint.setStrokeJoin(Paint.Join.ROUND);
	 * mPaint.setStrokeCap(Paint.Cap.ROUND); mPaint.setStrokeWidth(12); }
	 * 
	 * public class DrawingView extends View {
	 * 
	 * public int width; public int height; private Bitmap mBitmap; private
	 * Canvas mCanvas; private Path mPath; private Paint mBitmapPaint; Context
	 * context; private Paint circlePaint; private Path circlePath;
	 * 
	 * public DrawingView(Context c) { super(c); context = c; mPath = new
	 * Path(); mBitmapPaint = new Paint(Paint.DITHER_FLAG); circlePaint = new
	 * Paint(); circlePath = new Path(); circlePaint.setAntiAlias(true);
	 * circlePaint.setColor(Color.BLUE);
	 * circlePaint.setStyle(Paint.Style.STROKE);
	 * circlePaint.setStrokeJoin(Paint.Join.MITER);
	 * circlePaint.setStrokeWidth(4f); }
	 * 
	 * @Override protected void onSizeChanged(int w, int h, int oldw, int oldh)
	 * { super.onSizeChanged(w, h, oldw, oldh);
	 * 
	 * mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888); mCanvas =
	 * new Canvas(mBitmap);
	 * 
	 * }
	 * 
	 * @Override protected void onDraw(Canvas canvas) { super.onDraw(canvas);
	 * 
	 * canvas.drawBitmap(mBitmap, 0, 0, mBitmapPaint);
	 * 
	 * canvas.drawPath(mPath, mPaint);
	 * 
	 * // canvas.drawPath(circlePath, circlePaint); }
	 * 
	 * private float mX, mY; private static final float TOUCH_TOLERANCE = 4;
	 * 
	 * private void touch_start(float x, float y) { mPath.reset();
	 * mPath.moveTo(x, y); mX = x; mY = y; }
	 * 
	 * private void touch_move(float x, float y) { float dx = Math.abs(x - mX);
	 * float dy = Math.abs(y - mY); if (dx >= TOUCH_TOLERANCE || dy >=
	 * TOUCH_TOLERANCE) { mPath.quadTo(mX, mY, (x + mX) / 2, (y + mY) / 2); mX =
	 * x; mY = y;
	 * 
	 * circlePath.reset(); circlePath.addCircle(mX, mY, 30, Path.Direction.CW);
	 * } }
	 * 
	 * private void touch_up() { mPath.lineTo(mX, mY); circlePath.reset(); //
	 * commit the path to our offscreen mCanvas.drawPath(mPath, mPaint); // kill
	 * this so we don't double draw mPath.reset(); }
	 * 
	 * @Override public boolean onTouchEvent(MotionEvent event) { float x =
	 * event.getX(); float y = event.getY();
	 * 
	 * switch (event.getAction()) { case MotionEvent.ACTION_DOWN: touch_start(x,
	 * y); invalidate(); break; case MotionEvent.ACTION_MOVE: touch_move(x, y);
	 * invalidate(); break; case MotionEvent.ACTION_UP: touch_up();
	 * invalidate(); break; } return true; } }
	 */

	Uri imageUri = null;
	DrawingView surface;
	
	@Override protected void onCreate(Bundle savedInstanceState) {
	  super.onCreate(savedInstanceState);
	  setContentView(R.layout.manipulate_pic);
	  
//	  Log.d("Manipulate Image", "in intent");
	  
	  surface = (DrawingView) (findViewById(R.id.draw_surface));
	  //surface.setZOrderOnTop(true);
	  //surfaceHolder = surface.getHolder();
	  // surfaceHolder.addCallback(this);
	  //surfaceHolder.setFormat(PixelFormat.TRANSPARENT);
	  //drawPaint.setColor(Color.BLUE);
//	  drawPaint.setStyle(Style.FILL);
	  
	  /*surface.setOnTouchListener(new OnTouchListener(){
	  	  @Override public boolean onTouch(View arg0, MotionEvent arg1) {
		  // TODO
		  //Auto-generated method stub
	  
		  switch (arg1.getAction()){
	  
		  case MotionEvent.ACTION_DOWN:
			  if (surfaceHolder.getSurface().isValid()){
				  canvas = surfaceHolder.lockCanvas();
				 // drawPaint = new Paint();
				  //drawPaint.setColor(Color.BLUE);
				  canvas.drawCircle(arg1.getX(), arg1.getY(), 10, drawPaint);
				  surfaceHolder.unlockCanvasAndPost(canvas);
			  }
			  break;
		  case MotionEvent.ACTION_UP:
			  Log.d("draw", "action up");
			  break;
		  case MotionEvent.ACTION_MOVE:
			  Log.d("draw", "action move");
			//  surface.draw(canvas); 
			  break;
		  default:
			  Log.d("draw", "something happened");
		  }
		  
		  return false; } });
	  */

	  Intent respondIntent = getIntent();
	  Bundle extras = respondIntent.getExtras(); 
	  imageUri = Uri.parse(extras.getString("imageUri"));
	  
	  Log.d("uri:",imageUri.toString());
	  
	  
	  try {
		  	Bitmap bmp =  MediaStore.Images.Media.getBitmap(getContentResolver(), imageUri);
		  	ImageView img = (ImageView) findViewById(R.id.image_manipulate);
		  	img.setImageBitmap(bmp);
		  	// img.setImageResource(R.drawable.ic_launcher);
	  } catch (FileNotFoundException e) { // TODO Auto-generated catch block
		  e.printStackTrace(); }
	  	catch (IOException e) { 
	  		// TODO Auto-generate catch block
	  		e.printStackTrace(); }
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.img_gallery, menu);
		return true;
	}

	@Override public boolean onOptionsItemSelected(MenuItem item) { 
		// Handle action bar item clicks here. The action bar will
		// automatically handle  clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id ==  R.id.action_settings) {
				return true; }
		return super.onOptionsItemSelected(item);
	}

	@Override
	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		// TODO Auto-generated method stub
		// look up drawing
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
//		 canvas = surfaceHolder.lockCanvas();
//		 canvas.drawCircle(50, 20, 3, drawPaint);
//		 surfaceHolder.unlockCanvasAndPost(canvas);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub

	}
}
