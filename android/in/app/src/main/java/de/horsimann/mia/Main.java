package @@@package_dotted@@@;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.OpenableColumns;
import android.util.Log;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;


//@@@USE_ADMOB@@@import com.google.android.ump.ConsentForm;
//@@@USE_ADMOB@@@import com.google.android.ump.ConsentInformation;
//@@@USE_ADMOB@@@import com.google.android.ump.ConsentRequestParameters;
//@@@USE_ADMOB@@@import com.google.android.ump.FormError;
//@@@USE_ADMOB@@@import com.google.android.ump.UserMessagingPlatform;

//@@@USE_ADMOB@@@import com.google.android.gms.ads.AdError;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.AdRequest;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.FullScreenContentCallback;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.LoadAdError;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.MobileAds;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.OnUserEarnedRewardListener;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.initialization.InitializationStatus;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.initialization.OnInitializationCompleteListener;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.rewarded.RewardItem;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.rewarded.RewardedAd;
//@@@USE_ADMOB@@@import com.google.android.gms.ads.rewarded.RewardedAdLoadCallback;

//@@@USE_BILLING@@@import com.android.billingclient.api.BillingClient;
//@@@USE_BILLING@@@import com.android.billingclient.api.BillingClientStateListener;
//@@@USE_BILLING@@@import com.android.billingclient.api.BillingFlowParams;
//@@@USE_BILLING@@@import com.android.billingclient.api.BillingResult;
//@@@USE_BILLING@@@import com.android.billingclient.api.ConsumeParams;
//@@@USE_BILLING@@@import com.android.billingclient.api.ConsumeResponseListener;
//@@@USE_BILLING@@@import com.android.billingclient.api.ProductDetails;
//@@@USE_BILLING@@@import com.android.billingclient.api.ProductDetailsResponseListener;
//@@@USE_BILLING@@@import com.android.billingclient.api.Purchase;
//@@@USE_BILLING@@@import com.android.billingclient.api.PurchasesUpdatedListener;
//@@@USE_BILLING@@@import com.android.billingclient.api.QueryProductDetailsParams;

import org.libsdl.app.SDLActivity;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

/**
 * A sample wrapper class that just calls SDLActivity
 */

public class Main extends SDLActivity {
    private static final String TAG = "MIA";

    //
    // AdMob settings:
    //

    // Google AdMob Test Reward Ad: ca-app-pub-3940256099942544/5224354917
//@@@USE_ADMOB@@@    private static final String ADMOB_AD_REWARD_ID = "ca-app-pub-3940256099942544/5224354917";


    //
    // Play Billing settings:
    //
    // change these ids to the available product ids in the play store
    // order matters for the some engine, which just uses the indices 0, 1, ...
//@@@USE_BILLING@@@    private static final String[] BILLING_PRODUCTS = {"paid_0", "paid_1"};



    private static final int DOWNLOAD_FILE = 1;
    private static final int UPLOAD_FILE = 2;

    private static String download_file;
    private static String upload_file;


//@@@USE_ADMOB@@@    private static volatile boolean admob_ump_loaded = false;
//@@@USE_ADMOB@@@    private static volatile boolean admob_ad_reward_loaded = false;
//@@@USE_BILLING@@@    private static volatile boolean billing_loaded = false;


    private static native void oFileDownloadDone(boolean success, String client_user_file_name);

    private static native void oFileUploadDone(boolean success, String client_user_file_name);

//@@@USE_ADMOB@@@    private static native void aAdReward();

//@@@USE_BILLING@@@    private static native boolean aBillingPurchased(int id);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//@@@USE_ADMOB@@@       try {
//@@@USE_ADMOB@@@           ump_load();
//@@@USE_ADMOB@@@       } catch (Exception e) {
//@@@USE_ADMOB@@@           Log.e(TAG, "ump failed: " + e.toString());
//@@@USE_ADMOB@@@       }
//@@@USE_ADMOB@@@        try {
//@@@USE_ADMOB@@@            admob_load();
//@@@USE_ADMOB@@@        } catch (Exception e) {
//@@@USE_ADMOB@@@            Log.e(TAG, "admob failed: " + e.toString());
//@@@USE_ADMOB@@@        }
//@@@USE_BILLING@@@        try {
//@@@USE_BILLING@@@            billing_load();
//@@@USE_BILLING@@@        } catch (Exception e) {
//@@@USE_BILLING@@@            Log.e(TAG, "billing failed: " + e.toString());
//@@@USE_BILLING@@@        }

        // Check if we have write permission
        int permission = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permission != PackageManager.PERMISSION_GRANTED) {
            // We don't have permission so prompt the user
            ActivityCompat.requestPermissions(this,
                    new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    PackageManager.PERMISSION_GRANTED);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

        // sometimes the app is not fully closed after an onDestroy call.
        // in these cases, the app will try to reinit the jni C code, which is not possible.
        // mia's a_init for example o_assume's that its not already init'ed
        Log.d(TAG, "force exit");
        System.exit(0);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == DOWNLOAD_FILE) {
            if (resultCode != RESULT_OK) {
                Log.d(TAG, "downloading aborted");
                oFileDownloadDone(false, null);
                return;
            }

            // Get the URI of the selected file
            Uri uri = data.getData();

            // Extract the file name from the URI
            String user_file = null;
            if (uri != null) {
                Cursor cursor = null;
                try {
                    cursor = getContentResolver().query(uri, null, null, null, null);
                    if (cursor != null && cursor.moveToFirst()) {
                        int displayNameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                        user_file = cursor.getString(displayNameIndex);
                    }
                } finally {
                    if (cursor != null) {
                        cursor.close();
                    }
                }
            }

            Log.d(TAG, "downloading: " + download_file + " user_file: " + user_file);

            try {
                InputStream istream = new FileInputStream(download_file);
                OutputStream ostream = getContentResolver().openOutputStream(uri);
                byte[] buffer = new byte[1024];
                int length;
                while ((length = istream.read(buffer)) > 0) {
                    ostream.write(buffer, 0, length);
                }
                istream.close();
                ostream.close();
                oFileDownloadDone(true, user_file);
            } catch (Exception e) {
                Log.d(TAG, "downloading failed");
                e.printStackTrace();
                oFileDownloadDone(false, null);
            }
        }

        if (requestCode == UPLOAD_FILE) {
            if (resultCode != RESULT_OK) {
                Log.d(TAG, "upload aborted");
                oFileUploadDone(false, null);
                return;
            }

            // Get the URI of the selected file
            Uri uri = data.getData();

            // Extract the file name from the URI
            String user_file = null;
            if (uri != null) {
                Cursor cursor = null;
                try {
                    cursor = getContentResolver().query(uri, null, null, null, null);
                    if (cursor != null && cursor.moveToFirst()) {
                        int displayNameIndex = cursor.getColumnIndex(OpenableColumns.DISPLAY_NAME);
                        user_file = cursor.getString(displayNameIndex);
                    }
                } finally {
                    if (cursor != null) {
                        cursor.close();
                    }
                }
            }

            Log.d(TAG, "uploaded: " + upload_file + " user_file: " + user_file);

            try {
                InputStream istream = getContentResolver().openInputStream(uri);
                OutputStream ostream = new FileOutputStream(upload_file);
                byte[] buffer = new byte[1024];
                int length;
                while ((length = istream.read(buffer)) > 0) {
                    ostream.write(buffer, 0, length);
                }
                istream.close();
                ostream.close();
                oFileUploadDone(true, user_file);
            } catch (Exception e) {
                Log.d(TAG, "uploading failed");
                e.printStackTrace();
                oFileUploadDone(false, null);
            }
            ;
        }
    }

    public void oFileDownload(String file) {
        try {
            Log.d(TAG, "oFileDownload: " + file);
            // Extract the filename from the full path
            String filename = new File(file).getName();
            download_file = getFilesDir().getAbsolutePath() + "/" + file;

            Intent intent = new Intent(Intent.ACTION_CREATE_DOCUMENT);
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            String lower = filename.toLowerCase();
            if (lower.endsWith(".png")) {
                intent.setType("image/png");
            } else if (lower.endsWith(".gif")) {
                intent.setType("image/gif");
            } else if (lower.endsWith(".jpg") || lower.endsWith("jpeg")) {
                intent.setType("image/jpeg");
            } else {
                intent.setType("*/*");
            }

            intent.putExtra(Intent.EXTRA_TITLE, filename);

            startActivityForResult(intent, DOWNLOAD_FILE);
        } catch (Exception e) {
            Log.e(TAG, "oFileDownload failed: " + e.toString());
            oFileDownloadDone(false, null);
        }
    }


    public void oFileUpload(String file) {
        try {
            Log.d(TAG, "oFileUpload: " + file);
            // Extract the filename from the full path
            String filename = new File(file).getName();
            upload_file = getFilesDir().getAbsolutePath() + "/" + file;

            Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);
            intent.addCategory(Intent.CATEGORY_OPENABLE);
            String lower = filename.toLowerCase();
            if (lower.endsWith(".png")) {
                intent.setType("image/png");
            } else if (lower.endsWith(".gif")) {
                intent.setType("image/gif");
            } else if (lower.endsWith(".jpg") || lower.endsWith("jpeg")) {
                intent.setType("image/jpeg");
            } else {
                intent.setType("*/*");
            }

            intent.putExtra(Intent.EXTRA_TITLE, filename);

            startActivityForResult(intent, UPLOAD_FILE);
        } catch (Exception e) {
            Log.e(TAG, "oFileUpload failed: " + e.toString());
            oFileUploadDone(false, null);
        }
    }

    public static byte[] oFetchBlocking(String url, byte[] optPostData) {
        if(optPostData != null) {
            Log.d(TAG, "oFetchBlocking POST request: " + url);
        } else {
            Log.d(TAG, "oFetchBlocking GET request: " + url);
        }
        HttpURLConnection urlConnection = null;
        byte[] result = null;

        try {
            URL jurl = new URL(url);
            urlConnection = (HttpURLConnection) jurl.openConnection();

            // Set timeouts
            urlConnection.setReadTimeout(10000);
            urlConnection.setConnectTimeout(15000);

            if (optPostData != null) {
                // For POST request
                urlConnection.setRequestMethod("POST");
                urlConnection.setDoOutput(true);
                urlConnection.setRequestProperty("Content-Type", "application/octet-stream");

                OutputStream os = urlConnection.getOutputStream();
                os.write(optPostData);
            } else {
                // For GET request
                urlConnection.setRequestMethod("GET");
            }
            int responseCode = urlConnection.getResponseCode();
            if (responseCode == HttpURLConnection.HTTP_OK) {
                InputStream in = urlConnection.getInputStream();
                ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

                byte[] buffer = new byte[1024];
                int length;
                while ((length = in.read(buffer)) != -1) {
                    byteArrayOutputStream.write(buffer, 0, length);
                }
                result = byteArrayOutputStream.toByteArray();
            } else {
                Log.e(TAG, "oFetchBlocking failed: HTTP response code " + responseCode);
            }

        } catch (Exception e) {
            Log.e(TAG, "oFetchBlocking failed: " + e.toString());
        }

        if (urlConnection != null) {
            urlConnection.disconnect();
        }
        return result;
    }







//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    // UMP privacy message stuff:
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    private static ConsentInformation ump_consentInformation = null;
//@@@USE_ADMOB@@@    private static ConsentForm ump_consentForm = null;
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void ump_ShowForm() {
//@@@USE_ADMOB@@@        if (!admob_ump_loaded || ump_consentForm == null) {
//@@@USE_ADMOB@@@            Log.e(TAG, "show form failed, no form available");
//@@@USE_ADMOB@@@            return;
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@        ump_consentForm.show(
//@@@USE_ADMOB@@@                Main.this,
//@@@USE_ADMOB@@@                new ConsentForm.OnConsentFormDismissedListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onConsentFormDismissed(FormError formError) {
//@@@USE_ADMOB@@@                        // Handle dismissal by reloading form.
//@@@USE_ADMOB@@@                        Log.i(TAG, "UMP form dismessed, reloading");
//@@@USE_ADMOB@@@                        ump_loadForm();
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                });
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void ump_loadForm() {
//@@@USE_ADMOB@@@        UserMessagingPlatform.loadConsentForm(
//@@@USE_ADMOB@@@                this, new UserMessagingPlatform.OnConsentFormLoadSuccessListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onConsentFormLoadSuccess(ConsentForm consentForm) {
//@@@USE_ADMOB@@@                        Main.admob_ump_loaded = true;
//@@@USE_ADMOB@@@                        Main.ump_consentForm = consentForm;
//@@@USE_ADMOB@@@                        if (ump_consentInformation.getConsentStatus() == ConsentInformation.ConsentStatus.REQUIRED) {
//@@@USE_ADMOB@@@                            consentForm.show(
//@@@USE_ADMOB@@@                                    Main.this,
//@@@USE_ADMOB@@@                                    new ConsentForm.OnConsentFormDismissedListener() {
//@@@USE_ADMOB@@@                                        @Override
//@@@USE_ADMOB@@@                                        public void onConsentFormDismissed(FormError formError) {
//@@@USE_ADMOB@@@                                            // Handle dismissal by reloading form.
//@@@USE_ADMOB@@@                                            Log.i(TAG, "UMP form dismessed, reloading");
//@@@USE_ADMOB@@@                                            ump_loadForm();
//@@@USE_ADMOB@@@                                        }
//@@@USE_ADMOB@@@                                    });
//@@@USE_ADMOB@@@                        }
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                },
//@@@USE_ADMOB@@@                new UserMessagingPlatform.OnConsentFormLoadFailureListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onConsentFormLoadFailure(FormError formError) {
//@@@USE_ADMOB@@@                        // Handle the error.
//@@@USE_ADMOB@@@                        Log.i(TAG, "UMP loading user consent failed");
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                });
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void ump_load() {
//@@@USE_ADMOB@@@        // IAB get user consent
//@@@USE_ADMOB@@@        // Set tag for underage of consent. Here false means users are not underage.
//@@@USE_ADMOB@@@        ConsentRequestParameters params = new ConsentRequestParameters
//@@@USE_ADMOB@@@                .Builder()
//@@@USE_ADMOB@@@                .setTagForUnderAgeOfConsent(false)
//@@@USE_ADMOB@@@                .build();
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@        ump_consentInformation = UserMessagingPlatform.getConsentInformation(this);
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@//        // test for debugging
//@@@USE_ADMOB@@@//        iab_consentInformation.reset();
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@        ump_consentInformation.requestConsentInfoUpdate(
//@@@USE_ADMOB@@@                this,
//@@@USE_ADMOB@@@                params,
//@@@USE_ADMOB@@@                new ConsentInformation.OnConsentInfoUpdateSuccessListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onConsentInfoUpdateSuccess() {
//@@@USE_ADMOB@@@                        // The consent information state was updated.
//@@@USE_ADMOB@@@                        // You are now ready to check if a form is available.
//@@@USE_ADMOB@@@                        if (ump_consentInformation.isConsentFormAvailable()) {
//@@@USE_ADMOB@@@                            Log.i(TAG, "UMP info update succeeded, loading form...");
//@@@USE_ADMOB@@@                            ump_loadForm();
//@@@USE_ADMOB@@@                        } else {
//@@@USE_ADMOB@@@                            Log.i(TAG, "UMP info update succeeded, form not available");
//@@@USE_ADMOB@@@                        }
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                },
//@@@USE_ADMOB@@@                new ConsentInformation.OnConsentInfoUpdateFailureListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onConsentInfoUpdateFailure(FormError formError) {
//@@@USE_ADMOB@@@                        // Handle the error.
//@@@USE_ADMOB@@@                        Log.i(TAG, "UMP user consent info failed");
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                });
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    // ump jni glue functions:
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public boolean aAdUmpLoaded() {
//@@@USE_ADMOB@@@        return admob_ump_loaded;
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void aAdUmpShow() {
//@@@USE_ADMOB@@@        try {
//@@@USE_ADMOB@@@            // run on main thread
//@@@USE_ADMOB@@@            Handler mainHandler = new Handler(Looper.getMainLooper());
//@@@USE_ADMOB@@@            Runnable myRunnable = new Runnable() {
//@@@USE_ADMOB@@@                @Override
//@@@USE_ADMOB@@@                public void run() {
//@@@USE_ADMOB@@@                    ump_ShowForm();
//@@@USE_ADMOB@@@                }
//@@@USE_ADMOB@@@            };
//@@@USE_ADMOB@@@            mainHandler.post(myRunnable);
//@@@USE_ADMOB@@@        } catch (Exception e) {
//@@@USE_ADMOB@@@            Log.e(TAG, "aAdUmpShow failed: " + e.toString());
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    // admob stuff:
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    private static RewardedAd admob_rewardedAd = null;
//@@@USE_ADMOB@@@    private static boolean admob_isLoading = false;
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    private void admob_loadRewardedAd() {
//@@@USE_ADMOB@@@        if (admob_rewardedAd == null) {
//@@@USE_ADMOB@@@            admob_isLoading = true;
//@@@USE_ADMOB@@@            Main.admob_ad_reward_loaded = false;
//@@@USE_ADMOB@@@            AdRequest adRequest = new AdRequest.Builder().build();
//@@@USE_ADMOB@@@            RewardedAd.load(
//@@@USE_ADMOB@@@                    this,
//@@@USE_ADMOB@@@                    ADMOB_AD_REWARD_ID,
//@@@USE_ADMOB@@@                    adRequest,
//@@@USE_ADMOB@@@                    new RewardedAdLoadCallback() {
//@@@USE_ADMOB@@@                        @Override
//@@@USE_ADMOB@@@                        public void onAdFailedToLoad(LoadAdError loadAdError) {
//@@@USE_ADMOB@@@                            // Handle the error.
//@@@USE_ADMOB@@@                            Log.d(TAG, loadAdError.getMessage());
//@@@USE_ADMOB@@@                            admob_rewardedAd = null;
//@@@USE_ADMOB@@@                            Main.admob_isLoading = false;
//@@@USE_ADMOB@@@                            Main.admob_ad_reward_loaded = false;
//@@@USE_ADMOB@@@                            //Toast.makeText(SDLActivity.this, "onAdFailedToLoad", Toast.LENGTH_SHORT).show();
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@                            // retry
//@@@USE_ADMOB@@@                            admob_loadRewardedAdOnLooper();
//@@@USE_ADMOB@@@                        }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@                        @Override
//@@@USE_ADMOB@@@                        public void onAdLoaded(RewardedAd rewardedAd) {
//@@@USE_ADMOB@@@                            Main.admob_rewardedAd = rewardedAd;
//@@@USE_ADMOB@@@                            Log.d(TAG, "onAdLoaded");
//@@@USE_ADMOB@@@                            Main.admob_isLoading = false;
//@@@USE_ADMOB@@@                            //Toast.makeText(SDLActivity.this, "onAdLoaded", Toast.LENGTH_SHORT).show();
//@@@USE_ADMOB@@@                            Main.admob_ad_reward_loaded = true;
//@@@USE_ADMOB@@@                        }
//@@@USE_ADMOB@@@                    });
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    private void admob_loadRewardedAdOnLooper() {
//@@@USE_ADMOB@@@        // run on main thread
//@@@USE_ADMOB@@@        Handler mainHandler = new Handler(Looper.getMainLooper());
//@@@USE_ADMOB@@@        Runnable myRunnable = new Runnable() {
//@@@USE_ADMOB@@@            @Override
//@@@USE_ADMOB@@@            public void run() {
//@@@USE_ADMOB@@@                admob_loadRewardedAd();
//@@@USE_ADMOB@@@            }
//@@@USE_ADMOB@@@        };
//@@@USE_ADMOB@@@        mainHandler.post(myRunnable);
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    private void admob_showRewardedVideo() {
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@        if (admob_rewardedAd == null) {
//@@@USE_ADMOB@@@            Log.d("TAG", "The rewarded ad wasn't ready yet.");
//@@@USE_ADMOB@@@            return;
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@        admob_rewardedAd.setFullScreenContentCallback(
//@@@USE_ADMOB@@@                new FullScreenContentCallback() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onAdShowedFullScreenContent() {
//@@@USE_ADMOB@@@                        // Called when ad is shown.
//@@@USE_ADMOB@@@                        Log.d(TAG, "onAdShowedFullScreenContent");
//@@@USE_ADMOB@@@                        //Toast.makeText(SDLActivity.this, "onAdShowedFullScreenContent", Toast.LENGTH_SHORT)
//@@@USE_ADMOB@@@                        //        .show();
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onAdFailedToShowFullScreenContent(AdError adError) {
//@@@USE_ADMOB@@@                        // Called when ad fails to show.
//@@@USE_ADMOB@@@                        Log.d(TAG, "onAdFailedToShowFullScreenContent");
//@@@USE_ADMOB@@@                        // Don't forget to set the ad reference to null so you
//@@@USE_ADMOB@@@                        // don't show the ad a second time.
//@@@USE_ADMOB@@@                        admob_rewardedAd = null;
//@@@USE_ADMOB@@@                        //Toast.makeText(
//@@@USE_ADMOB@@@                        //                SDLActivity.this, "onAdFailedToShowFullScreenContent", Toast.LENGTH_SHORT)
//@@@USE_ADMOB@@@                        //        .show();
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onAdDismissedFullScreenContent() {
//@@@USE_ADMOB@@@                        // Called when ad is dismissed.
//@@@USE_ADMOB@@@                        // Don't forget to set the ad reference to null so you
//@@@USE_ADMOB@@@                        // don't show the ad a second time.
//@@@USE_ADMOB@@@                        admob_rewardedAd = null;
//@@@USE_ADMOB@@@                        Log.d(TAG, "onAdDismissedFullScreenContent");
//@@@USE_ADMOB@@@                        //Toast.makeText(SDLActivity.this, "onAdDismissedFullScreenContent", Toast.LENGTH_SHORT)
//@@@USE_ADMOB@@@                        //        .show();
//@@@USE_ADMOB@@@                        // Preload the next rewarded ad.
//@@@USE_ADMOB@@@                        Main.this.admob_loadRewardedAd();
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                });
//@@@USE_ADMOB@@@        Activity activityContext = Main.this;
//@@@USE_ADMOB@@@        admob_rewardedAd.show(
//@@@USE_ADMOB@@@                activityContext,
//@@@USE_ADMOB@@@                new OnUserEarnedRewardListener() {
//@@@USE_ADMOB@@@                    @Override
//@@@USE_ADMOB@@@                    public void onUserEarnedReward(RewardItem rewardItem) {
//@@@USE_ADMOB@@@                        // Handle the reward.
//@@@USE_ADMOB@@@                        Log.d("TAG", "The user earned the reward.");
//@@@USE_ADMOB@@@                        //int rewardAmount = rewardItem.getAmount();
//@@@USE_ADMOB@@@                        //String rewardType = rewardItem.getType();
//@@@USE_ADMOB@@@                        aAdReward();
//@@@USE_ADMOB@@@                    }
//@@@USE_ADMOB@@@                });
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void admob_load() {
//@@@USE_ADMOB@@@        // load AdMob
//@@@USE_ADMOB@@@        try {
//@@@USE_ADMOB@@@            MobileAds.initialize(this, new OnInitializationCompleteListener() {
//@@@USE_ADMOB@@@                @Override
//@@@USE_ADMOB@@@                public void onInitializationComplete(InitializationStatus initializationStatus) {
//@@@USE_ADMOB@@@                }
//@@@USE_ADMOB@@@            });
//@@@USE_ADMOB@@@        } catch(Exception e) {
//@@@USE_ADMOB@@@            Log.e(TAG, "MobileAds.initialize failed...");
//@@@USE_ADMOB@@@            return;
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@        admob_loadRewardedAd();
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@    // admob jni glue functions:
//@@@USE_ADMOB@@@    //
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public boolean aAdLoaded() {
//@@@USE_ADMOB@@@        return admob_ad_reward_loaded;
//@@@USE_ADMOB@@@    }
//@@@USE_ADMOB@@@
//@@@USE_ADMOB@@@    public void aAdShow() {
//@@@USE_ADMOB@@@        try {
//@@@USE_ADMOB@@@            // run on main thread
//@@@USE_ADMOB@@@            Handler mainHandler = new Handler(Looper.getMainLooper());
//@@@USE_ADMOB@@@            Runnable myRunnable = new Runnable() {
//@@@USE_ADMOB@@@                @Override
//@@@USE_ADMOB@@@                public void run() {
//@@@USE_ADMOB@@@                    admob_showRewardedVideo();
//@@@USE_ADMOB@@@                }
//@@@USE_ADMOB@@@            };
//@@@USE_ADMOB@@@            mainHandler.post(myRunnable);
//@@@USE_ADMOB@@@        } catch (Exception e) {
//@@@USE_ADMOB@@@            Log.e(TAG, "u_admob_reward_ad_show failed: " + e.toString());
//@@@USE_ADMOB@@@        }
//@@@USE_ADMOB@@@    }


//@@@USE_BILLING@@@    //
//@@@USE_BILLING@@@    // play billing stuff:
//@@@USE_BILLING@@@    //
//@@@USE_BILLING@@@    private BillingClient billing_billingClient = null;
//@@@USE_BILLING@@@    List<ProductDetails> billing_productDetailsList = null;
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    private void billing_buy(int idx) {
//@@@USE_BILLING@@@        if(!billing_loaded || billing_productDetailsList == null) {
//@@@USE_BILLING@@@            Log.e(TAG, "not loaded or productDetailsList is null");
//@@@USE_BILLING@@@            return;
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@        ProductDetails product = null;
//@@@USE_BILLING@@@        for (ProductDetails p : billing_productDetailsList) {
//@@@USE_BILLING@@@            if (p.getProductId().equals(BILLING_PRODUCTS[idx])) {
//@@@USE_BILLING@@@                product = p;
//@@@USE_BILLING@@@                break;
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@        if (product == null) {
//@@@USE_BILLING@@@            Log.e(TAG, "product not found!");
//@@@USE_BILLING@@@            return;
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        ArrayList<BillingFlowParams.ProductDetailsParams> productDetailsParamsList = new ArrayList<>();
//@@@USE_BILLING@@@        productDetailsParamsList.add(BillingFlowParams.ProductDetailsParams.newBuilder()
//@@@USE_BILLING@@@                .setProductDetails(product)
//@@@USE_BILLING@@@                .build());
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
//@@@USE_BILLING@@@                .setProductDetailsParamsList(productDetailsParamsList)
//@@@USE_BILLING@@@                .build();
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        // launch the billing flow
//@@@USE_BILLING@@@        BillingResult billingResult = billing_billingClient.launchBillingFlow(this, billingFlowParams);
//@@@USE_BILLING@@@        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
//@@@USE_BILLING@@@            Log.i(TAG, "launched billing flow");
//@@@USE_BILLING@@@        } else {
//@@@USE_BILLING@@@            Log.e(TAG, "failed to launch billing flow");
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@    }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    private void billing_query_products() {
//@@@USE_BILLING@@@        ArrayList<QueryProductDetailsParams.Product> products = new ArrayList<>();
//@@@USE_BILLING@@@        for (String billingProduct : BILLING_PRODUCTS) {
//@@@USE_BILLING@@@            products.add(QueryProductDetailsParams.Product.newBuilder()
//@@@USE_BILLING@@@                    .setProductId(billingProduct)
//@@@USE_BILLING@@@                    .setProductType(BillingClient.ProductType.INAPP)
//@@@USE_BILLING@@@                    .build());
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@        QueryProductDetailsParams queryProductDetailsParams =
//@@@USE_BILLING@@@                QueryProductDetailsParams.newBuilder()
//@@@USE_BILLING@@@                        .setProductList(products)
//@@@USE_BILLING@@@                        .build();
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        billing_billingClient.queryProductDetailsAsync(
//@@@USE_BILLING@@@                queryProductDetailsParams,
//@@@USE_BILLING@@@                new ProductDetailsResponseListener() {
//@@@USE_BILLING@@@                    public void onProductDetailsResponse(BillingResult billingResult,
//@@@USE_BILLING@@@                                                         List<ProductDetails> productDetailsList) {
//@@@USE_BILLING@@@                        // check billingResult
//@@@USE_BILLING@@@                        if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
//@@@USE_BILLING@@@                            Log.i(TAG, "got billing product infos");
//@@@USE_BILLING@@@                        } else {
//@@@USE_BILLING@@@                            Log.e(TAG, "failed to get billing product infos");
//@@@USE_BILLING@@@                            return;
//@@@USE_BILLING@@@                        }
//@@@USE_BILLING@@@                        // process returned productDetailsList
//@@@USE_BILLING@@@                        if (BILLING_PRODUCTS.length != productDetailsList.size()) {
//@@@USE_BILLING@@@                            Log.e(TAG, "got not the right number of products");
//@@@USE_BILLING@@@                            billing_loaded = false;
//@@@USE_BILLING@@@                            return;
//@@@USE_BILLING@@@                        }
//@@@USE_BILLING@@@                        boolean found_all = true;
//@@@USE_BILLING@@@                        for (int i = 0; i < BILLING_PRODUCTS.length; i++) {
//@@@USE_BILLING@@@                            boolean found = false;
//@@@USE_BILLING@@@                            for (int j = 0; j < productDetailsList.size(); j++) {
//@@@USE_BILLING@@@                                if (productDetailsList.get(j).getProductId().equals(BILLING_PRODUCTS[i])) {
//@@@USE_BILLING@@@                                    found = true;
//@@@USE_BILLING@@@                                    break;
//@@@USE_BILLING@@@                                }
//@@@USE_BILLING@@@                            }
//@@@USE_BILLING@@@                            if (!found) {
//@@@USE_BILLING@@@                                found_all = false;
//@@@USE_BILLING@@@                                break;
//@@@USE_BILLING@@@                            }
//@@@USE_BILLING@@@                        }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@                        if (found_all) {
//@@@USE_BILLING@@@                            Log.i(TAG, "found all products");
//@@@USE_BILLING@@@                            billing_productDetailsList = productDetailsList;
//@@@USE_BILLING@@@                        } else {
//@@@USE_BILLING@@@                            Log.e(TAG, "Found not all products by name");
//@@@USE_BILLING@@@                        }
//@@@USE_BILLING@@@                        billing_loaded = found_all;
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@                    }
//@@@USE_BILLING@@@                }
//@@@USE_BILLING@@@        );
//@@@USE_BILLING@@@    }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    private void billing_handlePurchase(Purchase purchase) {
//@@@USE_BILLING@@@        if (purchase.getPurchaseState() != Purchase.PurchaseState.PURCHASED) {
//@@@USE_BILLING@@@            Log.i(TAG, "purchase not purchased...");
//@@@USE_BILLING@@@            return;
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@        Log.e(TAG, "purchase: " + purchase.getPurchaseToken());
//@@@USE_BILLING@@@        boolean all_consumed = true;
//@@@USE_BILLING@@@        for (String product : purchase.getProducts()) {
//@@@USE_BILLING@@@            boolean consumed = false;
//@@@USE_BILLING@@@            for (int i = 0; i < BILLING_PRODUCTS.length; i++) {
//@@@USE_BILLING@@@                if (product.equals(BILLING_PRODUCTS[i])) {
//@@@USE_BILLING@@@                    Log.e(TAG, "purchase: " + purchase.getPurchaseToken() + " product: " + product);
//@@@USE_BILLING@@@                    consumed = aBillingPurchased(i);
//@@@USE_BILLING@@@                    break;
//@@@USE_BILLING@@@                }
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@            if (!consumed) {
//@@@USE_BILLING@@@                Log.e(TAG, "product not found: " + product);
//@@@USE_BILLING@@@                all_consumed = false;
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        if(!all_consumed) {
//@@@USE_BILLING@@@            Log.e(TAG, "consuming the purchase failed");
//@@@USE_BILLING@@@            return;
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        ConsumeParams consumeParams =
//@@@USE_BILLING@@@                ConsumeParams.newBuilder()
//@@@USE_BILLING@@@                        .setPurchaseToken(purchase.getPurchaseToken())
//@@@USE_BILLING@@@                        .build();
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        ConsumeResponseListener listener = new ConsumeResponseListener() {
//@@@USE_BILLING@@@            @Override
//@@@USE_BILLING@@@            public void onConsumeResponse(BillingResult billingResult, String purchaseToken) {
//@@@USE_BILLING@@@                if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
//@@@USE_BILLING@@@                    // Handle the success of the consume operation.
//@@@USE_BILLING@@@                    Log.i(TAG, "consumed the purchase");
//@@@USE_BILLING@@@                } else {
//@@@USE_BILLING@@@                    Log.e(TAG, "failed to consume the purchase?");
//@@@USE_BILLING@@@                }
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@        };
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        billing_billingClient.consumeAsync(consumeParams, listener);
//@@@USE_BILLING@@@    }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    private PurchasesUpdatedListener billing_purchasesUpdatedListener = new PurchasesUpdatedListener() {
//@@@USE_BILLING@@@        @Override
//@@@USE_BILLING@@@        public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {
//@@@USE_BILLING@@@            if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
//@@@USE_BILLING@@@                Log.i(TAG, "got a purchase");
//@@@USE_BILLING@@@            } else {
//@@@USE_BILLING@@@                Log.e(TAG, "purchase update failed?");
//@@@USE_BILLING@@@                return;
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@            for (Purchase p : purchases) {
//@@@USE_BILLING@@@                billing_handlePurchase(p);
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@    };
//@@@USE_BILLING@@@    private BillingClientStateListener billing_billingClientStateListener = new BillingClientStateListener() {
//@@@USE_BILLING@@@        @Override
//@@@USE_BILLING@@@        public void onBillingSetupFinished(BillingResult billingResult) {
//@@@USE_BILLING@@@            if (billingResult.getResponseCode() == BillingClient.BillingResponseCode.OK) {
//@@@USE_BILLING@@@                Log.i(TAG, "billing is ready");
//@@@USE_BILLING@@@                billing_query_products();
//@@@USE_BILLING@@@            } else {
//@@@USE_BILLING@@@                Log.e(TAG, "failed to built billing api");
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        @Override
//@@@USE_BILLING@@@        public void onBillingServiceDisconnected() {
//@@@USE_BILLING@@@            // Try to restart the connection on the next request to
//@@@USE_BILLING@@@            // Google Play by calling the startConnection() method.
//@@@USE_BILLING@@@            Log.i(TAG, "billing disconnected, reconnect...");
//@@@USE_BILLING@@@            billing_billingClient.startConnection(billing_billingClientStateListener);
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@    };
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    private void billing_load() {
//@@@USE_BILLING@@@        billing_loaded = false;
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        billing_billingClient = BillingClient.newBuilder(this)
//@@@USE_BILLING@@@                .setListener(billing_purchasesUpdatedListener)
//@@@USE_BILLING@@@                .enablePendingPurchases()
//@@@USE_BILLING@@@                .build();
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@        billing_billingClient.startConnection(billing_billingClientStateListener);
//@@@USE_BILLING@@@    }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    //
//@@@USE_BILLING@@@    // admob jni glue functions:
//@@@USE_BILLING@@@    //
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    public boolean aBillingLoaded() {
//@@@USE_BILLING@@@        return billing_loaded;
//@@@USE_BILLING@@@    }
//@@@USE_BILLING@@@
//@@@USE_BILLING@@@    public void aBillingBuy(final int idx) {
//@@@USE_BILLING@@@        try {
//@@@USE_BILLING@@@            if (!billing_loaded) {
//@@@USE_BILLING@@@                Log.e(TAG, "billing not loaded");
//@@@USE_BILLING@@@                return;
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@            if (idx < 0 || idx >= BILLING_PRODUCTS.length) {
//@@@USE_BILLING@@@                Log.wtf(TAG, "billing buy got an invalid idx?");
//@@@USE_BILLING@@@                return;
//@@@USE_BILLING@@@            }
//@@@USE_BILLING@@@            // run on main thread
//@@@USE_BILLING@@@            Handler mainHandler = new Handler(Looper.getMainLooper());
//@@@USE_BILLING@@@            Runnable myRunnable = new Runnable() {
//@@@USE_BILLING@@@                @Override
//@@@USE_BILLING@@@                public void run() {
//@@@USE_BILLING@@@                    billing_buy(idx);
//@@@USE_BILLING@@@                }
//@@@USE_BILLING@@@            };
//@@@USE_BILLING@@@            mainHandler.post(myRunnable);
//@@@USE_BILLING@@@        } catch (Exception e) {
//@@@USE_BILLING@@@            Log.e(TAG, "u_billing_buy failed: " + e.toString());
//@@@USE_BILLING@@@        }
//@@@USE_BILLING@@@    }

}
