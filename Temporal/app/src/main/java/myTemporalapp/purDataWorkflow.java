package myTemporalapp;


import io.temporal.workflow.WorkflowInterface;
import io.temporal.workflow.WorkflowMethod;
//public interface MathWorkflow {
@WorkflowInterface
public interface purDataWorkflow {    
   
    // @WorkflowMethod
    // int Addition(int a, int b);
    
    @WorkflowMethod
    String purchaseData_temp(String phoneNumber, String date, String time, int amountProd);

   


    // @WorkflowMethod
    // int Subtraction(int a, int b);

    // @WorkflowMethod
    // double devision(int a, int b);

    // @WorkflowMethod
    // int multiply(int a, int b);
    
}
