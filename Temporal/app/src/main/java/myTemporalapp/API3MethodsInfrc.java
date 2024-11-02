package myTemporalapp;

import java.sql.Connection;

import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface API3MethodsInfrc {
    
    @ActivityMethod
    public Connection connect_to_db(String dbname, String user, String password);

    @ActivityMethod
    public String purchaseData(String phoneNumber, String date, String time, int data);

    @ActivityMethod
    public String purchaseAirtime(String phoneNumber, String date, String time, int airtime);

    @ActivityMethod
    public String updateAirtime(String phoneNumber, int airtime);
}
