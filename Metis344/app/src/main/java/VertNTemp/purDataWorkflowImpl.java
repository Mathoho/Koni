package VertNTemp;
import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

/**
 * Implementation of the purDataWorkflow interface for purchasing Data.
 */
public class purDataWorkflowImpl implements purDataWorkflow{

    private static final String PUR_DATA = "Purchasing Data";
   
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
        put(PUR_DATA, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};
    
    // Activity stub for the API3Interface
    private final API3Interface activityAPI3 = Workflow.newActivityStub(API3Interface.class,defaultActivityOptions,perActivityMethodOptions);
    
    /**
     * Purchase data for a given phone number.
     *
     * @param phoneNumber The phone number for which to purchase data.
     * @param date        The date of the data purchase.
     * @param time        The time of the data purchase.
     * @param amountProd  The amount of data to purchase.
     * @return A string indicating the result of the data purchase.
     */
    @Override
    public String purchaseData_temp(String phoneNumber, String date, String time, int amountProd) {
        return activityAPI3.purchaseData(phoneNumber, date, time, amountProd);
    }
    
}
