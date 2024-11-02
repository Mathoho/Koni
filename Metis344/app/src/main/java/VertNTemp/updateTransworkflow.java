package VertNTemp;
import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;

@WorkflowInterface
public interface updateTransworkflow {
    
    @WorkflowMethod
    boolean updateTrans_temp(String date, String time, String message);
}
