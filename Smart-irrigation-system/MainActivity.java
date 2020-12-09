package com.example.agrismart;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.content.DialogInterface;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.iid.FirebaseInstanceId;
import com.google.firebase.iid.InstanceIdResult;

public class MainActivity extends AppCompatActivity {
    public TextView wvalT,tem,moi,light,st;
    public String wval;
    public String temp1="";
    public  String mois1="";
    public String lig="";
    public int mst=0;


    public int res;

    EditText t1;
    FirebaseDatabase database = FirebaseDatabase.getInstance();
    DatabaseReference wlevel = database.getReference("wlevel");
    DatabaseReference mois = database.getReference("Mois");
    DatabaseReference mot = database.getReference("Mstatus");
    DatabaseReference li = database.getReference("Light");
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        light=findViewById(R.id.lightV);
        wvalT=findViewById(R.id.waterV);
        moi=findViewById(R.id.moisV);
        st=findViewById(R.id.stat);


        li.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                Integer a = dataSnapshot.getValue(Integer.class);
                lig=a.toString();
                light.setText(lig+" C");
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
        mot.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                Integer a = dataSnapshot.getValue(Integer.class);
                mst=a;
                if(a==1){
                    st.setText("Motor is On");
                }else{
                    st.setText("Motor is Off");
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });
        wlevel.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // This method is called once with the initial value and again
                // whenever data at this location is updated.
                Integer a = dataSnapshot.getValue(Integer.class);
                wval=a.toString();
                wvalT.setText(wval+"%");
            }
            @Override
            public void onCancelled(DatabaseError error) {
                // Failed to read value

            }

        });
        mois.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                Integer a = dataSnapshot.getValue(Integer.class);
                moi.setText(a.toString()+"%");
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

    }
    public void wat(View view){
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
        builder.setTitle("Choose it");
        res=Integer.valueOf(mst);
        if(mst==1){
            builder.setMessage("Do you want to turn off the motor");
            res=0;
        }else{
            builder.setMessage("Do you want to turn on the motor");
            res=1;
        }

        builder.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                mot.setValue(res);
            }
        });
        builder.setNegativeButton("No", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                mot.setValue(mst);
            }
        });
        AlertDialog dialog = builder.create();
        dialog.show();
    }

}
