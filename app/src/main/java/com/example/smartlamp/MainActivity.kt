package com.example.smartlamp

import android.os.Bundle
import android.view.View
import android.widget.ImageButton
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.androidnetworking.AndroidNetworking
import com.androidnetworking.error.ANError
import com.androidnetworking.interfaces.JSONArrayRequestListener
import com.androidnetworking.interfaces.JSONObjectRequestListener
import okhttp3.OkHttpClient
import org.json.JSONArray
import org.json.JSONObject


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        AndroidNetworking.initialize(applicationContext);
        val lampbtn = findViewById<ImageButton>(R.id.imageButton)
        val lampbtn2 = findViewById<ImageButton>(R.id.imageButton2)
        var cond = true
        var cond2 = true
        AndroidNetworking.get("http://47.254.248.173/iot/service_lamp.php")
//            .addQueryParameter("id", "4")
            .build()
            .getAsJSONArray(object: JSONArrayRequestListener{
                override fun onResponse(response: JSONArray?) {
                    val json = response?.getJSONObject(0)
                    val json2 = response?.getJSONObject(1)
                    val status = json?.getString("status_lampu")
                    if(status.equals("on")){
                        lampbtn.setImageResource(R.drawable.lamp_on)
                        cond = false
                    }else{
                        lampbtn.setImageResource(R.drawable.lamp_off)
                        cond = true
                    }
                    val status22 = json2?.getString("status_lampu")
                    if(status.equals("on")){
                        lampbtn2.setImageResource(R.drawable.lamp_on)
                        cond2 = false
                    }else{
                        lampbtn2.setImageResource(R.drawable.lamp_off)
                        cond2 = true
                    }
                }

                override fun onError(error: ANError?) {
                    Toast.makeText(applicationContext,"$error",Toast.LENGTH_SHORT).show()
                }

            })

        lampbtn.setOnClickListener(View.OnClickListener {
            if(cond) {
                lampbtn.setImageResource(R.drawable.lamp_on)
                cond = false
                AndroidNetworking.get("http://47.254.248.173/iot/update.php")
//                    .addQueryParameter("id", "4")
                    .addQueryParameter("status", "on")
                    .build()
                    .getAsJSONArray(object: JSONArrayRequestListener{
                        override fun onResponse(response: JSONArray?) {

                        }

                        override fun onError(error: ANError?) {
//                            Toast.makeText(applicationContext,"$error",Toast.LENGTH_SHORT).show()
                        }

                    })


            }else{
                lampbtn.setImageResource(R.drawable.lamp_off)
                cond = true
                AndroidNetworking.get("http://47.254.248.173/iot/update.php")
//                    .addQueryParameter("id", "4")
                    .addQueryParameter("status", "on")
                    .build()
                    .getAsJSONArray(object: JSONArrayRequestListener{
                        override fun onResponse(response: JSONArray?) {

                        }

                        override fun onError(error: ANError?) {
//                            Toast.makeText(applicationContext,"$error",Toast.LENGTH_SHORT).show()
                        }

                    })

            }
        })
        lampbtn2.setOnClickListener(View.OnClickListener {
            if(cond2) {
                lampbtn.setImageResource(R.drawable.lamp_on)
                cond2 = false
                AndroidNetworking.get("http://47.254.248.173/iot/update1.php")
//                    .addQueryParameter("id", "4")
                    .addQueryParameter("status", "on")
                    .build()
                    .getAsJSONArray(object: JSONArrayRequestListener{
                        override fun onResponse(response: JSONArray?) {

                        }

                        override fun onError(error: ANError?) {
//                            Toast.makeText(applicationContext,"$error",Toast.LENGTH_SHORT).show()
                        }

                    })


            }else{
                lampbtn.setImageResource(R.drawable.lamp_off)
                cond2 = true
                AndroidNetworking.get("http://47.254.248.173/iot/update1.php")
//                    .addQueryParameter("id", "4")
                    .addQueryParameter("status", "on")
                    .build()
                    .getAsJSONArray(object: JSONArrayRequestListener{
                        override fun onResponse(response: JSONArray?) {

                        }

                        override fun onError(error: ANError?) {
//                            Toast.makeText(applicationContext,"$error",Toast.LENGTH_SHORT).show()
                        }

                    })

            }
        })
    }
}