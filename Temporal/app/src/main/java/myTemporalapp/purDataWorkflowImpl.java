package myTemporalapp;

import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

public class purDataWorkflowImpl implements purDataWorkflow{

    private static final String PUR_DATA = "Purchasing Data";
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
        put(PUR_DATA, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};
    //private final MathActivities activity = Workflow.newActivityStub(MathActivities.class,options);
    // private final API1MethodsInfrc activityAPI1 = Workflow.newActivityStub(API1MethodsInfrc.class,options);
    // private final API2MethodsInfrc activityAPI2 = Workflow.newActivityStub(API2MethodsInfrc.class,options);
    private final API3MethodsInfrc activityAPI3 = Workflow.newActivityStub(API3MethodsInfrc.class,defaultActivityOptions,perActivityMethodOptions);
    
    @Override
    public String purchaseData_temp(String phoneNumber, String date, String time, int amountProd) {
        return activityAPI3.purchaseData(phoneNumber, date, time, amountProd);
    }
    
}
