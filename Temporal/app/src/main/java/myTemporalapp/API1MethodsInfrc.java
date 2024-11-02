 package myTemporalapp;

import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface API1MethodsInfrc {
    
    @ActivityMethod
    public void connect_to_db(String dbname, String user, String password);

    @ActivityMethod
    public void new_customer(int user_id, String full_name, String identification_number, String pan, int current_balance, String email_address, String contact_number, String address, String date_of_birth);
    
    @ActivityMethod
    public double get_current_balance(String pan);
    
    @ActivityMethod
    public String transaction(String pan, double amount, String transaction_type);

    @ActivityMethod
    public int get_user_id(String pan);
}