package VertNTemp;
import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;

@WorkflowInterface
public interface AddTransWorflow {
    @WorkflowMethod
    String AddTrans_temp(int UserID, int amount, String date, String time);
}
