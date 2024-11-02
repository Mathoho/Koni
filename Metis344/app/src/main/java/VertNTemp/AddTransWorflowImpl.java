package VertNTemp;
import java.time.Duration;
import java.util.HashMap;
import java.util.Map;
import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

public class AddTransWorflowImpl implements AddTransWorflow{
    private static final String ADDTRANS = "Adding Trans";

    // Retry options for the workflow
    private final RetryOptions retryoptions = RetryOptions.newBuilder()
            .setInitialInterval(Duration.ofSeconds(1))
            .setMaximumInterval(Duration.ofSeconds(100))
            .setBackoffCoefficient(2)
            .setMaximumAttempts(500)
            .build();

    // Default activity options for the workflow
    private final ActivityOptions defaultActivityOptions = ActivityOptions.newBuilder()  
    .       setStartToCloseTimeout(Duration.ofSeconds(5))
            .setRetryOptions(retryoptions)
            .build();
   
    // Activity-specific options
    private final Map<String, ActivityOptions> perActivityMethodOptions = new HashMap<String, ActivityOptions>(){{
        put(ADDTRANS, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};
   
    // Activity stub for the API2Interface
    private final API2Interface activityAPI2 = Workflow.newActivityStub(API2Interface.class,defaultActivityOptions,perActivityMethodOptions);
    
    /**
     * Adds a transaction.
     *
     * @param UserID The user ID associated with the transaction.
     * @param amount The amount of the transaction.
     * @param date   The date of the transaction.
     * @param time   The time of the transaction.
     * @return A string indicating the result of the transaction addition.
     */
    @Override
    public String AddTrans_temp(int UserID, int amount, String date, String time) {
        return activityAPI2.addTrans(UserID, amount, date, time);
    }
}
