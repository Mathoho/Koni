package myTemporalapp;
import java.sql.Connection;

import io.temporal.api.common.v1.WorkflowExecution;
import io.temporal.client.WorkflowClient;
import io.temporal.client.WorkflowOptions;
import io.temporal.serviceclient.WorkflowServiceStubs;

public class TemporalMethods {
    public TemporalMethods(){

    }
    public  boolean updateTransTemp(Connection conn2, String date, String time, String msg, String PAN,API2Methods api2){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN "+ PAN+ "UPDATED_TRANS: at time: "+ time;
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.UPDATE_TRANS_TASK_QUEUE)
                    .build();
        updateTransworkflow workflow = client.newWorkflowStub(updateTransworkflow.class, options);
       
        
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::updateTrans_temp, date,time,msg);
        //String workflowId = WorkflowStub.fromTyped(payStsworkflow).getExecution().getWorkflowId();
        // Display workflow execution results
        System.out.println(wrkflwexc.getWorkflowId() + " updated the trans  ");
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());
        boolean rslt = api2.updateTransStatus(date, time, msg);
        //updateTransworkflow myWorkflow = Workflow.newWorkflowStub(updateTransworkflow.class);
        System.out.println("here before getting");
        boolean result2 = workflow.updateTrans_temp(date, time, msg);
        System.out.println("After calling N");
        return result2;
    }

    public  String purAirtmTemp(String phoneNumber, String date, String time, int amountProduct, String PAN,API3Methods api3){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN +" PURCHASE AIRTIME at time: " + time;
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.PURCHASE_AIRTIME_TASK_QUEUE)
                    .build();
        purAirtmeWorkflow workflow = client.newWorkflowStub(purAirtmeWorkflow.class, options);
        
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::purchaseAirtime_temp, phoneNumber,date,time,amountProduct);

        //String workflowId = WorkflowStub.fromTyped(payStsworkflow).getExecution().getWorkflowId();
        // Display workflow execution results
        System.out.println(wrkflwexc.getWorkflowId() + " purchased airtime of amount " + amountProduct);
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());

        String rslt = api3.purchaseAirtime(phoneNumber, date, time, amountProduct);
        return rslt;
    }

    public  String addTransTemp(int userID, int amount, String date, String time, String PAN,API2Methods api2) {
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN +" adding trans at TIME: "+time;
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.ADD_TRANS_TASK_QUEUE)
                    .build();
        AddTransWorflow workflow = client.newWorkflowStub(AddTransWorflow.class, options);
        WorkflowExecution addTranswe = WorkflowClient.start(workflow::AddTrans_temp, userID, amount,date,time);
        //String workflowId = WorkflowStub.fromTyped(payStsworkflow).getExecution().getWorkflowId();
        // Display workflow execution results
        System.out.println(addTranswe.getWorkflowId() + " added trans of amount to database " + amount);
        System.out.printf("\nWorkflowID: %s RunID: %s", addTranswe.getWorkflowId(), addTranswe.getRunId());
        String rslt = api2.addTrans(userID, amount, date, time);
        return rslt;
    }

    public  String transactionTemp(String PAN, int amount, String msg,String time,API1Methods api1){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+PAN+ " did a Trans " + msg + " of amount: " + amount+ " at time: "+time;
        WorkflowOptions options = null;
        switch (msg) {
            case "PAYMENT":
            System.out.println("making payment");
            options = WorkflowOptions.newBuilder()
            .setWorkflowId(WORKFLOW_ID)
            .setTaskQueue(Shared.TRANSACTION_PAYMENT_TASK_QUEUE)
            .build();
            break;
            case "REVERSAL":
                options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.TRANSACTION_REVERSAL_TASK_QUEUE)
                    .build();
                break;
            default:
                return "";
        }
        
        TransWorkflow workflow = client.newWorkflowStub(TransWorkflow.class, options);
        WorkflowExecution wrkflwexc = WorkflowClient.start(workflow::Transaction_temp, PAN, amount, msg);
        System.out.println(wrkflwexc.getWorkflowId() + " did a Trans " + msg + " of amount: " + amount);
        System.out.printf("\nWorkflowID: %s RunID: %s", wrkflwexc.getWorkflowId(), wrkflwexc.getRunId());
        
        String rslt = api1.transaction(PAN, amount, msg);

        return rslt;
    }

    public static String purDataTemp(String phoneNumber, String date, String time, int amountProduct, String PAN){
        WorkflowServiceStubs service = WorkflowServiceStubs.newLocalServiceStubs();
        WorkflowClient client = WorkflowClient.newInstance(service);
        final String WORKFLOW_ID = "MY PAN: "+ PAN+" PURCHASED DATA at time: "+ time;
        WorkflowOptions options = WorkflowOptions.newBuilder()
                    .setWorkflowId(WORKFLOW_ID)
                    .setTaskQueue(Shared.PURCHASE_DATA_TASK_QUEUE)
                    .build();
        purDataWorkflow workflow = client.newWorkflowStub(purDataWorkflow.class, options);
        String rslt = workflow.purchaseData_temp(phoneNumber, date, WORKFLOW_ID, amountProduct);
        WorkflowExecution purAirtmWe = WorkflowClient.start(workflow::purchaseData_temp, phoneNumber,date,time,amountProduct);

        //String workflowId = WorkflowStub.fromTyped(payStsworkflow).getExecution().getWorkflowId();
        // Display workflow execution results
        System.out.println(purAirtmWe.getWorkflowId() + " purchased airtime of amount " + amountProduct);
        System.out.printf("\nWorkflowID: %s RunID: %s", purAirtmWe.getWorkflowId(), purAirtmWe.getRunId());

        return rslt;
    }
}
