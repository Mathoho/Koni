
package VertNTemp;
import java.time.Duration;
import java.util.HashMap;
import java.util.Map;

import io.temporal.activity.ActivityOptions;
import io.temporal.common.RetryOptions;
import io.temporal.workflow.Workflow;

/**
 * Implementation of the TransWorkflow interface for Temporal workflow.
 */
public class TransWorkflowImpl implements TransWorkflow{
    
    private static final String TRANSACTION = "Transaction Payment";
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
    private final API1Interface activityAPI1 = Workflow.newActivityStub(API1Interface.class,defaultActivityOptions,perActivityMethodOptions);
    
    /**
     * Implementation of the Transaction_temp method.
     *
     * @param PAN    The PAN (Primary Account Number).
     * @param amount The transaction amount.
     * @param msg    A message related to the transaction.
     * @return A result string from the transaction activity.
     */
    @Override
    public String Transaction_temp(String PAN, int amount, String msg) { 
       return activityAPI1.transaction(PAN, amount, msg);
    }
}
