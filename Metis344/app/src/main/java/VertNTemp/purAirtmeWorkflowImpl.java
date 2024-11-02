package VertNTemp;
import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

public class purAirtmeWorkflowImpl implements purAirtmeWorkflow{
    private static final String PUR_AIRTM = "Purchase airtime";
    
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
        put(PUR_AIRTM, ActivityOptions.newBuilder().setHeartbeatTimeout(Duration.ofSeconds(5)).build());
    }};

    // Activity stub for the API3Interface
    private final API3Interface activityAPI3 = Workflow.newActivityStub(API3Interface.class,defaultActivityOptions,perActivityMethodOptions);
    
    /**
     * Purchase airtime for a given phone number.
     *
     * @param phoneNumber The phone number for which to purchase airtime.
     * @param date        The date of the airtime purchase.
     * @param time        The time of the airtime purchase.
     * @param amountProd  The amount of airtime to purchase.
     * @return A string indicating the result of the airtime purchase.
     */
    @Override
    public String purchaseAirtime_temp(String phoneNumber, String date, String time, int amountProd) {
        //TODO change to get the data From the Router
        return activityAPI3.purchaseAirtime(phoneNumber, date, time, amountProd);
    }
}
