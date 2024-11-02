package VertNTemp;
import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;

@WorkflowInterface
public interface TransWorkflow {
    @WorkflowMethod
    String Transaction_temp(String PAN,int amount,String msg);
}
