package myTemporalapp;


import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

public class updateTransworkflowImpl implements updateTransworkflow{
 
    private static final String UPDATE_TRANS = "Update Transaction";
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
    //private final MathActivities activity = Workflow.newActivityStub(MathActivities.class,options);
   // private final API1MethodsInfrc activityAPI1 = Workflow.newActivityStub(API1MethodsInfrc.class,options);
    private final API2MethodsInfrc activityAPI2 = Workflow.newActivityStub(API2MethodsInfrc.class,defaultActivityOptions,perActivityMethodOptions);
    //private final API3MethodsInfrc activityAPI3 = Workflow.newActivityStub(API3MethodsInfrc.class,options);
    

    @Override
    public boolean updateTrans_temp(String date, String time, String message) {
        //API2Methods api2 = new API2Methods();
        return activityAPI2.updateTransStatus(date, time, message);
    }
}
