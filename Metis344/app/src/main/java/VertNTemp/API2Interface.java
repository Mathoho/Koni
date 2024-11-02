package VertNTemp;
import io.temporal.activity.ActivityInterface;
import io.temporal.activity.ActivityMethod;

@ActivityInterface
public interface API2Interface {
    @ActivityMethod
    void connect_to_db(String dbname, String user, String password);

    @ActivityMethod
    int getSenderIDapi2();

    @ActivityMethod
    double getTransAmount();

    @ActivityMethod
    String getTransData(String[] conditions);

    @ActivityMethod
    String getTranstime();

    @ActivityMethod
    String getTransStatus(String[] conditions);

//==========================INSERTION FUNCTIONS=========================================//

    @ActivityMethod
    String addTrans(int senderId, double transAmount, String transDate, String transTime);

    @ActivityMethod
    boolean updateTransStatus(String date, String time, String status);


}
