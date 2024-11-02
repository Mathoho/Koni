package VertNTemp;

import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;

@WorkflowInterface
public interface purAirtmeWorkflow {
    @WorkflowMethod
    String purchaseAirtime_temp(String phoneNumber, String date, String time, int amountProd);

}
