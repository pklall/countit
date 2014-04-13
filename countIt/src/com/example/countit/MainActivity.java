package com.example.countit;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.sql.Date;
import java.text.SimpleDateFormat;

import android.support.v7.app.ActionBarActivity;
import android.support.v4.app.Fragment;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.util.Log;

public class MainActivity extends ActionBarActivity {

	private Uri returnedUri = null;
	
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home_layout);
      
        
        ImageView imgv = (ImageView) findViewById(R.id.thecount);
        Resources res = getResources(); // need this to fetch the drawable
        Drawable draw = res.getDrawable( R.drawable.thecount_transparent );
        imgv.setImageDrawable(draw);
	/*
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            getSupportFragmentManager().beginTransaction()
                    .add(R.id.container, new PlaceholderFragment())
                    .commit();
        }
        
        Log.d("TESTING", "Answer = " + Integer.toString(CounterJNIWrapper.processImage()));
	*/
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
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

    public View onCreateView(LayoutInflater inflater, ViewGroup container,
            Bundle savedInstanceState) {
        
    	// View rootView = inflater.inflate(R.layout.fragment_main, container, false);
        View rootView = inflater.inflate(R.layout.home_layout, container, false);

     
        
/*
        findViewById(R.id.take_pic_button).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {          	
            }
        });
*/
        
        return rootView;
    }

    
    String mCurrentPhotoPath;

    private File createImageFile() throws IOException {
        // Create an image file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date(0));
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = Environment.getExternalStoragePublicDirectory(
                Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(
            imageFileName,  /* prefix */
            ".jpg",         /* suffix */
            storageDir      /* directory */
        );

        // Save a file: path for use with ACTION_VIEW intents
        mCurrentPhotoPath = "file:" + image.getAbsolutePath();
        return image;
    }
    
    static final int REQUEST_IMAGE_CAPTURE = 1;
    static final int REQUEST_TAKE_PHOTO = 1;
    private static final int SELECT_PHOTO = 100;

    
    public void dispatchTakePictureIntent(View view){
    	Log.v("dispatch","dispatch picture action");
        
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        // Ensure that there's a camera activity to handle the intent
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            // Create the File where the photo should go

        	File photoFile = null;
            try {
                photoFile = createImageFile();
            } catch (IOException ex) {
                // Error occurred while creating the File
                Log.d("debug", "caught exception when creating file");
            }
            
            
            
            // Continue only if the File was successfully created
            if (photoFile != null) {
                returnedUri = Uri.fromFile(photoFile);
            	takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
                        returnedUri);
                startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
            } else {
            	// 
            }
        }
    }    
    @Override
    public void onActivityResult(int request, int result, Intent returnedIntent){
//    	Log.d("debug:",returnedUri.toString());
    	
    	switch(request){
    	
    		case REQUEST_TAKE_PHOTO:
    			Log.d("use photo:","got the photo, start intent");
    			Intent manipulateIntent = new Intent(this,ManipulateImgActivity.class);
    			manipulateIntent.putExtra("imageUri",returnedUri.toString());
    	    	startActivity(manipulateIntent);
    			break;
    		case SELECT_PHOTO:
    	        if(result == RESULT_OK){  
    	            Uri selectedImage = returnedIntent.getData();
    	            Intent manipulateSelectedIntent = new Intent(this,ManipulateImgActivity.class);
        			manipulateSelectedIntent.putExtra("imageUri",selectedImage.toString());
        	    	startActivity(manipulateSelectedIntent);
    	          //  InputStream imageStream = null;
    	          //  Bitmap newSelectedImage;
    	          //try {
    	        //	  
					//	imageStream = getContentResolver().openInputStream(selectedImage);
	    	         //   newSelectedImage = BitmapFactory.decodeStream(imageStream);
				//	} catch (FileNotFoundException e) {
						// TODO Auto-generated catch block
					//	e.printStackTrace();
				//	}
    	        	}
    			break;
    		default:
    			break;
    	}
    	
    }

    public void selectPicButtonClick(View view){
//    	Intent goToGalleryIntent = new Intent(this, GalleryActivity.class);
 //   	startActivity(goToGalleryIntent);
    	Intent photoPickerIntent = new Intent(Intent.ACTION_PICK);
    	photoPickerIntent.setType("image/*");
    	startActivityForResult(photoPickerIntent, SELECT_PHOTO);
    }


}


