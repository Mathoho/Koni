package myTemporalapp;


import java.sql.Connection;

import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface API2MethodsInfrc {
    
    
    public Connection connect_to_db(String dbname, String user, String password);
    @ActivityMethod
    public int getSenderIDapi2();

    @ActivityMethod
    public double getTransAmount();

    @ActivityMethod
    public String getTransData(String[] conditions);

    @ActivityMethod
    public String getTranstime();

    @ActivityMethod
    public String getTransStatus(String[] conditions);

    @ActivityMethod
    public String addTrans(int senderId, double transAmount, String transDate, String transTime);

    @ActivityMethod
    public boolean updateTransStatus(String date, String time, String status);
}
