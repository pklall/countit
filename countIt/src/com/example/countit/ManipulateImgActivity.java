package com.example.countit;

import java.io.FileNotFoundException;
import java.io.IOException;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class ManipulateImgActivity extends ActionBarActivity implements
		SurfaceHolder.Callback {

	Uri imageUri = null;
	DrawingView surface;

	int BITMAP_SIZE = 512;

	int count = -1;

	public void recompute() {
		((TextView) findViewById(R.id.text_count))
				.setText("Counting... please wait...");

		if (surface.pointX.size() == 0) {
			count = -1;
		} else {
			count = CounterJNIWrapper.processImage(surface.pointX,
					surface.pointY, surface.img, surface.overlay);
		}

		if (count == -1) {
			((TextView) findViewById(R.id.text_count))
					.setText("Tap an item you wish to count!");

			surface.overlay.eraseColor(Color.argb(0, 0, 0, 0));
		} else {
			((TextView) findViewById(R.id.text_count)).setText("Count: "
					+ count);
		}

		surface.invalidate();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.manipulate_pic);

		((Button) findViewById(R.id.reset_button))
				.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View arg0) {
						surface.pointX.clear();
						surface.pointY.clear();

						recompute();
					}
				});

		surface = (DrawingView) (findViewById(R.id.draw_surface));

		surface.onClickCallback = new Runnable() {
			@Override
			public void run() {
				recompute();
			}
		};

		Intent respondIntent = getIntent();
		Bundle extras = respondIntent.getExtras();
		imageUri = Uri.parse(extras.getString("imageUri"));

		try {
			surface.img = MediaStore.Images.Media.getBitmap(
					getContentResolver(), imageUri);

			float scaleFact = ((float) BITMAP_SIZE)
					/ Math.max(surface.img.getWidth(), surface.img.getHeight());

			surface.img = Bitmap.createScaledBitmap(surface.img,
					(int) (surface.img.getWidth() * scaleFact),
					(int) (surface.img.getHeight() * scaleFact), true);
			surface.img = surface.img.copy(Bitmap.Config.ARGB_8888, false);

			surface.overlay = Bitmap.createBitmap(surface.img.getWidth(),
					surface.img.getHeight(), Bitmap.Config.ARGB_8888);

			// ImageView img = (ImageView) findViewById(R.id.image_manipulate);
			// img.setImageBitmap(bmp);
		} catch (FileNotFoundException e) { // TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generate catch block
			e.printStackTrace();
		}
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {

		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.img_gallery, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// Handle action bar item clicks here. The action bar will
		// automatically handle clicks on the Home/Up button, so long
		// as you specify a parent activity in AndroidManifest.xml.
		int id = item.getItemId();
		if (id == R.id.action_settings) {
			return true;
		}
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
		// canvas = surfaceHolder.lockCanvas();
		// canvas.drawCircle(50, 20, 3, drawPaint);
		// surfaceHolder.unlockCanvasAndPost(canvas);
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub

	}
}
