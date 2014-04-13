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


	Uri imageUri = null;
	DrawingView surface;
	
	int BITMAP_HEIGHT = 2048;
	int BITMAP_WIDTH = 2048;
	
	@Override protected void onCreate(Bundle savedInstanceState) {
	  super.onCreate(savedInstanceState);
	  setContentView(R.layout.manipulate_pic);
	  	  
	  surface = (DrawingView) (findViewById(R.id.draw_surface));

	  Intent respondIntent = getIntent();
	  Bundle extras = respondIntent.getExtras(); 
	  imageUri = Uri.parse(extras.getString("imageUri"));
	  
	  Log.d("uri:",imageUri.toString());
	  
	  
	  try {
		  	Bitmap bmp =  MediaStore.Images.Media.getBitmap(getContentResolver(), imageUri);
		  	bmp = Bitmap.createScaledBitmap(bmp, BITMAP_HEIGHT, BITMAP_WIDTH, false);
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
