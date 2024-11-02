package VertNTemp;
import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

/**
 * Implementation of the updateTransworkflow interface for Temporal workflow.
 */
public class updateTransworkflowImpl implements updateTransworkflow{
 
    private static final String UPDATE_TRANS = "Update Transaction";

     // Retry options for activity executions
    private final RetryOptions retryoptions = RetryOptions.newBuilder()
            .setInitialInterval(Duration.ofSeconds(1))
            .setMaximumInterval(Duration.ofSeconds(100))
            .setBackoffCoefficient(2)
            .setMaximumAttempts(500)
            .build();

    private final ActivityOptions defaultActivityOptions = ActivityOptions.newBuilder()  
    .       setStartToCloseTimeout(Duration.ofSeconds(5))
            .setRetryOptions(retryoptions)
            .build();
   
    private final Map<String, ActivityOptions> perActivityMethodOptions = new HashMap<String, ActivityOptions>(){{
        put(UPDATE_TRANS, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};
   
    private final API2Interface activityAPI2 = Workflow.newActivityStub(API2Interface.class,defaultActivityOptions,perActivityMethodOptions);
    
    /**
     * Implementation of the updateTrans_temp method.
     *
     * @param date    The date of the transaction update.
     * @param time    The time of the transaction update.
     * @param message A message related to the transaction update.
     * @return true if the transaction update was successful; otherwise, false.
     */
    @Override
    public boolean updateTrans_temp(String date, String time, String message) {
        
        return activityAPI2.updateTransStatus(date, time, message);
    }
}
