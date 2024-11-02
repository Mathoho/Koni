package VertNTemp;
import java.sql.*;

import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface API3Interface {
    
    @ActivityMethod
    Connection connect_to_db(String dbname, String user, String password);

    @ActivityMethod
    String purchaseData(String phoneNumber, String date, String time, int data);

    @ActivityMethod
    String purchaseAirtime(String phoneNumber, String date, String time, int airtime);

    @ActivityMethod
    String updateAirtime(String phoneNumber, int airtime);
}
