package VertNTemp;
import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;
@WorkflowInterface
public interface purDataWorkflow {    
   
    @WorkflowMethod
    String purchaseData_temp(String phoneNumber, String date, String time, int amountProd);
    
}
