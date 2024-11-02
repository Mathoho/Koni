package myTemporalapp;

import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

public class TransWorkflowImpl implements TransWorkflow{
    
    private static final String TRANSACTION = "Transaction";
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
        put(TRANSACTION, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};
    //private final MathActivities activity = Workflow.newActivityStub(MathActivities.class,options);
    private final API1MethodsInfrc activityAPI1 = Workflow.newActivityStub(API1MethodsInfrc.class,defaultActivityOptions,perActivityMethodOptions);
    //private final API2MethodsInfrc activityAPI2 = Workflow.newActivityStub(API2MethodsInfrc.class,options);
    //private final API3MethodsInfrc activityAPI3 = Workflow.newActivityStub(API3MethodsInfrc.class,options);
    @Override
    public String Transaction_temp(String PAN, int amount, String msg) {
       //API1Methods api1 = new API1Methods();
       
       return activityAPI1.transaction(PAN, amount, msg);
    }
}
